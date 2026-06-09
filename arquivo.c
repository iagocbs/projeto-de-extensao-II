#include "arquivo.h"

void validarArquivo(char *string, int *opc) {
    FILE *arquivo;
    arquivo = fopen(string, "a+");

    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo...");
        *opc = 5;
        fclose(arquivo);
        return;
    }

    printf("Arquivo aberto com sucesso!\n");
    fclose(arquivo);
}

void puxarDados(Dados *vet, char *string, int limite) {
    int i = 0, j = 0;
    FILE *arquivo;
    arquivo = fopen(string, "r+");
    
    char delim[5] = "/ ";
    char valor[NOME_TAM] = {0};
    fgets(valor, NOME_TAM, arquivo);

    char *nome = strtok(valor, delim), data[NOME_TAM] = {0};
    strcpy(data, nome);

    while(j < 2) {
        strcat(data, "/");
        nome = strtok(NULL, delim);
        strcat(data, nome);
        j++;
    }

    rewind(arquivo); // Volta o ponteiro pro inicio / pro byte 0

    while(i < limite && fscanf(arquivo, "%d/%d/%d %lf", &vet[i].dia, &vet[i].mes, &vet[i].ano, &vet[i].litragem) == 4) {
        strcpy(vet[i].nome, data);
        i++;
    }

    while(i < limite) {
        strcpy(vet[i].nome, "00/00/0000\0");
        i++;
    }

    fclose(arquivo);
}