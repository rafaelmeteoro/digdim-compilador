#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexico.h"

/** Variáveis globais do léxico */
int linha;                      //contador de linhas
char caractere;                 //recebe o caracter do buffer
char *lexema;                   //armazena os lexemas formados à medida que os caracteres são consumidos
int tamanhoLexema;              //tamanho do vetor de lexema
int posicaoLexema;              //posição onde o lexema vai ser inserido
int tamanho_atual;

int getLinha(){
    return linha;
}

char *getLexema(){
    return lexema;
}

char getCaractere(){
    return caractere;
}

/** Construtor do lexico, inicia as variaveis e monta a tabela de palavras reservadas */
void lexicoConstrutor(FILE *_arquivo){
    construirBuffer(_arquivo);
    iniciaTabela();
    linha = 1;
    tamanhoLexema = 0;
    posicaoLexema = 0;
    tamanho_atual = TAMANHO_ORIGINAL_LEXEMA;
    lexema = (char* )malloc(TAMANHO_ORIGINAL_LEXEMA * sizeof(char));
    caractere = getNextChar();
}

void lexicoDestrutor(){
    destruirBuffer();
    free(lexema);
}

/**Função que implementa o AFD
Retorna um token */
int getNextToken(){
    reiniciaLexema();
    int estado = 1;
    while(true){
        switch(estado){
            case 1: if (caractere == '\n'){
                        estado = 1;
                        linha++;
                        caractere = getNextChar();
                        break;
                    }
                    if (caractere == '\r'){
                        estado = 1;
                        caractere = getNextChar();
                        break;
                    }
                    if (caractere == '\t'){
                        estado = 1;
                        caractere = getNextChar();
                        break;
                    }
                    if (isspace(caractere)){
                        estado = 1;
                        caractere = getNextChar();
                        break;
                    }
                    if (isalpha(caractere) || (caractere == '_')){
                        estado = 11;           //estado do autômato quando consome um alfanumerico
                        inserirNoLexema(caractere);     //se for um ID, incluir no lexema a ser passado pra respectiva tabela de simbolos
                        caractere = getNextChar();
                        break;
                    }
                    if (isdigit(caractere)){
                        estado =  2;          //estado do autômato quando consome um digito
                        inserirNoLexema(caractere);
                        caractere = getNextChar();
                        break;
                    }
                    switch (caractere){
                        case '"':
                            estado = 13;
                            inserirNoLexema(caractere);     //se for um literal, incluir no lexema a ser passado pra respectiva tabela de simbolos
                            caractere = getNextChar();
                            break;
                        case '\'':
                            estado = 15;
                            inserirNoLexema(caractere);
                            caractere = getNextChar();
                            break;
                        case '[': caractere = getNextChar(); return ABRECOLCH;
                        case ']': caractere = getNextChar(); return FECHACOLCH;
                        case '{': caractere = getNextChar(); return ABRECHAVE;
                        case '}': caractere = getNextChar(); return FECHACHAVE;
                        case '(': caractere = getNextChar(); return ABREPARENT;
                        case ')': caractere = getNextChar(); return FECHAPARENT;
                        case ',': caractere = getNextChar(); return VIRGULA;
                        case ';': caractere = getNextChar(); return PONTOVIRGULA;
                        case ':': caractere = getNextChar(); return DOISPONTOS;
                        case '+': caractere = getNextChar(); return ADICAO;
                        case '*': caractere = getNextChar(); return MULT;
                        case '%': caractere = getNextChar(); return MOD;
                        case EOF: return EOF;
                        case '=':
                            estado = 28;
                            caractere = getNextChar();
                            break;
                        case '<':
                            estado = 31;
                            caractere = getNextChar();
                            break;
                        case '!':
                            estado = 37;
                            caractere = getNextChar();
                            break;
                        case '-':
                            estado = 41;
                            caractere = getNextChar();
                            break;
                        case '/':
                            estado = 43;
                            caractere = getNextChar();
                            break;
                        case '|':
                            estado = 45;
                            caractere = getNextChar();
                            break;
                        case '&':
                            estado = 48;
                            caractere = getNextChar();
                            break;
                        case '>':
                            estado = 34;
                            caractere = getNextChar();
                            break;
                        case '.':
                            estado = 61;
                            inserirNoLexema(caractere);
                            caractere = getNextChar();
                            break;
                        default:
                            caractere = getNextChar();
                            erro(ERRO_CARACTER_INVALIDO, getLinha());
                            break;
                    }
            break;
            case 2: if (isdigit(caractere)){
                        estado = 2;
                        inserirNoLexema(caractere);
                        caractere = getNextChar();
                        break;
                    }else{
                         if (caractere == 'E' || caractere == 'e'){
                            estado = 7;
                            inserirNoLexema(caractere);
                            caractere = getNextChar();
                            break;
                         }else{
                             if (caractere == '.'){
                                 estado = 4;
                                 inserirNoLexema(caractere);
                                 caractere = getNextChar();
                                 break;
                             }
                             else{
                                 return NUMINT;
                             }
                        }
                   }
            case 4: if (isdigit(caractere)){
                        estado = 5;
                        inserirNoLexema(caractere);
                        caractere = getNextChar();
                        break;
                    }else{
                        if (caractere == 'E' || caractere == 'e'){
                            estado = 7;
                            inserirNoLexema(caractere);
                            caractere = getNextChar();
                            break;
                        }else{
                            estado = 6; break;
                        }
                    }
            case 5: if (isdigit(caractere)){
                        inserirNoLexema(caractere);
                        caractere = getNextChar();
                        break;
                    }else{
                        if (caractere == 'E' || caractere == 'e'){
                            estado = 7;
                            inserirNoLexema(caractere);
                            caractere = getNextChar();
                            break;
                        }else{
                            estado = 6; break;
                        }
                    }
            case 6:{
                return NUMREAL;
            }
            case 7: if (isdigit(caractere)){
                        estado = 5;
                        inserirNoLexema(caractere);
                        caractere = getNextChar();
                        break;
                    }
                    else{
                        if (caractere == '+' || caractere == '-'){
                            estado = 8;
                            inserirNoLexema(caractere);
                            caractere = getNextChar();
                            break;
                        }else{
                            estado = 1;
                            erro(ERRO_NUMERO, getLinha());
                            break;
                        }
                    }
            case 8: if (isdigit(caractere)){
                        estado = 5;
                        inserirNoLexema(caractere);
                        caractere = getNextChar();
                        break;
                    }else{
                        estado = 1;
                        erro(ERRO_NUMERO, getLinha());
                        break;
                    }
            case 10: if (isdigit(caractere)){
                         estado = 10;
                         inserirNoLexema(caractere);
                         caractere = getNextChar();
                         break;
                     }else{
                         if (caractere == 'E' || caractere == 'e'){
                            estado = 7;
                            inserirNoLexema(caractere);
                            caractere = getNextChar();
                            break;
                         }else{
                            estado = 6;
                            break;
                         }
                     }
            case 11: if (isdigit(caractere) || isalpha(caractere)){
                         estado = 11;
                         inserirNoLexema(caractere);    //se for um ID, incluir no lexema a ser passado pra respectiva tabela de simbolos
                         caractere = getNextChar();
                         break;
                     }else estado = 12;
            case 12:{
                estado = 1;
                return insereID(lexema);
                //return ID;
            }
            case 13: if (caractere == EOF){
                         estado = 1;
                         erro(ERRO_CONSTRUCAO_LITERAL, getLinha());
                         return EOF;
                     }else{
                         if(caractere == '"'){
                             estado = 14;
                             inserirNoLexema(caractere);   //se for um literal, incluir no lexema a ser passado pra respectiva tabela de simbolos
                             caractere = getNextChar();
                             break;
                         }else{
                             if(caractere == '\n'){
                                 estado = 1;
                                 erro(ERRO_CONSTRUCAO_LITERAL, getLinha());
                                 reiniciaLexema();
                                 break;
                             }else{
                                 estado = 13;
                                 inserirNoLexema(caractere);
                                 caractere = getNextChar();
                                 break;
                             }
                         }
                     }
            case 14:{
                return insereLiteral(lexema);
            }
            case 15: if (caractere == EOF){
                         erro(ERRO_CONSTRUCAO_CARACTER, getLinha());
                         return EOF;
                     }else{
                         estado = 59;
                         inserirNoLexema(caractere);
                         caractere = getNextChar();
                         break;
                     }
            case 16:{
                return insereLiteral(lexema);
            }
            case 28: if (caractere == '='){
                        caractere = getNextChar();
                        return IGUAL;
                     }else return ATRIBUICAO;
            case 31: if (caractere == '='){
                         caractere = getNextChar();
                         return MENORIGUAL;
                     }else return MENOR;
            case 34: if (caractere == '='){
                         caractere = getNextChar();
                         return MAIORIGUAL;
                     }else return MAIOR;
            case 37: if (caractere == '='){
                         caractere = getNextChar();
                         return DIFERENTE;
                     }else return NEGACAO;
            case 41: if (caractere == '>'){
                         caractere = getNextChar();
                         return SETA;
                     }else return SUBTRACAO;
            case 43: if (caractere == '/'){
                         estado = 52;
                         caractere = getNextChar();
                         break;
                     }else{
                        if (caractere == '*'){
                            estado = 55;
                            caractere = getNextChar();
                            break;
                        }else return DIV;
                     }
            case 45: if (caractere == '|'){
                        estado = 1;
                        caractere = getNextChar();
                        return OULOGICO;
                     }else return BIT_OU;
            case 48: if (caractere == '&'){
                         caractere = getNextChar();
                         return ELOGICO;
                     }else return BIT_E;
            case 52: if (caractere == '\n'){
                         estado = 1;
                         linha++;
                         caractere = getNextChar();
                         break;
                     }else{
                        caractere = getNextChar();
                        break;
                     }
            case 55: if (caractere == '*'){
                         estado = 56;
                         caractere = getNextChar();
                         break;
                     }else {
                         if (caractere == EOF){
                             erro(ERRO_COMENTARIO, getLinha());
                             return EOF;
                         }else {
                             caractere = getNextChar();
                             break;
                         }
                     }
            case 56: if (caractere == '/'){
                         estado = 1;
                         caractere = getNextChar();
                         break;
                     }else {
                         if (caractere == EOF){
                             erro(ERRO_COMENTARIO, getLinha());
                             return EOF;
                         }else {
                             estado = 55;
                             caractere = getNextChar();
                             break;
                         }
                     }
            case 59: if (caractere == '\''){
                         estado = 16;
                         inserirNoLexema(caractere);
                         caractere = getNextChar();
                         break;
                     }else {
                         estado = 1;
                         erro(ERRO_CONSTRUCAO_CARACTER, getLinha());
                         reiniciaLexema();
                         break;
                     }
            case 61: if (isdigit(caractere)){
                         estado = 10;
                         inserirNoLexema(caractere);
                         caractere = getNextChar();
                         break;
                     }else return PONTO;
            break;
        }
    }
}

/** Insere um caracter no vetor de lexema, se o tamanho do vetor exceder, incrementa-o */
void inserirNoLexema(char c) {
    tamanhoLexema++;
    if(tamanho_atual - posicaoLexema == 0) {
        tamanho_atual = tamanho_atual + LEXEMA_INCREMENTO;
        lexema = (char *)realloc(lexema, tamanho_atual * sizeof(char));
    }
    lexema[posicaoLexema] = caractere;
    posicaoLexema++;
    lexema[posicaoLexema] = '\0';
}

/** Reinicia o tamanho do vetor para tamanho original a cada retorno de um token */
void reiniciaLexema(){
    free(lexema);
    tamanhoLexema = 0;
    tamanho_atual = TAMANHO_ORIGINAL_LEXEMA;
    lexema = (char *)malloc(TAMANHO_ORIGINAL_LEXEMA * sizeof(char));
    posicaoLexema = 0;
}

/** Vetor com os nomes dos tokens e das palavras reservadas */
char *nomeToken[] = {(char *)"NUMINT",(char *)"NUMREAL",(char *)"ID",(char *)"LITERAL",(char *)"CARACTER",(char *)"ABRECOLCH",(char *)"FECHACOLCH",
(char *)"ABRECHAVE",(char *)"FECHACHAVE",(char *)"ABREPARENT",(char *)"FECHAPARENT",(char *)"VIRGULA",(char *)"SETA",(char *)"PONTO",(char *)"PONTOVIRGULA",
(char *)"DOISPONTOS",(char *)"ATRIBUICAO",(char *)"IGUAL",(char *)"MENOR",(char *)"MENORIGUAL",(char *)"MAIORIGUAL",(char *)"MAIOR",(char *)"DIFERENTE",
(char *)"ADICAO",(char *)"SUBTRACAO",(char *)"BIT_OU",(char *)"MULT",(char *)"DIV",(char *)"MOD",(char *)"BIT_E",(char *)"ELOGICO",(char *)"OULOGICO",
(char *)"NEGACAO",(char *)"if",(char *)"else",(char *)"while",(char *)"switch",(char *)"case",(char *)"break",(char *)"print",(char *)"readln",(char *)"return",
(char *)"throw",(char *)"try",(char *)"catch",(char *)"true",(char *)"false",(char *)"int",(char *)"float",(char *)"boolean",(char *)"char",(char *)"typedef",
(char *)"struct",(char *)"EOF"};

/** Retorna o nome do token de acordo com o seu valor */
/** Testa -1 pois o EOF é o seu valor */
char *getNomeToken(int indice){
    if(indice == -1) return nomeToken[FIMARQUIVO-100];
    else return nomeToken[indice-100];
}

/** Retorna um vetor de caracteres todo maiúsculo */
char *uperCase(char *nome){
    int tamanho = strlen(nome);
    char *copia = (char *)malloc((tamanho + 1) * sizeof(char));
    int i;
    for(i = 0; i <= tamanho; i++) copia[i] = toupper(nome[i]);
    return copia;
}
