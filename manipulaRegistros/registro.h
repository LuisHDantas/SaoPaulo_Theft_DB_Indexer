#ifndef REGISTRO_H
    #define REGISTRO_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>


    typedef struct cabecalho CABECALHO;
    typedef struct dados DADOS;

    //Funções para o cabecalho
    CABECALHO *criar_cabecalho(void);
    void desaloca_cabecalho(CABECALHO *cabecalho);
    
    //Funções para a funcionalidade 1
    DADOS *LerRegCsv(FILE *arquivoCSV);
    void EscreverRegBin(FILE *arquivoBIN, DADOS *registro);


#endif