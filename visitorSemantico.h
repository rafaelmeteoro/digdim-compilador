#ifndef _VISITORSEMANTICO_H
#define _VISITORSEMANTICO_H

#include "asa.h"
#include "tabelaSemantico.h"

class VisitorSemantico : public Visitor{     //VISITOR SEMANTICO HERDA DE VISITOR
    public:
        int escopo;
        int loop;
        TabelaSemantico *tabelaTipos;
        TabelaSemantico *tabelaEscopo;
        TypeNodo        *tipoFuncao;
        VarDeclNodo     *parametrosFuncao;
        bool            idFuncao;
        bool            idTipo;
        bool            verificaparametros;
        bool            temMain;
        bool            mParametrosFuncao;

    public:
        VisitorSemantico();     //CONSTRUTOR
        virtual ~VisitorSemantico();   //DESTRUTOR

        bool verificaOpBin(BinarioOpNodo *binOp, bool forcar);
        bool verificarLogOp(BinarioOpNodo *binOp);
        bool verificaOpBin(RelationalOpNodo *relOp, bool forcar);
        bool verificarLogOp(RelationalOpNodo *relOp);
        //COLOCAR OS OUTROS MÉTODOS QUE SERÃO IMPLEMENTADOS

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
