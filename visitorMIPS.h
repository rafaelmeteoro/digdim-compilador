#ifndef _VISITORMIPS_H
#define _VISITORMIPS_H

#include <stdio.h>

class VisitorMIPS;

#include "instrucaoMIPS.h"

class VisitorMIPS {
    private:
        int offsetReg;

        void addInstrucao(InstrucaoMIPS *i);
        bool iniciarCom(char *string, char *padrao);

    public:
        InstrucaoMIPS *primeiraInstrucao;

        VisitorMIPS();
        virtual ~VisitorMIPS();

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
        Temp* visita(ConstRI        *c);
        Temp* visita(ConstfRI       *c);
        Temp* visita(NameRI         *n);
        Temp* visita(TempRI         *t);
        Temp* visita(BinOpRI        *b);
        Temp* visita(MemRI          *m);
        Temp* visita(CallRI         *c);
        Temp* visita(ESeqRI         *es);
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
