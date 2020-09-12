#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "representacaoIntermediaria.h"
#include "lexico.h"

/*** Registradores temporários ***/
Temp::Temp(){
    static int proxTemp = 1;
    nome  = (char *) malloc(NAME_TEMP * sizeof (char));
    sprintf(nome, "temp$%d", proxTemp);
    usado = 0;
    reuso = false;
    troca = true;
    proxTemp++;
}

Temp::Temp(char *nomeTemp){
    nome  = (char *) malloc((strlen(nomeTemp) + 1) * sizeof (char));
    strcpy(nome, nomeTemp);
    usado = 0;
    reuso = false;
    troca = false;
}

Temp::~Temp(){
    free(nome);
}

void Temp::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void Temp::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

void Temp::aceita(InstrucaoVisitor* visitor){
    visitor->visita(this);
}

TempList::TempList(Temp *primeiro, TempList *proximo){
    this->primeiro = primeiro;
    this->proximo  = proximo;
}

TempList::~TempList(){
    delete primeiro;
    delete proximo;
}

Label::Label(){
    static int proxLabel = 1;
    nome = (char *) malloc(NAME_LABEL * sizeof (char));
    sprintf(nome, "label_%d", proxLabel);
    proxLabel++;
    type = INT;
}

Label::Label(char *nomeLabel){
    this->nome = (char *) malloc((strlen(nomeLabel) + 1) * sizeof (char));
    strcpy(this->nome, nomeLabel);
    type = INT;
}

Label::Label(char *prefixo, char *nome){
    this->nome = (char *) malloc((strlen(prefixo) + strlen(nome) + 1) * sizeof (char));
    strcpy(this->nome, prefixo);
    strcat(this->nome, nome);
    type = INT;
}

Label::~Label(){
    free(nome);
}

void Label::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void Label::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

LabelList::LabelList(Label* primeiro, LabelList* proximo){
    this->primeiro = primeiro;
    this->proximo  = proximo;
}

LabelList::~LabelList(){
    delete primeiro;
    delete proximo;
}

StmtRI::StmtRI(){}

StmtRI::~StmtRI(){}

StmtRIList::StmtRIList(StmtRI* primeiro, StmtRIList* proximo){
    this->primeiro = primeiro;
    this->proximo  = proximo;
}

StmtRIList::~StmtRIList(){
    delete primeiro;
    delete proximo;
}

void StmtRIList::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void StmtRIList::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

ExprRI::ExprRI(){
    type = INT;
}

ExprRI::~ExprRI(){}



ExprRIList::ExprRIList(ExprRI* primeiro, ExprRIList* proximo){
    this->primeiro = primeiro;
    this->proximo  = proximo;
}

ExprRIList::~ExprRIList(){
    delete primeiro;
    delete proximo;
}

void ExprRIList::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void ExprRIList::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

ExprRIListList::ExprRIListList(ExprRIList* primeiro, ExprRIListList* proximo){
    this->primeiro = primeiro;
    this->proximo  = proximo;
}

ExprRIListList::~ExprRIListList(){
    delete primeiro;
    delete proximo;
}

InFrame::InFrame(int offset, char* id, bool endereco) : AcessoLocal(){
    this->offset    = offset;
    this->id        = id;
    this->endereco  = endereco;
}

InFrame::~InFrame(){}

void InFrame::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void InFrame::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

ExprRI* InFrame::codigoAcesso(){
    if (endereco) return new MemRI(new MemRI(new BinOpRI(PLUS, new TempRI(new Temp((char *)"$fp")), new ConstRI(this->offset))));
    else return new MemRI(new BinOpRI(PLUS, new TempRI(new Temp((char *)"$fp")), new ConstRI(this->offset)));
}

InReg::InReg(Temp *temporario, char *id) : AcessoLocal(){
    this->temporario = temporario;
    this->id         = id;
}

InReg::~InReg(){}

ExprRI* InReg::codigoAcesso(){
    return (ExprRI*) (new TempRI(this->temporario));
}

void InReg::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void InReg::aceita(VisitorMIPS* visitor){
   visitor->visita(this);
}

ListaAcesso::ListaAcesso(AcessoLocal *primeiro, ListaAcesso *proximo){
    this->primeiro = primeiro;
    this->proximo  = proximo;
}

ListaAcesso::~ListaAcesso(){
    delete primeiro;
    delete proximo;
}

void ListaAcesso::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void ListaAcesso::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

Frame::Frame(){
    rotulo              = NULL;
    valorRetorno        = NULL;
    dadosLocais         = NULL;
    ultimoDadoLocal     = NULL;
    tamanhoFrame        = 128; //link estatico, ra salvo, 10 temporarios, 2 retornos, 4 parametros e 8 salvos (4 bytes cada)
    offsetLocal         = -12; //o link estatico e o ra sao salvos antes das vars
    offsetParametro     = 0;
    contadorParametros  = 0;
}

Frame::~Frame(){
    delete rotulo;
    delete dadosLocais;
}

FrameMIPS::FrameMIPS() : Frame(){
}

FrameMIPS::~FrameMIPS(){
    delete valorRetorno;
}

void FrameMIPS::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void FrameMIPS::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

void FrameMIPS::adicionaParametro(bool escapa, int tamBytes, char* nome, bool endereco)
{
    AcessoLocal *acesso;
    if(escapa){
        acesso = new InFrame(offsetParametro, nome, endereco);
        offsetParametro += tamBytes;
    }else acesso = new InReg(new Temp(), nome);

    if(ultimoDadoLocal){
        ultimoDadoLocal->proximo = new ListaAcesso(acesso, NULL);
        ultimoDadoLocal = ultimoDadoLocal->proximo;
    }else{
        dadosLocais = new ListaAcesso(acesso, NULL);
        ultimoDadoLocal = dadosLocais;
    }
}

void FrameMIPS::adicionaLocal(bool escapa, int tamBytes, char* nome){
    AcessoLocal *acesso;
    if(escapa){
        acesso = new InFrame(offsetLocal, nome, false);
        tamanhoFrame += tamBytes;
        offsetLocal -= tamBytes;
    }else acesso = new InReg(new Temp(), nome);

    if(ultimoDadoLocal){
        ultimoDadoLocal->proximo = new ListaAcesso(acesso, NULL);
        ultimoDadoLocal = ultimoDadoLocal->proximo;
    }else{
        dadosLocais = new ListaAcesso(acesso, NULL);
        ultimoDadoLocal = dadosLocais;
    }
}

AcessoLocal *FrameMIPS::buscaVariavel(char* variavel){
    ListaAcesso *listaAcesso = this->dadosLocais;
    while(listaAcesso){
        if(strcmp(variavel, listaAcesso->primeiro->id) == 0) return listaAcesso->primeiro;
        listaAcesso = listaAcesso->proximo;
    }
    return NULL;
}

Fragment::Fragment(){
    proximo = NULL;
}

Fragment::~Fragment(){
    delete proximo;
}

void Fragment::adicionaNoFim(Fragment *fragmento){
    if(proximo) proximo->adicionaNoFim(fragmento);
    else proximo = fragmento;
}

Procedure::Procedure(Frame *frame, StmtRI *body) : Fragment(){
    this->frame = frame;
    this->body  = body;
}

void Procedure::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void Procedure::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

void Procedure::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

VariavelNaoLocal::VariavelNaoLocal(int tipo, int quantidade, VariavelNaoLocal *proximo) : Fragment(){
    this->tipo       = tipo;
    this->quantidade = quantidade;
    this->proximo    = proximo;
}

void VariavelNaoLocal::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void VariavelNaoLocal::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

void VariavelNaoLocal::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

LiteralRI::LiteralRI(LabelRI *nome, LiteralNodo *literal) : Fragment(){
    this->literal   = literal;
    this->nome      = nome;
    this->proximo   = NULL;
}

void LiteralRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void LiteralRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

void LiteralRI::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

ConstRI::ConstRI(int valor) : ExprRI(){
    this->valor = valor;
}

ConstRI::~ConstRI(){}

void ConstRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void ConstRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}


Temp *ConstRI::aceita(VisitorMIPS* visitor){
    return visitor->visita(this);
}

ConstfRI::ConstfRI(float valor) : ExprRI(){
    this->valor = valor;
    this->type  = FLOAT;
}

ConstfRI::~ConstfRI(){}

void ConstfRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void ConstfRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

Temp *ConstfRI::aceita(VisitorMIPS* visitor){
    return visitor->visita(this);
}

NameRI::NameRI(Label *nome){
    this->nome = nome;
    this->type = nome->type;
}

NameRI::~NameRI(){
    delete nome;
}

void NameRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void NameRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

Temp *NameRI::aceita(VisitorMIPS* visitor){
    return visitor->visita(this);
}

TempRI::TempRI(Temp *temporario) : ExprRI(){
    this->temporario = temporario;
}

TempRI::~TempRI(){
    delete temporario;
}

void TempRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void TempRI::aceita(Canonizador *c) {
    c->visita(this);
}

Temp *TempRI::aceita(VisitorMIPS* visitor){
    return visitor->visita(this);
}

BinOpRI::BinOpRI(int operador, ExprRI *exp1, ExprRI *exp2) : ExprRI(){
    this->operador  = operador;
    this->exp1      = exp1;
    this->exp2      = exp2;
    this->type      = exp1->type;
}

BinOpRI::~BinOpRI(){
    delete exp1;
    delete exp2;
}

void BinOpRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void BinOpRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

Temp *BinOpRI::aceita(VisitorMIPS* visitor){
    return visitor->visita(this);
}

MemRI::MemRI(ExprRI *exp) : ExprRI(){
    this->exp  = exp;
    this->type = exp->type;
}

MemRI::~MemRI(){
    delete exp;
}

void MemRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void MemRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

Temp *MemRI::aceita(VisitorMIPS* visitor){
    return visitor->visita(this);
}

CallRI::CallRI(Label *funcao, ExprRIList *argumentos) : ExprRI(){
    this->funcao     = funcao;
    this->argumentos = argumentos;
    this->canonizado = false;
}

CallRI::~CallRI(){
    delete funcao;
    delete argumentos;
}

void CallRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void CallRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

Temp *CallRI::aceita(VisitorMIPS* visitor){
    return visitor->visita(this);
}

ESeqRI::ESeqRI(StmtRI *stmt, ExprRI *exp) : ExprRI(){
    this->stmt = stmt;
    this->exp  = exp;
    this->type = exp->type;
}

ESeqRI::~ESeqRI(){
    delete stmt;
    delete exp;
}

void ESeqRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void ESeqRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

Temp *ESeqRI::aceita(VisitorMIPS* visitor){
    return visitor->visita(this);
}

MoveRI::MoveRI(ExprRI *destino, ExprRI *origem) : StmtRI(){
    this->destino = destino;
    this->origem  = origem;
}

MoveRI::~MoveRI(){
    delete destino;
    delete origem;
}

void MoveRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void MoveRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

void MoveRI::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

ExprStmtRI::ExprStmtRI(ExprRI *exp) : StmtRI(){
    this->exp = exp;
}

ExprStmtRI::~ExprStmtRI(){
    delete exp;
}

void ExprStmtRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void ExprStmtRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

void ExprStmtRI::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

JumpRI::JumpRI(ExprRI *exp) : StmtRI(){
    this->exp = exp;
}

JumpRI::~JumpRI(){
    delete exp;
}

void JumpRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void JumpRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

void JumpRI::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

CJumpRI::CJumpRI(int operadorRel, ExprRI *esquerda, ExprRI *direita, Label *verdadeiro, Label *falso) : StmtRI(){
    this->operadorRel   = operadorRel;
    this->esquerda      = esquerda;
    this->direita       = direita;
    this->verdadeiro    = verdadeiro;
    this->falso         = falso;
    this->canonizado = false;
}

CJumpRI::~CJumpRI(){
    delete esquerda;
    delete direita;
    delete verdadeiro;
    delete falso;
}

void CJumpRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void CJumpRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

void CJumpRI::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

SeqRI::SeqRI(StmtRI *esquerda, StmtRI *direita) : StmtRI(){
    this->esquerda = esquerda;
    this->direita  = direita;
}

SeqRI::~SeqRI(){
    delete esquerda;
    delete direita;
}

void SeqRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void SeqRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

void SeqRI::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}

LabelRI::LabelRI(Label *nome) : StmtRI(){
    this->nome = nome;
}

LabelRI::~LabelRI(){
    delete nome;
}

void LabelRI::aceita(RIVisitor *visitor){
    visitor->visita(this);
}

void LabelRI::aceita(Canonizador *canonizador) {
    canonizador->visita(this);
}

void LabelRI::aceita(VisitorMIPS* visitor){
    visitor->visita(this);
}
