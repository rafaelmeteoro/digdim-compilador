#ifndef _REPRESENTACAOINTERMEDIARIA_H
#define	_REPRESENTACAOINTERMEDIARIA_H

#define PLUS     0
#define MINUS    1
#define MUL      2
#define DIVR     3
#define AND      4
#define OR       5
#define XOR      6
#define LSHIFT   7
#define RSHIFT   8
#define ARSHIFT  9

#define EQ      10
#define NE      11
#define LT      12
#define GT      13
#define LE      14
#define GE      15
#define ULT     16
#define ULE     17
#define UGT     18
#define UGE     19

#define STRING  0

#define NAME_TEMP  15
#define NAME_LABEL 15

class ExprRI;
class StmtRI;
class MemRI;
class TempList;
class LabelList;
class Fragment;
class VariavelNaoLocal;
class Temp;
class Label;
class StmtRIList;
class ExprRI;
class ExprRIList;
class ExprRIListList;
class InFrame;
class InReg;
class ListaAcesso;
class FrameMIPS;
class Procedure;
class LiteralRI;
class VariavelNaoLocal;
class LabelRI;
class ConstRI;
class ConstfRI;
class NameRI;
class TempRI;
class BinOpRI;
class MemRI;
class CallRI;
class ESeqRI;
class MoveRI;
class ExprStmtRI;
class JumpRI;
class CJumpRI;
class SeqRI;

class Canonizador;
class VisitorMIPS;
class InstrucaoVisitor;
class RIVisitor;

#include "asa.h"
#include "tabelaSemantico.h"
#include "visitorRepIntermediaria.h"
#include "canonizador.h"
#include "visitorMIPS.h"
#include "RIVisitor.h"
#include "instrucaoVisitor.h"

class Temp {
    public:
        bool troca;
        bool reuso;
        char *nome;
        int  usado;

    public:
        Temp();
        Temp(char *nomeTemp);
        virtual ~Temp();
        void aceita(RIVisitor *visitor);
        void aceita(VisitorMIPS* visitor);
        void aceita(InstrucaoVisitor* visitor);
};

class TempList {
    public:
        Temp        *primeiro;
        TempList    *proximo;

    public:
        TempList(Temp *primeiro, TempList *proximo);
        virtual ~TempList();
};

class Label {
    public:
        char    *nome;
        int     type;

    public:
        Label();
        Label(char *nomeLabel);
        Label(char *prefixo, char *nome);
        virtual ~Label();
        void aceita(RIVisitor *visitor);
        void aceita(VisitorMIPS* visitor);
};

class LabelList {
    public:
        Label       *primeiro;
        LabelList   *proximo;

    public:
        LabelList(Label *primeiro, LabelList *proximo);
        virtual ~LabelList();
};

class StmtRI {
    public:
        StmtRI();
        virtual ~StmtRI();
        virtual void aceita(RIVisitor *visitor) = 0;
        virtual void aceita(Canonizador *c) = 0;
        virtual void aceita(VisitorMIPS* visitor) = 0;
};

class StmtRIList {
    public:
        StmtRI      *primeiro;
        StmtRIList  *proximo;

    public:
        StmtRIList(StmtRI *primeiro, StmtRIList *proximo);
        virtual ~StmtRIList();
        void aceita(RIVisitor *visitor);
        void aceita(VisitorMIPS* visitor);
};

class ExprRI {
    public:
        int type;

    public:
        ExprRI();
        virtual ~ExprRI();
        virtual void aceita(RIVisitor *visitor) = 0;
        virtual void aceita(Canonizador *c) = 0;
        virtual Temp *aceita(VisitorMIPS* visitor) = 0;
};

class ExprRIList {
    public:
        ExprRI      *primeiro;
        ExprRIList  *proximo;

    public:
        ExprRIList(ExprRI *primeiro, ExprRIList *proximo);
        virtual ~ExprRIList();
        void aceita(RIVisitor *visitor);
        void aceita(VisitorMIPS* visitor);
};

class ExprRIListList {
    public:
        ExprRIList      *primeiro;
        ExprRIListList  *proximo;

    public:
        ExprRIListList(ExprRIList *primeiro, ExprRIListList *proximo);
        virtual ~ExprRIListList();
};

class AcessoLocal {
    public:
        char* id;

    public:
        virtual ExprRI *codigoAcesso() = 0;
        virtual void aceita(RIVisitor *visitor) = 0;
        virtual void aceita(VisitorMIPS* visitor) = 0;
};

class InFrame : public AcessoLocal {
    public:
        int  offset;
        bool endereco;

    public:
        InFrame(int offset, char *id, bool endereco);
        virtual ~InFrame();
        ExprRI *codigoAcesso();
        void aceita(RIVisitor *visitor);
        void aceita(VisitorMIPS* visitor);
};

class InReg : public AcessoLocal {
    public:
        Temp *temporario;

    public:
        InReg(Temp *temporario, char*id);
        virtual ~InReg();
        ExprRI *codigoAcesso();
        void aceita(RIVisitor *visitor);
        void aceita(VisitorMIPS* visitor);
};

class ListaAcesso {
    public:
        AcessoLocal *primeiro;
        ListaAcesso *proximo;

    public:
        ListaAcesso(AcessoLocal *primeiro, ListaAcesso *proximo);
        ~ListaAcesso();
        void aceita(RIVisitor *visitor);
        void aceita(VisitorMIPS* visitor);
};

class Frame {
    public:
        Label       *rotulo;
        ListaAcesso *dadosLocais;
        Temp        *valorRetorno;
        int         contadorParametros;
        int         tamanhoFrame;
        int         offsetLocal;
        int         offsetParametro;
        ListaAcesso *ultimoDadoLocal;

    public:
        Frame();
        virtual ~Frame();
        virtual void adicionaParametro(bool escapa, int tamBytes, char *nome, bool endereco) = 0;
        virtual void adicionaLocal(bool escapa, int tamBytes, char *nome) = 0;
        virtual AcessoLocal *buscaVariavel(char *variavel) = 0;
        virtual void aceita(RIVisitor *visitor) = 0;
        virtual void aceita(VisitorMIPS* visitor) = 0;
};

class FrameMIPS : public Frame {
    public:
        FrameMIPS();
        virtual ~FrameMIPS();
        void adicionaParametro(bool escapa, int tamBytes, char *nome, bool endereco);
        void adicionaLocal(bool escapa, int tamBytes, char *nome);
        AcessoLocal *buscaVariavel(char *variavel);
        void aceita(RIVisitor *visitor);
        void aceita(VisitorMIPS* visitor);
};

class Fragment {
    public:
        Fragment *proximo;

    public:
        Fragment();
        void adicionaNoFim(Fragment *fragmento);
        virtual ~Fragment();
        virtual void aceita(RIVisitor *visitor) = 0;
        virtual void aceita(Canonizador *canonizador) = 0;
        virtual void aceita(VisitorMIPS* visitor) = 0;
};

class Procedure : public Fragment {
    public:
        Frame  *frame;
        StmtRI *body;

    public:
        Procedure(Frame *frame, StmtRI *body);
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        void aceita(VisitorMIPS* visitor);
};

class LabelRI : public StmtRI {
    public:
        Label *nome;

    public:
        LabelRI(Label *nome);
        ~LabelRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        void aceita(VisitorMIPS* visitor);
};

class LiteralRI : public Fragment {
    public:
        LabelRI     *nome;
        LiteralNodo *literal;

    public:
        LiteralRI(LabelRI *nome, LiteralNodo *literal);
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        void aceita(VisitorMIPS* visitor);
};

class VariavelNaoLocal : public Fragment {
    public:
        Label   *nome;
        int     tipo;
        int     quantidade;

    public:
        VariavelNaoLocal(int tipo, int quantidade, VariavelNaoLocal *proximo);
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        void aceita(VisitorMIPS* visitor);
};

class ConstRI : public ExprRI {
    public:
        int valor;

    public:
        ConstRI(int valor);
        virtual ~ConstRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        Temp *aceita(VisitorMIPS* visitor);
};

class ConstfRI : public ExprRI {
    public:
        float valor;

    public:
        ConstfRI(float valor);
        virtual ~ConstfRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        Temp *aceita(VisitorMIPS* visitor);
};

class NameRI : public ExprRI {
    public:
        Label *nome;

    public:
        NameRI(Label *nome);
        virtual ~NameRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        Temp *aceita(VisitorMIPS* visitor);
};

class TempRI : public ExprRI {
    public:
    Temp *temporario;

    public:
        TempRI(Temp *temporario);
        virtual ~TempRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        Temp *aceita(VisitorMIPS* visitor);
};

class BinOpRI : public ExprRI {
    public:
        int     operador;
        ExprRI  *exp1;
        ExprRI  *exp2;

    public:
        BinOpRI(int operador, ExprRI *exp1, ExprRI *exp2);
        virtual ~BinOpRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        Temp *aceita(VisitorMIPS* visitor);
};

class MemRI : public ExprRI {
    public:
        ExprRI *exp;

    public:
        MemRI(ExprRI *exp);
        virtual ~MemRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        Temp *aceita(VisitorMIPS* visitor);
};

class CallRI : public ExprRI {
    public:
        Label       *funcao;
        ExprRIList  *argumentos;
        bool canonizado;

    public:
        CallRI(Label *funcao, ExprRIList *argumentos);
        virtual ~CallRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        Temp *aceita(VisitorMIPS* visitor);
};

class ESeqRI : public ExprRI {
    public:
        StmtRI *stmt;
        ExprRI *exp;

    public:
        ESeqRI(StmtRI *stmt, ExprRI *exp);
        virtual ~ESeqRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        Temp *aceita(VisitorMIPS* visitor);
};

class MoveRI : public StmtRI {
    public:
        ExprRI *destino;
        ExprRI *origem;

    public:
        MoveRI(ExprRI *destino, ExprRI *origem);
        virtual ~MoveRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        void aceita(VisitorMIPS* visitor);
};

class ExprStmtRI : public StmtRI {
    public:
        ExprRI *exp;

    public:
        ExprStmtRI(ExprRI *exp);
        virtual ~ExprStmtRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* c);
        void aceita(VisitorMIPS* visitor);
};

class JumpRI : public StmtRI {
    public:
        ExprRI *exp;

    public:
        JumpRI(ExprRI *exp);
        virtual ~JumpRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador *canonizador);
        void aceita(VisitorMIPS* visitor);
};

class CJumpRI : public StmtRI {
    public:
        int     operadorRel;
        ExprRI  *esquerda;
        ExprRI  *direita;
        Label   *verdadeiro;
        Label   *falso;
        bool canonizado;

    public:
        CJumpRI(int operadorRel, ExprRI *esquerda, ExprRI *direita, Label *verdadeiro, Label *falso);
        virtual ~CJumpRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador *canonizador);
        void aceita(VisitorMIPS* visitor);
};

class SeqRI : public StmtRI {
    public:
        StmtRI  *esquerda;
        StmtRI  *direita;

    public:
        SeqRI(StmtRI *esquerda, StmtRI *direita);
        virtual ~SeqRI();
        void aceita(RIVisitor *visitor);
        void aceita(Canonizador* canonizador);
        void aceita(VisitorMIPS* visitor);
};

#endif
