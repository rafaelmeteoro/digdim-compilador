#ifndef _INSTRUCAOVISITOR_H
#define _INSTRUCAOVISITOR_H

class InstrucaoVisitor;

#include "instrucaoMIPS.h"

class InstrucaoVisitor {
    private:
        TempList* tempsDisponiveis;

        void devolveTemp(Temp *temp);
        Temp* pegaTemp();

    public:
        InstrucaoVisitor();
        virtual ~InstrucaoVisitor();

        virtual void visita(Temp                    *t);
        virtual void visita(Syscall                 *s);
        virtual void visita(Secao                   *s);
        virtual void visita(Globl                   *g);
        virtual void visita(IntDecl                 *g);
        virtual void visita(FloatDecl               *g);
        virtual void visita(Asciiz                  *a);
        virtual void visita(Unaria                  *u);
        virtual void visita(UnariaLabel             *ul);
        virtual void visita(UnariaRegister          *ur);
        virtual void visita(BinariaImmediate        *bi);
        virtual void visita(BinariaLabel            *bl);
        virtual void visita(BinariaRegister         *br);
        virtual void visita(TernariaMemoria         *tm);
        virtual void visita(TernariaRegister        *tr);
        virtual void visita(TernariaLabel           *tl);
        virtual void visita(TernariaLabelImmediate  *tli);
        virtual void visita(TernariaImmediate       *ti);
};

#endif
