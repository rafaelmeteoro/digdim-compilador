#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>

#include "canonizador.h"

Canonizador::Canonizador() {
}

Canonizador::~Canonizador() {
}

/************************************************
        PROCESSAMENTO DE EXPRESSÕES
************************************************/

ExprRI* Canonizador::processaExpr(ExprRI *expr) {
    /** BinOp */
    if (typeid(*expr) == typeid(BinOpRI)) {
        BinOpRI *bo = (BinOpRI*)expr;
        if (typeid(*bo->exp1) == typeid(ESeqRI)) {
            mudancas++;
            ESeqRI *eseq = ((ESeqRI*)bo->exp1);
            ExprRI *e1 = eseq->exp;
            eseq->exp = bo;
            bo->exp1 = e1;
            return eseq;
        }
        if (typeid(*bo->exp2) == typeid(ESeqRI)) {
            mudancas++;
            ESeqRI *eseq = (ESeqRI*)bo->exp2;
            ExprRI *e2 = eseq->exp;
            ExprRI *e1 = bo->exp1;
            eseq->exp = bo;
            TempRI *tmp = new TempRI(new Temp());
            bo->exp2 = e2;
            bo->exp1 = tmp;
            return new ESeqRI(new MoveRI(tmp, e1), eseq);
        }
    }
    /** Mem */
    if (typeid(*expr) == typeid(MemRI)) {
        MemRI *m = (MemRI*)expr;
        if (typeid(*m->exp) == typeid(ESeqRI)) {
            mudancas++;
            ESeqRI *eseq = (ESeqRI*)m->exp;
            ExprRI *e1 = eseq->exp;
            eseq->exp = m;
            m->exp = e1;
            return eseq;
        }
    }
    /** Call */
    if (typeid(*expr) == typeid(CallRI)) {
        CallRI *c = (CallRI*)expr;
        if (!c->canonizado) {                   //verificar esse    canonizado
            mudancas++;
            TempRI *tmp = new TempRI(new Temp());
            c->canonizado = true;
            return new ESeqRI(new MoveRI(tmp, expr), tmp);
        }
    }
    /** ESeq */
    if (typeid (*expr) == typeid (ESeqRI)) {
        ESeqRI *eseq = (ESeqRI*)expr;
        if (typeid(*eseq->exp) == typeid(ESeqRI)) {
            mudancas++;
            ESeqRI *eseq2 = ((ESeqRI*)eseq->exp);
            StmtRI *s1 = eseq->stmt;
            StmtRI *s2 = eseq2->stmt;
            ExprRI *e = eseq2->exp;
            eseq->stmt = new SeqRI(s1, s2);
            eseq->exp = e;
            return expr;
        }
    }
    return expr;
}

/************************************************
        PROCESSAMENTO DE SENTENÇAS
************************************************/

StmtRI* Canonizador::processaStmt(StmtRI *stmt) {
    /** Move */
    if (typeid(*stmt) == typeid(MoveRI)) {
        MoveRI *m = (MoveRI*) stmt;
        if (typeid(*m->destino) == typeid(ESeqRI)) {
            mudancas++;
            ESeqRI *eseq = (ESeqRI*)m->destino;
            m->destino = eseq->exp;
            return new SeqRI(eseq->stmt, m);
        }
        if (typeid(*m->origem) == typeid(ESeqRI)) {
            mudancas++;
            ESeqRI *eseq = (ESeqRI*)m->origem;
            m->origem = eseq->exp;
            return new SeqRI(eseq->stmt, m);
        }
    }
    /** Jump */
    if (typeid(*stmt) == typeid(JumpRI)) {
        JumpRI *j = (JumpRI*)stmt;
        if (typeid(*j->exp) == typeid(ESeqRI)) {
            mudancas++;
            ESeqRI *eseq = (ESeqRI*) j->exp;
            StmtRI *s1 = eseq->stmt;
            ExprRI *expr = eseq->exp;
            j->exp = expr;
            return new SeqRI(s1, j);
        }
    }
    /**  CJump */
    if (typeid(*stmt) == typeid(CJumpRI)) {
        CJumpRI *c = (CJumpRI*)stmt;
        if (typeid (*c->esquerda) == typeid(ESeqRI)) {
            mudancas++;
            ESeqRI *eseq = (ESeqRI*)c->esquerda;
            c->esquerda = eseq->exp;
            return new SeqRI(eseq->stmt, c);
        }
        if (typeid(*c->direita) == typeid(ESeqRI)) {
            mudancas++;
            ESeqRI *eseq = (ESeqRI*)c->direita;
            c->direita = eseq->exp;
            ExprRI *e1 = c->esquerda;
            TempRI *tmp = new TempRI(new Temp());
            c->esquerda = tmp;
            return new SeqRI(new MoveRI(tmp, e1), new SeqRI(eseq->stmt, c));
        }
        if (!c->canonizado){     //verificar o canonizado
            mudancas++;
            c->canonizado = true;
            Label *label_falso = c->falso;
            //c->falso = new Label(c->falso->nome);
            c->falso = new Label();
            return new SeqRI(new SeqRI(c, new LabelRI(c->falso)), new JumpRI(new NameRI(label_falso)));
        }
    }
    /** Seq */
    if (typeid(*stmt) == typeid(SeqRI)) {
        SeqRI *seq = (SeqRI*) stmt;
        if(seq->esquerda){
            if (typeid(*seq->esquerda) == typeid(SeqRI)) {
                mudancas++;
                SeqRI *seq2 = (SeqRI*)seq->esquerda;
                seq->esquerda = seq2->esquerda;
                seq2->esquerda = seq2->direita;
                seq2->direita = seq->direita;
                seq->direita = seq2;
                return seq;
            }
        }
    }
    /** ExpStmtRI */
    if (typeid(*stmt) == typeid(ExprStmtRI)) {
        ExprStmtRI *es = (ExprStmtRI*)stmt;
        if (typeid(*es->exp) == typeid(ESeqRI)) {
            mudancas++;
            ESeqRI *eseq = (ESeqRI*)es->exp;
            StmtRI *s1 = eseq->stmt;
            ExprRI *expr = eseq->exp;
            es->exp = expr;
            return new SeqRI(s1, es);
        }
    }
    return stmt;
}

/*******************************************
            MÉTODOS VISITANTES
*******************************************/

void Canonizador::visita(Procedure *p) {
    if(p->body)     p->body = processaStmt(p->body);
    if(p->body)     p->body->aceita(this);
    if (p->proximo) p->proximo->aceita(this);
}

void Canonizador::visita(LiteralRI *l) {
    if (l->proximo) l->proximo->aceita(this);
}

void Canonizador::visita(VariavelNaoLocal *v) {
    if (v->proximo) v->proximo->aceita(this);
}

void Canonizador::visita(Temp *t) {
}

void Canonizador::visita(Label *l) {
}

void Canonizador::visita(ListaAcesso *la) {
}

void Canonizador::visita(InFrame *inF) {
}

void Canonizador::visita(InReg *inR) {
}

void Canonizador::visita(FrameMIPS *f) {
}

void Canonizador::visita(ExprRI *e) {
}

void Canonizador::visita(ExprRIList *el) {
}

void Canonizador::visita(ConstRI *c) {
}

void Canonizador::visita(ConstfRI *cf) {
}

void Canonizador::visita(NameRI *n) {
}

void Canonizador::visita(TempRI *t) {
}

void Canonizador::visita(BinOpRI *b) {
    if(b->exp1) b->exp1 = processaExpr(b->exp1);
    if(b->exp1) b->exp1->aceita(this);
    if(b->exp2) b->exp2 = processaExpr(b->exp2);
    if(b->exp2) b->exp2->aceita(this);
}

void Canonizador::visita(MemRI *m) {
    if(m->exp) m->exp = processaExpr(m->exp);
    if(m->exp) m->exp->aceita(this);
}

void Canonizador::visita(CallRI *c) {
}

void Canonizador::visita(ESeqRI *es) {
    if(es->stmt) es->stmt = processaStmt(es->stmt);
    if(es->stmt) es->stmt->aceita(this);
    if(es->exp)  es->exp = processaExpr(es->exp);
    if(es->exp)  es->exp->aceita(this);
}

void Canonizador::visita(StmtRI *s) {
}

void Canonizador::visita(StmtRIList *sl) {
}

void Canonizador::visita(MoveRI *m) {
    if(m->destino) m->destino = processaExpr(m->destino);
    if(m->destino) m->destino->aceita(this);
    if(m->origem)  m->origem = processaExpr(m->origem);
    if(m->origem)  m->origem->aceita(this);
}

void Canonizador::visita(ExprStmtRI *es) {
    if(es->exp) es->exp = processaExpr(es->exp);
    if(es->exp) es->exp->aceita(this);
}

void Canonizador::visita(JumpRI *j) {
}

void Canonizador::visita(CJumpRI *cj) {
    if(cj->esquerda) cj->esquerda = processaExpr(cj->esquerda);
    if(cj->esquerda) cj->esquerda->aceita(this);
    if(cj->direita)  cj->direita = processaExpr(cj->direita);
    if(cj->direita)  cj->direita->aceita(this);
}

void Canonizador::visita(SeqRI *s) {
    if(s->esquerda) s->esquerda = processaStmt(s->esquerda);
    if(s->esquerda) s->esquerda->aceita(this);
    if(s->direita)  s->direita = processaStmt(s->direita);
    if(s->direita)  s->direita->aceita(this);
}

void Canonizador::visita(LabelRI* l) {
}
