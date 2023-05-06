#include "indiceString.h"
#include "../../utils/utils.h"
#include "../../arquivos/arquivos.h"
#include "../../prints_e_erros/prints_e_erros.h"

struct dadosIndiceString{
    char chaveBusca[12];                    
    long int byteOffset;            //número relativo do registro do arquivo de dados referente à chave de busca
};

//aloca um registro de indice do tipo String
DADOS_STR *RegistroDadosStringCriar(void){
    DADOS_STR *registro = (DADOS_STR*) calloc(1, sizeof(DADOS_STR));

    //inicializa registro
    if(registro != NULL){
        for (int i = 0; i < 12; i++)
        {
            registro->chaveBusca[i] = '$';
        }
        
        registro->byteOffset = 0;
    }
    else{
        ErroAlocacao();
    }
    return registro;
}
//aloca um vetor de DADOS_STR
DADOS_STR *VetorIndicesStringCriar(int tamanho){
    DADOS_STR *vetor = (DADOS_STR*) calloc(tamanho, sizeof(DADOS_STR));
    if(vetor != NULL){
        return vetor;
    }
    else{
        ErroAlocacao();
        return NULL;
    }
}

//Escreve os indices no binario e atualiza o numero de registros para os casos de chaves nulas
void EscreveArqIndiceString(FILE* arqIndice, DADOS_STR indice, int* nroRegistro){
    if(indice.chaveBusca[0] != '$'){
        fwrite(&(indice.chaveBusca), sizeof(char), 12, arqIndice);
        fwrite(&(indice.byteOffset), sizeof(long int), 1, arqIndice);
    }else{
        (*nroRegistro)--;
    }
}


bool CriaIndiceString(char arqEntrada[], char arqSaida[], char campo[]){
    FILE *arqBin;
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalho = CabecalhoIndiceCriar();

    //abre os arquivos recebidos na entrada
    if(!AbreArquivo(&arqBin, arqEntrada, "rb", NULL)) return false;
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", arqBin, NULL)) return false;

    //Escreve cabecalho
    EscreveCabecalhoIndice(arqIndice, cabecalho);

    //fecha para atualizar em disco
    fclose(arqIndice);

    //abre novamente para uso
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", NULL)) return false;

    //ajusta o ponteiro para depois do cabecalho
    fseek(arqIndice, 5, SEEK_SET);

    //Aloca cabecalho e registro auxiliar para percorrer arquivo binario
    CABECALHO *cabecalho_registro = CabecalhoCriar();
    DADOS *registro_auxiliar = RegistroCriar();

    LeCabecalhoDoArqBinario(cabecalho_registro, arqBin);

    //Calcula o numero de registros não removidos e cria um vetor para os indices
    int nroRegistros = GetNroRegArq(cabecalho_registro) - GetNroRegRem(cabecalho_registro);
    DADOS_STR *indices = VetorIndicesStringCriar(nroRegistros);


    //Decide qual o tipo da chave de busca 
    int tipoCampo = TipoChaveBusca(campo);
    

    //Percorre arquivo binario lendo os registros
    int offsetlido = 17; //será o offset do próximo registro durante o loop
    int offsetanterior = 17;

    int flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido);
    int i;
    for(i=0; flag!=0; i++){
        if(GetRegistroRemovido(registro_auxiliar) == '0'){
            InsereCampoStringEmIndices(indices, registro_auxiliar, i, tipoCampo, offsetanterior);
        }

        DesalocaCamposVariaveis(registro_auxiliar);

        offsetanterior = offsetlido;

        flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido); 
    }
    //se nao existem registros no arquivo
    if(i==0) ErroArquivo();


    //ordenar
    mergeSortIndiceString(indices, 0, nroRegistros-1);

    //É preciso uma cópia para não interferir no for
    int copiaNroRegistro = nroRegistros;
    for (int i = 0; i < nroRegistros; i++)
    {
        EscreveArqIndiceString(arqIndice, indices[i], &copiaNroRegistro);
    }
    

    //ATUALIZA CABECALHO INDICE
    AtualizaStatusIndice(cabecalho, '1');
    AtualizaNroRegArqIndice(cabecalho, copiaNroRegistro);
    //Coloca o cursor no começo do arq para sobrescrever o cabcecalho
    fseek(arqIndice, 0, SEEK_SET); 
    EscreveCabecalhoIndice(arqIndice, cabecalho);
    DesalocaCabecalhoIndice(cabecalho);


    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_registro);
    DesalocaRegistro(registro_auxiliar);

    free(indices);

    fclose(arqBin);
    fclose(arqIndice);
    

    binarioNaTela(arqSaida);

    return true;

}

bool InsereCampoStringEmIndices(DADOS_STR *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset){
    DADOS_STR *registroDados = RegistroDadosStringCriar();
    
    switch(campo){
        //caso dataCrime
        case 2:{
            strncpySem0(registroDados->chaveBusca, GetRegistroDataCrime(registro_auxiliar), 10);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;   
        }
            
        //caso marcaCelular
        case 3:{
            strncpySem0(registroDados->chaveBusca, GetRegistroMarcaCelular(registro_auxiliar), 12);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }    

        //caso lugarCrime
        case 4:{
            strncpySem0(registroDados->chaveBusca, GetRegistroLugarCrime(registro_auxiliar), 12);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }  

        //caso descricaoCrime
        case 5:{
            strncpySem0(registroDados->chaveBusca, GetRegistroDescricaoCrime(registro_auxiliar), 12);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }   
    }

    free(registroDados);
}


void intercalaString(DADOS_STR* vetor, int inicio, int centro, int fim){
	DADOS_STR* vetorAux = VetorIndicesStringCriar((fim-inicio)+1);

	int i = inicio;	// indice da primeira lista ordenada
	int j = centro+1; //indice da segunda lista ordenada
	int k = 0; //indice do vetor auxiliar

	//compara e intercala os elementos do menor para o maior
	while(i <= centro && j <= fim){
		if (strncmp(vetor[i].chaveBusca, vetor[j].chaveBusca, 12) < 0){
            //verifica qual � o elemento menor entre as duas listas
			vetorAux[k] = vetor[i];
			i++; //proximo elemento da primeira metade
		}else if(strncmp(vetor[i].chaveBusca, vetor[j].chaveBusca, 12) == 0){
            if (vetor[i].byteOffset <= vetor[j].byteOffset){
                
                vetorAux[k] = vetor[i];
                i++; //proximo elemento da primeira metade
            }
            else{
                vetorAux[k] = vetor[j];
                j++; //proximo elemento da segunda metade
            }
        }
		else{
			vetorAux[k] = vetor[j];
			j++; //proximo elemento da segunda metade
		}
		k++;
	}

	while(i <= centro){//h� elementos na primeira metade ainda?
		vetorAux[k] = vetor[i];
		i++;
		k++;
	}

	while(j <= fim){//h� elementos na segunda metade ainda?
		vetorAux[k] = vetor[j];
		j++;
		k++;
	}

	//atualizando o vetor original com o vetor auxiliar(ordenado)
	for(i = inicio, k = 0; i <= fim; i++,k++)
		vetor[i] = vetorAux[k];
	free(vetorAux);
}

//Método de ordenação MergeSort
void mergeSortIndiceString(DADOS_STR* vetor, int inicio, int fim){
	//caso base
	if (fim <= inicio) return;

	//processo de divis�o
	int centro = (int)((inicio+fim)/2.0);
	mergeSortIndiceString(vetor, inicio, centro);
	mergeSortIndiceString(vetor, centro+1, fim);

	//processo de conquista
	intercalaString(vetor, inicio, centro, fim);
}
