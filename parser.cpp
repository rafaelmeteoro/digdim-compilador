#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"

/** Variavel do parser */
int proximoToken;

/** Variavel para imprimir ou não o parser */
bool imprimeParse = false;

void parserConstrutor(){
    proximoToken = getNextToken();
}

int casar(int token){
    if(token == proximoToken){
        if((token == ID) || (token == NUMINT) || (token == NUMREAL)){
            if(imprimeParse) fprintf(stdout, "MATCH: %s.%s\n", uperCase(getNomeToken(token)), getLexema());
        }else{
            if(imprimeParse) fprintf(stdout, "MATCH: %s\n", uperCase(getNomeToken(token)));
        }
        if(proximoToken != EOF) proximoToken = getNextToken();
        return proximoToken;
    }else return 0;
}

int casarOuPular(int tokenExperado, int sync[]){
    int casado = casar(tokenExperado);
    if((casado == 0) && (tokenExperado != EOF)){
        return pular(tokenExperado, sync);
    }
    return casado;
}

int pular(int token, int sync[]){
    if(pertence(sync)){
        erroSintatico(token, ERRO_TOKEN_EXPERADO, getLinha());
        return 1;
    }else{
        erroSintatico(token, ERRO_TOKEN_EXPERADO, getLinha());
        proximoToken = getNextToken();

        while((pertence(sync) == 0) || (proximoToken == token)){
            if(proximoToken == EOF){
                erroSintatico(proximoToken, ERRO_TOKEN_INVALIDO, getLinha());
                return 0;
            }
            proximoToken = getNextToken();
        }
    }
    return 0;
}

int pertence(int conjunto[]){
    int i = 0;
    while(conjunto[i] != EOF) if(conjunto[i++] == proximoToken) return 1;
    return 0;
}

void imprimeProducao(char *nome){
    if(imprimeParse) fprintf(stdout, "%s\n", nome);
}

/*****************************************
            PRODUÇÕES DA GRAMÁTICA
******************************************/
int Program_First[] = {INT, FLOAT, BOOLEAN, ID, CHAR, TYPEDEF, EOF};
int Program_Sync[] = {EOF};
ProgramNodo *Program(){
    imprimeProducao((char *)"PROGRAM");
    ProgramNodo *p = NULL;
    if(proximoToken == TYPEDEF){
        p =  new TypeDeclProgramNodo(TypeDecl(), Program(), getLinha());
    }else if(pertence(Program_First)){
        p = ProgramDef();
    }
    casarOuPular(EOF, Program_Sync);
    return p;
}

int ProgramLinha_First[] = {ABREPARENT, ABRECOLCH, EOF};
int ProgramLinha_Sync[] = {EOF};
ProgramNodo *ProgramLinha(TypeNodo *tipo, TypePointerNodo *pointer, IdNodo *id){
    imprimeProducao((char *)"PROGRAM_LINHA");
    if(proximoToken == ABREPARENT){
        return new FunctionDeclProgramNodo(FunctionDecl(tipo, id), ProgramDefLinha(), getLinha());
    }else{
        return new VarDeclProgramNodo(VarDeclPrim(tipo, pointer, id), Program(), getLinha());
    }
    return NULL;
}

int ProgramDef_First[] = {INT, FLOAT, BOOLEAN, ID, CHAR, EOF};
int ProgramDef_Sync[] = {ID, EOF};
ProgramNodo *ProgramDef(){
    imprimeProducao((char *)"PROGRAM_DEF");
    if((proximoToken == INT) || (proximoToken == FLOAT) || (proximoToken == BOOLEAN) || (proximoToken == CHAR) || (proximoToken == ID)){
        TypeNodo *tipo = Type();
        TypePointerNodo *pointer = Pointer();
        IdNodo *id = NULL;
        if(proximoToken == ID) id = new IdNodo(getLexema(), getLinha());
        casarOuPular(ID, ProgramDef_Sync);
        return ProgramLinha(tipo, pointer, id);
    }
    return NULL;
}

int VarDeclPrim_First[] = {ABRECOLCH, EOF};
int VarDeclPrim_Sync[] = {TYPEDEF, INT, FLOAT, BOOLEAN, ID, CHAR, EOF};
VarDeclNodo *VarDeclPrim(TypeNodo *tipo, TypePointerNodo *pointer, IdNodo *id){
    imprimeProducao((char *)"VAR_DECL_PRIM");
    if(proximoToken == ABRECOLCH){
        NumNodo *array = Array();
        IdListNodo *idList = IdListLinha();
        casarOuPular(PONTOVIRGULA, VarDeclPrim_Sync);
        return new VarDeclNodo(tipo, new IdListNodo(pointer, id, array, idList, getLinha()), NULL, getLinha());
    }else{
        IdListNodo *idList = IdListLinha();
        casarOuPular(PONTOVIRGULA, VarDeclPrim_Sync);
        return new VarDeclNodo(tipo, new IdListNodo(pointer, id, NULL, idList, getLinha()), NULL, getLinha());
    }
}

int ProgramDefLinha_First[] = {INT, FLOAT, BOOLEAN, ID, CHAR, TYPEDEF,  EOF};
int ProgramDefLinha_Sync[] = {EOF};
ProgramNodo *ProgramDefLinha(){
    imprimeProducao((char *)"PROGRAM_DEF_LINHA");
    if(pertence(ProgramDefLinha_First)){
        return Program();
    }
    return NULL;
}

int TypeDecl_First[] = {TYPEDEF, EOF};
int TypeDecl_Sync[] = {INT, FLOAT, BOOLEAN, ID, CHAR, TYPEDEF, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, TRY, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ABRECHAVE, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, STRUCT, ABRECHAVE, FECHACHAVE, PONTOVIRGULA, EOF};
TypeDeclNodo *TypeDecl(){
    imprimeProducao((char *)"TYPE_DECL");
    if(proximoToken == TYPEDEF){
        casar(proximoToken);
        casarOuPular(STRUCT, TypeDecl_Sync);
        casarOuPular(ABRECHAVE, TypeDecl_Sync);
        TypeNodo *tipo = Type();
        IdListNodo *idList = IdList();
        casarOuPular(PONTOVIRGULA, TypeDecl_Sync);
        VarDeclNodo *varDecl = new VarDeclNodo(tipo, idList, VarDecl(), getLinha());
        casarOuPular(FECHACHAVE, TypeDecl_Sync);
        IdNodo *id = (IdNodo*)Expr();
        casarOuPular(PONTOVIRGULA, TypeDecl_Sync);
        return new TypeDeclNodo(varDecl, id, TypeDecl(), getLinha());
    }
    return NULL;
}

int VarDecl_First[] = {INT, FLOAT, BOOLEAN, ID, CHAR, EOF};
int VarDecl_Sync[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, ABRECHAVE, TRY, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, TYPEDEF, FECHACHAVE, EOF};
VarDeclNodo *VarDecl(){
    imprimeProducao((char *)"VAR_DECL");
    if((proximoToken == INT) || (proximoToken == FLOAT) || (proximoToken == BOOLEAN) || (proximoToken == ID) || (proximoToken == CHAR)){
        TypeNodo *tipo = Type();
        TypePointerNodo *pointer = Pointer();
        IdNodo *id = NULL;
        if(proximoToken == ID) id = new IdNodo(getLexema(), getLinha());
        casarOuPular(ID, VarDecl_Sync);
        return VarDeclLinha(tipo, pointer, id);
    }else{
        return NULL;
    }
}

int VarDeclLinha_First[] = {ABRECOLCH, EOF};
int VarDeclLinha_Sync[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, ABRECHAVE, FECHACHAVE, TRY, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, TRUE, FALSE, FECHACHAVE, TYPEDEF, PONTOVIRGULA, EOF};
VarDeclNodo *VarDeclLinha(TypeNodo *tipo, TypePointerNodo *pointer, IdNodo *id){
    imprimeProducao((char *)"VAR_DECL_LINHA");
    if(proximoToken == ABRECOLCH){
        NumNodo *array = Array();
        IdListNodo *idListLinha = IdListLinha();
        casarOuPular(PONTOVIRGULA, VarDeclLinha_Sync);
        return new VarDeclNodo(tipo, new IdListNodo(pointer, id, array, idListLinha, getLinha()), VarDecl(), getLinha());
    }else{
        IdListNodo *idListLinha = IdListLinha();
        casarOuPular(PONTOVIRGULA, VarDeclLinha_Sync);
        return new VarDeclNodo(tipo, new IdListNodo(pointer, id, NULL, idListLinha, getLinha()), VarDecl(), getLinha());
    }
}

int IdList_First[] = {MULT, EOF};
int IdList_Sync[] = {PONTOVIRGULA, ID, EOF};
IdListNodo *IdList(){
    imprimeProducao((char *)"ID_LIST");
    if(proximoToken == MULT){
        TypePointerNodo *pointer = Pointer();
        IdNodo *id = NULL;
        if(proximoToken == ID) id = new IdNodo(getLexema(), getLinha());
        casarOuPular(ID, IdList_Sync);
        return new IdListNodo(pointer, id, Array(), IdListLinha(), getLinha());
    }else{
        Pointer();
        IdNodo *id = NULL;
        if(proximoToken == ID) id = new IdNodo(getLexema(), getLinha());
        casarOuPular(ID, IdList_Sync);
        return new IdListNodo(NULL, id, Array(), IdListLinha(), getLinha());
    }
}

int IdListLinha_First[] = {VIRGULA, EOF};
int IdListLinha_Sync[] = {PONTOVIRGULA, ID, EOF};
IdListNodo *IdListLinha(){
    imprimeProducao((char *)"ID_LIST_LINHA");
    if(proximoToken == VIRGULA){
        casar(proximoToken);
        TypePointerNodo *pointer = Pointer();
        IdNodo *id = NULL;
        if(proximoToken == ID) id = new IdNodo(getLexema(), getLinha());
        casarOuPular(ID, IdListLinha_Sync);
        return new IdListNodo(pointer, id, Array(), IdListLinha(), getLinha());
    }
    return NULL;
}

int Pointer_First[] = {MULT, EOF};
int Pointer_Sync[] = {ID, EOF};
TypePointerNodo *Pointer(){
    imprimeProducao((char *)"POINTER");
    if(proximoToken == MULT){
        casar(proximoToken);
        return new TypePointerNodo(INT, getLinha());
    }
    return NULL;
}

int Array_First[] = {ABRECOLCH, EOF};
int Array_Sync[] = {VIRGULA, EOF};
NumNodo *Array(){
    imprimeProducao((char *)"ARRAY");
    if(proximoToken == ABRECOLCH){
//        casar(proximoToken);
//        NumNodo *tamanho = NULL;
//        if(proximoToken == NUMINT) tamanho = new NumNodo(NUMINT, getLexema(), getLinha());
//        casarOuPular(NUMINT, Array_Sync);
//        casarOuPular(FECHACOLCH, Array_Sync);
//        return tamanho;
        casar(proximoToken);
        NumNodo *tamanho = NULL;
        if(proximoToken == NUMINT){
            tamanho = new NumNodo(NUMINT, getLexema(), getLinha());
            casarOuPular(NUMINT, Array_Sync);
            casarOuPular(FECHACOLCH, Array_Sync);
        }else{
            /** Pequena gambiarra para quando for declarar vetor sem numero em parametro, que ele seja aceito no semantico */
            tamanho = new NumNodo(NUMINT, (char*)"0", getLinha()); /** SE DER PROBLEMA, REMOVER ESSA LINHA, MAS VAI TER QUE PASSAR NUMERO DO VETOR EM PARAMETRO */
            casarOuPular(FECHACOLCH, Array_Sync);
        }
        return tamanho;
    }
    return NULL;
}

int FunctionDecl_First[] = {ABREPARENT, EOF};
int FunctionDecl_Sync[] = {INT, FLOAT, BOOLEAN, ID, CHAR, TYPEDEF, FECHAPARENT, ABRECHAVE, FECHACHAVE, EOF};
FunctionDeclNodo *FunctionDecl(TypeNodo *tipo, IdNodo *id){
    imprimeProducao((char *)"FUNCTION_DECL");
    if(proximoToken == ABREPARENT){
        casar(proximoToken);
        VarDeclNodo *formalList = FormalList();
        casarOuPular(FECHAPARENT, FunctionDecl_Sync);
        casarOuPular(ABRECHAVE, FunctionDecl_Sync);
        StatementListNodo *stmtList = StmtList();
        casarOuPular(FECHACHAVE, FunctionDecl_Sync);
        return new FunctionDeclNodo(tipo, id, formalList, NULL, stmtList, NULL, getLinha());
    }else{
        erroSintatico(proximoToken, ERRO_TOKEN_INVALIDO, getLinha());
        return NULL;
    }
}

int FormalList_First[] = {INT, FLOAT, BOOLEAN, ID, CHAR, EOF};
int FormalList_Sync[] = {FECHAPARENT, ID, EOF};
VarDeclNodo *FormalList(){
    imprimeProducao((char *)"FORMAL_LIST");
    if((proximoToken == INT) || (proximoToken == FLOAT) || (proximoToken == BOOLEAN) || (proximoToken == ID) || (proximoToken == CHAR)){
        TypeNodo *tipo = Type();
        TypePointerNodo *pointer = Pointer();
        IdNodo *id = NULL;
        if(proximoToken == ID) id = new IdNodo(getLexema(), getLinha());
        casarOuPular(ID, FormalList_Sync);
        NumNodo *array = Array();
        return new VarDeclNodo(tipo, new IdListNodo(pointer, id, array, NULL, getLinha()), FormalListLinha(), getLinha());
    }
    return NULL;
}

int FormalListLinha_First[] = {VIRGULA, EOF};
int FormalListLinha_Sync[] = {FECHAPARENT, ID, EOF};
VarDeclNodo *FormalListLinha(){
    imprimeProducao((char *)"FORMAL_LIST_LINHA");
    if(proximoToken == VIRGULA){
        casar(proximoToken);
        TypeNodo *tipo = Type();
        TypePointerNodo *pointer = Pointer();
        IdNodo *id = NULL;
        if(proximoToken == ID) id = new IdNodo(getLexema(), getLinha());
        casarOuPular(ID, FormalListLinha_Sync);
        NumNodo *array = Array();
        return new VarDeclNodo(tipo, new IdListNodo(pointer, id, array, NULL, getLinha()), FormalListLinha(), getLinha());
    }
    return NULL;
}

int Type_First[] = {INT, FLOAT, BOOLEAN, ID, CHAR, EOF};
int Type_Sync[] = {MULT, EOF};
TypeNodo *Type(){
    imprimeProducao((char *)"TYPE");
    switch(proximoToken){
        case INT:       casar(proximoToken);    return new TypeNodo(INT, getLinha());
        case FLOAT:     casar(proximoToken);    return new TypeNodo(FLOAT, getLinha());
        case BOOLEAN:   casar(proximoToken);    return new TypeNodo(BOOLEAN, getLinha());
        case CHAR:      casar(proximoToken);    return new TypeNodo(CHAR, getLinha());
        case ID:{
            IdNodo *id = new IdNodo(getLexema(), getLinha());
            casar(proximoToken);
            return new TypeIdNodo(id, getLinha());
        }
        default:
            erroSintatico(proximoToken, ERRO_TOKEN_INVALIDO, getLinha());
            break;
    }
    return NULL;
}

int StmtList_First[] = {ID, INT, FLOAT, BOOLEAN, CHAR, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, ABRECHAVE, TRY, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ABREPARENT, ABRECOLCH, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int StmtList_Sync[] = {CASE, INT, FLOAT, BOOLEAN, CHAR, ID, CARACTER, FECHACHAVE, EOF};
StatementListNodo *StmtList(){
    imprimeProducao((char *)"STMT_LIST");
    if(proximoToken == TYPEDEF){
        return (StatementListNodo*)new StatementListTypeNodo(TypeDecl(), StmtList(), getLinha());
    }else if(pertence(StmtList_First)){
        return (StatementListNodo*)new StatementListVarNodo(StmtList2(), VarDecl(), StmtListLinha(), getLinha());
    }
    return NULL;
}

int StmtList2_First[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, ABRECHAVE , TRY, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int StmtList2_Sync[] = {INT, FLOAT, BOOLEAN, CHAR, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, EOF};
StatementListNodo *StmtList2(){
    imprimeProducao((char*)"STMT_LIST_2");
    if(pertence(StmtList2_First)){
        return new StatementListNodo(Stmt(), StmtListLinha(), getLinha());
    }
    return NULL;
}

int StmtListLinha_First[] = {TYPEDEF, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, ABRECHAVE, TRY, SUBTRACAO, NEGACAO, ADICAO, MULT, ID, ABREPARENT, ABRECOLCH, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int StmtListLinha_Sync[] = {CASE, INT, FLOAT, BOOLEAN, CHAR, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, FECHACHAVE, EOF};
StatementListNodo *StmtListLinha(){
    imprimeProducao((char *)"STMT_LIST_LINHA");
    if(pertence(StmtListLinha_First)){
        return StmtList();
    }
    return NULL;
}

int Stmt_First[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, ABRECHAVE , TRY, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int Stmt_Sync[] = {TYPEDEF, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, TRY, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ABRECOLCH, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, ELSE, CATCH, ABREPARENT, FECHAPARENT, ABRECHAVE, FECHACHAVE, PONTOVIRGULA, PONTO, EOF};
StatementNodo *Stmt(){
    imprimeProducao((char *)"STMT");
    if(proximoToken == IF){
        casar(proximoToken);
        casarOuPular(ABREPARENT, Stmt_Sync);
        ExprNodo *expr = Expr();
        casarOuPular(FECHAPARENT, Stmt_Sync);
        StatementNodo *stmt1 = Stmt();
        StatementNodo *stmt2 = StmtLinha();
        return new IfNodo(expr, stmt1, stmt2, getLinha());
    }else if(proximoToken == WHILE){
        casar(proximoToken);
        casarOuPular(ABREPARENT, Stmt_Sync);
        ExprNodo *expr = Expr();
        casarOuPular(FECHAPARENT, Stmt_Sync);
        StatementNodo *stmt = Stmt();
        return new WhileNodo(expr, stmt, getLinha());
    }else if(proximoToken == SWITCH){
        casar(proximoToken);
        casarOuPular(ABREPARENT, Stmt_Sync);
        ExprNodo *expr = Expr();
        casarOuPular(FECHAPARENT, Stmt_Sync);
        casarOuPular(ABRECHAVE, Stmt_Sync);
        CaseBlockNodo *caseBlock = CaseBlock();
        casarOuPular(FECHACHAVE, Stmt_Sync);
        return new SwitchNodo(expr, caseBlock, getLinha());
    }else if(proximoToken == BREAK){
        casar(proximoToken);
        casarOuPular(PONTOVIRGULA, Stmt_Sync);
        return new BreakNodo(getLinha());
    }else if(proximoToken == PRINT){
        casar(proximoToken);
        casarOuPular(ABREPARENT, Stmt_Sync);
        ExprListNodo *exprList = ExprList();
        casarOuPular(FECHAPARENT, Stmt_Sync);
        casarOuPular(PONTOVIRGULA, Stmt_Sync);
        return new PrintlnNodo(exprList, getLinha());
    }else if(proximoToken == READLN){
        casar(proximoToken);
        casarOuPular(ABREPARENT, Stmt_Sync);
        ExprNodo *expr = Expr();
        casarOuPular(FECHAPARENT, Stmt_Sync);
        casarOuPular(PONTOVIRGULA, Stmt_Sync);
        return new ReadNodo(expr, getLinha());
    }else if(proximoToken == RETURN){
        casar(proximoToken);
        ExprNodo *expr = Expr();
        casarOuPular(PONTOVIRGULA, Stmt_Sync);
        return new ReturnNodo(expr, getLinha());
    }else if(proximoToken == THROW){
        casar(proximoToken);
        casarOuPular(PONTOVIRGULA, Stmt_Sync);
        return new ThrowNodo(getLinha());
    }else if(proximoToken == ABRECHAVE){
        casar(proximoToken);
        StatementListNodo *stmtList = StmtList();
        casarOuPular(FECHACHAVE, Stmt_Sync);
        return stmtList;
    }else if(proximoToken == TRY){
        casar(proximoToken);
        StatementNodo *stmt1 = Stmt();
        casarOuPular(CATCH, Stmt_Sync);
        casarOuPular(ABREPARENT, Stmt_Sync);
        casarOuPular(PONTO, Stmt_Sync);
        casarOuPular(PONTO, Stmt_Sync);
        casarOuPular(PONTO, Stmt_Sync);
        casarOuPular(FECHAPARENT, Stmt_Sync);
        StatementNodo *stmt2 = Stmt();
        return new TryNodo(stmt1, stmt2, getLinha());
    }else if(pertence(Stmt_First)){
        ExprNodo *expr = Expr();
        casarOuPular(PONTOVIRGULA, Stmt_Sync);
        return new ExprStatementNodo(expr, getLinha());
        //return expr;
    }
    return NULL;
}

int StmtLinha_First[] = {ELSE ,EOF};
int StmtLinha_Sync[] = {TYPEDEF, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, ABRECHAVE, TRY, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, ELSE, CATCH, EOF};
StatementNodo *StmtLinha(){
    imprimeProducao((char *)"STMT_LINHA");
    if(proximoToken == ELSE){
        casar(proximoToken);
        return Stmt();
    }
    return NULL;
}

int CaseBlock_First[] = {CASE, EOF};
int CaseBlock_Sync[] = {FECHACHAVE, NUMINT, NUMREAL, DOISPONTOS, EOF};
CaseBlockNodo *CaseBlock(){
    imprimeProducao((char *)"CASEBLOCK");
    if(proximoToken == CASE){
        casar(proximoToken);
        NumNodo *num = NULL;
        if(proximoToken == NUMINT){
            num = new NumNodo(NUMINT, getLexema(), getLinha());
            casarOuPular(NUMINT, CaseBlock_Sync);
        }else if(proximoToken == NUMREAL){
            num = new NumNodo(NUMREAL, getLexema(), getLinha());
            casarOuPular(NUMREAL, CaseBlock_Sync);
        }
        casarOuPular(DOISPONTOS, CaseBlock_Sync);
        return new CaseBlockNodo(num, CaseBlockLinha(), CaseBlock(), getLinha());
    }
    return NULL;
}

int CaseBlockLinha_First[] = {TYPEDEF, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, ABRECHAVE, TRY, SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CARACTER, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int CaseBlockLinha_Sync[] = {CASE, EOF};
StatementListNodo *CaseBlockLinha(){
    imprimeProducao((char *)"CASEBLOCK_LINHA");
    if(pertence(CaseBlockLinha_First)){
        return StmtList();
    }
    return NULL;
}

int ExprList_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int ExprList_Sync[] = {FECHAPARENT, EOF};
ExprListNodo *ExprList(){
    imprimeProducao((char *)"EXPR_LIST");
    if(pertence(ExprList_First)){
        return ExprListTail();
    }
    return NULL;
}

int ExprListTail_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int ExprListTail_Sync[] = {FECHAPARENT, EOF};
ExprListNodo *ExprListTail(){
    imprimeProducao((char *)"EXPR_LIST_TAIL");
    if(pertence(ExprListTail_First)){
        return new ExprListNodo(Expr(), ExprListTailLinha(), getLinha());
    }
    return NULL;
}

int ExprListTailLinha_First[] = {VIRGULA, EOF};
int ExprListTailLinha_Sync[] = {FECHAPARENT, EOF};
ExprListNodo *ExprListTailLinha(){
    imprimeProducao((char *)"EXPR_LIST_TAIL_LINHA");
    if(proximoToken == VIRGULA){
        casar(proximoToken);
        return ExprListTail();
    }
    return NULL;
}

int Expr_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int Expr_Sync[] = {MULT, DIV, MOD, BIT_E, ABRECOLCH, PONTOVIRGULA, VIRGULA, ATRIBUICAO, FECHAPARENT, FECHACOLCH, EOF};
ExprNodo *Expr(){
    imprimeProducao((char *)"EXPR");
    if(pertence(Expr_First)){
        return ExprLinha(Expr1());
    }
    return NULL;
}

int ExprLinha_First[] = {ATRIBUICAO, EOF};
int ExprLinha_Sync[] = {MULT, DIV, MOD, BIT_E, ID, ABRECOLCH, PONTOVIRGULA, VIRGULA, ATRIBUICAO, FECHAPARENT, FECHACOLCH, EOF};
ExprNodo *ExprLinha(ExprNodo *primeiraParte){
    imprimeProducao((char *)"EXPR_LINHA");
    if(proximoToken == ATRIBUICAO){
        casar(proximoToken);
        return new AssignNodo(primeiraParte, ExprLinha(Expr()), getLinha());
    }
    return primeiraParte;
}

int Expr1_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int Expr1_Sync[] = {ATRIBUICAO, EOF};
ExprNodo *Expr1(){
    imprimeProducao((char *)"EXPR1");
    if(pertence(Expr1_First)){
        return Expr1Linha(Expr2());
    }
    return NULL;
}

int Expr1Linha_First[] = {OULOGICO, EOF};
int Expr1Linha_Sync[] = {ATRIBUICAO, EOF};
ExprNodo *Expr1Linha(ExprNodo *primeiraParte){
    imprimeProducao((char *)"EXPR1_LINHA");
    if(pertence(Expr1Linha_First)){
        int op = BinOp1();
        return new OuLogicoNodo(op, primeiraParte, Expr1Linha(Expr2()), getLinha());
    }
    return primeiraParte;
}

int Expr2_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int Expr2_Sync[] = {OULOGICO, EOF};
ExprNodo *Expr2(){
    imprimeProducao((char *)"EXPR2");
    if(pertence(Expr2_First)){
        return Expr2Linha(Expr3());
    }
    return NULL;
}

int Expr2Linha_First[] = {ELOGICO, EOF};
int Expr2Linha_Sync[] = {OULOGICO, EOF};
ExprNodo *Expr2Linha(ExprNodo *primeiraParte){
    imprimeProducao((char *)"EXPR2_LINHA");
    if(pertence(Expr2Linha_First)){
        int op = BinOp2();
        return new ELogicoNodo(op, primeiraParte, Expr2Linha(Expr3()), getLinha());
    }
    return primeiraParte;
}

int Expr3_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int Expr3_Sync[] = {ELOGICO, EOF};
ExprNodo *Expr3(){
    imprimeProducao((char *)"EXPR3");
    if(pertence(Expr3_First)){
        return Expr3Linha(Expr4());
    }
    return NULL;
}

int Expr3Linha_First[] = {IGUAL, DIFERENTE, EOF};
int Expr3Linha_Sync[] = {ELOGICO, EOF};
ExprNodo *Expr3Linha(ExprNodo *primeiraParte){
    imprimeProducao((char *)"EXPR3_LINHA");
    if(pertence(Expr3Linha_First)){
        if(proximoToken == IGUAL){
            int op = BinOp3();
            return new IgualNodo(op, primeiraParte, Expr3Linha(Expr4()), getLinha());
        }else if(proximoToken == DIFERENTE){
            int op = BinOp3();
            return new DiferenteNodo(op, primeiraParte, Expr3Linha(Expr4()), getLinha());
        }
    }
    return primeiraParte;
}

int Expr4_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int Expr4_Sync[] = {IGUAL, DIFERENTE, EOF};
ExprNodo *Expr4(){
    imprimeProducao((char *)"EXPR4");
    if(pertence(Expr4_First)){
        return Expr4Linha(Expr5());
    }
    return NULL;
}

int Expr4Linha_First[] = {MENOR, MENORIGUAL, MAIORIGUAL, MAIOR, EOF};
int Expr4Linha_Sync[] = {IGUAL, DIFERENTE, EOF};
ExprNodo *Expr4Linha(ExprNodo *primeiraParte){
    imprimeProducao((char *)"EXPR4_LINHA");
    if(pertence(Expr4Linha_First)){
        if(proximoToken == MENOR){
            int op = BinOp4();
            return new MenorNodo(op, primeiraParte, Expr4Linha(Expr5()), getLinha());
        }else if(proximoToken == MENORIGUAL){
            int op = BinOp4();
            return new MenorIgualNodo(op, primeiraParte, Expr4Linha(Expr5()), getLinha());
        }else if(proximoToken == MAIOR){
            int op = BinOp4();
            return new MaiorNodo(op, primeiraParte, Expr4Linha(Expr5()), getLinha());
        }else if(proximoToken == MAIORIGUAL){
            int op = BinOp4();
            return new MaiorIgualNodo(op, primeiraParte, Expr4Linha(Expr5()), getLinha());
        }
    }
    return primeiraParte;
}

int Expr5_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int Expr5_Sync[] = {MENOR, MENORIGUAL, MAIOR, MAIORIGUAL, EOF};
ExprNodo *Expr5(){
    imprimeProducao((char *)"EXPR5");
    if(pertence(Expr5_First)){
        return Expr5Linha(Expr6());
    }
    return NULL;
}

int Expr5Linha_First[] = {ADICAO, SUBTRACAO, BIT_OU, EOF};
int Expr5Linha_Sync[] = {MENOR, MENORIGUAL, MAIOR, MAIORIGUAL, EOF};
ExprNodo *Expr5Linha(ExprNodo *primeiraParte){
    imprimeProducao((char *)"EXPR5_LINHA");
    if(proximoToken == ADICAO){
        int op = BinOp5();
        return new AdditionOpNodo(op, primeiraParte, Expr5Linha(Expr6()), getLinha());
    }else if(proximoToken == SUBTRACAO){
        int op = BinOp5();
        return new SubtractionOpNodo(op, primeiraParte, Expr5Linha(Expr6()), getLinha());
    }else if(proximoToken == BIT_OU){
        int op = BinOp5();
        return new BitWiseOpNodo(op, primeiraParte, Expr5Linha(Expr6()), getLinha());
    }
    return primeiraParte;
}

int Expr6_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int Expr6_Sync[] = {ADICAO, SUBTRACAO, BIT_OU, EOF};
ExprNodo *Expr6(){
    imprimeProducao((char *)"EXPR6");
    if(pertence(Expr6_First)){
        return Expr6Linha(Expr7());
    }
    return NULL;
}

int Expr6Linha_First[] = {MULT, DIV, MOD, BIT_E};
int Expr6Linha_Sync[] = {ADICAO, SUBTRACAO, BIT_OU, EOF};
ExprNodo *Expr6Linha(ExprNodo *primeiraParte){
    imprimeProducao((char *)"EXPR6_LINHA");
    if(proximoToken == MULT){
        int op = BinOp6();
        return new MultiplicationOpNodo(op, primeiraParte, Expr6Linha(Expr7()), getLinha());
    }else if(proximoToken == DIV){
        int op = BinOp6();
        return new DivisionOpNodo(op, primeiraParte, Expr6Linha(Expr7()), getLinha());
    }else if(proximoToken == MOD){
        int op = BinOp6();
        return new ModuleOpNodo(op, primeiraParte, Expr6Linha(Expr7()), getLinha());
    }else if(proximoToken == BIT_E){
        int op = BinOp6();
        return new BitWiseOpNodo(op, primeiraParte, Expr6Linha(Expr7()), getLinha());
    }
    return primeiraParte;
}

int Expr7_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECOLCH, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int Expr7_Sync[] = {MULT, DIV, MOD, BIT_E, EOF};
ExprNodo *Expr7(){
    imprimeProducao((char *)"EXPR7");
    if(proximoToken == SUBTRACAO){
        UnaryOp();
        return new SignNodo(SUBTRACAO, Expr7(), getLinha());
    }else if(proximoToken == NEGACAO){
        UnaryOp();
        return new NotNodo(Expr7(), getLinha());
    }else if(proximoToken == ADICAO){
        UnaryOp();
        return new SignNodo(ADICAO, Expr7(), getLinha());
    }else if(proximoToken == MULT){
        UnaryOp();
        return new PointerValueNodo(Expr7(), getLinha());
    }else if(proximoToken == BIT_E){
        UnaryOp();
        return new AddressValueNodo(Expr7(), getLinha());
    }else if((proximoToken == ID) || (proximoToken == ABREPARENT) || (proximoToken == ABRECOLCH) || (proximoToken == LITERAL) || (proximoToken == CHAR) || (proximoToken == NUMINT) || (proximoToken == NUMREAL) || (proximoToken == CARACTER) || (proximoToken == TRUE) || (proximoToken == FALSE)){
        return Expr8();
    }
    return NULL;
}

int Expr8_First[] = {ID, ABREPARENT, LITERAL, CHAR, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int Expr8_Sync[] = {MULT, DIV, MOD, BIT_E, EOF};
ExprNodo *Expr8(){
    imprimeProducao((char *)"EXPR8");
    if(pertence(Expr8_First)){
        return Expr10Linha(Expr10());
    }
    return NULL;
}

int Expr10_First[] = {ID, ABREPARENT, LITERAL, CARACTER, NUMINT, NUMREAL, CARACTER, TRUE, FALSE, EOF};
int Expr10_Sync[] = {PONTO, SETA, FECHAPARENT, FECHACOLCH, PONTOVIRGULA, EOF};
ExprNodo *Expr10(){
    imprimeProducao((char *)"EXPR10");
    if(proximoToken == ID){
        IdNodo *id = NULL;
        if(proximoToken == ID) id = new IdNodo(getLexema(), getLinha());
        casar(proximoToken);
        return id;
    }else if(proximoToken == ABREPARENT){
        casar(proximoToken);
        ExprListNodo *exprList = ExprList();
        casarOuPular(FECHAPARENT, Expr10_Sync);
        return (ExprNodo*)exprList;
    }else if(proximoToken == LITERAL){
        LiteralNodo *literal = new LiteralNodo(getLexema(), getLinha());
        casar(proximoToken);
        return literal;
    }else if(proximoToken == CARACTER){
        CharNodo *Char = new CharNodo(getLexema(), getLinha());
        casar(proximoToken);
        return Char;
    }else if(proximoToken == NUMINT){
        NumNodo *num = new NumNodo(NUMINT, getLexema(), getLinha());
        casar(proximoToken);
        return num;
    }else if(proximoToken == NUMREAL){
        NumNodo *num = new NumNodo(NUMREAL, getLexema(), getLinha());
        casar(proximoToken);
        return num;
    }else if(proximoToken == TRUE){
        casar(proximoToken);
        return new BooleanNodo(TRUE, getLinha());
    }else if(proximoToken == FALSE){
        casar(proximoToken);
        return new BooleanNodo(FALSE, getLinha());
    }
    return NULL;
}

int Expr10Linha_First[] = {ABREPARENT, PONTO, SETA, ABRECOLCH, EOF};
int Expr10Linha_Sync[] = {PONTO, SETA, FECHAPARENT, FECHACOLCH, EOF};
ExprNodo *Expr10Linha(ExprNodo *primeiraParte){
    imprimeProducao((char *)"EXPR10_LINHA");
    if(proximoToken == ABREPARENT){
        casar(proximoToken);
        ExprListNodo *exprList = ExprList();
        casarOuPular(FECHAPARENT, Expr10Linha_Sync);
        ExprNodo *expr = Expr10Linha(NULL);
        return new CallNodo((IdNodo*)primeiraParte, exprList, expr, getLinha());
    }else if(proximoToken == PONTO){
        casar(PONTO);
        return new NameExprNodo((IdNodo*)primeiraParte, Expr8(), getLinha());
    }else if(proximoToken == SETA){
        casar(SETA);
        return new PointerValueExprNodo((IdNodo*)primeiraParte, Expr8(), getLinha());
    }else if(proximoToken == ABRECOLCH){
        casar(ABRECOLCH);
        ExprNodo *expr = Expr();
        casarOuPular(FECHACOLCH, Expr10Linha_Sync);
        return Expr10Linha(new ArrayNodo(primeiraParte, expr, getLinha()));
    }
    return primeiraParte;
}

int UnaryOp_First[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E};
int UnaryOp_Sync[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECHAVE, LITERAL, CHAR, NUMINT, NUMREAL, TRUE, FALSE, EOF};
void UnaryOp(){
    imprimeProducao((char *)"UNARYOP");
    if(proximoToken == SUBTRACAO){
        casar(proximoToken);
    }else if(proximoToken == NEGACAO){
        casar(proximoToken);
    }else if(proximoToken == ADICAO){
        casar(proximoToken);
    }else if(proximoToken == MULT){
        casar(proximoToken);
    }else if(proximoToken == BIT_E){
        casar(proximoToken);
    }
}

int BinOp1_First[] = {OULOGICO, EOF};
int BinOp1_Sync[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECHAVE, LITERAL, CHAR, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int BinOp1(){
    imprimeProducao((char *)"BINOP1");
    if(proximoToken == OULOGICO){
        casar(proximoToken);
        return OULOGICO;
    }
    return 0;
}

int BinOp2_First[] = {ELOGICO, EOF};
int BinOp2_Sync[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECHAVE, LITERAL, CHAR, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int BinOp2(){
    imprimeProducao((char *)"BINOP2");
    if(proximoToken == ELOGICO){
        casar(proximoToken);
        return ELOGICO;
    }
    return 0;
}

int BinOp3_First[] = {IGUAL, DIFERENTE, EOF};
int BinOp3_Sync[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECHAVE, LITERAL, CHAR, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int BinOp3(){
    imprimeProducao((char *)"BINOP3");
    if(proximoToken == IGUAL){
        casar(proximoToken);
        return IGUAL;
    }else if(proximoToken == DIFERENTE){
        casar(proximoToken);
        return DIFERENTE;
    }
    return 0;
}

int BinOp4_First[] = {MENOR, MENORIGUAL, MAIORIGUAL, MAIOR, EOF};
int BinOp4_Sync[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECHAVE, LITERAL, CHAR, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int BinOp4(){
    imprimeProducao((char *)"BINOP4");
    if(proximoToken == MENOR){
        casar(proximoToken);
        return MENOR;
    }else if(proximoToken == MENORIGUAL){
        casar(proximoToken);
        return MENORIGUAL;
    }else if(proximoToken == MAIORIGUAL){
        casar(proximoToken);
        return MAIORIGUAL;
    }else if(proximoToken == MAIOR){
        casar(proximoToken);
        return MAIOR;
    }
    return 0;
}

int BinOp5_First[] = {ADICAO, SUBTRACAO, BIT_OU, EOF};
int BinOp5_Sync[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECHAVE, LITERAL, CHAR, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int BinOp5(){
    imprimeProducao((char *)"BINOP5");
    if(proximoToken == ADICAO){
        casar(proximoToken);
        return ADICAO;
    }else if(proximoToken == SUBTRACAO){
        casar(proximoToken);
        return SUBTRACAO;
    }else if(proximoToken == BIT_OU){
        casar(proximoToken);
        return BIT_OU;
    }
    return 0;
}

int BinOp6_First[] = {MULT, DIV, MOD, BIT_E, EOF};
int BinOp6_Sync[] = {SUBTRACAO, NEGACAO, ADICAO, MULT, BIT_E, ID, ABREPARENT, ABRECHAVE, LITERAL, CHAR, NUMINT, NUMREAL, TRUE, FALSE, EOF};
int BinOp6(){
    imprimeProducao((char *)"BINOP6");
    if(proximoToken == MULT){
        casar(proximoToken);
        return MULT;
    }else if(proximoToken == DIV){
        casar(proximoToken);
        return DIV;
    }else if(proximoToken == MOD){
        casar(proximoToken);
        return MOD;
    }else if(proximoToken == BIT_E){
        casar(proximoToken);
        return BIT_E;
    }
    return 0;
}
