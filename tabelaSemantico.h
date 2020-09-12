#ifndef _TABELASEMANTICO_H
#define _TABELASEMANTICO_H

#include "asa.h"
#include "representacaoIntermediaria.h"

class TypeNodo;
class NumNodo;
class VarDeclNodo;

#define TAMANHO_TABELA 211

class TabelaSemantico;
class ItemTabela;

class TabelaSemantico{
    public:
        ItemTabela *tabela[TAMANHO_TABELA];

    public:
        TabelaSemantico();
        virtual ~TabelaSemantico();
        void incluir(ItemTabela *item);
        bool remover(ItemTabela *item);
        ItemTabela *buscar(ItemTabela *item);
};

class ItemTabela{
    public:
        char *identificador;
        ItemTabela *prox;

    public:
        ItemTabela();
        virtual ~ItemTabela();
        int hash();
        virtual bool igual(ItemTabela *item) = 0;
};

class ItemEscopo : public ItemTabela{
    public:
        bool        funcao;
        bool        tipo;
        int         escopo;
        TypeNodo    *type;
        NumNodo     *tamanhoArray;
        VarDeclNodo *formalList;

    public:
        ItemEscopo(char *identificador, int escopo);
        ~ItemEscopo();
        bool igual(ItemTabela *item);
};

class ItemTipo : public ItemTabela{
    public:
        int tamanho;
        TabelaSemantico *tabelaEscopo;
        VarDeclNodo     *variaveis;

    public:
        ItemTipo(char *identificador, TabelaSemantico *tabelaEscopo);
        ~ItemTipo();
        bool igual(ItemTabela *item);

};

#endif
