#ifndef _BUFFER_H
#define _BUFFER_H

#define TAMANHO_BUFFER 1024
#define FIM_DE_ARQUIVO 26
#define SENTINELA   '\0'

void construirBuffer(FILE *_arquivo);
void destruirBuffer();
void recarregarBuffer();
char getNextChar();

#endif
