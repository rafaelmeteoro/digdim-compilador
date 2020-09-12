#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include <stdlib.h>

#include "visitorSemantico.h"
#include "erro.h"

VisitorSemantico::VisitorSemantico(){      //CONSTRUTOR DO VISITOR DO SEMANTICO
    this->escopo            = 0;
    this->loop              = 0;
    this->tabelaEscopo      = new TabelaSemantico();
    this->tabelaTipos       = new TabelaSemantico();
    this->tipoFuncao        = NULL;
    this->parametrosFuncao  = NULL;
    this->idFuncao          = false;
    this->idTipo            = false;
    this->verificaparametros = false;
    this->temMain           = false;
    this->mParametrosFuncao = true;
}

VisitorSemantico::~VisitorSemantico(){      //DETRUTOR DO VISITOR DO SEMANTICO
    delete this->tabelaEscopo;
    delete this->tabelaTipos;
}

bool VisitorSemantico::verificaOpBin(BinarioOpNodo *binOp, bool forcar){
    int linha = binOp->linha;
    if(!binOp->expr1 || !binOp->expr2 || !binOp->expr1->type || !binOp->expr2->type) return false;

    if(binOp->expr1->array || binOp->expr2->array){
        erroSemantico(ERRO_OP_ARRAY, NULL, 0, linha);
        return false;
    }

    int lTipo = binOp->expr1->type->tipo;
    if(((lTipo == INT) || (lTipo == FLOAT)) && (lTipo == binOp->expr2->type->tipo)){
        if(forcar){
            binOp->type = new TypeNodo(BOOLEAN, linha);
        }else{
            binOp->type = new TypeNodo(lTipo, linha);
        }
        return false;
    }else return true;
}

bool VisitorSemantico::verificaOpBin(RelationalOpNodo *relOp, bool forcar){
    int linha = relOp->linha;
    if(!relOp->expr1 || !relOp->expr2 || !relOp->expr1->type || !relOp->expr2->type) return false;

    if(relOp->expr1->array || relOp->expr2->array){
        erroSemantico(ERRO_OP_ARRAY, NULL, 0, linha);
        return false;
    }

    int lTipo = relOp->expr1->type->tipo;
    if(((lTipo == INT) || (lTipo == FLOAT)) && (lTipo == relOp->expr2->type->tipo)){
        if(forcar){
            relOp->type = new TypeNodo(BOOLEAN, linha);
        }else{
            relOp->type = new TypeNodo(lTipo, linha);
        }
        return false;
    }else return true;
}

bool VisitorSemantico::verificarLogOp(BinarioOpNodo *binOp){
    int linha = binOp->linha;
    if(!binOp->expr1 || !binOp->expr2 || !binOp->expr1->type || !binOp->expr2->type) return false;

    if(binOp->expr1->array || binOp->expr2->array){
        erroSemantico(ERRO_OP_ARRAY, NULL, 0, linha);
        return false;
    }

    if((binOp->expr1->type->tipo == BOOLEAN) && (binOp->expr2->type->tipo == BOOLEAN)){
        binOp->type = new TypeNodo(BOOLEAN, linha);
        return false;
    }else return true;
}

bool VisitorSemantico::verificarLogOp(RelationalOpNodo *relOp){
    int linha = relOp->linha;
    if(!relOp->expr1 || !relOp->expr2 || !relOp->expr1->type || !relOp->expr2->type) return false;

    if(relOp->expr1->array || relOp->expr2->array){
        erroSemantico(ERRO_OP_ARRAY, NULL, 0, linha);
        return false;
    }

    if((relOp->expr1->type->tipo == BOOLEAN) && (relOp->expr2->type->tipo == BOOLEAN)){
        relOp->type = new TypeNodo(BOOLEAN, linha);
        return false;
    }else return true;
}

void VisitorSemantico::visita(Nodo *nodo){
    Visitor::visita(nodo);
}

void VisitorSemantico::visita(ProgramNodo *program){
    Visitor::visita(program);
}

void VisitorSemantico::visita(StatementNodo *statement){
    Visitor::visita(statement);
}

void VisitorSemantico::visita(ExprNodo *expr){
    Visitor::visita(expr);
}

void VisitorSemantico::visita(TypeNodo *type){
    Visitor::visita(type);
}

void VisitorSemantico::visita(TypeIdNodo *typeId){
    this->idTipo = true;
    Visitor::visita(typeId);
    this->idTipo = false;
}

void VisitorSemantico::visita(TypePointerNodo *typePointer){
    Visitor::visita(typePointer);
}

void VisitorSemantico::visita(IdListNodo *idList){
    this->idTipo = false;
    Visitor::visita(idList);
    this->idTipo = false;
}

void VisitorSemantico::visita(VarDeclNodo *varDecl){
    int linha = varDecl->linha;
    IdListNodo *idList = varDecl->idList;
    while(idList){
        char *variavel = idList->id->nome;
        ItemEscopo *item = new ItemEscopo(variavel, this->escopo);
        if(varDecl->tipo == NULL) return;
        if(typeid(*(varDecl->tipo)) == typeid(TypeIdNodo)){
            IdNodo *id = new IdNodo(varDecl->idList->id->nome, 0);
            item->type = new TypeIdNodo(id, linha);
        }else{
            item->type = new TypeNodo(varDecl->tipo->tipo, linha);
        }
        item->tamanhoArray = ((idList->array) ? new NumNodo(idList->array->tipo, idList->array->valor, varDecl->linha) : NULL);
        if(this->tabelaEscopo->buscar(item)){
            erroSemantico(ERRO_ID_DECLARADO, variavel, item->escopo, linha);
            delete item;
        }else this->tabelaEscopo->incluir(item);
        idList = idList->idList;
    }
    Visitor::visita(varDecl);
}

void VisitorSemantico::visita(TypeDeclNodo *typeDecl){
    if(typeDecl->id != NULL){
        ItemEscopo *item = new ItemEscopo(typeDecl->id->nome, 0);
        if(tabelaEscopo->buscar(item)){
            erroSemantico(ERRO_ID_DECLARADO, item->identificador, item->escopo, typeDecl->linha);
        }else{
            TabelaSemantico *tabelaEscopoAntiga = tabelaEscopo; //guardando tabela
            tabelaEscopo = new TabelaSemantico();

            if(typeDecl->varDecl) typeDecl->varDecl->aceita(this);

            ItemTipo *itemTabela = new ItemTipo(typeDecl->id->nome, tabelaEscopo);
            itemTabela->variaveis = typeDecl->varDecl;
            tabelaTipos->incluir(itemTabela);

            tabelaEscopo = tabelaEscopoAntiga; //recuperando tabela

            ItemEscopo *itemEscopo = new ItemEscopo(typeDecl->id->nome, 0);
            itemEscopo->funcao = false;
            itemEscopo->tipo = true;
            tabelaEscopo->incluir(itemEscopo);
        }
        delete item;
    }
    if(typeDecl->id){
        this->idTipo = true;
        typeDecl->id->aceita(this);
        this->idTipo = false;
    }
    if(typeDecl->typeDecl) typeDecl->typeDecl->aceita(this);
}

void VisitorSemantico::visita(FunctionDeclNodo *functionDecl){
    int linha = functionDecl->linha;
    IdNodo *id = functionDecl->id;
    if(id){
        if (!strcmp(id->nome, (char *)"main")) temMain = true;
        this->idFuncao = true;
        char *identificador = id->nome;
        if(functionDecl->tipo == NULL) return;
        ItemEscopo *item = new ItemEscopo(identificador, escopo);
        item->funcao = true;
        item->type = new TypeNodo(functionDecl->tipo->tipo, linha);
        this->tipoFuncao = item->type;
        item->tamanhoArray = NULL;
        item->formalList = functionDecl->varListParametros;
        if ((!strcmp(id->nome, (char *)"main")) && item->formalList) erroSemantico(ERRO_ARGUMENTOS_MAIN, identificador, item->escopo, linha);
        if(tabelaEscopo->buscar(item)){
            erroSemantico(ERRO_ID_DECLARADO, identificador, item->escopo, linha);
            delete item;
        }else tabelaEscopo->incluir(item);
    }

    this->escopo = 1;
    Visitor::visita(functionDecl);
    this->tipoFuncao = NULL;
    this->escopo = 0;

    for(int i = 0; i < TAMANHO_TABELA; i++){
        ItemEscopo *item = (ItemEscopo*)tabelaEscopo->tabela[i];
        while(item){
            if(item->escopo == 1){
                ItemEscopo *aux = (ItemEscopo*)item->prox;
                tabelaEscopo->remover(item);
                item = aux;
            }else{
                item = (ItemEscopo*)item->prox;
            }
        }
    }
}

void VisitorSemantico::visita(VarDeclProgramNodo *varDeclProgram){
    Visitor::visita(varDeclProgram);
}

void VisitorSemantico::visita(TypeDeclProgramNodo *typeDeclProgram){
    Visitor::visita(typeDeclProgram);
}

void VisitorSemantico::visita(FunctionDeclProgramNodo *functionDeclProgram){
    Visitor::visita(functionDeclProgram);
}

void VisitorSemantico::visita(StatementListNodo *statementList){
    Visitor::visita(statementList);
}

void VisitorSemantico::visita(StatementListTypeNodo *statementType){
    Visitor::visita(statementType);
}

void VisitorSemantico::visita(StatementListVarNodo *stmtListVar){
    Visitor::visita(stmtListVar);
}

void VisitorSemantico::visita(IfNodo *If){
    Visitor::visita(If);
    if(!If->expr || !If->expr->type) return;
    if(If->expr->type->tipo != BOOLEAN) erroSemantico(ERRO_CONDICAO, (char*)"IF", 0, If->linha);
}

void VisitorSemantico::visita(WhileNodo *While){
    this->loop++;
    Visitor::visita(While);
    if(While->expr == NULL || While->expr->type == NULL) return;
    if(While->expr->type->tipo != BOOLEAN) erroSemantico(ERRO_CONDICAO, (char*)"WHILE", 0, While->linha);
    this->loop--;
}

void VisitorSemantico::visita(SwitchNodo *Switch){
    this->loop++;
    Visitor::visita(Switch);
    if(!Switch->expr || !Switch->expr->type) return;
    if((Switch->expr->type->tipo != INT) || (Switch->expr->array)) erroSemantico(ERRO_CONDICAO_SWITCH, NULL, 0, Switch->linha);
    this->loop--;
}

void VisitorSemantico::visita(BreakNodo *Break){
    Visitor::visita(Break);
    if(this->loop == 0){
        erroSemantico(ERRO_CONDICAO_BREAK, NULL, 0, Break->linha);
    }
}

void VisitorSemantico::visita(PrintlnNodo *print){
    if(print->exprList){
        Visitor::visita(print);
    }else{
        erroSemantico(ERRO_CONDICAO_PRINT, NULL, 0, print->linha);
    }

}

void VisitorSemantico::visita(ReadNodo *read){
    Visitor::visita(read);
    if((typeid(*read->expr) != typeid(IdNodo)) && (typeid(*read->expr) != typeid(ArrayNodo)) && (typeid(*read->expr) != typeid(NameExprNodo)) && (typeid(*read->expr) != typeid(PointerValueExprNodo))){
        erroSemantico(ERRO_CONDICAO_READ, NULL, 0, read->linha);
    }
}

void VisitorSemantico::visita(ReturnNodo *Return){
    Visitor::visita(Return);
    if((this->tipoFuncao != NULL) && (Return->expr != NULL) && (Return->expr->type != NULL) && (Return->expr->type->tipo != this->tipoFuncao->tipo)){
        erroSemantico(ERRO_CONDICAO_RETURN, NULL, 0, Return->linha);
    }
}

void VisitorSemantico::visita(ThrowNodo *Throw){
    Visitor::visita(Throw);
}

void VisitorSemantico::visita(TryNodo *Try){
    Visitor::visita(Try);
}

void VisitorSemantico::visita(ExprStatementNodo *exprStatement){
    Visitor::visita(exprStatement);
}

void VisitorSemantico::visita(IdNodo *id){
    int linha = id->linha;
    char *var = id->nome;
    ItemEscopo *item = new ItemEscopo(var, this->escopo);
    ItemEscopo *itemTabela = (ItemEscopo*)this->tabelaEscopo->buscar(item);
    bool temEscopoInterno = (itemTabela != NULL);
    if (temEscopoInterno){
        id->global = true;
        if (idFuncao){
            if(itemTabela->funcao){
                if(itemTabela->type){
                    id->type = new TypeNodo(itemTabela->type->tipo, linha);
                }
            }else{
                delete item;
                item = new ItemEscopo(var, 0);
                itemTabela = (ItemEscopo*)this->tabelaEscopo->buscar(item);
                if(itemTabela == NULL || !itemTabela->funcao)erroSemantico(ERRO_ID_NAO_FUNCAO, NULL, 0, linha);
            }
        }else{
            if(itemTabela->funcao){
                erroSemantico(ERRO_ID_INADEQUADO, NULL, 0, linha);
            }else{
                if(this->idTipo){
                    if(itemTabela->tipo){ /** SE DER ERRO VOLTAR AKI */
                        erroSemantico(ERRO_ID_ESPERADO, NULL, 0, linha);
                    }
                }else{
                    if(itemTabela->tipo){
                        erroSemantico(ERRO_ID_INADEQUADO, NULL, 0, linha);
                    }else{
                        if(itemTabela->type){
                            if(itemTabela->type->tipo == ID){
                                IdNodo *idTipo = new IdNodo(((TypeIdNodo*)itemTabela->type)->id->nome, linha);
                                id->type = new TypeIdNodo(idTipo, linha);
                            }else{
                                id->type = new TypeNodo(itemTabela->type->tipo, linha);
                            }
                            if(itemTabela->tamanhoArray){
                                int tipo = itemTabela->tamanhoArray->tipo;
                                char *valor = itemTabela->tamanhoArray->valor;
                                id->array = new NumNodo(tipo, valor, linha);
                            }
                            id->global = true;
                        }
                    }
                }
            }
        }
    }else{
        id->global = false;
        delete item;
        item = new ItemEscopo(var, 0);
        itemTabela = (ItemEscopo*)this->tabelaEscopo->buscar(item);
        if(itemTabela == NULL){
            if(this->idTipo){
                ItemTipo *itemT = new ItemTipo(var, NULL);
                if(!this->tabelaTipos->buscar(itemT)) //erro
                delete itemT;
            }else erroSemantico(ERRO_CONSTRUCAO_CARACTER, NULL, 0, linha); //erro modificar o erro
        }else{
            if(this->idFuncao){
                if(itemTabela->funcao){
                    if(itemTabela->type){
                        id->type = new TypeNodo(itemTabela->type->tipo, linha);
                    }
                }else{
                    erroSemantico(ERRO_ID_NAO_FUNCAO, NULL, 0, linha);
                }
            }else{
                if(itemTabela->funcao){
                    erroSemantico(ERRO_ID_INADEQUADO, NULL, 0, linha);
                }else{
                    if(!this->idTipo){
                        if(itemTabela->tipo){
                            erroSemantico(ERRO_ID_ESPERADO, NULL, 0, linha);
                        }else{
                            if (itemTabela->tipo) {
                                erroSemantico(ERRO_ID_INADEQUADO, NULL, 0, linha);
                            } else {
                                if(itemTabela->type){
                                    if(itemTabela->type->tipo == ID){
                                        IdNodo *idTipo = new IdNodo(((TypeIdNodo*)itemTabela->type)->id->nome, linha);
                                        id->type = new TypeIdNodo(idTipo, linha);
                                    }else{
                                        id->type = new TypeNodo(itemTabela->type->tipo, linha);
                                    }
                                }
                                if(itemTabela->tamanhoArray){
                                    int tipo = itemTabela->tamanhoArray->tipo;
                                    char *valor = itemTabela->tamanhoArray->valor;
                                    id->array = new NumNodo(tipo, valor, linha);
                                }
                                id->global = false;
                            }
                        }
                    }
                }
            }
        }
    }
    delete item;

    if(this->idFuncao) this->idFuncao = false;

    Visitor::visita(id);
}

void VisitorSemantico::visita(NumNodo *num){
    Visitor::visita(num);
    if(num->tipo == NUMINT){
        num->type = new TypeNodo(INT, num->linha);
    }else if(num->tipo == NUMREAL){
        num->type = new TypeNodo(FLOAT, num->linha);
    }
}

void VisitorSemantico::visita(LiteralNodo *literal){
    Visitor::visita(literal);
    literal->type = new TypeNodo(LITERAL, literal->linha);
}

void VisitorSemantico::visita(CharNodo *Char){
    Visitor::visita(Char);
    Char->type = new TypeNodo(CHAR, Char->linha);
}

void VisitorSemantico::visita(BooleanNodo *boolean){
    Visitor::visita(boolean);
    boolean->type = new TypeNodo(BOOLEAN, boolean->linha);
}

void VisitorSemantico::visita(ExprListNodo *exprList){
    if(exprList->expr){
        exprList->expr->aceita(this);
        if(this->verificaparametros){
            if(this->parametrosFuncao == NULL){
                if(this->mParametrosFuncao){
                    erroSemantico(ERRO_CALL_PARAMETROS, NULL, 0, exprList->linha);
                }
            }else{
                if((exprList->expr->type != NULL) && (this->parametrosFuncao->tipo != NULL) && (exprList->expr->type->tipo != this->parametrosFuncao->tipo->tipo)){
                    //erro incompatibilidade de tipos
                }
                if((this->parametrosFuncao->idList != NULL) && (this->parametrosFuncao->idList->array != NULL)){
                    /** Parametro é array*/
                    if(((exprList->expr->array != NULL) && (strcmp(exprList->expr->array->valor, this->parametrosFuncao->idList->array->valor))) || (exprList->expr->array == NULL)){
                        //erro parametro nao e arranjo
                    }
                }else{
                    /** Parametro não é array*/
                    if(exprList->expr->array != NULL){
                        //erro
                    }
                }
                this->parametrosFuncao = this->parametrosFuncao->varDecl;
            }
        }
    }
    if(exprList->exprList) exprList->exprList->aceita(this);
}

void VisitorSemantico::visita(CallNodo *call){
    this->verificaparametros = true;
    VarDeclNodo *paramFunc = this->parametrosFuncao;
    bool mParamFunc = this->mParametrosFuncao;
    if(call->id != NULL){
        if(typeid(*call->id) == typeid(IdNodo)){
            this->idFuncao = true;

            ItemEscopo *item = new ItemEscopo(call->id->nome, 0);
            ItemTabela *itemTabela;
            if((itemTabela = this->tabelaEscopo->buscar(item))){
                this->parametrosFuncao = ((ItemEscopo*)itemTabela)->formalList;
                this->mParametrosFuncao = true;
            }
            delete item;
        }else{
            erroSemantico(ERRO_CALL, NULL, 0, call->linha);
        }
    }
    Visitor::visita(call);
    if(this->parametrosFuncao){
        erroSemantico(ERRO_CALL_PARAMETROS, NULL, 0, call->linha);
    }

    this->parametrosFuncao = paramFunc;
    this->mParametrosFuncao = mParamFunc;
    this->verificaparametros = false;
}

void VisitorSemantico::visita(PointerValueExprNodo *pointerValueExpr){
    if(pointerValueExpr->id){
        pointerValueExpr->id->aceita(this);
        if(pointerValueExpr->id){
            ItemTipo *item = new ItemTipo(pointerValueExpr->id->nome, NULL);
            ItemTipo *itemTabela = (ItemTipo*)this->tabelaTipos->buscar(item);
            if(itemTabela){
                TabelaSemantico *tabelaTemp = this->tabelaEscopo;
                this->tabelaEscopo = itemTabela->tabelaEscopo;
                if(pointerValueExpr->expr) pointerValueExpr->expr->aceita(this);
                this->tabelaEscopo = tabelaTemp;
            }
            delete item;
        }else{
            erroSemantico(ERRO_ATRIBUTO, NULL, 0, pointerValueExpr->linha);
        }
    }
    if(pointerValueExpr->expr->type) pointerValueExpr->type = pointerValueExpr->expr->type;
}

void VisitorSemantico::visita(NameExprNodo *nameExpr){
    if(nameExpr->id){
        nameExpr->id->aceita(this);
        if(nameExpr->id){
            ItemTipo *item = new ItemTipo(nameExpr->id->nome, NULL);
            ItemTipo *itemTabela = (ItemTipo*)this->tabelaTipos->buscar(item);
            if(itemTabela){
                TabelaSemantico *tabelaTemp = this->tabelaEscopo;
                this->tabelaEscopo = itemTabela->tabelaEscopo;
                if(nameExpr->expr) nameExpr->expr->aceita(this);
                this->tabelaEscopo = tabelaTemp;
            }
            delete item;
        }else{
            erroSemantico(ERRO_ATRIBUTO, NULL, 0, nameExpr->linha);
        }
    }
    if(nameExpr->expr->type) nameExpr->type = nameExpr->expr->type;
}

void VisitorSemantico::visita(AddressValueNodo *addresValue){
    Visitor::visita(addresValue);
    if(addresValue->expr && addresValue->expr->type){
        addresValue->type = addresValue->expr->type;
    }
}

void VisitorSemantico::visita(PointerValueNodo *pointerValue){
    Visitor::visita(pointerValue);
    if(pointerValue->expr && pointerValue->expr->type){
        pointerValue->type = pointerValue->expr->type;
    }
}

void VisitorSemantico::visita(ArrayNodo *array){
    Visitor::visita(array);
    if((array->expr != NULL) && (array->expr->array == NULL)){
        erroSemantico(ERRO_EXPR_ARRANJO, NULL, 0, array->linha);
    }
    if((array->indice != NULL) && (array->indice->type != NULL)){
        if(array->indice->type->tipo != INT){
            erroSemantico(ERRO_INDICE_ARRANJO, NULL, 0, array->linha);
        }else{
            if((array->expr != NULL) && (array->expr->array != NULL)){
                NumNodo *indice = dynamic_cast<NumNodo*> (array->indice);
                if(indice){
                    int indiceInteiro = atoi(indice->valor);
                    if((indiceInteiro < 0) || (indiceInteiro >= atoi(array->expr->array->valor))) erroSemantico(ERRO_LIMITE_ARRANJO, NULL, 0, array->linha);
                }else{
                    SignNodo *indiceN = dynamic_cast<SignNodo*>(array->indice);
                    if(indiceN && indiceN->operador == SUBTRACAO) erroSemantico(ERRO_LIMITE_ARRANJO, NULL, 0, array->linha);
                    //VERIFICAR COMO TESTAR INDICE NEGATIVO
                }
            }
        }
    }
    if((array->expr != NULL) && (array->expr->type != NULL)){
        array->type = new TypeNodo(array->expr->type->tipo, array->linha);
    }
}

void VisitorSemantico::visita(RelationalOpNodo *relationalOp){
    Visitor::visita(relationalOp);
}

void VisitorSemantico::visita(MenorNodo *menor){
    Visitor::visita(menor);
    if(verificaOpBin(menor, true)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"MENOR", 0, menor->linha);
}

void VisitorSemantico::visita(MenorIgualNodo *menorIgual){
    Visitor::visita(menorIgual);
    if(verificaOpBin(menorIgual, true)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"MENOR OU IGUAL", 0, menorIgual->linha);
}

void VisitorSemantico::visita(MaiorNodo *maior){
    Visitor::visita(maior);
    if(verificaOpBin(maior, true)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"MAIOR", 0, maior->linha);
}

void VisitorSemantico::visita(MaiorIgualNodo *maiorIgual){
    Visitor::visita(maiorIgual);
    if(verificaOpBin(maiorIgual, true)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"MAIOR OU IGUAL", 0, maiorIgual->linha);
}

void VisitorSemantico::visita(IgualNodo *igual){
    Visitor::visita(igual);
    if(verificaOpBin(igual, true) && verificarLogOp(igual)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"IGUALDADE", 0, igual->linha);
}

void VisitorSemantico::visita(DiferenteNodo *diferente){
    Visitor::visita(diferente);
    if(verificaOpBin(diferente, true) && verificarLogOp(diferente)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"DIFERENCA", 0, diferente->linha);
}

void VisitorSemantico::visita(ELogicoNodo *eLogico){
    Visitor::visita(eLogico);
    if(verificarLogOp(eLogico)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"E", 0, eLogico->linha);
}

void VisitorSemantico::visita(OuLogicoNodo *ouLogico){
    Visitor::visita(ouLogico);
    if(verificarLogOp(ouLogico)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"OU", 0, ouLogico->linha);
}

void VisitorSemantico::visita(BinarioOpNodo *binOp){
    Visitor::visita(binOp);
}

void VisitorSemantico::visita(AdditionOpNodo *additionOp){
    Visitor::visita(additionOp);
    if(verificaOpBin(additionOp, false)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"SOMA", 0, additionOp->linha);
}

void VisitorSemantico::visita(SubtractionOpNodo *subtractionOp){
    Visitor::visita(subtractionOp);
    if(verificaOpBin(subtractionOp, false)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"SUBTRACAO", 0, subtractionOp->linha);
}

void VisitorSemantico::visita(MultiplicationOpNodo *multiplicationOp){
    Visitor::visita(multiplicationOp);
    if(verificaOpBin(multiplicationOp, false)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"MULTIPLICACAO", 0, multiplicationOp->linha);
}

void VisitorSemantico::visita(DivisionOpNodo *divisionOp){
    Visitor::visita(divisionOp);
    if(verificaOpBin(divisionOp, false)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"DIVISAO", 0, divisionOp->linha);
}

void VisitorSemantico::visita(ModuleOpNodo *moduleOp){
    Visitor::visita(moduleOp);
    if(verificaOpBin(moduleOp, false)) erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"MODULO", 0, moduleOp->linha);
}

void VisitorSemantico::visita(BooleanOpNodo *booleanOp){
    Visitor::visita(booleanOp);
    booleanOp->type = new TypeNodo(BOOLEAN, booleanOp->linha);
}

void VisitorSemantico::visita(BitWiseOpNodo *bitWiseOp){
    Visitor::visita(bitWiseOp);
}

void VisitorSemantico::visita(NotNodo *Not){
    Visitor::visita(Not);
    if(!Not->expr || Not->expr->type == NULL) return;

    if(Not->expr->array){
        erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"NEGACAO", 0, Not->linha);
    }

    if(Not->expr->type->tipo == BOOLEAN) Not->type = new TypeNodo(BOOLEAN, Not->linha);
    else erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"NEGACAO", 0, Not->linha);
}

void VisitorSemantico::visita(SignNodo *sign){
    Visitor::visita(sign);
    if(!sign->expr || sign->expr->type == NULL) return;

    if(sign->expr->array){
        erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"SINAL", 0, sign->linha);
    }

    if((sign->expr->type->tipo == INT) || (sign->expr->type->tipo == FLOAT)) sign->operador = SUBTRACAO;
    else erroSemantico(ERRO_INCOMPATIBILIDADE, (char*)"SINAL", 0, sign->linha);
}

void VisitorSemantico::visita(AssignNodo *assign){
    Visitor::visita(assign);
    if(!assign->expr1 || !assign->expr2 || assign->expr1->type == NULL || assign->expr2->type == NULL) return;
    if(assign->expr1->type->tipo == assign->expr2->type->tipo){
        assign->type = new TypeNodo(assign->expr1->type->tipo, assign->linha);
        if(((typeid(*assign->expr1) != typeid(IdNodo)) && (typeid(*assign->expr1) != typeid(NameExprNodo)) && (typeid(*assign->expr1) != typeid(ArrayNodo))) || (assign->expr1->array != NULL)){
            erroSemantico(ERRO_LVALUE, NULL, 0, assign->linha);
        }
    }else erroSemantico(ERRO_INCOMP_IGUALDADE, NULL, 0, assign->linha);
}

void VisitorSemantico::visita(CaseBlockNodo *caseblock){
    Visitor::visita(caseblock);
    if(!caseblock->num || caseblock->num->type) return;
    if(caseblock->num->type->tipo != INT) erroSemantico(ERRO_CONDICAO_CASEBLOCK, NULL, 0, caseblock->linha);
}
