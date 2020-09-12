#ifndef _ERRO_H
#define _ERRO_H

#include "lexico.h"

#define ERRO_ABRIR_ARQUIVO          0
#define ERRO_CONSTRUCAO_LITERAL     1
#define ERRO_CONSTRUCAO_CARACTER    2
#define ERRO_COMENTARIO             3
#define ERRO_NUMERO                 4
#define ERRO_CARACTER_INVALIDO      5
#define ERRO_TOKEN_EXPERADO         6
#define ERRO_TOKEN_INVALIDO         7
#define ERRO_ID_NAO_DECLARADO       8
#define ERRO_ID_DECLARADO           9
#define ERRO_OP_ARRAY               10
#define ERRO_INCOMPATIBILIDADE      11
#define ERRO_INCOMP_IGUALDADE       12
#define ERRO_CONDICAO               13
#define ERRO_CONDICAO_WHILE         14
#define ERRO_CONDICAO_SWITCH        15
#define ERRO_CONDICAO_PRINT         16
#define ERRO_CONDICAO_CASEBLOCK     17
#define ERRO_CONDICAO_BREAK         18
#define ERRO_CONDICAO_READ          19
#define ERRO_CONDICAO_RETURN        20
#define ERRO_CALL                   21
#define ERRO_CALL_PARAMETROS        22
#define ERRO_LVALUE                 23
#define ERRO_ID_NAO_FUNCAO          24
#define ERRO_ID_INADEQUADO          25
#define ERRO_ID_ESPERADO            26
#define ERRO_EXPR_ARRANJO           27
#define ERRO_INDICE_ARRANJO         28
#define ERRO_LIMITE_ARRANJO         29
#define ERRO_ATRIBUTO               30
#define ERRO_ID_TIPO_ESPERADO       31
#define ERRO_ID_TIPO                32
#define ERRO_ID_FUNCAO              33
#define ERRO_ARGUMENTOS_MAIN               34

int getTemErros();

void erro(int tipo, int linha);

void erroSintatico(int token, int tipoErro, int linha);

void erroSemantico(int tipoErro, char *texto, int escopo, int linha);

#endif
