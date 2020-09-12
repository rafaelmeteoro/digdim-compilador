#ifndef _VISITOR_H
#define _VISITOR_H

#include "classesasa.h"

class Visitor{
public:
    virtual void visita(Nodo                *nodo) = 0;
    virtual void visita(ProgramNodo         *program) = 0;
    virtual void visita(StatementNodo       *statement) = 0;
    virtual void visita(ExprNodo            *expr) = 0;
    virtual void visita(TypeNodo            *type) = 0;
    virtual void visita(TypeIdNodo          *typeId) = 0;
    virtual void visita(TypePointerNodo     *typePointer) = 0;
    virtual void visita(IdListNodo          *idList) = 0;
    virtual void visita(VarDeclNodo         *varDecl) = 0;
    virtual void visita(TypeDeclNodo        *typeDecl) = 0;
    virtual void visita(FunctionDeclNodo    *functionDecl) = 0;
    virtual void visita(VarDeclProgramNodo  *varDeclProgram) = 0;
    virtual void visita(TypeDeclProgramNodo *typeDeclProgram) = 0;
    virtual void visita(FunctionDeclProgramNodo    *functionDeclProgram) = 0;
    virtual void visita(StatementListNodo   *statementList) = 0;
    virtual void visita(StatementListTypeNodo *stmtListType) = 0;
    virtual void visita(StatementListVarNodo  *stmtListVar) = 0;
    virtual void visita(IfNodo              *If) = 0;
    virtual void visita(WhileNodo           *While) = 0;
    virtual void visita(SwitchNodo          *Switch) = 0;
    virtual void visita(BreakNodo           *Break) = 0;
    virtual void visita(PrintlnNodo         *print) = 0;
    virtual void visita(ReadNodo            *read) = 0;
    virtual void visita(ReturnNodo          *Return) = 0;
    virtual void visita(ThrowNodo           *Throw) = 0;
    virtual void visita(TryNodo             *Try) = 0;
    virtual void visita(ExprStatementNodo   *exprStatement) = 0;
    virtual void visita(IdNodo              *id) = 0;
    virtual void visita(NumNodo             *num) = 0;
    virtual void visita(LiteralNodo         *literal) = 0;
    virtual void visita(CharNodo            *Char) = 0;
    virtual void visita(BooleanNodo         *boolean) = 0;
    virtual void visita(ExprListNodo        *exprList) = 0;
    virtual void visita(CallNodo            *call) = 0;
    virtual void visita(PointerValueExprNodo *pointerValueExpr) = 0;
    virtual void visita(NameExprNodo        *nameExpr) = 0;
    virtual void visita(AddressValueNodo    *addressValue) = 0;
    virtual void visita(PointerValueNodo    *pointerValue) = 0;
    virtual void visita(ArrayNodo           *array) = 0;
    virtual void visita(RelationalOpNodo    *relationalOp) = 0;
    virtual void visita(MenorNodo           *menor) = 0;
    virtual void visita(MenorIgualNodo      *menorIgual) = 0;
    virtual void visita(MaiorNodo           *maior) = 0;
    virtual void visita(MaiorIgualNodo      *maiorIgual) = 0;
    virtual void visita(IgualNodo           *igual) = 0;
    virtual void visita(DiferenteNodo       *diferente) = 0;
    virtual void visita(ELogicoNodo         *eLogico) = 0;
    virtual void visita(OuLogicoNodo        *ouLogico) = 0;
    virtual void visita(BinarioOpNodo       *binOp) = 0;
    virtual void visita(AdditionOpNodo      *additionOp) = 0;
    virtual void visita(SubtractionOpNodo   *subtractionOp) = 0;
    virtual void visita(MultiplicationOpNodo *multiplicationOp) = 0;
    virtual void visita(DivisionOpNodo      *divisionOp) = 0;
    virtual void visita(ModuleOpNodo        *moduleOp) = 0;
    virtual void visita(BooleanOpNodo       *booleanOp) = 0;
    virtual void visita(BitWiseOpNodo       *bitWiseOp) = 0;
    virtual void visita(NotNodo             *Not) = 0;
    virtual void visita(SignNodo            *sign) = 0;
    virtual void visita(AssignNodo          *assign) = 0;
    virtual void visita(CaseBlockNodo       *caseblock) = 0;
};

class ImprimiArvore : public Visitor{
    public:
        int nivel;

    public:
        ImprimiArvore();
        void imprimi(Nodo *nodo, char *texto);
        void visita(Nodo                *nodo);
        void visita(ProgramNodo         *program);
        void visita(StatementNodo       *statement);
        void visita(ExprNodo            *expr);
        void visita(TypeNodo            *type);
        void visita(TypeIdNodo          *typeId);
        void visita(TypePointerNodo     *typePointer);
        void visita(IdListNodo          *idList);
        void visita(VarDeclNodo         *varDecl);
        void visita(TypeDeclNodo        *typeDecl);
        void visita(FunctionDeclNodo    *functionDecl);
        void visita(VarDeclProgramNodo  *varDeclProgram);
        void visita(TypeDeclProgramNodo *typeDeclProgram);
        void visita(FunctionDeclProgramNodo    *functionDeclProgram);
        void visita(StatementListNodo   *statementList);
        void visita(StatementListTypeNodo *stmtListType);
        void visita(StatementListVarNodo  *stmtListVar);
        void visita(IfNodo              *If);
        void visita(WhileNodo           *While);
        void visita(SwitchNodo          *Switch);
        void visita(BreakNodo           *Break);
        void visita(PrintlnNodo         *print);
        void visita(ReadNodo            *read);
        void visita(ReturnNodo          *Return);
        void visita(ThrowNodo           *Throw);
        void visita(TryNodo             *Try);
        void visita(ExprStatementNodo   *exprStatement);
        void visita(IdNodo              *id);
        void visita(NumNodo             *num);
        void visita(LiteralNodo         *literal);
        void visita(CharNodo            *Char);
        void visita(BooleanNodo         *boolean);
        void visita(ExprListNodo        *exprList);
        void visita(CallNodo            *call);
        void visita(PointerValueExprNodo *pointerValueExpr);
        void visita(NameExprNodo        *nameExpr);
        void visita(AddressValueNodo    *addressValue);
        void visita(PointerValueNodo    *pointerValue);
        void visita(ArrayNodo           *array);
        void visita(RelationalOpNodo    *relationalOp);
        void visita(MenorNodo           *menor);
        void visita(MenorIgualNodo      *menorIgual);
        void visita(MaiorNodo           *maior);
        void visita(MaiorIgualNodo      *maiorIgual);
        void visita(IgualNodo           *igual);
        void visita(DiferenteNodo       *diferente);
        void visita(ELogicoNodo         *eLogico);
        void visita(OuLogicoNodo        *ouLogico);
        void visita(BinarioOpNodo       *binOp);
        void visita(AdditionOpNodo      *additionOp);
        void visita(SubtractionOpNodo   *subtractionOp);
        void visita(MultiplicationOpNodo *multiplicationOp);
        void visita(DivisionOpNodo      *divisionOp);
        void visita(ModuleOpNodo        *moduleOp);
        void visita(BooleanOpNodo       *booleanOp);
        void visita(BitWiseOpNodo       *bitWiseOp);
        void visita(NotNodo             *Not);
        void visita(SignNodo            *sign);
        void visita(AssignNodo          *assign);
        void visita(CaseBlockNodo       *caseblock);
};

#endif
