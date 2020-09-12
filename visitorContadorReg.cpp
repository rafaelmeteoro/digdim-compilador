#include <stdio.h>
#include <typeinfo>

#include "visitorContadorReg.h"

VisitorContadorReg::VisitorContadorReg() : InstrucaoVisitor(){}

VisitorContadorReg::~VisitorContadorReg(){}

void VisitorContadorReg::visita(Temp *t){
    t->usado++;
}

void VisitorContadorReg::visita(Syscall *s){
    InstrucaoVisitor::visita(s);
}

void VisitorContadorReg::visita(Secao *s){
    InstrucaoVisitor::visita(s);
}

void VisitorContadorReg::visita(Globl *g){
    InstrucaoVisitor::visita(g);
}

void VisitorContadorReg::visita(IntDecl *i){
    InstrucaoVisitor::visita(i);
}

void VisitorContadorReg::visita(FloatDecl *f){
    InstrucaoVisitor::visita(f);
}

void VisitorContadorReg::visita(Asciiz *a){
    InstrucaoVisitor::visita(a);
}

void VisitorContadorReg::visita(Unaria *u){
    InstrucaoVisitor::visita(u);
}

void VisitorContadorReg::visita(UnariaLabel *ul){
    InstrucaoVisitor::visita(ul);
}

void VisitorContadorReg::visita(UnariaRegister *ur){
    InstrucaoVisitor::visita(ur);
}

void VisitorContadorReg::visita(BinariaImmediate *bi){
    InstrucaoVisitor::visita(bi);
}

void VisitorContadorReg::visita(BinariaLabel *bl){
    InstrucaoVisitor::visita(bl);
}

void VisitorContadorReg::visita(BinariaRegister *br){
    InstrucaoVisitor::visita(br);
}

void VisitorContadorReg::visita(TernariaMemoria *tm){
    InstrucaoVisitor::visita(tm);
}

void VisitorContadorReg::visita(TernariaRegister *tr){
    InstrucaoVisitor::visita(tr);
}

void VisitorContadorReg::visita(TernariaLabel *tl){
    InstrucaoVisitor::visita(tl);
}

void VisitorContadorReg::visita(TernariaLabelImmediate *tli){
    InstrucaoVisitor::visita(tli);
}

void VisitorContadorReg::visita(TernariaImmediate *ti){
    InstrucaoVisitor::visita(ti);
}
