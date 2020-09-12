#ifndef _LEXICO_H
#define _LEXICO_H

#include "erro.h"
#include "buffer.h"
#include "simbolos.h"

#define NUMINT          100
#define NUMREAL         101
#define ID              102
#define LITERAL         103
#define CARACTER        104
#define ABRECOLCH       105
#define FECHACOLCH      106
#define ABRECHAVE       107
#define FECHACHAVE      108
#define ABREPARENT      109
#define FECHAPARENT     110
#define VIRGULA         111
#define SETA            112
#define PONTO           113
#define PONTOVIRGULA    114
#define DOISPONTOS      115
#define ATRIBUICAO      116
#define IGUAL           117
#define MENOR           118
#define MENORIGUAL      119
#define MAIORIGUAL      120
#define MAIOR           121
#define DIFERENTE       122
#define ADICAO          123
#define SUBTRACAO       124
#define BIT_OU          125
#define MULT            126
#define DIV             127
#define MOD             128
#define BIT_E           129
#define ELOGICO         130
#define OULOGICO        131
#define NEGACAO         132
#define IF              133
#define ELSE            134
#define WHILE           135
#define SWITCH          136
#define CASE            137
#define BREAK           138
#define PRINT           139
#define READLN          140
#define RETURN          141
#define THROW           142
#define TRY             143
#define CATCH           144
#define TRUE            145
#define FALSE           146
#define INT             147
#define FLOAT           148
#define BOOLEAN         149
#define CHAR            150
#define TYPEDEF         151
#define STRUCT          152
#define FIMARQUIVO      153

#define TAMANHO_ORIGINAL_LEXEMA 21
#define LEXEMA_INCREMENTO   10

/** Tamanho da extensão .cmm */
#define TAMANHO_EXTENSAO 4

int getLinha();
char *getLexema();
char getCaractere();
void lexicoConstrutor(FILE *_arquivo);
void lexicoDestrutor();
int getNextToken();
void inserirNoLexema(char c);
void reiniciaLexema();
char *getNomeToken(int indice);
char *uperCase(char *nome);

#endif
