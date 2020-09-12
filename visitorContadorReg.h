#ifndef _VISITORCONTADORREG_H
#define _VISITORCONTADORREG_H

#include "instrucaoVisitor.h"

class VisitorContadorReg : public InstrucaoVisitor{
    public:
        VisitorContadorReg();
        virtual ~VisitorContadorReg();

        void visita(Temp                    *t);
        void visita(Syscall                 *s);
        void visita(Secao                   *s);
        void visita(Globl                   *g);
        void visita(IntDecl                 *i);
        void visita(FloatDecl               *f);
        void visita(Asciiz                  *a);
        void visita(Unaria                  *u);
        void visita(UnariaLabel             *ul);
        void visita(UnariaRegister          *ur);
        void visita(BinariaImmediate        *bi);
        void visita(BinariaLabel            *bl);
        void visita(BinariaRegister         *br);
        void visita(TernariaMemoria         *tm);
        void visita(TernariaRegister        *tr);
        void visita(TernariaLabel           *tl);
        void visita(TernariaLabelImmediate  *tli);
        void visita(TernariaImmediate       *ti);
};

#endif
