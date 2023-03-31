#include "registro.h"
#include "../prints_e_erros/prints_e_erros.h"

struct cabecalho{
    char status;                    //indica consistencia do arquivo (0 - inconsistente, 1 - consistente) 
    long int proxByteOffset;        //indica o valor do proximo ByteOffset 
    int nroRegArq;                  // armazena o numero de registros no arquivo (total, inclusive removidos)                
    int nroRegRem;                  // armazena o numero de registros removidos
};

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

//================================================================================
CABECALHO *criar_cabecalho(void){
    CABECALHO *cabecalho = (CABECALHO*) malloc(sizeof(CABECALHO));

    if(cabecalho != NULL){
        cabecalho->proxByteOffset = 0;
        cabecalho->nroRegArq = 0;
        cabecalho->nroRegRem = 0;
    }
    else{
        imprime_erro_alocacao();
    }
    return (cabecalho);
}

void desaloca_cabecalho(CABECALHO *cabecalho){
    if(cabecalho != NULL){
        free(cabecalho);
    }
    else{
        imprime_erro_padrao();
    }
}


/* void imprime_cabecalho(CABECALHO *cabecalho){
    printf("%ld %d %d", cabecalho->proxByteOffset, cabecalho->nroRegArq, cabecalho->nroRegRem);
} */
//================================================================================


