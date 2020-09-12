#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asa.h"
#include "lexico.h"

ProgramNodo::ProgramNodo(int linha){
    this->linha = linha;
}

ProgramNodo::~ProgramNodo(){}

StatementNodo::StatementNodo(int linha){
    this->linha = linha;
}

StatementNodo::~StatementNodo(){}

ExprNodo::ExprNodo(int linha){
    this->linha = linha;
    this->type  = NULL;
    this->array = NULL;
}

ExprNodo::~ExprNodo(){
    delete this->type;
    delete this->array;
}

TypeNodo::TypeNodo(int tipo, int linha){
    this->tipo  = tipo;
    this->linha = linha;
}

TypeNodo::~TypeNodo(){}

void TypeNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

TypeIdNodo::TypeIdNodo(IdNodo *id, int linha) : TypeNodo(ID, linha){
    this->id = id;
}

TypeIdNodo::~TypeIdNodo(){
    delete this->id;
}

void TypeIdNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

TypePointerNodo::TypePointerNodo(int tipo, int linha) : TypeNodo(tipo, linha){}

TypePointerNodo::~TypePointerNodo(){}

void TypePointerNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

/*****************************************
    CLASSES QUE HERDAM DE PROGRAMNODO
******************************************/
IdListNodo::IdListNodo(TypePointerNodo *pointer, IdNodo *id, NumNodo *array, IdListNodo *idList, int linha){
    this->pointer   = pointer;
    this->id        = id;
    this->array     = array;
    this->idList    = idList;
    this->linha     = linha;
}

IdListNodo::~IdListNodo(){
    delete this->pointer;
    delete this->id;
    delete this->array;
    delete this->idList;
}

void IdListNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

VarDeclNodo::VarDeclNodo(TypeNodo *tipo, IdListNodo *idList, VarDeclNodo *varDecl, int linha) : ProgramNodo(linha){
    this->tipo      = tipo;
    this->idList    = idList;
    this->varDecl   = varDecl;
}

VarDeclNodo::~VarDeclNodo(){
    delete this->tipo;
    delete this->idList;
    delete this->varDecl;
}

void VarDeclNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

void VarDeclNodo::aceitaRI(VisitorRepInt *rvisitor){
    rvisitor->visita(this);
}

TypeDeclNodo::TypeDeclNodo(VarDeclNodo *varDecl, IdNodo *id, TypeDeclNodo *typeDecl, int linha) : ProgramNodo(linha){
    this->varDecl   = varDecl;
    this->id        = id;
    this->typeDecl  = typeDecl;
}

TypeDeclNodo::~TypeDeclNodo(){
    delete this->varDecl;
    delete this->id;
    delete this->typeDecl;
}

void TypeDeclNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

void TypeDeclNodo::aceitaRI(VisitorRepInt *rvisitor){
    rvisitor->visita(this);
}

FunctionDeclNodo::FunctionDeclNodo(TypeNodo *tipo, IdNodo *id, VarDeclNodo *varListParametros, VarDeclNodo *varListVarLocais, StatementListNodo *stmtList, FunctionDeclNodo *functionDecl, int linha) : ProgramNodo(linha){
    this->tipo              = tipo;
    this->id                = id;
    this->varListParametros = varListParametros;
    this->varListVarLocais  = varListVarLocais;
    this->stmtList          = stmtList;
    this->functionDecl      = functionDecl;
}

FunctionDeclNodo::~FunctionDeclNodo(){
    delete this->tipo;
    delete this->id;
    delete this->varListParametros;
    delete this->varListVarLocais;
    delete this->stmtList;
    delete this->functionDecl;
}

void FunctionDeclNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

void FunctionDeclNodo::aceitaRI(VisitorRepInt *rvisitor){
    rvisitor->visita(this);
}

VarDeclProgramNodo::VarDeclProgramNodo(VarDeclNodo *varDecl, ProgramNodo *program, int linha) : ProgramNodo(linha){
    this->varDecl = varDecl;
    this->program = program;
}

VarDeclProgramNodo::~VarDeclProgramNodo(){
    delete this->varDecl;
    delete this->program;
}

void VarDeclProgramNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

void VarDeclProgramNodo::aceitaRI(VisitorRepInt *rvisitor){
    rvisitor->visita(this);
}

TypeDeclProgramNodo::TypeDeclProgramNodo(TypeDeclNodo *typeDecl, ProgramNodo *program, int linha) : ProgramNodo(linha){
    this->typeDecl = typeDecl;
    this->program  = program;
}

TypeDeclProgramNodo::~TypeDeclProgramNodo(){
    delete this->typeDecl;
    delete this->program;
}

void TypeDeclProgramNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

void TypeDeclProgramNodo::aceitaRI(VisitorRepInt *rvisitor){
    rvisitor->visita(this);
}

FunctionDeclProgramNodo::FunctionDeclProgramNodo(FunctionDeclNodo *functionDecl, ProgramNodo *program, int linha) : ProgramNodo(linha){
    this->functionDecl  = functionDecl;
    this->program       = program;
}

FunctionDeclProgramNodo::~FunctionDeclProgramNodo(){
    delete this->functionDecl;
    delete this->program;
}

void FunctionDeclProgramNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

void FunctionDeclProgramNodo::aceitaRI(VisitorRepInt *rvisitor){
    rvisitor->visita(this);
}

/*****************************************
    CLASSES QUE HERDAM DE STATEMENTNODO
******************************************/
StatementListNodo::StatementListNodo(StatementNodo *stmt, StatementListNodo *stmtList, int linha) : StatementNodo(linha){
    this->stmt      = stmt;
    this->stmtList  = stmtList;
}

StatementListNodo::~StatementListNodo(){
    delete this->stmt;
    delete this->stmtList;
}

void StatementListNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *StatementListNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

StatementListTypeNodo::StatementListTypeNodo(TypeDeclNodo *typeDecl, StatementListNodo *stmtList, int linha) : StatementNodo(linha){
    this->typeDecl = typeDecl;
    this->stmtList = stmtList;
}

StatementListTypeNodo::~StatementListTypeNodo(){
    delete this->typeDecl;
    delete this->stmtList;
}

void StatementListTypeNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *StatementListTypeNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

StatementListVarNodo::StatementListVarNodo(StatementListNodo *stmtList2, VarDeclNodo *varDecl, StatementListNodo *stmtListLinha, int linha) : StatementNodo(linha){
    this->stmtList2     = stmtList2;
    this->varDecl       = varDecl;
    this->stmtListLinha = stmtListLinha;
}

StatementListVarNodo::~StatementListVarNodo(){
    delete this->stmtList2;
    delete this->varDecl;
    delete this->stmtListLinha;
}

void StatementListVarNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *StatementListVarNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

IfNodo::IfNodo(ExprNodo *expr, StatementNodo *stmt1, StatementNodo *stmt2, int linha) : StatementNodo(linha){
    this->expr  = expr;
    this->stmt1 = stmt1;
    this->stmt2 = stmt2;
}

IfNodo::~IfNodo(){
    delete this->expr;
    delete this->stmt1;
    delete this->stmt2;
}

void IfNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *IfNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

WhileNodo::WhileNodo(ExprNodo *expr, StatementNodo *stmt, int linha) : StatementNodo(linha){
    this->expr = expr;
    this->stmt = stmt;
}

WhileNodo::~WhileNodo(){
    delete this->expr;
    delete this->stmt;
}

void WhileNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *WhileNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

SwitchNodo::SwitchNodo(ExprNodo *expr, CaseBlockNodo *caseBlock, int linha) : StatementNodo(linha){
    this->expr      = expr;
    this->caseBlock = caseBlock;
}

SwitchNodo::~SwitchNodo(){
    delete this->expr;
    delete this->caseBlock;
}

void SwitchNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *SwitchNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

BreakNodo::BreakNodo(int linha) : StatementNodo(linha){}

void BreakNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *BreakNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

PrintlnNodo::PrintlnNodo(ExprListNodo *exprList, int linha) : StatementNodo(linha){
    this->exprList = exprList;
}

PrintlnNodo::~PrintlnNodo(){
    delete this->exprList;
}

void PrintlnNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *PrintlnNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

ReadNodo::ReadNodo(ExprNodo *expr, int linha) : StatementNodo(linha){
    this->expr = expr;
}

ReadNodo::~ReadNodo(){
    delete this->expr;
}

void ReadNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *ReadNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

ReturnNodo::ReturnNodo(ExprNodo *expr, int linha) : StatementNodo(linha){
    this->expr = expr;
}

ReturnNodo::~ReturnNodo(){
    delete this->expr;
}

void ReturnNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *ReturnNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

ThrowNodo::ThrowNodo(int linha) : StatementNodo(linha){}

void ThrowNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *ThrowNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

TryNodo::TryNodo(StatementNodo *stmt1, StatementNodo *stmt2, int linha) : StatementNodo(linha){
    this->stmt1 = stmt1;
    this->stmt2 = stmt2;
}

TryNodo::~TryNodo(){
    delete this->stmt1;
    delete this->stmt2;
}

void TryNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *TryNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

ExprStatementNodo::ExprStatementNodo(ExprNodo *expr, int linha) : StatementNodo(linha){
    this->expr = expr;
}

ExprStatementNodo::~ExprStatementNodo(){
    delete this->expr;
}

void ExprStatementNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *ExprStatementNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

/******************************************
        CLASSES QUE HERDAM DE EXPRNODO
******************************************/
IdNodo::IdNodo(char *nome, int linha) : ExprNodo(linha){
    int tamanho = strlen(nome);
    this->nome = (char*)malloc(tamanho * sizeof(char));
    strcpy(this->nome, nome);
    this->global = false;
}

IdNodo::~IdNodo(){
    delete this->nome;
}

void IdNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *IdNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

NumNodo::NumNodo(int tipo, char *valor, int linha) : ExprNodo(linha){
    //int tamanho = strlen(valor);
    int tamanho = atoi(valor);
    this->valor = (char*)malloc(tamanho * sizeof(char));
    strcpy(this->valor, valor);
    this->tipo  = tipo;
}

NumNodo::~NumNodo(){
    free(this->valor);
}

void NumNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *NumNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

LiteralNodo::LiteralNodo(char *literal, int linha) : ExprNodo(linha){
    int tamanho = strlen(literal);
    this->literal = (char*)malloc(tamanho * sizeof(char));
    strcpy(this->literal, literal);
}

LiteralNodo::~LiteralNodo(){
    free(this->literal);
}

void LiteralNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *LiteralNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

CharNodo::CharNodo(char *Char, int linha) : ExprNodo(linha){
    int tamanho = strlen(Char);
    this->Char = (char*)malloc(tamanho * sizeof(char));
    strcpy(this->Char, Char);
}

CharNodo::~CharNodo(){
    free(this->Char);
}

void CharNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *CharNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

BooleanNodo::BooleanNodo(int boolean, int linha) : ExprNodo(linha){
    this->boolean = boolean;
}

BooleanNodo::~BooleanNodo(){}

void BooleanNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *BooleanNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

ExprListNodo::ExprListNodo(ExprNodo *expr, ExprListNodo *exprList, int linha){
    this->linha     = linha;
    this->expr      = expr;
    this->exprList  = exprList;
}

ExprListNodo::~ExprListNodo(){
    delete this->expr;
    delete this->exprList;
}

void ExprListNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *ExprListNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

CallNodo::CallNodo(IdNodo *id, ExprListNodo *exprList, ExprNodo *expr, int linha) : ExprNodo(linha){
    this->id        = id;
    this->exprList  = exprList;
    this->expr      = expr;
}

CallNodo::~CallNodo(){
    delete this->id;
    delete this->exprList;
    delete this->expr;
}

void CallNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *CallNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

PointerValueExprNodo::PointerValueExprNodo(IdNodo *id, ExprNodo *expr, int linha) : ExprNodo(linha){
    this->id    = id;
    this->expr  = expr;
}

PointerValueExprNodo::~PointerValueExprNodo(){
    delete this->id;
    delete this->expr;
}

void PointerValueExprNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *PointerValueExprNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

NameExprNodo::NameExprNodo(IdNodo *id, ExprNodo *expr, int linha) : ExprNodo(linha){
    this->id    = id;
    this->expr  = expr;
}

NameExprNodo::~NameExprNodo(){
    delete this->id;
    delete this->expr;
}

void NameExprNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *NameExprNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

AddressValueNodo::AddressValueNodo(ExprNodo *expr, int linha) : ExprNodo(linha){
    this->expr = expr;
}

AddressValueNodo::~AddressValueNodo(){
    delete this->expr;
}

void AddressValueNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *AddressValueNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

PointerValueNodo::PointerValueNodo(ExprNodo *expr, int linha) : ExprNodo(linha){
    this->expr = expr;
}

PointerValueNodo::~PointerValueNodo(){
    delete this->expr;
}

void PointerValueNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *PointerValueNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

ArrayNodo::ArrayNodo(ExprNodo *expr, ExprNodo *indice, int linha) : ExprNodo(linha){
    this->expr      = expr;
    this->indice    = indice;
}

ArrayNodo::~ArrayNodo(){
    delete this->expr;
    delete this->indice;
}

void ArrayNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *ArrayNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

RelationalOpNodo::RelationalOpNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha) : ExprNodo(linha){
    this->op_rel = op_rel;
    this->expr1  = expr1;
    this->expr2  = expr2;
}

RelationalOpNodo::~RelationalOpNodo(){
}

void RelationalOpNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *RelationalOpNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

MenorNodo::MenorNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha) : RelationalOpNodo(op_rel, expr1, expr2, linha){}

MenorNodo::~MenorNodo(){
    delete this->expr1;
    delete this->expr2;
}

void MenorNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *MenorNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

MenorIgualNodo::MenorIgualNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha) : RelationalOpNodo(op_rel, expr1, expr2, linha){}

MenorIgualNodo::~MenorIgualNodo(){
    delete this->expr1;
    delete this->expr2;
}

void MenorIgualNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *MenorIgualNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

MaiorNodo::MaiorNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha) : RelationalOpNodo(op_rel, expr1, expr2, linha){}

MaiorNodo::~MaiorNodo(){
    delete this->expr1;
    delete this->expr2;
}

void MaiorNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *MaiorNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

MaiorIgualNodo::MaiorIgualNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha) : RelationalOpNodo(op_rel, expr1, expr2, linha){}

MaiorIgualNodo::~MaiorIgualNodo(){
    delete this->expr1;
    delete this->expr2;
}

void MaiorIgualNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *MaiorIgualNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

IgualNodo::IgualNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha) : RelationalOpNodo(op_rel, expr1, expr2, linha){}

IgualNodo::~IgualNodo(){
    delete this->expr1;
    delete this->expr2;
}

void IgualNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *IgualNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

DiferenteNodo::DiferenteNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha) : RelationalOpNodo(op_rel, expr1, expr2, linha){}

DiferenteNodo::~DiferenteNodo(){
    delete this->expr1;
    delete this->expr2;
}

void DiferenteNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *DiferenteNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

ELogicoNodo::ELogicoNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha) : RelationalOpNodo(op_rel, expr1, expr2, linha){}

ELogicoNodo::~ELogicoNodo(){
    delete this->expr1;
    delete this->expr2;
}

void ELogicoNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *ELogicoNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

OuLogicoNodo::OuLogicoNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha) : RelationalOpNodo(op_rel, expr1, expr2, linha){}

OuLogicoNodo::~OuLogicoNodo(){
    delete this->expr1;
    delete this->expr2;
}

void OuLogicoNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *OuLogicoNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

BinarioOpNodo::BinarioOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha) : ExprNodo(linha){
    this->op    = op;
    this->expr1 = expr1;
    this->expr2 = expr2;
}

BinarioOpNodo::~BinarioOpNodo(){
}

void BinarioOpNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *BinarioOpNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

AdditionOpNodo::AdditionOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha) : BinarioOpNodo(op, expr1, expr2, linha){}

AdditionOpNodo::~AdditionOpNodo(){
    delete this->expr1;
    delete this->expr2;
}

void AdditionOpNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *AdditionOpNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

SubtractionOpNodo::SubtractionOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha) : BinarioOpNodo(op, expr1, expr2, linha){}

SubtractionOpNodo::~SubtractionOpNodo(){
    delete this->expr1;
    delete this->expr2;
}

void SubtractionOpNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *SubtractionOpNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

MultiplicationOpNodo::MultiplicationOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha) : BinarioOpNodo(op, expr1, expr2, linha){}

MultiplicationOpNodo::~MultiplicationOpNodo(){
    delete this->expr1;
    delete this->expr2;
}

void MultiplicationOpNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *MultiplicationOpNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

DivisionOpNodo::DivisionOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha) : BinarioOpNodo(op, expr1, expr2, linha){}

DivisionOpNodo::~DivisionOpNodo(){
    delete this->expr1;
    delete this->expr2;
}

void DivisionOpNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *DivisionOpNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

ModuleOpNodo::ModuleOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha) : BinarioOpNodo(op, expr1, expr2, linha){}

ModuleOpNodo::~ModuleOpNodo(){
    delete this->expr1;
    delete this->expr2;
}

void ModuleOpNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *ModuleOpNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

BooleanOpNodo::BooleanOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha) : BinarioOpNodo(op, expr1, expr2, linha){}

BooleanOpNodo::~BooleanOpNodo(){
    delete this->expr1;
    delete this->expr2;
}

void BooleanOpNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *BooleanOpNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

BitWiseOpNodo::BitWiseOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha) : BinarioOpNodo(op, expr1, expr2, linha){}

BitWiseOpNodo::~BitWiseOpNodo(){
    delete this->expr1;
    delete this->expr2;
}

void BitWiseOpNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *BitWiseOpNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

NotNodo::NotNodo(ExprNodo *expr, int linha) : ExprNodo(linha){
    this->expr = expr;
}

NotNodo::~NotNodo(){
    delete this->expr;
}

void NotNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *NotNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

SignNodo::SignNodo(int operador, ExprNodo *expr, int linha) : ExprNodo(linha){
    this->operador  = operador;
    this->expr      = expr;
}

SignNodo::~SignNodo(){
    delete this->expr;
}

void SignNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *SignNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

AssignNodo::AssignNodo(ExprNodo *expr1, ExprNodo *expr2, int linha) : ExprNodo(linha){
    this->expr1 = expr1;
    this->expr2 = expr2;
}

AssignNodo::~AssignNodo(){
    delete this->expr1;
    delete this->expr2;
}

void AssignNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

ExprRI *AssignNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}

CaseBlockNodo::CaseBlockNodo(NumNodo *num, StatementListNodo *stmtList, CaseBlockNodo *caseBlock, int linha){
    this->num       = num;
    this->stmtList  = stmtList;
    this->caseBlock = caseBlock;
    this->linha     = linha;
}

CaseBlockNodo::~CaseBlockNodo(){
    delete this->num;
    delete this->stmtList;
    delete this->caseBlock;
}

void CaseBlockNodo::aceita(Visitor *visitor){
    visitor->visita(this);
}

StmtRI *CaseBlockNodo::aceitaRI(VisitorRepInt *rvisitor){
    return rvisitor->visita(this);
}
