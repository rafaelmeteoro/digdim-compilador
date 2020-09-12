#ifndef _PARSER_H
#define _PARSER_H

#include "lexico.h"
#include "asa.h"

/*****************************************
            FUNÇÕES DO PARSER
******************************************/

void parserConstrutor();
int casar(int token);
int pular(int token, int sync[]);
int casarOuPular(int token, int sync[]);
int pertence(int sync[]);
void imprimeProducao(char *nome);

/*****************************************
            PRODUÇÕES DA GRAMÁTICA
******************************************/
ProgramNodo *Program();
ProgramNodo *ProgramLinha(TypeNodo *tipo, TypePointerNodo *pointer, IdNodo *id);
ProgramNodo *ProgramDef();
VarDeclNodo *VarDeclPrim(TypeNodo *tipo, TypePointerNodo *pointer, IdNodo *id);
ProgramNodo *ProgramDefLinha();
TypeDeclNodo *TypeDecl();
VarDeclNodo *VarDecl();
VarDeclNodo *VarDeclLinha(TypeNodo *tipo, TypePointerNodo *pointer, IdNodo *id);
IdListNodo *IdList();
IdListNodo *IdListLinha();
TypePointerNodo *Pointer();
NumNodo *Array();
FunctionDeclNodo *FunctionDecl(TypeNodo *tipo, IdNodo *id);
VarDeclNodo *FormalList();
VarDeclNodo *FormalListLinha();
TypeNodo *Type();
StatementListNodo *StmtList();
StatementListNodo *StmtList2();
StatementListNodo *StmtListLinha();
StatementNodo *Stmt();
StatementNodo *StmtLinha();
CaseBlockNodo *CaseBlock();
StatementListNodo *CaseBlockLinha();
ExprListNodo *ExprList();
ExprListNodo *ExprListTail();
ExprListNodo *ExprListTailLinha();
ExprNodo *Expr();
ExprNodo *ExprLinha(ExprNodo *primeiraParte);
ExprNodo *Expr1();
ExprNodo *Expr1Linha(ExprNodo *primeiraParte);
ExprNodo *Expr2();
ExprNodo *Expr2Linha(ExprNodo *primeiraParte);
ExprNodo *Expr3();
ExprNodo *Expr3Linha(ExprNodo *primeiraParte);
ExprNodo *Expr4();
ExprNodo *Expr4Linha(ExprNodo *primeiraParte);
ExprNodo *Expr5();
ExprNodo *Expr5Linha(ExprNodo *primeiraParte);
ExprNodo *Expr6();
ExprNodo *Expr6Linha(ExprNodo *primeiraParte);
ExprNodo *Expr7();
ExprNodo *Expr8();
ExprNodo *Expr10();
ExprNodo *Expr10Linha(ExprNodo *primeiraParte);
void UnaryOp();
int BinOp1();
int BinOp2();
int BinOp3();
int BinOp4();
int BinOp5();
int BinOp6();

#endif
