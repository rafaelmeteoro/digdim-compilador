#ifndef _VISITORREPINTERMEDIARIA_H
#define _VISITORREPINTERMEDIARIA_H

#include "representacaoIntermediaria.h"
#include "tabelaSemantico.h"
#include "asa.h"
#include "lexico.h"

class VisitorRepInt;
class TabelaSemantico;

/*****************************************************************
            VISITOR DA REPRESENTAÇÃO INTERMEDIÁRIA
*****************************************************************/

class VisitorRepInt {
    public:
        bool naFuncao, naListaParametros, naListaTipos;
        int qtdLiterais;
        int contadorParametros;
        TempList        *caseBlockValue;
        ExprRIListList  *argumentosCall;
        TabelaSemantico *tabela;
        LabelList       *listaFinaisLoop;
        TempList        *switchExprs;
        Fragment        *primeiroFragmento;

        int tamanhoTipo(TypeNodo *tipo);
        int calculaOffset(TypeNodo *tipo, char *nome);
        void adicionaFragmento(Fragment *fragmento);
        void adicionaArgumentoCall(ExprRI *exp);

        VariavelNaoLocal *novaVariavel(TypeNodo *tipo, NumNodo *num);
        StmtRI *constroiArvoreImpressao(ExprRIList *expList);
        Fragment *ultimoFragmento();


    public:
        VisitorRepInt(TabelaSemantico *tabela);
        virtual ~VisitorRepInt();

        void visita(TypeDeclProgramNodo     *tdpn);
        void visita(FunctionDeclNodo        *fdn);
        void visita(FunctionDeclProgramNodo *fdpn);
        void visita(VarDeclProgramNodo      *vdpn);
        void visita(TypeNodo                *tn);
        void visita(TypeIdNodo              *tin);
        void visita(TypeDeclNodo            *tdn);
        void visita(VarDeclNodo             *vdn);
        MemRI *visita(IdNodo                *id);
        StmtRI* visita(StatementListNodo    *sln);
        StmtRI* visita(StatementListTypeNodo *sltn);
        StmtRI* visita(StatementListVarNodo *slvn);
        StmtRI* visita(IfNodo               *ifn);
        StmtRI* visita(WhileNodo            *w);
        StmtRI* visita(SwitchNodo           *sw);
        StmtRI* visita(BreakNodo            *br);
        StmtRI* visita(PrintlnNodo          *pr);
        StmtRI* visita(ReadNodo             *re);
        StmtRI* visita(ReturnNodo           *ret);
        StmtRI* visita(ThrowNodo            *thr);
        StmtRI* visita(TryNodo              *tryn);
        StmtRI* visita(ExprStatementNodo    *esn);
        ExprRI* visita(ExprNodo             *e);
        ExprRI* visita(AssignNodo           *an);
        ExprRI* visita(OuLogicoNodo         *oulog);
        ExprRI* visita(ELogicoNodo          *elog);
        ExprRI* visita(IgualNodo            *igual);
        ExprRI* visita(DiferenteNodo        *dif);
        ExprRI* visita(RelationalOpNodo     *rel);
        ExprRI* visita(MenorNodo            *menor);
        ExprRI* visita(MenorIgualNodo       *menorI);
        ExprRI* visita(MaiorNodo            *maior);
        ExprRI* visita(MaiorIgualNodo       *maiorI);
        ExprRI* visita(BinarioOpNodo        *bin);
        ExprRI* visita(AdditionOpNodo       *add);
        ExprRI* visita(SubtractionOpNodo    *sub);
        ExprRI* visita(BitWiseOpNodo        *bit);
        ExprRI* visita(MultiplicationOpNodo *mult);
        ExprRI* visita(DivisionOpNodo       *div);
        ExprRI* visita(ModuleOpNodo         *mod);
        ExprRI* visita(NotNodo              *notn);
        ExprRI* visita(SignNodo             *sig);
        ExprRI* visita(NumNodo              *num);
        ExprRI* visita(CharNodo             *ch);
        ExprRI* visita(BooleanNodo          *bo);
        ExprRI* visita(PointerValueExprNodo *p);
        ExprRI* visita(NameExprNodo         *n);
        ExprRI* visita(AddressValueNodo     *a);
        ExprRI* visita(PointerValueNodo     *p);
        MemRI* visita(ArrayNodo             *array);
        StmtRI* visita(ExprListNodo         *el);
        ExprRI* visita(CallNodo             *call);
        ExprRI* visita(LiteralNodo          *literal);
        StmtRI* visita(CaseBlockNodo        *cb);
};

#endif
