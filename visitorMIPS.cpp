#include <string.h>
#include <typeinfo>

#include "visitorMIPS.h"

VisitorMIPS::VisitorMIPS() {
    primeiraInstrucao = NULL;
}

VisitorMIPS::~VisitorMIPS() {
    delete primeiraInstrucao;
}

void VisitorMIPS::addInstrucao(InstrucaoMIPS *i) {
    if (primeiraInstrucao) primeiraInstrucao->add(i);
    else primeiraInstrucao = i;
}

bool VisitorMIPS::iniciarCom(char *string, char *padrao) {
    char *t = string;
    char *t2 = padrao;
    for (; *(t) != 0 && *(t2) == *(t); t++, t2++);
    return *(t2) == 0;
}

void VisitorMIPS::visita(Procedure *p) {
    p->frame->aceita(this);
    p->body->aceita(this);
    if (p->proximo) p->proximo->aceita(this);
}

void VisitorMIPS::visita(LiteralRI *l) {
    addInstrucao(new Secao((char*)"rdata"));
    addInstrucao(new Unaria(l->nome->nome));
    addInstrucao(new Asciiz(l->literal->literal));
    if (l->proximo) l->proximo->aceita(this);
}

void VisitorMIPS::visita(VariavelNaoLocal *v) {
    if (v->nome) {
        addInstrucao(new Secao((char*)"data"));
        addInstrucao(new Globl(v->nome->nome));
        addInstrucao(new Unaria(v->nome));
    }
    if (v->tipo == INT) addInstrucao(new IntDecl(v->quantidade));
    if (v->tipo == FLOAT) addInstrucao(new FloatDecl(v->quantidade));

    if (v->proximo) v->proximo->aceita(this);
}

void VisitorMIPS::visita(Temp *t) {
}

void VisitorMIPS::visita(Label *l) {
}

void VisitorMIPS::visita(ListaAcesso *la) {
}

void VisitorMIPS::visita(InFrame *inF) {
}

void VisitorMIPS::visita(InReg *inR) {
}

void VisitorMIPS::visita(FrameMIPS *f) {
    addInstrucao(new Secao((char*)"text"));
    addInstrucao(new Globl(f->rotulo->nome));
    addInstrucao(new Unaria(f->rotulo));
    //aloca frame
    addInstrucao(new TernariaImmediate((char*)"subu", new Temp((char*)"$sp"), new Temp((char*)"$sp"), f->tamanhoFrame));
    //guarda o $fp anterior
    addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$fp"), f->tamanhoFrame - 4, new Temp((char*)"$sp")));
    //calcula o novo $fp
    addInstrucao(new TernariaImmediate((char*) "add", new Temp((char*)"$fp"), new Temp((char*)"$sp"), f->tamanhoFrame));
    //guarda $ra anterior
    addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*) "$ra"), -8, new Temp((char*)"$fp")));

    offsetReg = f->offsetLocal;
}

void VisitorMIPS::visita(ExprRIList *el) {
}

void VisitorMIPS::visita(ExprRI *e) {
}

Temp* VisitorMIPS::visita(ConstRI *c) {
    if (c->valor == 0) {
        return new Temp((char*)"$zero");
    } else {
        Temp *t = new Temp();
        addInstrucao(new BinariaImmediate((char*)"li", t, c->valor));
        return t;
    }
}

Temp* VisitorMIPS::visita(ConstfRI *c) {
    return new Temp();
}

Temp* VisitorMIPS::visita(NameRI* n) {
    Temp *t = new Temp();
    addInstrucao(new BinariaLabel((char*)"la", t, n->nome));
    return t;
}

Temp* VisitorMIPS::visita(TempRI *t) {
    return t->temporario;
}

Temp* VisitorMIPS::visita(BinOpRI *b) {
    Temp *t = new Temp();
    ConstRI *c;
    switch (b->operador) {
        case PLUS:
            if ((c = dynamic_cast<ConstRI*>(b->exp1))) {
                Temp *t2 = b->exp2->aceita(this);
                addInstrucao(new TernariaImmediate((char*)"add", t, t2, c->valor));
            } else if ((c = dynamic_cast<ConstRI*>(b->exp2))) {
                Temp *t1 = b->exp1->aceita(this);
                addInstrucao(new TernariaImmediate((char*)"add", t, t1, c->valor));
            } else {
                Temp *t1 = b->exp1->aceita(this);
                Temp *t2 = b->exp2->aceita(this);
                addInstrucao(new TernariaRegister((char*)"add", t, t1, t2));
            }
            break;
        case MINUS:
            if ((c = dynamic_cast<ConstRI*>(b->exp2))) {
                Temp *t1 = b->exp1->aceita(this);
                addInstrucao(new TernariaImmediate((char*)"sub", t, t1, c->valor));
            } else {
                Temp *t1 = b->exp1->aceita(this);
                Temp *t2 = b->exp2->aceita(this);
                addInstrucao(new TernariaRegister((char *)"sub", t, t1, t2));
            }
            break;
        case MUL:
            if ((c = dynamic_cast<ConstRI*>(b->exp1))) {
                Temp *t2 = b->exp2->aceita(this);
                addInstrucao(new TernariaImmediate((char*)"mulo", t, t2, c->valor));
            } else if ((c = dynamic_cast<ConstRI*>(b->exp2))) {
                Temp *t1 = b->exp1->aceita(this);
                addInstrucao(new TernariaImmediate((char*)"mulo", t, t1, c->valor));
            } else {
                Temp *t1 = b->exp1->aceita(this);
                Temp *t2 = b->exp2->aceita(this);
                addInstrucao(new TernariaRegister((char*)"mulo", t, t1, t2));
            }
            break;
        case DIV:
            if ((c = dynamic_cast<ConstRI*>(b->exp2))) {
                Temp *t1 = b->exp1->aceita(this);
                addInstrucao(new TernariaImmediate((char*)"div", t, t1, c->valor));
            } else {
                Temp *t1 = b->exp1->aceita(this);
                Temp *t2 = b->exp2->aceita(this);
                addInstrucao(new TernariaRegister((char*)"div", t, t1, t2));
            }
            break;
        case AND:
            if ((c = dynamic_cast<ConstRI*>(b->exp1))) {
                Temp *t2 = b->exp2->aceita(this);
                addInstrucao(new TernariaImmediate((char*)"and", t, t2, c->valor));
            } else if ((c = dynamic_cast<ConstRI*> (b->exp2))) {
                Temp *t1 = b->exp1->aceita(this);
                addInstrucao(new TernariaImmediate((char*)"and", t, t1, c->valor));
            } else {
                Temp *t1 = b->exp1->aceita(this);
                Temp *t2 = b->exp2->aceita(this);
                addInstrucao(new TernariaRegister((char*)"and", t, t1, t2));
            }
            break;
        case OR:
            if ((c = dynamic_cast<ConstRI*>(b->exp1))) {
                Temp *t2 = b->exp2->aceita(this);
                addInstrucao(new TernariaImmediate((char*)"or", t, t2, c->valor));
            } else if ((c = dynamic_cast<ConstRI*> (b->exp2))) {
                Temp *t1 = b->exp1->aceita(this);
                addInstrucao(new TernariaImmediate((char*)"or", t, t1, c->valor));
            } else {
                Temp *t1 = b->exp1->aceita(this);
                Temp *t2 = b->exp2->aceita(this);
                addInstrucao(new TernariaRegister((char *)"or", t, t1, t2));
            }
            break;
    }
    return t;
}

Temp* VisitorMIPS::visita(MemRI *m) {
    if (typeid(*m->exp) == typeid(BinOpRI)) {
        BinOpRI *binop = (BinOpRI*)m->exp;
        if (binop->operador == PLUS) {
            if (typeid(*binop->exp1) == typeid(ConstRI)) {
                ConstRI *c = (ConstRI*)binop->exp1;
                Temp *rb = binop->exp2->aceita(this);
                Temp *rd = new Temp();
                addInstrucao(new TernariaMemoria((char*)"lw", rd, c->valor, rb));
                return rd;
            }
            if (typeid(*binop->exp2) == typeid(ConstRI)) {
                ConstRI *c = (ConstRI*)binop->exp2;
                Temp *rb = binop->exp1->aceita(this);
                Temp *rd = new Temp();
                addInstrucao(new TernariaMemoria((char*)"lw", rd, c->valor, rb));
               return rd;
            }
        }
    }

    if (typeid(*m->exp) == typeid(ConstRI)) {
        ConstRI *c = (ConstRI*)m->exp;
        Temp *rd = new Temp();
        addInstrucao(new TernariaMemoria((char*)"lw", rd, c->valor, new Temp((char*)"$zero")));
        return rd;
    }

    Temp *rb = m->exp->aceita(this);
    Temp *rd = new Temp();
    addInstrucao(new TernariaMemoria((char*)"lw", rd, 0, rb));
    return rd;
}

Temp* VisitorMIPS::visita(CallRI *c) {
    Temp *temp;
    if (iniciarCom(c->funcao->nome, (char*)"print_")) {
        int tipo = ((TempRI*)c->argumentos->primeiro)->type;
        temp = ((TempRI*)c->argumentos->primeiro)->temporario;
        if (tipo == FLOAT) {
            addInstrucao(new BinariaImmediate((char*)"li", new Temp((char*)"$v0"), 2));
            addInstrucao(new BinariaRegister((char*) "move", new Temp((char*) "$f12"), temp));
        } else if (tipo == STRING) {
            addInstrucao(new BinariaImmediate((char*)"li", new Temp((char*)"$v0"), 4));
            addInstrucao(new BinariaRegister((char*)"move", new Temp((char*)"$a0"), temp));
        } else {
            addInstrucao(new BinariaImmediate((char*)"li", new Temp((char*)"$v0"), 1));
            addInstrucao(new BinariaRegister((char*)"move", new Temp((char*)"$a0"), temp));
        }
        addInstrucao(new Syscall());
    } else if (iniciarCom(c->funcao->nome, (char*)"readln_")) {
        temp = c->argumentos->primeiro->aceita(this);
        //só le inteiros
        addInstrucao(new BinariaImmediate((char*)"li", new Temp((char*)"$v0"), 5));
        addInstrucao(new Syscall());
        addInstrucao(new BinariaRegister((char*)"move", temp, new Temp((char*)"$v0")));
    } else {
        //guarda temps
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$t0"), offsetReg - 4, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$t1"), offsetReg - 8, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$t2"), offsetReg - 12, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$t3"), offsetReg - 16, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$t4"), offsetReg - 20, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$t5"), offsetReg - 24, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$t6"), offsetReg - 28, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$t7"), offsetReg - 32, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$t8"), offsetReg - 36, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$t9"), offsetReg - 40, new Temp((char*)"$fp")));
        //guarda retornos
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$v0"), offsetReg - 44, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$v1"), offsetReg - 48, new Temp((char*)"$fp")));
        //guarda argumentos
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$a0"), offsetReg - 52, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$a1"), offsetReg - 56, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$a2"), offsetReg - 60, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$a3"), offsetReg - 64, new Temp((char*)"$fp")));
        //guarda saved-values
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$s0"), offsetReg - 68, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$s1"), offsetReg - 72, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$s2"), offsetReg - 76, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$s3"), offsetReg - 80, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$s4"), offsetReg - 84, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$s5"), offsetReg - 88, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$s6"), offsetReg - 92, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"sw", new Temp((char*)"$s7"), offsetReg - 96, new Temp((char*)"$fp")));

        ExprRIList *params = c->argumentos;
        int deslocamento = 0;
        while (params) {
            addInstrucao(new TernariaMemoria((char*)"sw", params->primeiro->aceita(this), deslocamento, new Temp((char*)"$sp")));
            deslocamento += 4;
            params = params->proximo;
        }
        addInstrucao(new UnariaLabel((char*)"jal", c->funcao));

        //recupera temps
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$t0"), offsetReg - 4, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$t1"), offsetReg - 8, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$t2"), offsetReg - 12, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$t3"), offsetReg - 16, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$t4"), offsetReg - 20, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$t5"), offsetReg - 24, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$t6"), offsetReg - 28, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$t7"), offsetReg - 32, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$t8"), offsetReg - 36, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$t9"), offsetReg - 40, new Temp((char*)"$fp")));

        //guarda retorno
        temp = new Temp();
        addInstrucao(new BinariaRegister((char*)"move", temp, new Temp((char*)"$v0")));

        //recupera retornos
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$v0"), offsetReg - 44, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$v1"), offsetReg - 48, new Temp((char*)"$fp")));
        //recupera argumentos
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$a0"), offsetReg - 52, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$a1"), offsetReg - 56, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$a2"), offsetReg - 60, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$a3"), offsetReg - 64, new Temp((char*)"$fp")));
        //recupera saved-values
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$s0"), offsetReg - 68, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$s1"), offsetReg - 72, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$s2"), offsetReg - 76, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$s3"), offsetReg - 80, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$s4"), offsetReg - 84, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$s5"), offsetReg - 88, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$s6"), offsetReg - 92, new Temp((char*)"$fp")));
        addInstrucao(new TernariaMemoria((char*)"lw", new Temp((char*)"$s7"), offsetReg - 96, new Temp((char*)"$fp")));
    }
    return temp;
}

Temp* VisitorMIPS::visita(ESeqRI *es) {
    es->stmt->aceita(this);
    return es->exp->aceita(this);
}

void VisitorMIPS::visita(StmtRIList *sl) {
}

void VisitorMIPS::visita(StmtRI *s) {
}

void VisitorMIPS::visita(MoveRI *m) {
    if(m->destino && m->origem){
        if (typeid(*m->destino) == typeid(TempRI) && typeid(*m->origem) == typeid(ConstRI)) {
            addInstrucao(new BinariaImmediate((char*)"li", ((TempRI*)m->destino)->aceita(this), ((ConstRI*)m->origem)->valor));
            return;
        }
    }
    if (typeid(*m->destino) == typeid(MemRI)) {
        MemRI *mem = (MemRI*)m->destino;
        if (typeid(*mem->exp) == typeid(BinOpRI)) {
            BinOpRI *binop = (BinOpRI*)mem->exp;
            if (binop->operador == PLUS) {
                if (typeid(*binop->exp1) == typeid(ConstRI)) {
                    ConstRI *c = (ConstRI*)binop->exp1;
                    Temp *tb = binop->exp2->aceita(this);
                    Temp *ts = m->origem->aceita(this);
                    addInstrucao(new TernariaMemoria((char*)"sw", ts, c->valor, tb));
                    return;
                }
                if (typeid (*binop->exp2) == typeid (ConstRI)) {
                    ConstRI *c = (ConstRI*)binop->exp2;
                    Temp *tb = binop->exp1->aceita(this);
                    Temp *ts = m->origem->aceita(this);
                    addInstrucao(new TernariaMemoria((char*)"sw", ts, c->valor, tb));
                    return;
                }
            }
        }
        if (typeid(*mem->exp) == typeid(ConstRI)) {
            ConstRI *c = (ConstRI*)mem->exp;
            Temp *ts = m->origem->aceita(this);
            addInstrucao(new TernariaMemoria((char*)"sw", ts, c->valor, new Temp((char*)"$zero")));
            return;
        }
        Temp *tb = mem->exp->aceita(this);
        Temp *ts = m->origem->aceita(this);
        addInstrucao(new TernariaMemoria((char*)"sw", ts, 0, tb));
        return;
    }

    Temp *to = NULL;
    Temp *td = NULL;

    if(m->origem){
        to = m->origem->aceita(this);
    }
    if(m->destino){
        td = m->destino->aceita(this);
    }

    addInstrucao(new BinariaRegister((char*)"move", td, to));
}

void VisitorMIPS::visita(ExprStmtRI *es) {
    es->exp->aceita(this);
}

void VisitorMIPS::visita(JumpRI *j) {
    if (typeid (*j->exp) == typeid (NameRI)) {
        addInstrucao(new UnariaLabel((char*)"b", ((NameRI*)j->exp)->nome));
        return;
    }
    addInstrucao(new UnariaRegister((char*)"jr", j->exp->aceita(this)));
}

void VisitorMIPS::visita(CJumpRI *cj) {
    ConstRI *c;
    switch (cj->operadorRel) {
        case EQ:
            if ((c = dynamic_cast<ConstRI*>(cj->esquerda))) {
                Temp *e2 = cj->direita->aceita(this);
                addInstrucao(new TernariaLabelImmediate((char*)"beq", e2, c->valor, cj->verdadeiro));
            } else if ((c = dynamic_cast<ConstRI*>(cj->direita))) {
                Temp *e1 = cj->esquerda->aceita(this);
                addInstrucao(new TernariaLabelImmediate((char*)"beq", e1, c->valor, cj->verdadeiro));
            } else {
                Temp *e1 = cj->esquerda->aceita(this);
                Temp *e2 = cj->direita->aceita(this);
                addInstrucao(new TernariaLabel((char*)"beq", e1, e2, cj->verdadeiro));
            }
            break;
        case NE:
            if ((c = dynamic_cast<ConstRI*>(cj->esquerda))) {
                Temp *e2 = cj->direita->aceita(this);
                addInstrucao(new TernariaLabelImmediate((char*)"bne", e2, c->valor, cj->verdadeiro));
            } else if ((c = dynamic_cast<ConstRI*>(cj->direita))) {
                Temp *e1 = cj->esquerda->aceita(this);
                addInstrucao(new TernariaLabelImmediate((char*)"bne", e1, c->valor, cj->verdadeiro));
            } else {
                Temp *e1 = cj->esquerda->aceita(this);
                Temp *e2 = cj->direita->aceita(this);
                addInstrucao(new TernariaLabel((char*)"bne", e1, e2, cj->verdadeiro));
            }
            break;
        case LT:
            if ((c = dynamic_cast<ConstRI*>(cj->direita))) {
                Temp *e1 = cj->esquerda->aceita(this);
                addInstrucao(new TernariaLabelImmediate((char*)"blt", e1, c->valor, cj->verdadeiro));
            } else {
                Temp *e1 = cj->esquerda->aceita(this);
                Temp *e2 = cj->direita->aceita(this);
                addInstrucao(new TernariaLabel((char*)"blt", e1, e2, cj->verdadeiro));
            }
            break;
        case GT:
            if ((c = dynamic_cast<ConstRI*>(cj->direita))) {
                Temp *e1 = cj->esquerda->aceita(this);
                addInstrucao(new TernariaLabelImmediate((char*)"bgt", e1, c->valor, cj->verdadeiro));
            } else {
                Temp *e1 = cj->esquerda->aceita(this);
                Temp *e2 = cj->direita->aceita(this);
                addInstrucao(new TernariaLabel((char*)"bgt", e1, e2, cj->verdadeiro));
            }
            break;
        case LE:
            if ((c = dynamic_cast<ConstRI*>(cj->direita))) {
                Temp *e1 = cj->esquerda->aceita(this);
                addInstrucao(new TernariaLabelImmediate((char*)"ble", e1, c->valor, cj->verdadeiro));
            } else {
                Temp *e1 = cj->esquerda->aceita(this);
                Temp *e2 = cj->direita->aceita(this);
                addInstrucao(new TernariaLabel((char *)"ble", e1, e2, cj->verdadeiro));
            }
            break;
        case GE:
            if ((c = dynamic_cast<ConstRI*>(cj->direita))) {
                Temp *e1 = cj->esquerda->aceita(this);
                addInstrucao(new TernariaLabelImmediate((char*)"bge", e1, c->valor, cj->verdadeiro));
            } else {
                Temp *e1 = cj->esquerda->aceita(this);
                Temp *e2 = cj->direita->aceita(this);
                addInstrucao(new TernariaLabel((char*)"bge", e1, e2, cj->verdadeiro));
            }
            break;
   }
}

void VisitorMIPS::visita(SeqRI *s) {
    if(s->esquerda) s->esquerda->aceita(this);
    if(s->direita) s->direita->aceita(this);
}

void VisitorMIPS::visita(LabelRI *l) {
    if (strcmp(l->nome->nome, (char*)"funcvar") != 0) addInstrucao(new Unaria(l->nome));
}
