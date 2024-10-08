#ifndef REGISTRO_H
    #define REGISTRO_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../cabecalho/cabecalho.h"
    #include "../cabecalho/cabecalhoIndice.h"
    #include "../arquivos/arquivos.h"

    struct dados{
        char removido;                  //indica se a struct foi removida (1 = removido, 0 = nao)
        int idCrime;                    
        char dataCrime[10];
        int numeroArtigo;
        char marcaCelular[12];
        char *lugarCrime;
        char *descricaoCrime;
        char delimitador;               //delimitador do fim da struct
    };

    typedef struct dados DADOS;

    typedef enum {INDICE, ARVORE} TipoIndice; 

    DADOS *RegistroCriar(void);
    DADOS **VetorRegistrosCriar(int tamanho);
    int LerRegBinario(FILE *arqBin, DADOS *registro, long int *offsetlido);
    void DesalocaRegistro(DADOS *registro);
    void DesalocaVetorRegistro(DADOS** vetor, int tamanho);
    void DesalocaCamposVariaveis(DADOS *registro);
    int TamanhoRegistro(DADOS *registro);
    
    //Funções para a funcionalidade 1
    DADOS *LerRegistroCsv(FILE *arquivoCSV, int *flagFuncionou);
    void EscreverRegistroBin(FILE *arquivoBIN, DADOS *registro, CABECALHO *cabecalho);
    void EscreverRegistroBinSemDelimitador(FILE *arquivoBIN, DADOS *registro, CABECALHO *cabecalho);
   

    //Função para a funcionalidade 2
    bool ImprimirBinario(FILE *arqBin);
    void ImprimeRegistroBinario(DADOS *registro);

    //Funcionalidade 4
    DADOS *LeRegistroPorByteOffset(FILE *arqBin, long int byteOffset);

    //Funcionalidade 6
    void RecebeRegistro(DADOS *registro);
    bool InsereRegistro(DADOS *registro, char *arqEntrada, char *nomeArqIndice, char *campo, char *dado, TipoIndice tipoArquivo);

    //Func5
    void avancaCursorSemRemovido(FILE *arqBin, DADOS *registro);


    bool TestaNulo(DADOS *registro, int tipoCampo);

    //Func7
    bool InsereRegistroNoArqInd(DADOS *registro, char *nomeArqIndice, char *dado, int tipoCampo, int byteoffsetanterior);

    //Funções Get do registro de Dados
    char GetRegistroRemovido(DADOS *registro);
    int GetRegistroIdCrime(DADOS *registro);
    int GetRegistroNroArtigo(DADOS *registro);
    char *GetRegistroDataCrime(DADOS *registro);
    char *GetRegistroMarcaCelular(DADOS *registro);
    char *GetRegistroLugarCrime(DADOS *registro);
    char *GetRegistroDescricaoCrime(DADOS *registro);
#endif