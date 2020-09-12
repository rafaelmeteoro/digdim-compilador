#ifndef _INSTRUCAOMIPS_H
#define _INSTRUCAOMIPS_H

#include <stdio.h>

class InstrucaoMIPS;
class Syscall;
class Secao;
class Globl;
class IntDecl;
class FloatDecl;
class Asciiz;
class Unaria;
class UnariaLabel;
class UnariaRegister;
class BinariaImmediate;
class BinariaLabel;
class BinariaRegister;
class TernariaMemoria;
class TernariaRegister;
class TernariaLabel;
class TernariaLabelImmediate;
class TernariaImmediate;

#include "representacaoIntermediaria.h"
#include "instrucaoVisitor.h"

class InstrucaoMIPS {
    private:
        InstrucaoMIPS *ultimo;

    public:
        char *nome;
        InstrucaoMIPS *proximo;

    public:
        InstrucaoMIPS(char *nome);
        virtual ~InstrucaoMIPS();
        void add(InstrucaoMIPS *i);
        virtual void imprime(FILE *saida) = 0;
        virtual void aceita(InstrucaoVisitor *v) = 0;
};

class Syscall : public InstrucaoMIPS {
    public:
        Syscall();
        virtual ~Syscall();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class Secao : public InstrucaoMIPS {
    public:
        Secao(char *nome);
        virtual ~Secao();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class Globl : public InstrucaoMIPS {
    public:
        Globl(char *nome);
        virtual ~Globl();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class IntDecl : public InstrucaoMIPS {
    public:
        int qtde;

    public:
        IntDecl(int qtde);
        virtual ~IntDecl();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class FloatDecl : public InstrucaoMIPS {
    public:
        int qtde;

    public:
        FloatDecl(int qtde);
        virtual ~FloatDecl();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class Asciiz : public InstrucaoMIPS {
    public:
        char *valor;

    public:
        Asciiz(char *valor);
        virtual ~Asciiz();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class Unaria : public InstrucaoMIPS {
    public:
        Label *label;

    public:
        Unaria(Label *label);
        virtual ~Unaria();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class UnariaLabel : public InstrucaoMIPS {
    public:
        Label *label;

    public:
        UnariaLabel(char *nome, Label *label);
        virtual ~UnariaLabel();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class UnariaRegister : public InstrucaoMIPS {
    public:
        Temp *reg;

    public:
        UnariaRegister(char *nome, Temp *reg);
        virtual ~UnariaRegister();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class BinariaImmediate : public InstrucaoMIPS {
    public:
        Temp *reg;
        int immediate;

    public:
        BinariaImmediate(char *nome, Temp *reg, int immediate);
        virtual ~BinariaImmediate();
       void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class BinariaLabel : public InstrucaoMIPS {
    public:
        Temp *reg;
        Label *label;

    public:
        BinariaLabel(char *nome, Temp *reg, Label *label);
        virtual ~BinariaLabel();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class BinariaRegister : public InstrucaoMIPS {
    public:
        Temp *reg1;
        Temp *reg2;

    public:
        BinariaRegister(char *nome, Temp *reg1, Temp *reg2);
        virtual ~BinariaRegister();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class TernariaMemoria : public InstrucaoMIPS {
    public:
        Temp *reg1;
        Temp *reg2;
        int immediate;

    public:
        TernariaMemoria(char *nome, Temp *reg1, int immediate, Temp *reg2);
        virtual ~TernariaMemoria();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class TernariaRegister : public InstrucaoMIPS {
    public:
        Temp *reg1;
        Temp *reg2;
        Temp *reg3;

    public:
        TernariaRegister(char *nome, Temp *reg1, Temp *reg2, Temp *reg3);
        virtual ~TernariaRegister();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class TernariaLabel : public InstrucaoMIPS {
    public:
        Temp *reg1;
        Temp *reg2;
        Label *label;

    public:
        TernariaLabel(char *nome, Temp *reg1, Temp *reg2, Label *label);
        virtual ~TernariaLabel();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class TernariaLabelImmediate : public InstrucaoMIPS {
    public:
        Temp *reg1;
        int immediate;
        Label *label;

    public:
        TernariaLabelImmediate(char *nome, Temp *reg1, int immediate, Label *label);
        virtual ~TernariaLabelImmediate();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

class TernariaImmediate : public InstrucaoMIPS {
    public:
        Temp *reg1;
        Temp *reg2;
        int immediate;

    public:
        TernariaImmediate(char *nome, Temp *reg1, Temp *reg2, int immediate);
        virtual ~TernariaImmediate();
        void imprime(FILE *saida);
        void aceita(InstrucaoVisitor *v);
};

#endif
