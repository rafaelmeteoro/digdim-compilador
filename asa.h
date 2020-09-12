#ifndef _ASA_H
#define _ASA_H

#include "classesasa.h"
#include "visitor.h"
#include "representacaoIntermediaria.h"
#include "visitorRepIntermediaria.h"

class VisitorRepInt;

/*****************************************
            CLASSES VIRTUAIS
******************************************/
class Nodo{
    public:
        virtual void aceita(Visitor *visitor) = 0;
};

class ProgramNodo : public Nodo{
    public:
        int linha;

    public:
        ProgramNodo(int linha);
        virtual ~ProgramNodo();
        virtual void aceita(Visitor *visitor) = 0;
        virtual void aceitaRI(VisitorRepInt *rvisitor) = 0;
};

class StatementNodo : public Nodo{
    public:
        int linha;

    public:
        StatementNodo(int linha);
        virtual ~StatementNodo();
        virtual void aceita(Visitor *visitor) = 0;
        virtual StmtRI *aceitaRI(VisitorRepInt *rvisitor) = 0;
};

class ExprNodo : public Nodo{
    public:
        int         linha;
        TypeNodo    *type;
        NumNodo     *array;

    public:
        ExprNodo(int linha);
        virtual ~ExprNodo();
        virtual void aceita(Visitor *visitor) = 0;
        virtual ExprRI *aceitaRI(VisitorRepInt *rvisitor) = 0;
};

class TypeNodo : public Nodo{
    public:
        int tipo;
        int linha;

    public:
        TypeNodo(int tipo, int linha);
        virtual ~TypeNodo();
        void aceita(Visitor *visitor);
};

class TypeIdNodo : public TypeNodo{
    public:
        IdNodo *id;

    public:
        TypeIdNodo(IdNodo *id, int linha);
        virtual ~TypeIdNodo();
        void aceita(Visitor *visitor);
};

class TypePointerNodo : public TypeNodo{
    public:
        TypePointerNodo(int tipo, int linha);
        virtual ~TypePointerNodo();
        void aceita(Visitor *visitor);
};

/*****************************************
    CLASSES QUE HERDAM DE PROGRAMNODO
******************************************/
class IdListNodo : public Nodo{
    public:
        TypePointerNodo *pointer;
        IdNodo          *id;
        NumNodo         *array;
        IdListNodo      *idList;
        int             linha;

    public:
        IdListNodo(TypePointerNodo *pointer, IdNodo *id, NumNodo *array, IdListNodo *idList, int linha);
        virtual ~IdListNodo();
        void aceita(Visitor *visitor);
};

class VarDeclNodo : public ProgramNodo{
    public:
        TypeNodo    *tipo;
        IdListNodo  *idList;
        VarDeclNodo *varDecl;

    public:
        VarDeclNodo(TypeNodo *tipo, IdListNodo *idList, VarDeclNodo *varDecl, int linha);
        ~VarDeclNodo();
        void aceita(Visitor *visitor);
        void aceitaRI(VisitorRepInt *rvisitor);
};

class TypeDeclNodo : public ProgramNodo{
    public:
        VarDeclNodo  *varDecl;
        IdNodo       *id;
        TypeDeclNodo *typeDecl;

    public:
        TypeDeclNodo(VarDeclNodo *varDecl, IdNodo *id, TypeDeclNodo *typeDecl, int linha);
        ~TypeDeclNodo();
        void aceita(Visitor *visitor);
        void aceitaRI(VisitorRepInt *rvisitor);
};

class FunctionDeclNodo : public ProgramNodo{
    public:
        TypeNodo            *tipo;
        IdNodo              *id;
        VarDeclNodo         *varListParametros; //parametros
        VarDeclNodo         *varListVarLocais; //variaveis locais
        StatementListNodo   *stmtList;
        FunctionDeclNodo    *functionDecl;

    public:
        FunctionDeclNodo(TypeNodo *tipo, IdNodo *id, VarDeclNodo *varListParametros, VarDeclNodo *varListVarLocais, StatementListNodo *stmtList, FunctionDeclNodo* functionDecl, int linha);
        ~FunctionDeclNodo();
        void aceita(Visitor *visitor);
        void aceitaRI(VisitorRepInt *rvisitor);
};

class VarDeclProgramNodo : public ProgramNodo{
    public:
        VarDeclNodo *varDecl;
        ProgramNodo *program;

    public:
        VarDeclProgramNodo(VarDeclNodo *varDecl, ProgramNodo *program, int linha);
        ~VarDeclProgramNodo();
        void aceita(Visitor *visitor);
        void aceitaRI(VisitorRepInt *rvisitor);
};

class TypeDeclProgramNodo : public ProgramNodo{
    public:
        TypeDeclNodo *typeDecl;
        ProgramNodo  *program;

    public:
        TypeDeclProgramNodo(TypeDeclNodo *typeDecl, ProgramNodo *program, int linha);
        ~TypeDeclProgramNodo();
        void aceita(Visitor *visitor);
        void aceitaRI(VisitorRepInt *rvisitor);
};

class FunctionDeclProgramNodo : public ProgramNodo{
    public:
        FunctionDeclNodo *functionDecl;
        ProgramNodo      *program;

    public:
        FunctionDeclProgramNodo(FunctionDeclNodo *functionDecl, ProgramNodo *program, int linha);
        ~FunctionDeclProgramNodo();
        void aceita(Visitor *visitor);
        void aceitaRI(VisitorRepInt *rvisitor);
};

/*****************************************
    CLASSES QUE HERDAM DE STATEMENTNODO
******************************************/
class StatementListNodo : public StatementNodo{
    public:
        StatementNodo       *stmt;
        StatementListNodo   *stmtList;

    public:
        StatementListNodo(StatementNodo *stmt, StatementListNodo *stmtList, int linha);
        ~StatementListNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class StatementListTypeNodo : public StatementNodo{
    public:
        TypeDeclNodo        *typeDecl;
        StatementListNodo   *stmtList;

    public:
        StatementListTypeNodo(TypeDeclNodo *typeDecl, StatementListNodo *stmtList, int linha);
        ~StatementListTypeNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class StatementListVarNodo : public StatementNodo{
    public:
        StatementListNodo   *stmtList2;
        VarDeclNodo         *varDecl;
        StatementListNodo   *stmtListLinha;

    public:
        StatementListVarNodo(StatementListNodo *stmtList2, VarDeclNodo *varDecl, StatementListNodo *stmtListLinha, int linha);
        ~StatementListVarNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class IfNodo : public StatementNodo{
    public:
        ExprNodo        *expr;
        StatementNodo   *stmt1;
        StatementNodo   *stmt2;

    public:
        IfNodo(ExprNodo *expr, StatementNodo *stmt1, StatementNodo *stmt2, int linha);
        ~IfNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class WhileNodo : public StatementNodo{
    public:
        ExprNodo        *expr;
        StatementNodo   *stmt;

    public:
        WhileNodo(ExprNodo *expr, StatementNodo *stmt, int linha);
        ~WhileNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class SwitchNodo : public StatementNodo{
    public:
        ExprNodo        *expr;
        CaseBlockNodo   *caseBlock;

    public:
        SwitchNodo(ExprNodo *expr, CaseBlockNodo *caseBlock, int linha);
        ~SwitchNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class BreakNodo : public StatementNodo{
    public:
        BreakNodo(int linha);
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class PrintlnNodo : public StatementNodo{
    public:
        ExprListNodo *exprList;

    public:
        PrintlnNodo(ExprListNodo *exprList, int linha);
        ~PrintlnNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class ReadNodo : public StatementNodo{
    public:
        ExprNodo *expr;

    public:
        ReadNodo(ExprNodo *expr, int linha);
        ~ReadNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class ReturnNodo : public StatementNodo{
    public:
        ExprNodo *expr;

    public:
        ReturnNodo(ExprNodo *expr, int linha);
        ~ReturnNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class ThrowNodo : public StatementNodo{
    public:
        ThrowNodo(int linha);
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class TryNodo : public StatementNodo{
    public:
        StatementNodo *stmt1; //try
        StatementNodo *stmt2; //catch

    public:
        TryNodo(StatementNodo *stmt1, StatementNodo *stmt2, int linha);
        ~TryNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class ExprStatementNodo : public StatementNodo{
    public:
        ExprNodo *expr;

    public:
        ExprStatementNodo(ExprNodo *expr, int linha);
        ~ExprStatementNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

/******************************************
        CLASSES QUE HERDAM DE EXPRNODO
******************************************/
class IdNodo : public ExprNodo{
    public:
        char *nome;
        bool global;

    public:
        IdNodo(char *nome, int linha);
        ~IdNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class NumNodo : public ExprNodo{
    public:
        int     tipo;
        char    *valor;

    public:
        NumNodo(int tipo, char *valor, int linha);
        ~NumNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class LiteralNodo : public ExprNodo{
    public:
        char *literal;

    public:
        LiteralNodo(char *literal, int linha);
        ~LiteralNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class CharNodo : public ExprNodo{
    public:
        char *Char;

    public:
        CharNodo(char *Char, int linha);
        ~CharNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class BooleanNodo : public ExprNodo{
    public:
        int boolean;

    public:
        BooleanNodo(int boolean, int linha);
        ~BooleanNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class ExprListNodo : public Nodo{
    public:
        int             linha;
        ExprNodo        *expr;
        ExprListNodo    *exprList;

    public:
        ExprListNodo(ExprNodo *expr, ExprListNodo *exprList, int linha);
        virtual ~ExprListNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

class CallNodo : public ExprNodo{
    public:
        IdNodo          *id;
        ExprListNodo    *exprList;
        ExprNodo        *expr;

    public:
        CallNodo(IdNodo *id, ExprListNodo *exprList, ExprNodo *expr, int linha);
        ~CallNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class PointerValueExprNodo : public ExprNodo{
    public:
        IdNodo      *id;
        ExprNodo    *expr;

    public:
        PointerValueExprNodo(IdNodo *id, ExprNodo *expr, int linha);
        ~PointerValueExprNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class NameExprNodo : public ExprNodo{
    public:
        IdNodo      *id;
        ExprNodo    *expr;

    public:
        NameExprNodo(IdNodo *id, ExprNodo *expr, int linha);
        ~NameExprNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class AddressValueNodo : public ExprNodo{
    public:
        ExprNodo *expr;

    public:
        AddressValueNodo(ExprNodo *expr, int linha);
        ~AddressValueNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class PointerValueNodo : public ExprNodo{
    public:
        ExprNodo *expr;

    public:
        PointerValueNodo(ExprNodo *expr, int linha);
        ~PointerValueNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class ArrayNodo : public ExprNodo{
    public:
        ExprNodo *expr;
        ExprNodo *indice;

    public:
        ArrayNodo(ExprNodo *expr, ExprNodo *indice, int linha);
        ~ArrayNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class RelationalOpNodo : public ExprNodo{
    public:
        int      op_rel;
        ExprNodo *expr1;
        ExprNodo *expr2;

    public:
        RelationalOpNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~RelationalOpNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class MenorNodo : public RelationalOpNodo{
    public:
        MenorNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~MenorNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class MenorIgualNodo : public RelationalOpNodo{
    public:
        MenorIgualNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~MenorIgualNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class MaiorNodo : public RelationalOpNodo{
    public:
        MaiorNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~MaiorNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class MaiorIgualNodo : public RelationalOpNodo{
    public:
        MaiorIgualNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~MaiorIgualNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class IgualNodo : public RelationalOpNodo{
    public:
        IgualNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~IgualNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class DiferenteNodo : public RelationalOpNodo{
    public:
        DiferenteNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~DiferenteNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class ELogicoNodo : public RelationalOpNodo{
    public:
        ELogicoNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~ELogicoNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class OuLogicoNodo : public RelationalOpNodo{
    public:
        OuLogicoNodo(int op_rel, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~OuLogicoNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class BinarioOpNodo : public ExprNodo{
    public:
        int      op;
        ExprNodo *expr1;
        ExprNodo *expr2;

    public:
        BinarioOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~BinarioOpNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class AdditionOpNodo : public BinarioOpNodo{
    public:
        AdditionOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~AdditionOpNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class SubtractionOpNodo : public BinarioOpNodo{
    public:
        SubtractionOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~SubtractionOpNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class MultiplicationOpNodo : public BinarioOpNodo{
    public:
        MultiplicationOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~MultiplicationOpNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class DivisionOpNodo : public BinarioOpNodo{
    public:
        DivisionOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~DivisionOpNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class ModuleOpNodo : public BinarioOpNodo{
    public:
        ModuleOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~ModuleOpNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class BooleanOpNodo : public BinarioOpNodo{
    public:
        BooleanOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~BooleanOpNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class BitWiseOpNodo : public BinarioOpNodo{
    public:
        BitWiseOpNodo(int op, ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~BitWiseOpNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class NotNodo : public ExprNodo{
    public:
        ExprNodo *expr;

    public:
        NotNodo(ExprNodo *expr, int linha);
        ~NotNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class SignNodo : public ExprNodo{
    public:
        int operador;
        ExprNodo *expr;

    public:
        SignNodo(int operador, ExprNodo *expr, int linha);
        ~SignNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class AssignNodo : public ExprNodo{
    public:
        ExprNodo *expr1;
        ExprNodo *expr2;

    public:
        AssignNodo(ExprNodo *expr1, ExprNodo *expr2, int linha);
        ~AssignNodo();
        void aceita(Visitor *visitor);
        ExprRI *aceitaRI(VisitorRepInt *rvisitor);
};

class CaseBlockNodo : public Nodo{
    public:
        NumNodo             *num;
        StatementListNodo   *stmtList;
        CaseBlockNodo       *caseBlock;
        int                 linha;

    public:
        CaseBlockNodo(NumNodo *num, StatementListNodo *stmtList, CaseBlockNodo *caseBlock, int linha);
        virtual ~CaseBlockNodo();
        void aceita(Visitor *visitor);
        StmtRI *aceitaRI(VisitorRepInt *rvisitor);
};

#endif
