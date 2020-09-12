#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "simbolos.h"

/*****************************************
        CONSTRUTORES SIMBOLOS
******************************************/
SimboloID::SimboloID(int indice, SimboloID *prox){
    this->indice = indice;
    this->prox = prox;
}

SimboloLiteral::SimboloLiteral(int indice, SimboloLiteral *prox){
    this->indice = indice;
    this->prox = prox;
}

SimboloReservada::SimboloReservada(int indice, int token, SimboloReservada *prox){
    this->indice = indice;
    this->token = token;
    this->prox = prox;
}

/*****************************************
        CONSTRUTOR TABELA
******************************************/
TabelaGenerica::TabelaGenerica(){
    this->proxPosicao = 0;
    this->tamanhoVetorLexema = TAMANHO_INICAL_LEXEMA;
    this->vetorLexemas = (char*)malloc(TAMANHO_INICAL_LEXEMA * sizeof(char));
    for(int i = 0; i < TAMANHO_TABELA; i++) this->tabela[i] = NULL;
}

/*****************************************
        DESTRUTOR TABELA
******************************************/

TabelaGenerica::~TabelaGenerica(){
    for(int i = 0; i < TAMANHO_TABELA; i++){
        if(this->tabela[i] != NULL){
            while(this->tabela[i] != NULL){
                Simbolo *aux = this->tabela[i];
                this->tabela[i] = this->tabela[i]->prox;
                free(aux);
            }
        }
    }
    free(this->vetorLexemas);
}

/*****************************************
            INSERIR NAS TABELAS
******************************************/

/** Função para inserir o lexema na TabelaID, cria o simbolo e o insere na tabela */
void TabelaID::insere(char *lexema){
    int indiceTabela = hash(lexema);

    SimboloID *no = new SimboloID(this->proxPosicao, NULL);
    if(this->tabela[indiceTabela] == NULL){
        this->tabela[indiceTabela] = no;
    }else{
        no->prox = this->tabela[indiceTabela]->prox;
        this->tabela[indiceTabela]->prox = no;
    }

    /** Verifica se o lexema cabe dentro do vetorLexemas */
    if((this->proxPosicao + strlen(lexema) + 1) >= this->tamanhoVetorLexema){
        /** Se não couber, realloca para o vetorLexema o tamanho do lexema + tamanhoVetorLexema */
        this->vetorLexemas = (char*)realloc(this->vetorLexemas, (strlen(lexema) + this->tamanhoVetorLexema + 1) * sizeof(char));
        this->tamanhoVetorLexema += strlen(lexema) + 1;
    }

    strcpy(this->vetorLexemas + this->proxPosicao, lexema);
    this->proxPosicao += strlen(lexema) + 1;
    this->vetorLexemas[this->proxPosicao] = '\0';
}

/** Função para inserir o lexema na TabelaLiterais, cria o simbolo e o insere na tabela */
void TabelaLiterais::insere(char *lexema){
    int indiceTabela = hash(lexema);

    SimboloLiteral *no = new SimboloLiteral(this->proxPosicao, NULL);
    if(this->tabela[indiceTabela] == NULL){
        this->tabela[indiceTabela] = no;
    }else{
        no->prox = this->tabela[indiceTabela]->prox;
        this->tabela[indiceTabela]->prox = no;
    }

    /** Verifica se o lexema cabe dentro do vetorLexemas */
    if((this->proxPosicao + strlen(lexema) + 1) >= this->tamanhoVetorLexema){
        /** Se não couber, realloca para o vetorLexema o tamanho do lexema + tamanhoVetorLexema */
        this->vetorLexemas = (char*)realloc(this->vetorLexemas, (strlen(lexema) + this->tamanhoVetorLexema + 1) * sizeof(char));
        this->tamanhoVetorLexema += strlen(lexema) + 1;
    }

    strcpy(this->vetorLexemas + this->proxPosicao, lexema);
    this->proxPosicao += strlen(lexema) + 1;
    this->vetorLexemas[this->proxPosicao] = '\0';
}

/** Função para inserir o lexema na TabelaReservada, cria o simbolo e o insere na tabela com ser respectivo token */
void TabelaReservada::insere(char *lexema, int token){
    int indiceTabela = hash(lexema);

    SimboloReservada *no = new SimboloReservada(this->proxPosicao, token, NULL);
    if(this->tabela[indiceTabela] == NULL){
        this->tabela[indiceTabela] = no;
    }else{
        no->prox = this->tabela[indiceTabela]->prox;
        this->tabela[indiceTabela]->prox = no;
    }

    /** Verifica se o lexema cabe dentro do vetorLexemas */
    if((this->proxPosicao + strlen(lexema) + 1) >= this->tamanhoVetorLexema){
        /** Se não couber, realloca para o vetorLexema o tamanho do lexema + tamanhoVetorLexema */
        this->vetorLexemas = (char*)realloc(this->vetorLexemas, (strlen(lexema) + this->tamanhoVetorLexema + 1) * sizeof(char));
        this->tamanhoVetorLexema += strlen(lexema) + 1;
    }

    strcpy(this->vetorLexemas + this->proxPosicao, lexema);
    this->proxPosicao += strlen(lexema) + 1;
    this->vetorLexemas[this->proxPosicao] = '\0';
}

/*****************************************
        BUSCAR SIMBOLO NA TABELA
******************************************/
Simbolo *TabelaGenerica::busca(char *lexema){
    int indiceTabela = hash(lexema);
    if(this->tabela[indiceTabela] != NULL){
        Simbolo *noAux = this->tabela[indiceTabela];
        while(noAux != NULL){
            if(strcmp(lexema, TabelaGenerica::retornaLexema(noAux->indice)) == 0) return noAux;
            noAux = noAux->prox;
        }
    }
    return NULL;
}

/*****************************************
        BUSCAR LEXEMA NA TABELA
******************************************/
char *TabelaGenerica::retornaLexema(int indice){
    return (this->vetorLexemas + indice);
}

/*****************************************
        FUNÇÃO HASH TIRADA DO LIVRO
******************************************/
int TabelaGenerica::hash(char *s){
    char *p;
    unsigned h = 0, g;
    for(p = s; *p != EOS; p = p + 1){
        h = (h << 4) + (*p);
        if((g = h & 0xf0000000)){
            h = h ^ (g >> 24);
            h = h ^ g;
        }
    }
    return h % TAMANHO_TABELA;
}


/*****************************************
            IMPRIMIR TABELAS
******************************************/

/** TabelaID */
void TabelaID::imprimirTabela(){
    char* linha = (char*)"---------------------------------------";
    fprintf(stdout, "TABELA DE SIMBOLOS: IDENTIFICADORES\n");
    fprintf(stdout, "%s\n", linha);

    for(int i = 0; i < TAMANHO_TABELA; i++){
        if(this->tabela[i] != NULL){
            Simbolo *noAux = this->tabela[i];
            while(noAux != NULL){
                fprintf(stdout, "%s\n",this->retornaLexema(noAux->indice));
                noAux = noAux->prox;
            }
        }
    }
    fprintf(stdout, "\n");
}

/** TabelaLiterais*/
void TabelaLiterais::imprimirTabela(){
    char* linha = (char*)"---------------------------------------";
    fprintf(stdout, "TABELA DE SIMBOLOS: LITERAIS\n");
    fprintf(stdout, "%s\n", linha);

    for(int i = 0; i < TAMANHO_TABELA; i++){
        if(this->tabela[i] != NULL){
            Simbolo *noAux = this->tabela[i];
            while(noAux != NULL){
                fprintf(stdout, "%s\n", this->retornaLexema(noAux->indice));
                noAux = noAux->prox;
            }
        }
    }
    fprintf(stdout, "\n");
}

/** TabelaReservada */
void TabelaReservada::imprimirTabela(){
    char* linha = (char*)"---------------------------------------";
    fprintf(stdout, "TABELA DE SIMBOLOS: PALAVRAS RESERVADAS\n");
    fprintf(stdout, "%s\n", linha);
    fprintf(stdout, "LEXEMA \t\t\tToken numerico\n");
    fprintf(stdout, "%s\n", linha);

    for(int i = 0; i < TAMANHO_TABELA; i++){
        if(this->tabela[i] != NULL){
            SimboloReservada *noAux = (SimboloReservada*)this->tabela[i];
            while(noAux != NULL){
                fprintf(stdout, "%s\t\t\t%d\n", this->retornaLexema(noAux->indice), noAux->token);
                noAux = (SimboloReservada*)noAux->prox;
            }
        }
    }
    fprintf(stdout, "\n");
}

/*****************************************
        TABELAS E INICIALIZAÇÃO
******************************************/
TabelaID        tabelaID;
TabelaLiterais  tabelaLiterais;
TabelaReservada tabelaReservada;

void iniciaTabela(){
    tabelaReservada.insere(getNomeToken(IF),IF);
    tabelaReservada.insere(getNomeToken(ELSE),ELSE);
    tabelaReservada.insere(getNomeToken(WHILE),WHILE);
    tabelaReservada.insere(getNomeToken(SWITCH),SWITCH);
    tabelaReservada.insere(getNomeToken(CASE), CASE);
    tabelaReservada.insere(getNomeToken(BREAK),BREAK);
    tabelaReservada.insere(getNomeToken(PRINT),PRINT);
    tabelaReservada.insere(getNomeToken(READLN),READLN);
    tabelaReservada.insere(getNomeToken(RETURN),RETURN);
    tabelaReservada.insere(getNomeToken(THROW),THROW);
    tabelaReservada.insere(getNomeToken(TRY),TRY);
    tabelaReservada.insere(getNomeToken(CATCH),CATCH);
    tabelaReservada.insere(getNomeToken(TRUE),TRUE);
    tabelaReservada.insere(getNomeToken(FALSE),FALSE);
    tabelaReservada.insere(getNomeToken(INT),INT);
    tabelaReservada.insere(getNomeToken(FLOAT),FLOAT);
    tabelaReservada.insere(getNomeToken(BOOLEAN),BOOLEAN);
    tabelaReservada.insere(getNomeToken(CHAR),CHAR);
    tabelaReservada.insere(getNomeToken(TYPEDEF),TYPEDEF);
    tabelaReservada.insere(getNomeToken(STRUCT),STRUCT);
}

void imprimirTabelas(){
    tabelaReservada.imprimirTabela();
    tabelaID.imprimirTabela();
    tabelaLiterais.imprimirTabela();
}

/** Funções usadas no lexico para inserir os lexemas nas determinadas tabelas */
int insereID(char *lexema){
    /** Verifica se o lexema esta na tabelaReservada, se não estiver insere na tabelaID */
    SimboloReservada *reservado = (SimboloReservada*)tabelaReservada.busca(lexema);
    if(reservado != NULL) return reservado->token;

    Simbolo *aux = tabelaID.busca(lexema);
    if(aux == NULL){
        tabelaID.insere(lexema);
        return ID;
    }else return ID;
}

int insereLiteral(char *lexema){
    Simbolo *aux = tabelaLiterais.busca(lexema);
    if(aux == NULL){
        /** Se o lexema começar com aspas simples (') é um caracter*/
        if(lexema[0] == '\''){
            tabelaLiterais.insere(lexema);
            return CARACTER;
        }else{
            tabelaLiterais.insere(lexema);
            return LITERAL;
        }
    }else{
        if(lexema[0] == '\''){
            return CARACTER;
        }else{
            return LITERAL;
        }
    }
}
