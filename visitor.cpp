#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexico.h"
#include "visitor.h"
#include "asa.h"

void Visitor::visita(Nodo *nodo){
    nodo->aceita(this);
}

void Visitor::visita(ProgramNodo *program){
    program->aceita(this);
}

void Visitor::visita(StatementNodo *statement){
    statement->aceita(this);
}

void Visitor::visita(ExprNodo *expr){
    expr->aceita(this);
}

void Visitor::visita(TypeNodo *type){
}

void Visitor::visita(TypeIdNodo *typeId){
    if(typeId->id) typeId->id->aceita(this);
}

void Visitor::visita(TypePointerNodo *typePointer){
}

void Visitor::visita(IdListNodo *idList){
    if(idList->pointer) idList->pointer->aceita(this);
    if(idList->id)      idList->id->aceita(this);
    if(idList->array)   idList->array->aceita(this);
    if(idList->idList)  idList->idList->aceita(this);
}

void Visitor::visita(VarDeclNodo *varDecl){
    if(varDecl->tipo)       varDecl->tipo->aceita(this);
    if(varDecl->idList)     varDecl->idList->aceita(this);
    if(varDecl->varDecl)    varDecl->varDecl->aceita(this);
}

void Visitor::visita(TypeDeclNodo *typeDecl){
    if(typeDecl->varDecl)   typeDecl->varDecl->aceita(this);
    if(typeDecl->id)        typeDecl->id->aceita(this);
    if(typeDecl->typeDecl)  typeDecl->typeDecl->aceita(this);
}

void Visitor::visita(FunctionDeclNodo *functionDecl){
    if(functionDecl->tipo)              functionDecl->tipo->aceita(this);
    if(functionDecl->id)                functionDecl->id->aceita(this);
    if(functionDecl->varListParametros) functionDecl->varListParametros->aceita(this);
    if(functionDecl->varListVarLocais)  functionDecl->varListVarLocais->aceita(this);
    if(functionDecl->stmtList)          functionDecl->stmtList->aceita(this);
    if(functionDecl->functionDecl)      functionDecl->functionDecl->aceita(this);
}

void Visitor::visita(VarDeclProgramNodo *varDeclProgram){
    if(varDeclProgram->varDecl) varDeclProgram->varDecl->aceita(this);
    if(varDeclProgram->program) varDeclProgram->program->aceita(this);
}

void Visitor::visita(TypeDeclProgramNodo *typeDeclProgram){
    if(typeDeclProgram->typeDecl)   typeDeclProgram->typeDecl->aceita(this);
    if(typeDeclProgram->program)    typeDeclProgram->program->aceita(this);
}

void Visitor::visita(FunctionDeclProgramNodo *functionDeclProgram){
    if(functionDeclProgram->functionDecl)   functionDeclProgram->functionDecl->aceita(this);
    if(functionDeclProgram->program)        functionDeclProgram->program->aceita(this);
}

void Visitor::visita(StatementListNodo *statementList){
    if(statementList->stmt)     statementList->stmt->aceita(this);
    if(statementList->stmtList) statementList->stmtList->aceita(this);
}

void Visitor::visita(StatementListTypeNodo *stmtListType){
    if(stmtListType->typeDecl) stmtListType->typeDecl->aceita(this);
    if(stmtListType->stmtList) stmtListType->stmtList->aceita(this);
}

void Visitor::visita(StatementListVarNodo *stmtListVar){
    if(stmtListVar->stmtList2)      stmtListVar->stmtList2->aceita(this);
    if(stmtListVar->varDecl)        stmtListVar->varDecl->aceita(this);
    if(stmtListVar->stmtListLinha)  stmtListVar->stmtListLinha->aceita(this);
}

void Visitor::visita(IfNodo *If){
    if(If->expr)    If->expr->aceita(this);
    if(If->stmt1)   If->stmt1->aceita(this);
    if(If->stmt2)   If->stmt2->aceita(this);
}

void Visitor::visita(WhileNodo *While){
    if(While->expr) While->expr->aceita(this);
    if(While->stmt) While->stmt->aceita(this);
}

void Visitor::visita(SwitchNodo *Switch){
    if(Switch->expr)        Switch->expr->aceita(this);
    if(Switch->caseBlock)   Switch->caseBlock->aceita(this);
}

void Visitor::visita(BreakNodo *Break){
}

void Visitor::visita(PrintlnNodo *print){
    if(print->exprList) print->exprList->aceita(this);
}

void Visitor::visita(ReadNodo *read){
    if(read->expr) read->expr->aceita(this);
}

void Visitor::visita(ReturnNodo *Return){
    if(Return->expr) Return->expr->aceita(this);
}

void Visitor::visita(ThrowNodo *Throw){
    if(Throw) Throw->aceita(this);
}

void Visitor::visita(TryNodo *Try){
    if(Try->stmt1) Try->stmt1->aceita(this);
    if(Try->stmt2) Try->stmt2->aceita(this);
}

void Visitor::visita(ExprStatementNodo *exprStatement){
    if(exprStatement->expr) exprStatement->expr->aceita(this);
}

void Visitor::visita(IdNodo *id){
}

void Visitor::visita(NumNodo *num){
}

void Visitor::visita(LiteralNodo *literal){
}

void Visitor::visita(CharNodo *Char){
}

void Visitor::visita(BooleanNodo *boolean){
}

void Visitor::visita(ExprListNodo *exprList){
    if(exprList->expr)      exprList->expr->aceita(this);
    if(exprList->exprList)  exprList->exprList->aceita(this);
}

void Visitor::visita(CallNodo *call){
    if(call->id)        call->id->aceita(this);
    if(call->exprList)  call->exprList->aceita(this);
    if(call->expr)      call->expr->aceita(this);
}

void Visitor::visita(PointerValueExprNodo *pointerValueExpr){
    if(pointerValueExpr->id)    pointerValueExpr->id->aceita(this);
    if(pointerValueExpr->expr)  pointerValueExpr->expr->aceita(this);
}

void Visitor::visita(NameExprNodo *nameExpr){
    if(nameExpr->id)    nameExpr->id->aceita(this);
    if(nameExpr->expr)  nameExpr->expr->aceita(this);
}

void Visitor::visita(AddressValueNodo *addressValue){
    if(addressValue->expr) addressValue->expr->aceita(this);
}

void Visitor::visita(PointerValueNodo *pointerValue){
    if(pointerValue->expr) pointerValue->expr->aceita(this);
}

void Visitor::visita(ArrayNodo *array){
    if(array->expr)     array->expr->aceita(this);
    if(array->indice)   array->indice->aceita(this);
}

void Visitor::visita(RelationalOpNodo *relationalOp){
    if(relationalOp->expr1) relationalOp->expr1->aceita(this);
    if(relationalOp->expr2) relationalOp->expr2->aceita(this);
}

void Visitor::visita(MenorNodo *menor){
    if(menor->expr1) menor->expr1->aceita(this);
    if(menor->expr2) menor->expr2->aceita(this);
}

void Visitor::visita(MenorIgualNodo *menorIgual){
    if(menorIgual->expr1) menorIgual->expr1->aceita(this);
    if(menorIgual->expr2) menorIgual->expr2->aceita(this);
}

void Visitor::visita(MaiorNodo *maior){
    if(maior->expr1) maior->expr1->aceita(this);
    if(maior->expr2) maior->expr2->aceita(this);
}

void Visitor::visita(MaiorIgualNodo *maiorIgual){
    if(maiorIgual->expr1) maiorIgual->expr1->aceita(this);
    if(maiorIgual->expr2) maiorIgual->expr2->aceita(this);
}

void Visitor::visita(IgualNodo *igual){
    if(igual->expr1) igual->expr1->aceita(this);
    if(igual->expr2) igual->expr2->aceita(this);
}

void Visitor::visita(DiferenteNodo *diferente){
    if(diferente->expr1) diferente->expr1->aceita(this);
    if(diferente->expr2) diferente->expr2->aceita(this);
}

void Visitor::visita(ELogicoNodo *eLogico){
    if(eLogico->expr1) eLogico->expr1->aceita(this);
    if(eLogico->expr2) eLogico->expr2->aceita(this);
}

void Visitor::visita(OuLogicoNodo *ouLogico){
    if(ouLogico->expr1) ouLogico->expr1->aceita(this);
    if(ouLogico->expr2) ouLogico->expr2->aceita(this);
}

void Visitor::visita(BinarioOpNodo *binOp){
    if(binOp->expr1) binOp->expr1->aceita(this);
    if(binOp->expr2) binOp->expr2->aceita(this);
}

void Visitor::visita(AdditionOpNodo *additionOp){
    if(additionOp->expr1) additionOp->expr1->aceita(this);
    if(additionOp->expr2) additionOp->expr2->aceita(this);
}

void Visitor::visita(SubtractionOpNodo *subtractionOp){
    if(subtractionOp->expr1) subtractionOp->expr1->aceita(this);
    if(subtractionOp->expr2) subtractionOp->expr2->aceita(this);
}

void Visitor::visita(MultiplicationOpNodo *multiplicationOp){
    if(multiplicationOp->expr1) multiplicationOp->expr1->aceita(this);
    if(multiplicationOp->expr2) multiplicationOp->expr2->aceita(this);
}

void Visitor::visita(DivisionOpNodo *divisionOp){
    if(divisionOp->expr1) divisionOp->expr1->aceita(this);
    if(divisionOp->expr2) divisionOp->expr2->aceita(this);
}

void Visitor::visita(ModuleOpNodo *moduleOp){
    if(moduleOp->expr1) moduleOp->expr1->aceita(this);
    if(moduleOp->expr2) moduleOp->expr2->aceita(this);
}

void Visitor::visita(BooleanOpNodo *booleanOp){
    if(booleanOp->expr1) booleanOp->expr1->aceita(this);
    if(booleanOp->expr2) booleanOp->expr2->aceita(this);
}

void Visitor::visita(BitWiseOpNodo *bitWiseOp){
    if(bitWiseOp->expr1) bitWiseOp->expr1->aceita(this);
    if(bitWiseOp->expr2) bitWiseOp->expr2->aceita(this);
}

void Visitor::visita(NotNodo *Not){
    if(Not->expr) Not->expr->aceita(this);
}

void Visitor::visita(SignNodo *sign){
    if(sign->expr) sign->expr->aceita(this);
}

void Visitor::visita(AssignNodo *assign){
    if(assign->expr1) assign->expr1->aceita(this);
    if(assign->expr2) assign->expr2->aceita(this);
}

void Visitor::visita(CaseBlockNodo *caseblock){
    if(caseblock->num)          caseblock->num->aceita(this);
    if(caseblock->stmtList)     caseblock->stmtList->aceita(this);
    if(caseblock->caseBlock)    caseblock->caseBlock->aceita(this);
}

/*****************************************
            IMPRIME ARVORE
******************************************/
ImprimiArvore::ImprimiArvore(){
    this->nivel = 0;
}

void ImprimiArvore::imprimi(Nodo *nodo, char *texto){
    for(int i = 0; i < nivel; i++){
        fprintf(stdout, "   ");
    }
    fprintf(stdout, "- %s\n", texto);
}

void ImprimiArvore::visita(Nodo *nodo){
    imprimi(nodo, (char*)"NODO");
    nivel++;
    Visitor::visita(nodo);
    nivel--;
}

void ImprimiArvore::visita(ProgramNodo *program){
    imprimi(program, (char*)"PROGRAM_NODO");
    nivel++;
    Visitor::visita(program);
    nivel--;
}

void ImprimiArvore::visita(StatementNodo *statement){
    imprimi(statement, (char*)"STATEMENT_NODO");
    nivel++;
    Visitor::visita(statement);
    nivel--;
}

void ImprimiArvore::visita(ExprNodo *expr){
    imprimi(expr, (char*)"EXPR_NODO");
    nivel++;
    Visitor::visita(expr);
    nivel--;
}

void ImprimiArvore::visita(TypeNodo *type){
    char *t = getNomeToken(type->tipo);
    char *texto = (char*)malloc((strlen(t) + 11) * sizeof(char));
    sprintf(texto, "TYPE_NODO.%s", t);
    imprimi(type, texto);
    free(texto);
    nivel++;
    Visitor::visita(type);
    nivel--;
}

void ImprimiArvore::visita(TypeIdNodo *typeId){
    imprimi(typeId, (char*)"TYPE_ID_NODO");
    nivel++;
    Visitor::visita(typeId);
    nivel--;
}

void ImprimiArvore::visita(TypePointerNodo *typePointer){
    imprimi(typePointer, (char*)"TYPE_POINTER_NODO");
    nivel++;
    Visitor::visita(typePointer);
    nivel--;
}

void ImprimiArvore::visita(IdListNodo *idList){
    imprimi(idList, (char*)"ID_LIS_NODO");
    nivel++;
    Visitor::visita(idList);
    nivel--;
}

void ImprimiArvore::visita(VarDeclNodo *varDecl){
    imprimi(varDecl, (char*)"VAR_DECL_NODO");
    nivel++;
    Visitor::visita(varDecl);
    nivel--;
}

void ImprimiArvore::visita(TypeDeclNodo *typeDecl){
    imprimi(typeDecl, (char*)"TYPE_DECL_NODO");
    nivel++;
    Visitor::visita(typeDecl);
    nivel--;
}

void ImprimiArvore::visita(FunctionDeclNodo *functionDecl){
    imprimi(functionDecl, (char*)"FUNCTION_DECL_NODO");
    nivel++;
    Visitor::visita(functionDecl);
    nivel--;
}

void ImprimiArvore::visita(VarDeclProgramNodo *varDeclProgram){
    imprimi(varDeclProgram, (char*)"VAR_DECL_PROGRAM_NODO");
    nivel++;
    Visitor::visita(varDeclProgram);
    nivel--;
}

void ImprimiArvore::visita(TypeDeclProgramNodo *typeDeclProgram){
    imprimi(typeDeclProgram, (char*)"TYPE_DECL_PROGRAM_NODO");
    nivel++;
    Visitor::visita(typeDeclProgram);
    nivel--;
}

void ImprimiArvore::visita(FunctionDeclProgramNodo *functionDeclProgram){
    imprimi(functionDeclProgram, (char*)"FUNCTION_DECL_PROGRAM_NODO");
    nivel++;
    Visitor::visita(functionDeclProgram);
    nivel--;
}

void ImprimiArvore::visita(StatementListNodo *statementList){
    imprimi(statementList, (char*)"STATEMENT_LIST_NODO");
    nivel++;
    Visitor::visita(statementList);
    nivel--;
}

void ImprimiArvore::visita(StatementListTypeNodo *statementListType){
    imprimi(statementListType, (char*)"STATEMENT_LIST_TYPE_NODO");
    nivel++;
    Visitor::visita(statementListType);
    nivel--;
}

void ImprimiArvore::visita(StatementListVarNodo *statementListVar){
    imprimi(statementListVar, (char*)"STATEMENT_LIST_VAR_NODO");
    nivel++;
    Visitor::visita(statementListVar);
    nivel--;
}

void ImprimiArvore::visita(IfNodo *If){
    imprimi(If, (char*)"IF_NODO");
    nivel++;
    Visitor::visita(If);
    nivel--;
}

void ImprimiArvore::visita(WhileNodo *While){
    imprimi(While, (char*)"WHILE_NODO");
    nivel++;
    Visitor::visita(While);
    nivel--;
}

void ImprimiArvore::visita(SwitchNodo *Switch){
    imprimi(Switch, (char*)"SWITCH_NODO");
    nivel++;
    Visitor::visita(Switch);
    nivel--;
}

void ImprimiArvore::visita(BreakNodo *Break){
    imprimi(Break, (char*)"BREAK_NODO");
    nivel++;
    Visitor::visita(Break);
    nivel--;
}

void ImprimiArvore::visita(PrintlnNodo *print){
    imprimi(print, (char*)"PRINT_NODO");
    nivel++;
    Visitor::visita(print);
    nivel--;
}

void ImprimiArvore::visita(ReadNodo *read){
    imprimi(read, (char*)"READ_NODO");
    nivel++;
    Visitor::visita(read);
    nivel--;
}

void ImprimiArvore::visita(ReturnNodo *Return){
    imprimi(Return, (char*)"RETURN_NODO");
    nivel++;
    Visitor::visita(Return);
    nivel--;
}

void ImprimiArvore::visita(ThrowNodo *Throw){
    imprimi(Throw, (char*)"THROW_NODO");
    nivel++;
    Visitor::visita(Throw);
    nivel--;
}

void ImprimiArvore::visita(TryNodo *Try){
    imprimi(Try, (char*)"TRY_NODO");
    nivel++;
    Visitor::visita(Try);
    nivel--;
}

void ImprimiArvore::visita(ExprStatementNodo *exprStatement){
    imprimi(exprStatement, (char*)"EXPR_STATEMENT_NODO");
    nivel++;
    Visitor::visita(exprStatement);
    nivel--;
}

void ImprimiArvore::visita(IdNodo *id){
    char *nome = id->nome;
    char *texto = (char*)malloc((strlen(nome) + 9) * sizeof(char));
    sprintf(texto, "ID_NODO.%s",nome);
    imprimi(id, texto);
    free(texto);
    nivel++;
    Visitor::visita(id);
    nivel--;
}

void ImprimiArvore::visita(NumNodo *num){
    char *valor = num->valor;
    char *texto = (char*)malloc((strlen(valor) + 10) * sizeof(char));
    sprintf(texto, "NUM_NODO.%s",valor);
    imprimi(num, texto);
    free(texto);
    nivel++;
    Visitor::visita(num);
    nivel--;
}

void ImprimiArvore::visita(LiteralNodo *literal){
    char *lit = literal->literal;
    char *texto = (char*)malloc((strlen(lit) + 14) * sizeof(char));
    sprintf(texto, "LITERAL_NODO.%s", lit);
    imprimi(literal, texto);
    free(texto);
    nivel++;
    Visitor::visita(literal);
    nivel--;
}

void ImprimiArvore::visita(CharNodo *Char){
    char *caractere = Char->Char;
    char *texto = (char*)malloc((strlen(caractere) + 11) * sizeof(char));
    sprintf(texto, "CHAR_NODO.%s", caractere);
    imprimi(Char, texto);
    free(texto);
    nivel++;
    Visitor::visita(Char);
    nivel--;
}

void ImprimiArvore::visita(BooleanNodo *boolean){
    int tipo = boolean->boolean;
    char *tipoBoolan;
    if(tipo == TRUE){
        tipoBoolan = (char*)"TRUE";
    }else if(tipo == FALSE){
        tipoBoolan = (char*)"FALSE";
    }
    char *texto = (char*)malloc((strlen(tipoBoolan) + 14) * sizeof(char)); //O numero 14 se refere ao tamanho ta string BOOLEAN_NODO. + 1
    sprintf(texto, "BOOLEAN_NODO.%s", tipoBoolan);
    imprimi(boolean, texto);
    free(texto);
    nivel++;
    Visitor::visita(boolean);
    nivel--;
}

void ImprimiArvore::visita(ExprListNodo *exprList){
    imprimi(exprList, (char*)"EXPR_LIST_NODO");
    nivel++;
    Visitor::visita(exprList);
    nivel--;
}

void ImprimiArvore::visita(CallNodo *call){
    imprimi(call, (char*)"CALL_NODO");
    nivel++;
    Visitor::visita(call);
    nivel--;
}

void ImprimiArvore::visita(PointerValueExprNodo *pointerValueExpr){
    imprimi(pointerValueExpr, (char*)"POINTER_VALUE_EXPR_NODO");
    nivel++;
    Visitor::visita(pointerValueExpr);
    nivel--;
}

void ImprimiArvore::visita(NameExprNodo *nameExpr){
    imprimi(nameExpr, (char*)"NAME_EXPR_NODO");
    nivel++;
    Visitor::visita(nameExpr);
    nivel--;
}

void ImprimiArvore::visita(AddressValueNodo *addressValue){
    imprimi(addressValue, (char*)"ADDRES_VALUE_NODO");
    nivel++;
    Visitor::visita(addressValue);
    nivel--;
}

void ImprimiArvore::visita(PointerValueNodo *pointerValue){
    imprimi(pointerValue, (char*)"POINTER_VALUE_NODO");
    nivel++;
    Visitor::visita(pointerValue);
    nivel--;
}

void ImprimiArvore::visita(ArrayNodo *array){
    imprimi(array, (char*)"ARRAY_NODO");
    nivel++;
    Visitor::visita(array);
    nivel--;
}

void ImprimiArvore::visita(RelationalOpNodo *relationalOp){
    imprimi(relationalOp, (char*)"RELATIONAL_OP_NODO");
    nivel++;
    Visitor::visita(relationalOp);
    nivel--;
}

void ImprimiArvore::visita(MenorNodo *menor){
    imprimi(menor, (char*)"MENOR_NODO");
    nivel++;
    Visitor::visita(menor);
    nivel--;
}

void ImprimiArvore::visita(MenorIgualNodo *menorIgual){
    imprimi(menorIgual, (char*)"MENOR_IGUAL_NODO");
    nivel++;
    Visitor::visita(menorIgual);
    nivel--;
}

void ImprimiArvore::visita(MaiorNodo *maior){
    imprimi(maior, (char*)"MAIOR_NODO");
    nivel++;
    Visitor::visita(maior);
    nivel--;
}

void ImprimiArvore::visita(MaiorIgualNodo *maiorIgual){
    imprimi(maiorIgual, (char*)"MAIOR_IGUAL_NODO");
    nivel++;
    Visitor::visita(maiorIgual);
    nivel--;
}

void ImprimiArvore::visita(IgualNodo *igual){
    imprimi(igual, (char*)"IGUAL_NODO");
    nivel++;
    Visitor::visita(igual);
    nivel--;
}

void ImprimiArvore::visita(DiferenteNodo *diferente){
    imprimi(diferente, (char*)"DIFERENTE_NODO");
    nivel++;
    Visitor::visita(diferente);
    nivel--;
}

void ImprimiArvore::visita(ELogicoNodo *eLogico){
    imprimi(eLogico, (char*)"E_LOGICO_NODO");
    nivel++;
    Visitor::visita(eLogico);
    nivel--;
}

void ImprimiArvore::visita(OuLogicoNodo *ouLogico){
    imprimi(ouLogico, (char*)"OU_LOGICO_NODO");
    nivel++;
    Visitor::visita(ouLogico);
    nivel--;
}

void ImprimiArvore::visita(BinarioOpNodo *binOp){
    imprimi(binOp, (char*)"BINARIO_OP_NODO");
    nivel++;
    Visitor::visita(binOp);
    nivel--;
}

void ImprimiArvore::visita(AdditionOpNodo *additionOp){
    imprimi(additionOp, (char*)"ADDITION_OP_NODO");
    nivel++;
    Visitor::visita(additionOp);
    nivel--;
}

void ImprimiArvore::visita(SubtractionOpNodo *subtractionOp){
    imprimi(subtractionOp, (char*)"SUBTRACTION_OP_NODO");
    nivel++;
    Visitor::visita(subtractionOp);
    nivel--;
}

void ImprimiArvore::visita(MultiplicationOpNodo *multiplicationOp){
    imprimi(multiplicationOp, (char*)"MULTIPLICATION_OP_NODO");
    nivel++;
    Visitor::visita(multiplicationOp);
    nivel--;
}

void ImprimiArvore::visita(DivisionOpNodo *divisionOp){
    imprimi(divisionOp, (char*)"DIVISION_OP_NODO");
    nivel++;
    Visitor::visita(divisionOp);
    nivel--;
}

void ImprimiArvore::visita(ModuleOpNodo *moduleOp){
    imprimi(moduleOp, (char*)"MODULE_OP_NODO");
    nivel++;
    Visitor::visita(moduleOp);
    nivel--;
}

void ImprimiArvore::visita(BooleanOpNodo *booleanOp){
    imprimi(booleanOp, (char*)"BOOLEAN_OP_NODO");
    nivel++;
    Visitor::visita(booleanOp);
    nivel--;
}

void ImprimiArvore::visita(BitWiseOpNodo *bitWiseOp){
    imprimi(bitWiseOp, (char*)"BIT_WISE_OP_NODO");
    nivel++;
    Visitor::visita(bitWiseOp);
    nivel--;
}

void ImprimiArvore::visita(NotNodo *Not){
    imprimi(Not, (char*)"NOT_NODO");
    nivel++;
    Visitor::visita(Not);
    nivel--;
}

void ImprimiArvore::visita(SignNodo *sign){
    imprimi(sign, (char*)"SIGN_NODO");
    nivel++;
    Visitor::visita(sign);
    nivel--;
}

void ImprimiArvore::visita(AssignNodo *assign){
    imprimi(assign, (char*)"ASSIGN_NODO");
    nivel++;
    Visitor::visita(assign);
    nivel--;
}

void ImprimiArvore::visita(CaseBlockNodo *caseblock){
    imprimi(caseblock, (char*)"CASEBLOCK_NODO");
    nivel++;
    Visitor::visita(caseblock);
    nivel--;
}
