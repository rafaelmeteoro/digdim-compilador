#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "erro.h"

static int temErros = 0;

int getTemErros(){
    return temErros;
}

void erro(int tipo, int linha){
    temErros = 1;
    switch(tipo){
        case ERRO_ABRIR_ARQUIVO : fprintf(stderr, "ERRO AO ABIRAR ARQUIVO.\n");
            break;
        case ERRO_CONSTRUCAO_LITERAL : fprintf(stderr, "ERRO AO FORMAR LITERAL (LINHA %d).\n", linha);
            break;
        case ERRO_CONSTRUCAO_CARACTER : fprintf(stderr, "ERRO AO FORMAR CARACTERE (LINHA %d).\n", linha);
            break;
        case ERRO_COMENTARIO : fprintf(stderr, "ERRO AO FORMAR COMENTARIO (LINHA %d).\n", linha);
            break;
        case ERRO_NUMERO : fprintf(stderr, "ERRO AO FORMAR NUMERO (LINHA %d).\n", linha);
            break;
        case ERRO_CARACTER_INVALIDO : fprintf(stderr, "ERRO, CARACTER INVALIDO (LINHA %d).\n", linha);
            break;
    }
}

void erroSintatico(int token, int tipoErro, int linha){
    temErros = 1;
    if(tipoErro == ERRO_TOKEN_EXPERADO){
        fprintf(stderr, "ERRO SINTATICO: %s ESPERADO. (LINHA: %d).\n", getNomeToken(token), linha);
    }else if(tipoErro == ERRO_TOKEN_INVALIDO){
        fprintf(stderr, "ERRO SINTATICO: %s NAO ESPERADO. (LINHA: %d).\n", getNomeToken(token), linha);
    }
}

void erroSemantico(int tipoErro, char *texto, int escopo, int linha){
    temErros = 1;
    if(tipoErro == ERRO_OP_ARRAY){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: OPERANDO E OPERACOES BINARIAS NAO PODEM SER ARRAYS.\n", linha);
    }else if(tipoErro == ERRO_CONDICAO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: CONDICAO DO %s DEVE SER BOOLEANO.\n", linha, texto);
    }else if(tipoErro == ERRO_CONDICAO_WHILE){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: CONDICAO DO \"WHILE\" DEVE SER BOOLEANO.\n", linha);
    }else if(tipoErro == ERRO_CONDICAO_SWITCH){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: CONDICAO DO \"SWITCH\" DEVE SER UM VALOR INTEIRO.\n", linha);
    }else if(tipoErro == ERRO_CONDICAO_PRINT){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: PRINT PRECISA DE PARAMETROS.\n", linha);
    }else if(tipoErro == ERRO_CONDICAO_CASEBLOCK){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: CONDICAO DO CASE DEVE SER UM VALOR ITEIRO.\n", linha);
    }else if(tipoErro == ERRO_CONDICAO_BREAK){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: BREAK USADO NO LUGAR ERRADO.\n", linha);
    }else if(tipoErro == ERRO_CONDICAO_READ){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: EXPRESSAO DO READ ERRADO.\n", linha);
    }else if(tipoErro == ERRO_CONDICAO_RETURN){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: TIPO DE RETORNO INCOMPATIVEL.\n", linha);
    }else if(tipoErro == ERRO_CALL){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: EXPRESSAO DA CHAMADA DE FUNCAO.\n", linha);
    }else if(tipoErro == ERRO_CALL_PARAMETROS){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: QUANTIDADE DE PARAMETROS INCORRETO.\n", linha);
    }else if(tipoErro == ERRO_LVALUE){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: LADO ESQUERDO DA ATRIBUICAO NAO E UM L-VALUE.\n", linha);
    }else if(tipoErro == ERRO_ID_NAO_FUNCAO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: IDENTIFICADOR NA CHAMADA NAO E FUNCAO.\n", linha);
    }else if(tipoErro == ERRO_ID_INADEQUADO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: UTILIZACAO INADEQUADA DE ID NA FUNCAO.\n", linha);
    }else if(tipoErro == ERRO_ID_ESPERADO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: IDENTIFICADOR DE TIPO ESPERADO.\n", linha);
    }else if(tipoErro == ERRO_EXPR_ARRANJO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: EXPRESSAO ANTES DE '[' DEVE SER ARRANJO.\n", linha);
    }else if(tipoErro == ERRO_INDICE_ARRANJO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: INDICE DO ARRANJO DEVE SER INTEIRO.\n", linha);
    }else if(tipoErro == ERRO_LIMITE_ARRANJO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: INDICE DO ARRANJO FORA DOS LIMITES.\n", linha);
    }else if(tipoErro == ERRO_ATRIBUTO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: ANTES DO '.' DEVE SER UM IDENTIFICADOR.\n", linha);
    }else if(tipoErro == ERRO_INCOMPATIBILIDADE){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: INCOMPATIBILIDADE DE TIPOS NO OPERADOR LOGICO %s.\n", linha, texto);
    }else if(tipoErro == ERRO_INCOMP_IGUALDADE){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: INCOMPATIBILIDADE DE TIPOS NA ATRIBUICAO.\n", linha);
    }else if(tipoErro == ERRO_ID_NAO_DECLARADO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: IDENTIFICADOR %s NAO DECLARADO.\n", linha, texto);
    }else if(tipoErro == ERRO_ARGUMENTOS_MAIN){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: A FUNCAO %s NAO PODE TER ARGUMENTOS.\n", linha, texto);
    }else if(tipoErro == ERRO_ID_DECLARADO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: IDENTIFICADOR '%s' JA DECLARADO NO ESCOPO %d.\n", linha, texto, escopo);
    }else if(tipoErro == ERRO_ID_TIPO_ESPERADO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: IDENTIFICADOR DE TIPO ESPERADO.\n", linha);
    }else if(tipoErro == ERRO_ID_TIPO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: IDENTIFICADOR DE TIPO UTILIZADO ERRADO.\n", linha);
    }else if(tipoErro == ERRO_ID_FUNCAO){
        fprintf(stderr, "ERRO SEMANTICO NA LINHA %d: IDENTIFICADOR DE FUNCAO UTILIZADO ERRADO.\n", linha);
    }
}
