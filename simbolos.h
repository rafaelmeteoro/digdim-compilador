#ifndef _SIMBOLOS_H
#define _SIMBOLOS_H

#include "lexico.h"

#define TAMANHO_TABELA          211
#define TAMANHO_INICAL_LEXEMA   101
#define INCREMENTO_LEXEMA       50
#define EOS                     '\0'

/*****************************************
            CLASSES SIMBOLOS
******************************************/

class Simbolo{
    /** Atributo de Simbolo */
    public:
        int indice;
        Simbolo *prox;
};

class SimboloID : public Simbolo{
    /** Construtor de SimboloID */
    public:
        SimboloID(int indice, SimboloID *prox);
};

class SimboloLiteral : public Simbolo{
    /** Construtor de SimboloLiteral */
    public:
        SimboloLiteral(int indice, SimboloLiteral *prox);
};

class SimboloReservada : public Simbolo{
    /** Atributos de SimboloReservada */
    public:
        int token;

    /** Construtor de SimboloReservada */
    public:
        SimboloReservada(int indice, int token, SimboloReservada *prox);
};

/*****************************************
            CLASSES TABELAS
******************************************/

/** Classe Tabela que receberá os simbolos */
class TabelaGenerica{
    /** Atributos genéricos da Tabela */
    public:
        int proxPosicao;                    /** Indica em que posição algum lexema vai ser inserido */
        unsigned int tamanhoVetorLexema;    /** Tamanho atual do vetor de lexemas */
        char *vetorLexemas;                 /** Vetor com os lexemas dos simbolos */
        Simbolo *tabela[TAMANHO_TABELA];

    public:
        TabelaGenerica();
        ~TabelaGenerica();
        Simbolo *busca(char *lexema);
        char *retornaLexema(int indice);
        int hash(char *s);
};

class TabelaID : public TabelaGenerica{
    /** Construtores e funções da TabelaID */
    public:
        void imprimirTabela();
        void insere(char *lexema);
};

class TabelaLiterais : public TabelaGenerica{
    /** Construtores e funções da TabelaLiteral */
    public:
        void imprimirTabela();
        void insere(char *lexema);
};

class TabelaReservada : public TabelaGenerica{
    /** Construtores e funções da TabelaReservada */
    public:
        void imprimirTabela();
        void insere(char *lexema, int token);
};

/*****************************************
            FUNÇÕES EXTRAS
******************************************/

/** Inicia a tabela de Reservadas */
void iniciaTabela();

/** Imprime todas as tabelas */
void imprimirTabelas();

/** Funções para inserer nas tabelas*/
int insereID(char *lexema);
int insereLiteral(char *lexema);

#endif
