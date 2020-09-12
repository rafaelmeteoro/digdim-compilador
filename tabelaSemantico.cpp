#include <stdio.h>
#include <string.h>

#include "tabelaSemantico.h"

/*****************************************
            TABELA SEMANTICO
******************************************/
TabelaSemantico::TabelaSemantico(){
    for(int i = 0; i < TAMANHO_TABELA; i++){
        this->tabela[i] = NULL;
    }
}

TabelaSemantico::~TabelaSemantico(){
    for(int i = 0; i < TAMANHO_TABELA; i++){
        ItemTabela *aux = this->tabela[i];
        while(aux != NULL){
            ItemTabela *auxProx = aux->prox;
            delete aux;
            aux = auxProx;
        }
    }
}

void TabelaSemantico::incluir(ItemTabela *item){
    int indice = item->hash();
    ItemTabela *itemTabela = this->tabela[indice];
    while(itemTabela != NULL){
        if(item->igual(itemTabela)){
            return;
        }
        itemTabela = itemTabela->prox;
    }
    item->prox = this->tabela[indice];
    this->tabela[indice] = item;
}

bool TabelaSemantico::remover(ItemTabela *item){
    int indice = item->hash();
    ItemTabela *itemTabela = this->tabela[indice];
    ItemTabela *itemTabelaAnt = NULL;
    while(itemTabela != NULL){
        if(item->igual(itemTabela)){
            if(itemTabelaAnt == NULL) this->tabela[indice] = itemTabela->prox;
            else itemTabelaAnt->prox = itemTabela->prox;
            delete item;
            return true;
        }
        itemTabelaAnt = itemTabela;
        itemTabela = itemTabela->prox;
    }
    return false;
}

ItemTabela *TabelaSemantico::buscar(ItemTabela *item){
    int indice = item->hash();
    ItemTabela *itemTabela = this->tabela[indice];
    while(itemTabela != NULL){
        if(item->igual(itemTabela)){
            return itemTabela;
        }
        itemTabela = itemTabela->prox;
    }
    return NULL;
}

/*****************************************
            ITEM TABELA
******************************************/
ItemTabela::ItemTabela(){
    this->prox = NULL;
}

ItemTabela::~ItemTabela(){}

int ItemTabela::hash(){
    char *c = this->identificador;
    char *p;
    unsigned int h = 0, g;
    for (p = c; *p != '\0'; p = p + 1) {
        h = (h << 4) + (*p);
        if ((g = h & 0xf0000000)) {
            h = h ^ (g >> 4);
            h = h ^g;
        }
    }
    return h % TAMANHO_TABELA;
}

/*****************************************
            ITEM ESCOPO
******************************************/
ItemEscopo::ItemEscopo(char *identificador, int escopo) : ItemTabela(){
    this->identificador = identificador;
    this->escopo = escopo;
    this->formalList = NULL;
    this->funcao = false;
    this->tipo = false;
}

ItemEscopo::~ItemEscopo(){}

bool ItemEscopo::igual(ItemTabela *item){
    return (!strcmp(this->identificador, item->identificador)) && (this->escopo == ((ItemEscopo*)item)->escopo);
}

/*****************************************
            ITEM TIPO
******************************************/
ItemTipo::ItemTipo(char *identificador, TabelaSemantico *tabelaEscopo){
    this->identificador = identificador;
    this->tabelaEscopo  = tabelaEscopo;
}

ItemTipo::~ItemTipo(){
    delete this->tabelaEscopo;
}

bool ItemTipo::igual(ItemTabela *item){
    return (!strcmp(this->identificador, ((ItemTipo*)item)->identificador));
}
