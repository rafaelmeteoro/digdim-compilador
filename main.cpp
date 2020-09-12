#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "erro.h"
#include "lexico.h"
#include "simbolos.h"
#include "parser.h"
#include "visitorSemantico.h"
#include "visitorRepIntermediaria.h"
#include "representacaoIntermediaria.h"
#include "RIVisitor.h"
#include "canonizador.h"
#include "visitorContadorReg.h"

int main(int argc, char *argv[]){

    char *nomeArquivo, *nomeArquivoSaida;
    FILE *arqEntrada = NULL;
    FILE *arqSaida = NULL;

    //entrada stdin
    if (argc == 1) {
        lexicoConstrutor(stdin);
    } else {
        //entrada de um arquivo
        if (argc > 1) {
            int size = strlen(argv[1]);
            nomeArquivo = (char*) malloc(sizeof(char) * (size + 5));

            if (nomeArquivo == NULL) {
                return 0;
            }

            strcpy(nomeArquivo, argv[1]);

            if (size > 4) {
                //testa a extensão .cmm do arquivo
                if (!((argv[1][size - 4] == '.') && (argv[1][size - 3] == 'c') && (argv[1][size - 2] == 'm') && (argv[1][size - 1] == 'm') ))
                    strcat(nomeArquivo, ".cmm");
            } else strcat(nomeArquivo, ".cmm");

            arqEntrada = fopen(nomeArquivo, "r");
            if (!arqEntrada) {
                free(nomeArquivo);
                return 1;
            }
        }

        //saída em arquivo
        if (argc == 3) {
            int size = strlen(argv[2]);
            nomeArquivoSaida = (char*) malloc(sizeof(char) * (size + 5));

            if (nomeArquivoSaida == NULL) {
                //imprimeErro(MEMORIA_INSUFICIENTE_PARAMETRO, 0);
                return 0;
            }

            strcpy(nomeArquivoSaida, argv[2]);

            if (size > 4) {
                //testa a extensão .asm do arquivo
                if (!((argv[2][size - 4] == '.') && (argv[2][size - 3] == 'a') && (argv[2][size - 2] == 's') && (argv[2][size - 1] == 'm') ))
                    strcat(nomeArquivoSaida, ".asm");
            }else strcat(nomeArquivoSaida, ".asm");

            arqSaida = fopen(nomeArquivoSaida, "w");
            if (!arqSaida) {
                free(nomeArquivoSaida);
                return 1;
            }
        } else {
            int tamanho = strlen(nomeArquivo);
            nomeArquivoSaida = (char*) malloc(sizeof(char) * tamanho);
            strcpy(nomeArquivoSaida, nomeArquivo);
            nomeArquivoSaida[tamanho-4] = '\0';
            strcat(nomeArquivoSaida, ".asm");

            arqSaida = fopen(nomeArquivoSaida, "w");
        }

        lexicoConstrutor(arqEntrada);
        free(nomeArquivo);
        free(nomeArquivoSaida);
    }


   // lexicoConstrutor(arquivo);

//    int token = getNextToken();
//    while(token != EOF){
//        fprintf(stdout,"%s", uperCase(getNomeToken(token)));
//        fprintf(stdout,"%s", strupr(getNomeToken(token)));
//        if(token == NUMINT || token == NUMREAL || token == ID || token == LITERAL || token == CARACTER) fprintf(stdout, ".%s", getLexema());
//        fprintf(stdout,"\n");
//        token = getNextToken();
//    }
//    fprintf(stdout,"%s\n", getNomeToken(FIMARQUIVO));

    parserConstrutor();
    ProgramNodo *p = Program();

    fprintf(stdout,"\n");

    ImprimiArvore       *imprime = new ImprimiArvore();
    VisitorSemantico    *semantico = new VisitorSemantico();

    if(p){
        p->aceita(imprime);
        fprintf(stdout,"\n");
        p->aceita(semantico);

        if(!getTemErros()){
            VisitorRepInt *ri = new VisitorRepInt(semantico->tabelaTipos);
            p->aceitaRI(ri);

            Canonizador *c = new Canonizador();
            do{
                c->mudancas = 0;
                ri->primeiroFragmento->aceita(c);
            }while(c->mudancas > 0);

            VisitorRepIntImprime *riImp = new VisitorRepIntImprime();
            ri->primeiroFragmento->aceita(riImp);

            VisitorMIPS *mips = new VisitorMIPS();
            ri->primeiroFragmento->aceita(mips);

            InstrucaoMIPS *instr = mips->primeiraInstrucao;

            VisitorContadorReg *vreg = new VisitorContadorReg();
            instr->aceita(vreg);

            InstrucaoVisitor *vi = new InstrucaoVisitor();
            instr->aceita(vi);

            InstrucaoMIPS *i = instr;
            do{
                i->imprime(arqSaida);
                i = i->proximo;
            }while(i);

            delete vi;
            delete vreg;
            delete mips;
            delete c;
            delete riImp;
            delete ri;
        }

        delete p;
    }

    delete imprime;
    delete semantico;

    fprintf(stdout,"\n");

    imprimirTabelas();
    lexicoDestrutor();
    return 0;
}
