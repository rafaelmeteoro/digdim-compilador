#include <stdio.h>

#include "buffer.h"

FILE* arquivo;
char buffer[TAMANHO_BUFFER+1];
int bufferPosicao; //Posição do buffer a ser retornado

/** Constrói o buffer lendo TAMANHO_BUFFER do arquivo para dentro do buffer */
void construirBuffer(FILE *_arquivo){
    arquivo = _arquivo;
    bufferPosicao = 0;
    int contador = fread(buffer, sizeof(char), TAMANHO_BUFFER * sizeof(char), arquivo);
    if(contador < TAMANHO_BUFFER){
        buffer[contador] = SENTINELA;
    }
}

/** Fecha o arquivo de leitura */
void destruirBuffer(){
    fclose(arquivo);
}

void recarregarBuffer(){
    int contador = fread(buffer, sizeof(char), TAMANHO_BUFFER * sizeof(char), arquivo);
    if(contador < TAMANHO_BUFFER){
        buffer[contador] = SENTINELA;
    }
    bufferPosicao = 0;
}

/** Retorna o caracter do buffer na posição bufferPosicao - 1*/
char getNextChar(){
    if(feof(arquivo) and buffer[bufferPosicao] == SENTINELA){
        return EOF;
    }else if(bufferPosicao >= TAMANHO_BUFFER){ //if(buffer[bufferPosicao] == SENTINELA){
        recarregarBuffer();
        return getNextChar();
    }else{
        bufferPosicao++;
        return buffer[bufferPosicao-1];
    }
}
