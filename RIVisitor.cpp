#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RIVisitor.h"
#include "lexico.h"

RIVisitor::RIVisitor(){}

RIVisitor::~RIVisitor(){}

void RIVisitor::visita(Temp *t){
    t->aceita(this);
}

void RIVisitor::visita(Label *l){
    l->aceita(this);
}

void RIVisitor::visita(StmtRI *st){
    st->aceita(this);
}

void RIVisitor::visita(StmtRIList* stl){
    if(stl->primeiro)   stl->primeiro->aceita(this);
    if(stl->proximo)    stl->proximo->aceita(this);
}

void RIVisitor::visita(ExprRI *e){
    e->aceita(this);
}

void RIVisitor::visita(ExprRIList *el){
    if(el->primeiro)    el->primeiro->aceita(this);
    if(el->proximo)     el->proximo->aceita(this);
}

void RIVisitor::visita(InFrame *inF){
    inF->aceita(this);
}

void RIVisitor::visita(InReg *inR){
    if(inR->temporario) inR->temporario->aceita(this);
}

void RIVisitor::visita(ListaAcesso *la){
    if(la->primeiro)    la->primeiro->aceita(this);
    if(la->proximo)     la->proximo->aceita(this);
}

void RIVisitor::visita(FrameMIPS *fm){
    if(fm->rotulo)          fm->rotulo->aceita(this);
    if(fm->dadosLocais)     fm->dadosLocais->aceita(this);
    if(fm->valorRetorno)    fm->valorRetorno->aceita(this);
    if(fm->ultimoDadoLocal) fm->ultimoDadoLocal->aceita(this);
}

void RIVisitor::visita(Procedure *p){
    if(p->proximo)  p->proximo->aceita(this);
    if(p->frame)    p->frame->aceita(this);
    if(p->body)     p->body->aceita(this);
}

void RIVisitor::visita(LiteralRI *l){
    if(l->proximo)  l->proximo->aceita(this);
    if(l->nome)     l->nome->aceita(this);
}

void RIVisitor::visita(VariavelNaoLocal *v){
    if(v->proximo)  v->proximo->aceita(this);
    if(v->nome)     v->nome->aceita(this);
}

void RIVisitor::visita(LabelRI *l){
    if(l->nome) l->nome->aceita(this);
}

void RIVisitor::visita(ConstRI *c){
    c->aceita(this);
}

void RIVisitor::visita(ConstfRI *cf){
    cf->aceita(this);
}

void RIVisitor::visita(NameRI *n){
    if(n->nome) n->nome->aceita(this);
}

void RIVisitor::visita(TempRI *t){
    if(t->temporario) t->temporario->aceita(this);
}

void RIVisitor::visita(BinOpRI *b){
    if(b->exp1) b->exp1->aceita(this);
    if(b->exp2) b->exp2->aceita(this);
}

void RIVisitor::visita(MemRI *m){
    if(m->exp) m->exp->aceita(this);
}

void RIVisitor::visita(CallRI *c){
    c->aceita(this);
}

void RIVisitor::visita(ESeqRI *es){
    if(es->stmt)    es->stmt->aceita(this);
    if(es->exp)     es->exp->aceita(this);
}

void RIVisitor::visita(MoveRI *m){
    if(m->destino) m->destino->aceita(this);
    if(m->origem)  m->origem->aceita(this);
}

void RIVisitor::visita(ExprStmtRI *es){
    if(es->exp) es->exp->aceita(this);
}

void RIVisitor::visita(JumpRI *j){
    if(j->exp) j->exp->aceita(this);
}

void RIVisitor::visita(CJumpRI *cj){
    if(cj->esquerda)    cj->esquerda->aceita(this);
    if(cj->direita)     cj->direita->aceita(this);
    if(cj->verdadeiro)  cj->verdadeiro->aceita(this);
    if(cj->falso)       cj->falso->aceita(this);
}

void RIVisitor::visita(SeqRI *s){
    if(s->esquerda) s->esquerda->aceita(this);
    if(s->direita)  s->direita->aceita(this);
}

/***************************************************************************
            VISITOR DE IMPRESSÃO DA REPRESENTAÇÃO INTERMEDIÁRIA
***************************************************************************/

char *operacoes[] = {(char*)"PLUS",(char*)"MINUS",(char*)"MUL",(char*)"DIV",(char*)"AND", (char*)"OR",(char*)"XOR",(char*)"LSHIFT",(char*)"RSHIFT",(char*)"ARSHIFT",(char*)"EQ",(char*)"NE",(char*)"LT",(char*)"GT",(char*)"LE",(char*)"GE",(char*)"ULT",(char*)"ULE",(char*)"UGT",(char*)"UGE"};

VisitorRepIntImprime::VisitorRepIntImprime() {
    nivel = 0;

    fprintf(stdout, "\n\n+--------------------------------------------------------------------------+\n");
    fprintf(stdout, "|           R E P R E S E N T A C A O   I N T E R M E D I A R I A          |\n");
    fprintf(stdout, "+--------------------------------------------------------------------------+\n\n");
}

VisitorRepIntImprime::~VisitorRepIntImprime() {}

void VisitorRepIntImprime::imprime(char *text) {
    for (int i = 0; i < nivel; i++) printf("    ");
    fprintf(stdout, "- %s\n", text);
}

void VisitorRepIntImprime::imprimePropriedade(char *obj, char *prop) {
    for (int i = 0; i < nivel; i++) printf("    ");
    fprintf(stdout, "- %s: %s\n", obj, prop);
}

void VisitorRepIntImprime::imprimePropriedadeInt(char *obj, int prop) {
    for (int i = 0; i < nivel; i++) printf("    ");
    fprintf(stdout, "- %s: %d\n", obj, prop);
}

void VisitorRepIntImprime::imprimePropriedadeFloat(char *obj, float prop) {
    for (int i = 0; i < nivel; i++) printf("    ");
    fprintf(stdout, "- %s: %f\n", obj, prop);
}

char* operacao(int i) {
    return operacoes[i];
}

/** Visitas */
void VisitorRepIntImprime::visita(Temp *t) {
    imprimePropriedade((char*)"Temp", t->nome);
}

void VisitorRepIntImprime::visita(Label *l) {
    imprimePropriedade((char*)"Label", l->nome);
}

void VisitorRepIntImprime::visita(StmtRI *s){
}

void VisitorRepIntImprime::visita(StmtRIList *sl) {
    imprime((char*)"StmtRIList");

    nivel++;
    sl->primeiro->aceita(this);
    if (sl->proximo) sl->proximo->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(ExprRI *e){}

void VisitorRepIntImprime::visita(ExprRIList *el) {
    imprime((char*)"ExprRIList");

    nivel++;
    el->primeiro->aceita(this);
    if (el->proximo) el->proximo->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(InFrame *inF) {
    imprime((char*)"InFrame");

    nivel++;
    imprimePropriedadeInt((char*)"Offset", inF->offset);
    inF->codigoAcesso()->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(InReg *inR) {
    imprime((char*)"InReg");

    nivel++;
    inR->temporario->aceita(this);
    inR->codigoAcesso()->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(ListaAcesso *la) {
    imprime((char*)"ListaAcesso");

    nivel++;
    la->primeiro->aceita(this);
    if (la->proximo) la->proximo->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(FrameMIPS *f) {
    imprime((char*)"FrameMIPS");

    nivel++;
    f->rotulo->aceita(this);
    imprimePropriedadeInt((char*)"Tamanho do frame", f->tamanhoFrame);
    if (f->dadosLocais) f->dadosLocais->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(Procedure *p) {
    imprime((char *)"Procedure");

    nivel++;
    p->frame->aceita(this);
    if(p->body) p->body->aceita(this);
    nivel--;
    imprime((char*)"");
    if (p->proximo)p->proximo->aceita(this);
}

void VisitorRepIntImprime::visita(LiteralRI *l) {
    imprime((char *) "LiteralRI");

    nivel++;
    l->nome->aceita(this);
    imprimePropriedade((char*)"Literal", l->literal->literal);
    nivel--;
    imprime((char*)"");
    if (l->proximo) l->proximo->aceita(this);
}

void VisitorRepIntImprime::visita(VariavelNaoLocal *v) {
    imprime((char *)"VariavelNaoLocal");

    nivel++;
    v->nome->aceita(this);
    imprimePropriedade((char*)"Tipo", getNomeToken(v->tipo));
    imprimePropriedadeInt((char*)"Tamanho", v->quantidade);
    nivel--;
    imprime((char*)"");
    if (v->proximo) v->proximo->aceita(this);
}

void VisitorRepIntImprime::visita(LabelRI *l) {
    imprime((char*)"LabelRI");

    nivel++;
    l->nome->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(ConstRI *c) {
    imprime((char*)"ConstRI");

    nivel++;
    imprimePropriedadeInt((char*)"Valor", c->valor);
    nivel--;
}

void VisitorRepIntImprime::visita(ConstfRI *c) {
    imprime((char*)"ConstfRI");

    nivel++;
    imprimePropriedadeFloat((char*)"Valor", c->valor);
    nivel--;
}

void VisitorRepIntImprime::visita(NameRI *n) {
    imprime((char*)"NameRI");

    nivel++;
    n->nome->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(TempRI *t) {
    imprime((char*)"TempRI");

    nivel++;
    t->temporario->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(BinOpRI *b) {
    imprime((char*)"BinOpRI");

    nivel++;
    imprimePropriedade((char*)"Operacao", operacao(b->operador));
    b->exp1->aceita(this);
    b->exp2->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(MemRI *m) {
    imprime((char*)"MemRI");

    nivel++;
    m->exp->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(CallRI *c) {
    imprime((char*)"CallRI");

    nivel++;
    c->funcao->aceita(this);
    if (c->argumentos) c->argumentos->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(ESeqRI *es) {
    imprime((char*)"ESeqRI");

    nivel++;
    es->stmt->aceita(this);
    es->exp->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(MoveRI *m) {
    imprime((char*)"MoveRI");

    nivel++;
    if(m->destino) m->destino->aceita(this);
    if(m->origem)  m->origem->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(ExprStmtRI *es) {
    imprime((char*)"ExpStmtRI");

    nivel++;
    es->exp->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(JumpRI *j) {
    imprime((char*)"JumpRI");

    nivel++;
    j->exp->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(CJumpRI *cj) {
    imprime((char*)"CJumpRI");

    nivel++;
    imprimePropriedade((char*)"Operacao", operacao(cj->operadorRel));
    cj->esquerda->aceita(this);
    cj->direita->aceita(this);
    cj->verdadeiro->aceita(this);
    cj->falso->aceita(this);
    nivel--;
}

void VisitorRepIntImprime::visita(SeqRI *s) {
    imprime((char*)"SeqRI");

    nivel++;
    if(s->esquerda) s->esquerda->aceita(this);
    if(s->direita) s->direita->aceita(this);
    nivel--;
}
