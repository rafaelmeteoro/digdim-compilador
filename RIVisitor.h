#ifndef _RIVISITOR_H
#define _RIVISITOR_H

class RIVisitor;
class VisitorRepIntImprime;

#include "representacaoIntermediaria.h"
#include "asa.h"

class RIVisitor{
    public:
        RIVisitor();
        virtual ~RIVisitor();
        virtual void visita(Temp             *t) = 0;
        virtual void visita(Label            *l) = 0;
        virtual void visita(StmtRI           *s) = 0;
        virtual void visita(StmtRIList       *sl) = 0;
        virtual void visita(ExprRI           *e) = 0;
        virtual void visita(ExprRIList       *el) = 0;
        virtual void visita(InFrame          *inF) = 0;
        virtual void visita(InReg            *inR) = 0;
        virtual void visita(ListaAcesso      *la) = 0;
        virtual void visita(FrameMIPS        *f) = 0;
        virtual void visita(Procedure        *p) = 0;
        virtual void visita(LiteralRI        *l) = 0;
        virtual void visita(VariavelNaoLocal *v) = 0;
        virtual void visita(LabelRI          *l) = 0;
        virtual void visita(ConstRI          *c) = 0;
        virtual void visita(ConstfRI         *c) = 0;
        virtual void visita(NameRI           *n) = 0;
        virtual void visita(TempRI           *t) = 0;
        virtual void visita(BinOpRI          *b) = 0;
        virtual void visita(MemRI            *m) = 0;
        virtual void visita(CallRI           *c) = 0;
        virtual void visita(ESeqRI           *es) = 0;
        virtual void visita(MoveRI           *m) = 0;
        virtual void visita(ExprStmtRI       *es) = 0;
        virtual void visita(JumpRI           *j) = 0;
        virtual void visita(CJumpRI          *cj) = 0;
        virtual void visita(SeqRI            *s) = 0;
};

/***************************************************************************
            VISITOR DE IMPRESSÃO DA REPRESENTAÇÃO INTERMEDIÁRIA
***************************************************************************/
class VisitorRepIntImprime : public RIVisitor {
private:
    int nivel;
    void imprime(char *text);
    void imprimePropriedade(char *obj, char *prop);
    void imprimePropriedadeInt(char *obj, int prop);
    void imprimePropriedadeFloat(char *obj, float prop);

public:
    VisitorRepIntImprime();
    ~VisitorRepIntImprime();

    void visita(Temp            *t);
    void visita(Label           *l);
    void visita(StmtRI          *s);
    void visita(StmtRIList      *sl);
    void visita(ExprRI          *e);
    void visita(ExprRIList      *el);
    void visita(InFrame         *inF);
    void visita(InReg           *inR);
    void visita(ListaAcesso     *la);
    void visita(FrameMIPS       *f);
    void visita(Procedure       *p);
    void visita(LiteralRI       *l);
    void visita(VariavelNaoLocal *v);
    void visita(LabelRI         *l);
    void visita(ConstRI         *c);
    void visita(ConstfRI        *c);
    void visita(NameRI          *n);
    void visita(TempRI          *t);
    void visita(BinOpRI         *b);
    void visita(MemRI           *m);
    void visita(CallRI          *c);
    void visita(ESeqRI          *es);
    void visita(MoveRI          *m);
    void visita(ExprStmtRI      *es);
    void visita(JumpRI          *j);
    void visita(CJumpRI         *cj);
    void visita(SeqRI           *s);
};

#endif
