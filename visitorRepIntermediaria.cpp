#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <typeinfo>

#include "visitorRepIntermediaria.h"
#include "representacaoIntermediaria.h"

/**************************************************************
            VISITOR DA REPRESENTAÇÃO INTERMEDIÁRIA
**************************************************************/

VisitorRepInt::VisitorRepInt(TabelaSemantico *tabela) {
    this->tabela            = tabela;
    this->naFuncao          = false;
    this->naListaParametros = false;
    this->naListaTipos      = false;
    this->qtdLiterais       = 0;
    this->caseBlockValue    = NULL;
    this->argumentosCall    = NULL;
    this->listaFinaisLoop   = NULL;
    this->switchExprs       = NULL;
    this->primeiroFragmento = NULL;
}

VisitorRepInt::~VisitorRepInt() {
    delete primeiroFragmento;
}

int VisitorRepInt::tamanhoTipo(TypeNodo *tipo){
    if (typeid(tipo) == typeid(TypeIdNodo)){
        ItemTipo *itTipo = new ItemTipo(((TypeIdNodo*)tipo)->id->nome, NULL);
        int tamanhoTipo = ((ItemTipo*)tabela->buscar(itTipo))->tamanho;
        delete itTipo;
        return tamanhoTipo;
    } else return 4;
}

int VisitorRepInt::calculaOffset(TypeNodo *tipo, char *nomeProp){
    int tamanho = 0;
    ItemTipo *itTipo = new ItemTipo(((TypeIdNodo*)tipo)->id->nome, NULL);
    VarDeclNodo *vd = ((ItemTipo*)tabela->buscar(itTipo))->variaveis;
    while (vd) {
        IdListNodo *idList = vd->idList;
        while(idList){
            int tamTipo = tamanhoTipo(vd->tipo);
            if (strcmp(nomeProp, idList->id->nome)) {
                delete itTipo;
                return tamanho;
            }
            tamanho += (idList->array == NULL) ? tamTipo : atoi(idList->array->valor) * tamTipo;
            idList = idList->idList;
        }
        vd = vd->varDecl;
    }
    return 0;
}

void VisitorRepInt::adicionaFragmento(Fragment *fragmento) {
    if (primeiroFragmento) primeiroFragmento->adicionaNoFim(fragmento);
    else primeiroFragmento = fragmento;
}

void VisitorRepInt::adicionaArgumentoCall(ExprRI *exp) {
    if (argumentosCall->primeiro == NULL) {
        argumentosCall->primeiro = new ExprRIList(exp, NULL);
    } else {
        ExprRIList *temporario = argumentosCall->primeiro;
        while (temporario->proximo) temporario = temporario->proximo;
        temporario->proximo = new ExprRIList(exp, NULL);
    }
}

VariavelNaoLocal* VisitorRepInt::novaVariavel(TypeNodo *tipo, NumNodo *num){
    if(tipo->tipo == INT || tipo->tipo == FLOAT){
        return new VariavelNaoLocal(tipo->tipo, (num == NULL) ? 1 : atoi(num->valor), NULL);
    }else if(tipo->tipo == BOOLEAN){
        return new VariavelNaoLocal(INT, (num == NULL) ? 1 : atoi(num->valor), NULL);
    }else if(tipo->tipo == CHAR){
        return new VariavelNaoLocal(CHAR, (num == NULL) ? 1 : atoi(num->valor), NULL);
    }else{
        ItemTipo *item = new ItemTipo(((TypeIdNodo*)tipo)->id->nome, NULL);
        ItemTipo *itemTipo = (ItemTipo*)tabela->buscar(item);
        VarDeclNodo *vd = itemTipo->variaveis;

        VariavelNaoLocal *vnl = NULL;

        if(num == NULL){
            while(vd){
                IdListNodo *idList = vd->idList;
                while(idList){
                    if(vnl){
                        vnl->adicionaNoFim(novaVariavel(vd->tipo, idList->array));
                    }else{
                        vnl = novaVariavel(vd->tipo, idList->array);
                    }
                    idList = idList->idList;
                }
                vd = vd->varDecl;
            }
        }else{
            for(int i = 0; i < atoi(num->valor); i++){
                while(vd){
                    IdListNodo *idList = vd->idList;
                    while(idList){
                        if(vnl){
                            vnl->adicionaNoFim(novaVariavel(vd->tipo, idList->array));
                        }else{
                            vnl = novaVariavel(vd->tipo, idList->array);
                        }
                        idList = idList->idList;
                    }
                    vd = vd->varDecl;
                }
            }
        }
        delete item;
        return vnl;
    }
}

StmtRI* VisitorRepInt::constroiArvoreImpressao(ExprRIList *expList) {
    if (expList->proximo) return new SeqRI(new ExprStmtRI(new CallRI(new Label((char*)"print_"), new ExprRIList(expList->primeiro, NULL))), constroiArvoreImpressao(expList->proximo));
    else return new ExprStmtRI(new CallRI(new Label((char*)"print_"), new ExprRIList(expList->primeiro, NULL)));
}

/** retorna o ultimo fragmento da lista */
Fragment* VisitorRepInt::ultimoFragmento() {
    Fragment *fragmento = primeiroFragmento;
    while (fragmento->proximo) fragmento = fragmento->proximo;
    return fragmento;
}

/*****************************************
            MÉTODOS VISITA
******************************************/
void VisitorRepInt::visita(TypeDeclProgramNodo *tdpn){
    tdpn->typeDecl->aceitaRI(this);
    tdpn->program->aceitaRI(this);
}

void VisitorRepInt::visita(FunctionDeclNodo *fd){
    contadorParametros = 0;
    FrameMIPS *frame_mips = new FrameMIPS();
    if(strcmp(fd->id->nome, (char*)"main")) frame_mips->rotulo = new Label((char*)"func_", fd->id->nome);
    else frame_mips->rotulo = new Label(fd->id->nome);

    Procedure *p = new Procedure(frame_mips, NULL);
    adicionaFragmento(p);

    this->naFuncao = true;
    this->naListaParametros = true;

    if(fd->varListParametros) fd->varListParametros->aceitaRI(this);
    this->naListaParametros = false;
    if(fd->varListVarLocais) fd->varListVarLocais->aceitaRI(this);

    StmtRI *corpo = fd->stmtList->aceitaRI(this);
    if(strcmp(fd->id->nome, (char*)"main")){
        ReturnNodo *ret = new ReturnNodo(new BooleanNodo(FALSE, fd->linha), fd->linha);
        p->body = new SeqRI(corpo, ret->aceitaRI(this));
    }else{
        p->body = corpo;
    }

    frame_mips->contadorParametros = contadorParametros;
    frame_mips->tamanhoFrame += contadorParametros * 4;
}

void VisitorRepInt::visita(FunctionDeclProgramNodo *fdp){
    this->naListaTipos = false;
    this->naFuncao = true;
    if(fdp->functionDecl)   fdp->functionDecl->aceitaRI(this);
    this->naListaTipos = false;
    if(fdp->program)        fdp->program->aceitaRI(this);
}

void VisitorRepInt::visita(VarDeclProgramNodo *vdpn){
    if(vdpn->varDecl) vdpn->varDecl->aceitaRI(this);
    if(vdpn->program) vdpn->program->aceitaRI(this);
}

void VisitorRepInt::visita(TypeNodo *tn){}

void VisitorRepInt::visita(TypeIdNodo *tin){}

void VisitorRepInt::visita(TypeDeclNodo *tdn){
    naListaTipos = true;
    if(tdn->varDecl) tdn->varDecl->aceitaRI(this);
    naListaTipos = false;

    VarDeclNodo *temp = tdn->varDecl;
    ItemTipo *item;
    int tamanho = 0;

    while(temp){
        IdListNodo *idList = temp->idList;
        while(idList){
            int tamTipo = tamanhoTipo(temp->tipo);
            tamanho += (idList->array == NULL) ? tamTipo : atoi(idList->array->valor) * tamTipo;
            idList = idList->idList;
        }
        temp = temp->varDecl;
    }
    item = new ItemTipo(tdn->id->nome, NULL);
    ItemTipo *itemAux = (ItemTipo*)tabela->buscar(item);
    itemAux->tamanho = tamanho;
    delete item;
    if(tdn->typeDecl) tdn->typeDecl->aceitaRI(this);
}

void VisitorRepInt::visita(VarDeclNodo *vdn){
    int tamTipo = tamanhoTipo(vdn->tipo);
    IdListNodo *idList = vdn->idList;
    while(idList){
        if(!naListaTipos){
            if(naFuncao){
                if(naListaParametros){
                    ((Procedure*) ultimoFragmento())->frame->adicionaParametro(true, 4, idList->id->nome, idList->array != NULL);
                }else{
                    ((Procedure*) ultimoFragmento())->frame->adicionaLocal(true, ((idList->array == NULL) ? 1 : atoi(idList->array->valor)) * tamTipo, idList->id->nome);
                }
            }else{
                VariavelNaoLocal *vnl = novaVariavel(vdn->tipo, idList->array);
                vnl->nome = new Label((char*)"glob_", idList->id->nome);
                adicionaFragmento(vnl);
            }
        }
        idList = idList->idList;
    }
    if(vdn->varDecl) vdn->varDecl->aceitaRI(this);
}

MemRI *VisitorRepInt::visita(IdNodo *id){
    if(!id->global){
        return new MemRI(new NameRI(new Label((char*)"glob_", id->nome)));
    }else{
        return (MemRI*)((Procedure*)ultimoFragmento())->frame->buscaVariavel(id->nome)->codigoAcesso();
    }
}

StmtRI *VisitorRepInt::visita(StatementListNodo *sl) {
    if (sl->stmtList) return new SeqRI(sl->stmt->aceitaRI(this), sl->stmtList->aceitaRI(this));
    else return sl->stmt->aceitaRI(this);
}

/**VERIFICAR ESSES DOIS METODOS*/
StmtRI *VisitorRepInt::visita(StatementListTypeNodo *slt){
    if(slt->typeDecl) slt->typeDecl->aceitaRI(this);
    if(slt->stmtList) return slt->stmtList->aceitaRI(this);
    else return NULL;
}

StmtRI *VisitorRepInt::visita(StatementListVarNodo *slv){
    StmtRI *s1 = NULL;
    StmtRI *s2 = NULL;

    if(slv->stmtList2)      s1 = slv->stmtList2->aceitaRI(this);
    if(slv->varDecl)        slv->varDecl->aceitaRI(this);
    if(slv->stmtListLinha)  s2 = slv->stmtListLinha->aceitaRI(this);
    return new SeqRI(s1, s2);
}

StmtRI* VisitorRepInt::visita(IfNodo *i){
    if(i->stmt2){
        Label *labelThen = new Label();
        Label *labelElse = new Label();
        Label *labelFim  = new Label();
        return new SeqRI(new CJumpRI(NE, i->expr->aceitaRI(this), new ConstRI(0), labelThen, labelElse), new SeqRI(new LabelRI(labelThen), new SeqRI(new SeqRI(i->stmt1->aceitaRI(this), new JumpRI(new NameRI(labelFim))), new SeqRI(new LabelRI(labelElse), new SeqRI(i->stmt2->aceitaRI(this), new LabelRI(labelFim))))));
    }else{
        Label *labelThen = new Label();
        Label *labelFim  = new Label();
        return new SeqRI(new CJumpRI(NE,
                                     i->expr->aceitaRI(this),
                                     new ConstRI(0),
                                     labelThen,
                                     labelFim),
                         new SeqRI(new LabelRI(labelThen),
                                   new SeqRI(i->stmt1->aceitaRI(this),
                                             new LabelRI(labelFim))));
    }
}

StmtRI* VisitorRepInt::visita(WhileNodo *w) {
    Label *labelExpr    = new Label();
    Label *labelInicio  = new Label();
    Label *labelFim     = new Label();

    if (w->stmt) return new SeqRI(new LabelRI(labelExpr), new SeqRI(new CJumpRI(NE, w->expr->aceitaRI(this), new ConstRI(0), labelInicio, labelFim), new SeqRI(new LabelRI(labelInicio), new SeqRI(w->stmt->aceitaRI(this), new SeqRI(new JumpRI(new NameRI(labelExpr)), new LabelRI(labelFim))))));
    else return new SeqRI(new LabelRI(labelExpr), new SeqRI(new CJumpRI(NE, w->expr->aceitaRI(this), new ConstRI(0), labelExpr, labelFim), new LabelRI(labelFim)));
}

StmtRI* VisitorRepInt::visita(SwitchNodo *s) {
    ExprRI *exp = s->expr->aceitaRI(this);

    if (s->caseBlock) {
        switchExprs = new TempList(new Temp(), switchExprs);
        listaFinaisLoop = new LabelList(new Label(), listaFinaisLoop);
        caseBlockValue = new TempList(new Temp(), caseBlockValue);

        StmtRI *stmtRetorno = new SeqRI(new SeqRI(new MoveRI(new TempRI(switchExprs->primeiro),exp),new SeqRI(new MoveRI(new TempRI(caseBlockValue->primeiro), new ConstRI(0)), s->caseBlock->aceitaRI(this))), new LabelRI(listaFinaisLoop->primeiro));

        caseBlockValue = caseBlockValue->proximo;
        listaFinaisLoop = listaFinaisLoop->proximo;
        switchExprs = switchExprs->proximo;

        return stmtRetorno;
    } else return new ExprStmtRI(exp);
}

StmtRI* VisitorRepInt::visita(BreakNodo *b) {
    return new JumpRI(new NameRI(listaFinaisLoop->primeiro));
}

StmtRI* VisitorRepInt::visita(PrintlnNodo *p) {
    StmtRI *stmt;
    argumentosCall = new ExprRIListList(NULL, argumentosCall);
    stmt = new SeqRI(p->exprList->aceitaRI(this), constroiArvoreImpressao(argumentosCall->primeiro));
    argumentosCall = argumentosCall->proximo;

    return stmt;
}

StmtRI* VisitorRepInt::visita(ReadNodo *r) {
    ExprRI *exp = r->expr->aceitaRI(this);
    if (typeid(*exp) == typeid(MemRI)) {
        MemRI *end = (MemRI*)exp;
        return new ExprStmtRI(new CallRI(new Label((char*)"readln_"), new ExprRIList(end->exp, NULL)));
    } else return new ExprStmtRI(new ConstRI(0));
}

StmtRI *VisitorRepInt::visita(ReturnNodo *r) {
    Temp *t = new Temp();
    return new SeqRI(
            new MoveRI(new TempRI(new Temp((char*)"$v0")), r->expr->aceitaRI(this)),
            new SeqRI(
            new MoveRI(new TempRI(new Temp((char*)"$ra")), new MemRI(new BinOpRI(PLUS, new TempRI(new Temp((char*)"$fp")), new ConstRI(-8)))),
            new SeqRI(
            new MoveRI(new TempRI(t), new MemRI(new BinOpRI(PLUS, new TempRI(new Temp((char*)"$fp")), new ConstRI(-4)))),
            new SeqRI(
            new MoveRI(new TempRI(new Temp((char*)"$sp")), new TempRI(new Temp((char*)"$fp"))),
            new SeqRI(
            new MoveRI(new TempRI(new Temp((char*)"$fp")), new TempRI(t)),
            new JumpRI(new TempRI(new Temp((char*)"$ra"))))))));
}

StmtRI *VisitorRepInt::visita(ThrowNodo *t) {
    return new LabelRI(new Label((char*)"Throw"));
}

StmtRI *VisitorRepInt::visita(TryNodo *t) {
    return new SeqRI(t->stmt1->aceitaRI(this), t->stmt2->aceitaRI(this));
}

StmtRI *VisitorRepInt::visita(ExprStatementNodo *es){
    return new ExprStmtRI(es->expr->aceitaRI(this));
}

/** /////////////////// VERIFICAR AKI ////////////////// */
//ExprRI *VisitorRepInt::visita(ExprNodo *e){
//
//}

ExprRI *VisitorRepInt::visita(AssignNodo *a) {
    TempRI *t = new TempRI(new Temp());
    return new ESeqRI(new SeqRI(new MoveRI(t, a->expr2->aceitaRI(this)), new MoveRI(a->expr1->aceitaRI(this), t)), t);
}

ExprRI* VisitorRepInt::visita(OuLogicoNodo *oulog) {
    TempRI *temp = new TempRI(new Temp());
    Label *label1 = new Label();
    Label *label2 = new Label();
    Label *label3 = new Label();

    return new ESeqRI(new SeqRI(new MoveRI(temp, new ConstRI(1)), new SeqRI(new CJumpRI(NE, oulog->expr1->aceitaRI(this), new ConstRI(1), label1, label2), new SeqRI(new LabelRI(label1), new SeqRI(new CJumpRI(NE, oulog->expr2->aceitaRI(this), new ConstRI(1), label3, label2), new SeqRI(new LabelRI(label3), new SeqRI(new MoveRI(temp, new ConstRI(0)), new LabelRI(label2))))))), temp);
}

ExprRI* VisitorRepInt::visita(ELogicoNodo *elog) {
    TempRI *temp = new TempRI(new Temp());
    Label *label1 = new Label();
    Label *label2 = new Label();
    Label *label3 = new Label();

    return new ESeqRI(new SeqRI(new MoveRI(temp, new ConstRI(0)), new SeqRI(new CJumpRI(NE, elog->expr1->aceitaRI(this), new ConstRI(0), label1, label2), new SeqRI(new LabelRI(label1),new SeqRI(new CJumpRI(NE, elog->expr2->aceitaRI(this),  new ConstRI(0), label3, label2), new SeqRI(new LabelRI(label3), new SeqRI(new MoveRI(temp, new ConstRI(1)), new LabelRI(label2))))))), temp);
}

ExprRI* VisitorRepInt::visita(IgualNodo *igual){
    TempRI *temp = new TempRI(new Temp());
    Label  *label1 = new Label();
    Label  *label2 = new Label();

    return new ESeqRI(new SeqRI(new MoveRI(temp, new ConstRI(1)), new SeqRI(new CJumpRI(EQ, igual->expr1->aceitaRI(this), igual->expr2->aceitaRI(this), label1, label2), new SeqRI(new SeqRI(new LabelRI(label2), new MoveRI(temp, new ConstRI(0))), new LabelRI(label1)))), temp);
}

ExprRI* VisitorRepInt::visita(DiferenteNodo *dif){
    TempRI *temp = new TempRI(new Temp());
    Label  *label1 = new Label();
    Label  *label2 = new Label();

    return new ESeqRI(new SeqRI(new MoveRI(temp, new ConstRI(1)), new SeqRI(new CJumpRI(NE, dif->expr1->aceitaRI(this), dif->expr2->aceitaRI(this), label1, label2), new SeqRI(new SeqRI(new LabelRI(label2), new MoveRI(temp, new ConstRI(0))), new LabelRI(label1)))), temp);
}

ExprRI* VisitorRepInt::visita(RelationalOpNodo *rel){
/**VERIFICAR*/
    return NULL;
}

ExprRI* VisitorRepInt::visita(MenorNodo *mn){
    TempRI* temp = new TempRI(new Temp());
    Label* label1 = new Label();
    Label* label2 = new Label();

    return new ESeqRI(new SeqRI(new MoveRI(temp, new ConstRI(1)), new SeqRI(new CJumpRI(LT, mn->expr1->aceitaRI(this), mn->expr2->aceitaRI(this), label1, label2), new SeqRI(new SeqRI(new LabelRI(label2), new MoveRI(temp, new ConstRI(0))), new LabelRI(label1)))), temp);

}

ExprRI* VisitorRepInt::visita(MenorIgualNodo *min){
    TempRI* temp = new TempRI(new Temp());
    Label* label1 = new Label();
    Label* label2 = new Label();

    return new ESeqRI(new SeqRI(new MoveRI(temp, new ConstRI(1)), new SeqRI(new CJumpRI(LE, min->expr1->aceitaRI(this), min->expr2->aceitaRI(this), label1, label2), new SeqRI(new SeqRI(new LabelRI(label2), new MoveRI(temp, new ConstRI(0))), new LabelRI(label1)))), temp);

}

ExprRI* VisitorRepInt::visita(MaiorNodo *ma){
    TempRI* temp = new TempRI(new Temp());
    Label* label1 = new Label();
    Label* label2 = new Label();

    return new ESeqRI(new SeqRI(new MoveRI(temp, new ConstRI(1)), new SeqRI(new CJumpRI(GT, ma->expr1->aceitaRI(this), ma->expr2->aceitaRI(this), label1, label2), new SeqRI(new SeqRI(new LabelRI(label2), new MoveRI(temp, new ConstRI(0))), new LabelRI(label1)))), temp);
}

ExprRI* VisitorRepInt::visita(MaiorIgualNodo *mai){
    TempRI* temp = new TempRI(new Temp());
    Label* label1 = new Label();
    Label* label2 = new Label();

    return new ESeqRI(new SeqRI(new MoveRI(temp, new ConstRI(1)), new SeqRI(new CJumpRI(GE, mai->expr1->aceitaRI(this), mai->expr2->aceitaRI(this), label1, label2), new SeqRI(new SeqRI(new LabelRI(label2), new MoveRI(temp, new ConstRI(0))), new LabelRI(label1)))), temp);
}

ExprRI* VisitorRepInt::visita(BinarioOpNodo *bin){
/**VERIFICAR*/
    return NULL;
}

ExprRI* VisitorRepInt::visita(AdditionOpNodo *aOp) {
    return new BinOpRI(PLUS, aOp->expr1->aceitaRI(this), aOp->expr2->aceitaRI(this));
}

ExprRI* VisitorRepInt::visita(SubtractionOpNodo *sOp) {
    return new BinOpRI(MINUS, sOp->expr1->aceitaRI(this), sOp->expr2->aceitaRI(this));
}

ExprRI* VisitorRepInt::visita(BitWiseOpNodo *bitOp){
    int operador = bitOp->op == BIT_OU ? OR : AND;
    return new BinOpRI(operador, bitOp->expr1->aceitaRI(this), bitOp->expr2->aceitaRI(this));
}

ExprRI* VisitorRepInt::visita(MultiplicationOpNodo *mOp){
    return new BinOpRI(MUL, mOp->expr1->aceitaRI(this), mOp->expr2->aceitaRI(this));
}

ExprRI* VisitorRepInt::visita(DivisionOpNodo *dOp){
    return new BinOpRI(DIVR, dOp->expr1->aceitaRI(this), dOp->expr2->aceitaRI(this));
}

ExprRI* VisitorRepInt::visita(ModuleOpNodo *mod){
    TempRI* temp = new TempRI(new Temp());
    return new ESeqRI(new SeqRI(new MoveRI(temp, mod->expr1->aceitaRI(this)), new MoveRI(temp, new BinOpRI(MINUS, temp, new BinOpRI(DIVR, temp, mod->expr2->aceitaRI(this))))), temp);
}

ExprRI* VisitorRepInt::visita(NotNodo *n){
    return new BinOpRI(PLUS, new ConstRI(1), new BinOpRI(MINUS, new ConstRI(0), n->expr->aceitaRI(this)));
}

ExprRI* VisitorRepInt::visita(SignNodo *sig){
    if(sig->operador == ADICAO){
        return sig->expr->aceitaRI(this);
    }else{
        return new BinOpRI(MUL, new ConstRI(-1), sig->expr->aceitaRI(this));
    }
}

ExprRI* VisitorRepInt::visita(NumNodo *n){
    if(n->tipo == NUMINT){
        return new ConstRI(atoi(n->valor));
    }else{
        return new ConstfRI(atof(n->valor));
    }
}

ExprRI* VisitorRepInt::visita(CharNodo *ch){
/**VERIFICAR*/
    return NULL;
}

ExprRI* VisitorRepInt::visita(BooleanNodo *b){
    if (b->boolean == TRUE) return new ConstRI(1);
    else return new ConstRI(0);
}

ExprRI* VisitorRepInt::visita(PointerValueExprNodo *p){
/**VERIFICAR*/
    return NULL;
}

ExprRI* VisitorRepInt::visita(NameExprNodo *n){
/**VERIFICAR*/
    return NULL;
}

ExprRI* VisitorRepInt::visita(AddressValueNodo *a){
/**VERIFICAR*/
    return NULL;
}

ExprRI* VisitorRepInt::visita(PointerValueNodo *p){
/**VERIFICAR*/
    return NULL;
}

MemRI* VisitorRepInt::visita(ArrayNodo *a){
    return new MemRI(new BinOpRI(PLUS, ((MemRI *) a->expr->aceitaRI(this))->exp, new BinOpRI(MUL, new ConstRI(tamanhoTipo(a->type)), a->indice->aceitaRI(this))));
}

StmtRI* VisitorRepInt::visita(ExprListNodo *el){
    TempRI *temp = new TempRI(new Temp());
    adicionaArgumentoCall(temp);
    ExprRI *exprRI = el->expr->aceitaRI(this);
    temp->type = exprRI->type;
    bool endereco = false;
    if (el->expr && el->expr->type && (typeid(*el->expr->type) == typeid(TypeIdNodo))) endereco = true;
    if (el->expr->array || endereco) { //arranjo ou tipo
        ExprRI *exprAnterior = exprRI;
        exprRI = ((MemRI*)exprRI)->exp;
        ((MemRI*)exprAnterior)->exp = NULL;
        delete exprAnterior;
    }
    if (el->exprList) return new SeqRI(new MoveRI(temp, exprRI), el->exprList->aceitaRI(this));
    else return new MoveRI(temp, exprRI);
}

ExprRI* VisitorRepInt::visita(CallNodo *c){
    Label *label = NULL;
    if (strcmp(((IdNodo*)c->id)->nome, (char*)"main")) label = new Label((char*)"func_", (((IdNodo*)c->id)->nome));
    else label = new Label((((IdNodo*)c->expr)->nome));

    if (c->exprList) {
        argumentosCall = new ExprRIListList(NULL, argumentosCall);
        ExprRI* exprRI = new ESeqRI(c->exprList->aceitaRI(this), new CallRI(label, argumentosCall->primeiro));
        int contadorArgumentos = 0;
        ExprRIList *listaArgumentos = argumentosCall->primeiro;
        while (listaArgumentos) {
            contadorArgumentos++;
            listaArgumentos = listaArgumentos->proximo;
        }
        contadorParametros = (contadorArgumentos > contadorParametros) ? contadorArgumentos : contadorParametros;
        argumentosCall = argumentosCall->proximo;

        return exprRI;
    } else return new CallRI(label, NULL);
}

ExprRI* VisitorRepInt::visita(LiteralNodo *l) {
    char *num = (char*)malloc(16 * sizeof(char));
    sprintf(num, "str_%d", qtdLiterais++);
    Label *label = new Label(num);
    label->type = STRING;
    Fragment *fragmento = new LiteralRI(new LabelRI(label), l);
    fragmento->proximo = primeiroFragmento;
    primeiroFragmento = fragmento;
    return new NameRI(label);
}

StmtRI* VisitorRepInt::visita(CaseBlockNodo *cb) {
    StmtRI *ramoEsquerda = NULL;
    StmtRI *ramoDireita = NULL;
    Label *proximoCaseBlock = new Label();
    Label *labelStmt = new Label();
    Label *labelStmt2 = new Label();
    Label *labelStmt3 = new Label();

    if (cb->stmtList) return new SeqRI(new CJumpRI(EQ, new TempRI(caseBlockValue->primeiro), new ConstRI(1), labelStmt, labelStmt2), new SeqRI(new LabelRI(labelStmt2), new SeqRI(new CJumpRI(EQ, cb->num->aceitaRI(this), new TempRI(switchExprs->primeiro), labelStmt3,  proximoCaseBlock), new SeqRI(new LabelRI(labelStmt3), new SeqRI(new MoveRI(new TempRI(caseBlockValue->primeiro),  new ConstRI(1)), new SeqRI(new LabelRI(labelStmt), cb->stmtList->aceitaRI(this)))))));
    else return new SeqRI(new CJumpRI(EQ, new TempRI(caseBlockValue->primeiro), new ConstRI(1), proximoCaseBlock, labelStmt2), new SeqRI(new LabelRI(labelStmt2), new SeqRI(new CJumpRI(EQ, cb->num->aceitaRI(this), new TempRI(switchExprs->primeiro), labelStmt3, proximoCaseBlock), new SeqRI(new LabelRI(labelStmt3), new MoveRI(new TempRI(caseBlockValue->primeiro), new ConstRI(1))))));

    if (cb->caseBlock->caseBlock) ramoDireita = new SeqRI(new LabelRI(proximoCaseBlock), cb->caseBlock->aceitaRI(this));
    else ramoDireita = new LabelRI(proximoCaseBlock);

    return new SeqRI(ramoEsquerda, ramoDireita);
}
