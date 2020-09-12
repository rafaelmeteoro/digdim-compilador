#ifndef _CANONIZADOR_H
#define _CANONIZADOR_H

#include "representacaoIntermediaria.h"

class Canonizador;

class Canonizador {
private:
    ExprRI* processaExpr(ExprRI *expr);
    StmtRI* processaStmt(StmtRI *stmt);

public:
    Canonizador();
    virtual ~Canonizador();

     int mudancas;

    void visita(Procedure       *p);
    void visita(LiteralRI       *l);
    void visita(VariavelNaoLocal *v);
    void visita(Temp            *t);
    void visita(Label           *l);
    void visita(ListaAcesso     *la);
    void visita(InFrame         *inF);
    void visita(InReg           *inR);
    void visita(FrameMIPS       *f);
    void visita(ExprRI          *e);
    void visita(ExprRIList      *el);
    void visita(ConstRI         *c);
    void visita(ConstfRI        *cf);
    void visita(NameRI          *n);
    void visita(TempRI          *t);
    void visita(BinOpRI         *b);
    void visita(MemRI           *m);
    void visita(CallRI          *c);
    void visita(ESeqRI          *es);
    void visita(StmtRI          *s);
    void visita(StmtRIList      *sl);
    void visita(MoveRI          *m);
    void visita(ExprStmtRI      *es);
    void visita(JumpRI          *j);
    void visita(CJumpRI         *cj);
    void visita(SeqRI           *s);
    void visita(LabelRI         *l);
};

#endif
