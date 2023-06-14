#ifndef B_ESTRELA_H
    #define B_ESTRELA_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "../manipulaRegistros/registro.h"
    #include "../parBusca/parBusca.h"
    #include "cabecalhoBEstrela.h"

    #define MAXCHAVES 4
    #define TAM_PAGE 76
    #define NIL -1

    typedef enum {PROMOTION, NO_PROMOTION, ERROR} ValoresRetorno;
    typedef enum { ACHOU, NAO_ACHOU } buscaRetorno;
    
    struct chave {
        int C;
        long int Pr;
    };
    typedef struct chave CHAVE;

    struct btPage
    {
        int nivel;
        int n; //Numero de chaves presentes no no
        int P[MAXCHAVES+1]; //RRN dos nos filhos
        CHAVE chaves[MAXCHAVES];
    };
    typedef struct btPage BTPAGE;
    

    bool ArvoreCriar(char nomeArqBin[], char arqIndiceArvore[]);

    DADOS** BuscaIndiceArvore(char *nomeArqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, int *qtdEncontrados);

    int InsereChave(BTPAGE *pagina, int tamanho, CHAVE chave, int RRN_Direita);

    bool ArvoreInserir(FILE *arvore, DADOS *registro, CABECALHO_B *cabecalho, long int byteoffset);

    ValoresRetorno Insert(FILE *arqArvore, int CURRENT_RRN, CHAVE KEY, CHAVE *PROMO_KEY, int *PROMO_R_CHILD);

    int BuscaBinaria(BTPAGE* pagina, int posicaoInicial, int posicaoFinal, int chave, bool *encontrou);
#endif