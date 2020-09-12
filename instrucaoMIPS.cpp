#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instrucaoMIPS.h"

InstrucaoMIPS::InstrucaoMIPS(char *nome) {
    if (nome) {
        this->nome = (char*)malloc((strlen(nome)+1) * sizeof (char));
        strcpy(this->nome, nome);
    } else {
        this->nome = NULL;
    }
    ultimo = this;
    proximo = NULL;
}

InstrucaoMIPS::~InstrucaoMIPS() {
    delete proximo;
}

void InstrucaoMIPS::add(InstrucaoMIPS *i) {
    InstrucaoMIPS *temp = ultimo;
    while (temp->proximo) temp = temp->proximo;
    temp->proximo = i;
    ultimo = i;
}

Secao::Secao(char *nome) : InstrucaoMIPS(nome) {
}

Secao::~Secao() {
}

void Secao::imprime(FILE *saida) {
    fprintf(saida, "\n\t.%s\n", nome);
}

void Secao::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

Syscall::Syscall() : InstrucaoMIPS((char*)"syscall") {
}

Syscall::~Syscall() {
}

void Syscall::imprime(FILE *saida) {
    fprintf(saida, "\t%s\n", nome);
}

void Syscall::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

Globl::Globl(char *nome) : InstrucaoMIPS(nome) {
}

Globl::~Globl() {
}

void Globl::imprime(FILE *saida) {
    fprintf(saida, "\t.globl\t%s\n", nome);
}

void Globl::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

IntDecl::IntDecl(int qtde) : InstrucaoMIPS((char*)"word") {
    this->qtde = qtde;
}

IntDecl::~IntDecl() {
}

void IntDecl::imprime(FILE *saida) {
    fprintf(saida, "\t.%s", nome);
    for (int i = 0; i < qtde; i++) {
        fprintf(saida, " 0");
        if (i != qtde - 1) fprintf(saida, ",");
    }
    fprintf(saida, "\n");
}

void IntDecl::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

FloatDecl::FloatDecl(int qtde) : InstrucaoMIPS((char*)"float") {
    this->qtde = qtde;
}

FloatDecl::~FloatDecl() {
}

void FloatDecl::imprime(FILE *saida) {
    fprintf(saida, "\t.%s", nome);
    for (int i = 0; i < qtde; i++) {
        fprintf(saida, " 0.0");
        if (i != qtde - 1) fprintf(saida, ",");
    }
    fprintf(saida, "\n");
}

void FloatDecl::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

Asciiz::Asciiz(char *valor) : InstrucaoMIPS((char*)"asciiz") {
    this->valor = valor;
}

Asciiz::~Asciiz() {
}

void Asciiz::imprime(FILE *saida) {
    fprintf(saida, "\t.%s\t%s\n", nome, valor);
}

void Asciiz::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

Unaria::Unaria(Label *label) : InstrucaoMIPS(NULL) {
    this->label = label;
}

Unaria::~Unaria() {
}

void Unaria::imprime(FILE *saida) {
    fprintf(saida, "%s:\n", label->nome);
}

void Unaria::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

UnariaLabel::UnariaLabel(char *nome, Label *label) : InstrucaoMIPS(nome) {
    this->label = label;
}

UnariaLabel::~UnariaLabel() {
}

void UnariaLabel::imprime(FILE *saida) {
    fprintf(saida, "\t%s\t%s\n", nome, label->nome);
}

void UnariaLabel::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

UnariaRegister::UnariaRegister(char *nome, Temp *reg) : InstrucaoMIPS(nome) {
    this->reg = reg;
}

UnariaRegister::~UnariaRegister() {
}

void UnariaRegister::imprime(FILE *saida) {
    fprintf(saida, "\t%s\t%s\n", nome, reg->nome);
}

void UnariaRegister::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

BinariaImmediate::BinariaImmediate(char *nome, Temp *reg, int immediate) : InstrucaoMIPS(nome) {
    this->reg = reg;
    this->immediate = immediate;
}

BinariaImmediate::~BinariaImmediate() {
}

void BinariaImmediate::imprime(FILE *saida) {
    fprintf(saida, "\t%s\t%s, %d\n", nome, reg->nome, immediate);
}

void BinariaImmediate::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

BinariaLabel::BinariaLabel(char *nome, Temp *reg, Label* label) : InstrucaoMIPS(nome) {
    this->reg = reg;
    this->label = label;
}

BinariaLabel::~BinariaLabel() {
}

void BinariaLabel::imprime(FILE *saida) {
    fprintf(saida, "\t%s\t%s, %s\n", nome, reg->nome, label->nome);
}

void BinariaLabel::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

BinariaRegister::BinariaRegister(char *nome, Temp *reg1, Temp *reg2) : InstrucaoMIPS(nome) {
    this->reg1 = reg1;
    this->reg2 = reg2;
}

BinariaRegister::~BinariaRegister() {
}

void BinariaRegister::imprime(FILE *saida) {
    if(reg1 && reg2){
        fprintf(saida, "\t%s\t%s, %s\n", nome, reg1->nome, reg2->nome);
    }
}

void BinariaRegister::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

TernariaMemoria::TernariaMemoria(char *nome, Temp *reg1, int immediate, Temp *reg2) : InstrucaoMIPS(nome) {
    this->reg1 = reg1;
    this->immediate = immediate;
    this->reg2 = reg2;
}

TernariaMemoria::~TernariaMemoria() {
}

void TernariaMemoria::imprime(FILE *saida) {
    fprintf(saida, "\t%s\t%s, %d(%s)\n", nome, reg1->nome, immediate, reg2->nome);
}

void TernariaMemoria::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

TernariaRegister::TernariaRegister(char *nome, Temp *reg1, Temp *reg2, Temp *reg3) : InstrucaoMIPS(nome) {
    this->reg1 = reg1;
    this->reg2 = reg2;
    this->reg3 = reg3;
}

TernariaRegister::~TernariaRegister() {
}

void TernariaRegister::imprime(FILE *saida) {
    fprintf(saida, "\t%s\t%s, %s, %s\n", nome, reg1->nome, reg2->nome, reg3->nome);
}

void TernariaRegister::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

TernariaLabel::TernariaLabel(char *nome, Temp *reg1, Temp *reg2, Label *label) : InstrucaoMIPS(nome) {
    this->reg1 = reg1;
    this->reg2 = reg2;
    this->label = label;
}

TernariaLabel::~TernariaLabel() {
}

void TernariaLabel::imprime(FILE *saida) {
    fprintf(saida, "\t%s\t%s, %s, %s\n", nome, reg1->nome, reg2->nome, label->nome);
}

void TernariaLabel::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

TernariaLabelImmediate::TernariaLabelImmediate(char* nome, Temp* reg1, int immediate, Label* label) : InstrucaoMIPS(nome) {
    this->reg1 = reg1;
    this->immediate = immediate;
    this->label = label;
}

TernariaLabelImmediate::~TernariaLabelImmediate() {
}

void TernariaLabelImmediate::imprime(FILE *saida) {
    fprintf(saida, "\t%s\t%s, %d, %s\n", nome, reg1->nome, immediate, label->nome);
}

void TernariaLabelImmediate::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}

TernariaImmediate::TernariaImmediate(char *nome, Temp *reg1, Temp *reg2, int immediate) : InstrucaoMIPS(nome) {
    this->reg1 = reg1;
    this->reg2 = reg2;
    this->immediate = immediate;
}

TernariaImmediate::~TernariaImmediate() {
}

void TernariaImmediate::imprime(FILE *saida) {
    fprintf(saida, "\t%s\t%s, %s, %d\n", nome, reg1->nome, reg2->nome, immediate);
}

void TernariaImmediate::aceita(InstrucaoVisitor *v) {
    v->visita(this);
}
