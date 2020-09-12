#include <stdlib.h>
#include <stdio.h>

#include "instrucaoVisitor.h"
#include "erro.h"

InstrucaoVisitor::InstrucaoVisitor() {
    tempsDisponiveis = new TempList(new Temp((char *) "$t0"), new TempList(new Temp((char *) "$t1"),
                                    new TempList(new Temp((char *) "$t2"), new TempList(new Temp((char *) "$t3"),
                                    new TempList(new Temp((char *) "$t4"), new TempList(new Temp((char *) "$t5"),
                                    new TempList(new Temp((char *) "$t6"), new TempList(new Temp((char *) "$t7"),
                                    new TempList(new Temp((char *) "$t8"), new TempList(new Temp((char *) "$t9"),
                                    NULL))))))))));
}

InstrucaoVisitor::~InstrucaoVisitor() {
    delete tempsDisponiveis;
}

void InstrucaoVisitor::devolveTemp(Temp *temp) {
    Temp *tempLista = new Temp(temp->nome);
    tempsDisponiveis = new TempList(tempLista, tempsDisponiveis);
}

Temp *InstrucaoVisitor::pegaTemp() {
    if(!tempsDisponiveis) {
        fprintf(stdout, "ERRO: NAO HA TEMPORARIOS DISPONIVEIS.\n");
    }

    Temp *temp = tempsDisponiveis->primeiro;
    TempList *tempDel = tempsDisponiveis;
    tempsDisponiveis = tempsDisponiveis->proximo;
    tempDel->primeiro = NULL;
    tempDel->proximo = NULL;
    delete tempDel;
    return temp;
}

void InstrucaoVisitor::visita(Temp *t) {
    if (t->troca) {
        free(t->nome);
        Temp *novoTemp = pegaTemp();
        t->nome = novoTemp->nome;
        novoTemp->nome = NULL;
        delete novoTemp;
        t->reuso = true;
        t->troca = false;
    }

    t->usado--;
    if(t->usado == 0 && t->reuso) devolveTemp(t);
}

void InstrucaoVisitor::visita(Syscall *s) {
    if(s->proximo) s->proximo->aceita(this);
}

void InstrucaoVisitor::visita(Secao *s) {
    if(s->proximo) s->proximo->aceita(this);
}

void InstrucaoVisitor::visita(Globl *g) {
    if(g->proximo) g->proximo->aceita(this);
}

void InstrucaoVisitor::visita(IntDecl *i) {
    if(i->proximo) i->proximo->aceita(this);
}

void InstrucaoVisitor::visita(FloatDecl *f) {
    if(f->proximo) f->proximo->aceita(this);
}

void InstrucaoVisitor::visita(Asciiz *a) {
    if(a->proximo) a->proximo->aceita(this);
}

void InstrucaoVisitor::visita(Unaria *u) {
    if(u->proximo) u->proximo->aceita(this);
}

void InstrucaoVisitor::visita(UnariaLabel *ul) {
    if(ul->proximo) ul->proximo->aceita(this);
}

void InstrucaoVisitor::visita(UnariaRegister *ur) {
    if(ur->reg)     ur->reg->aceita(this);
    if(ur->proximo) ur->proximo->aceita(this);
}

void InstrucaoVisitor::visita(BinariaImmediate *bi) {
    if(bi->reg)     bi->reg->aceita(this);
    if(bi->proximo) bi->proximo->aceita(this);
}

void InstrucaoVisitor::visita(BinariaLabel *bl) {
    if(bl->reg)     bl->reg->aceita(this);
    if(bl->proximo) bl->proximo->aceita(this);
}

void InstrucaoVisitor::visita(BinariaRegister *br) {
    if(br->reg1)    br->reg1->aceita(this);
    if(br->reg2)    br->reg2->aceita(this);
    if(br->proximo) br->proximo->aceita(this);
}

void InstrucaoVisitor::visita(TernariaMemoria *tm) {
    if(tm->reg1)    tm->reg1->aceita(this);
    if(tm->reg2)    tm->reg2->aceita(this);
    if(tm->proximo) tm->proximo->aceita(this);
}

void InstrucaoVisitor::visita(TernariaRegister *tr) {
    if(tr->reg1)    tr->reg1->aceita(this);
    if(tr->reg2)    tr->reg2->aceita(this);
    if(tr->reg3)    tr->reg3->aceita(this);
    if(tr->proximo) tr->proximo->aceita(this);
}

void InstrucaoVisitor::visita(TernariaLabel *tl) {
    if(tl->reg1)    tl->reg1->aceita(this);
    if(tl->reg2)    tl->reg2->aceita(this);
    if(tl->proximo) tl->proximo->aceita(this);
}

void InstrucaoVisitor::visita(TernariaLabelImmediate *tli) {
    if(tli->reg1)    tli->reg1->aceita(this);
    if(tli->proximo) tli->proximo->aceita(this);
}

void InstrucaoVisitor::visita(TernariaImmediate *tl) {
    if(tl->reg1)    tl->reg1->aceita(this);
    if(tl->reg2)    tl->reg2->aceita(this);
    if(tl->proximo) tl->proximo->aceita(this);
}
