#include "arquivo.h"

void validarArquivo(char *string, int *opc, int *limite) {
    int i = 0;
    Dados aux;
    
    if(string != NULL) {
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

    FILE *binario;
    binario = fopen("dados.bin","rb");

    if(binario == NULL) {
        fclose(binario);
        return;
    }

    while(fread(&aux, sizeof(Dados), 1, binario)) {
        i++;
    }
    
    *limite = i;
    fclose(binario);
}
void puxarDados(Dados *vet, char *string, int limite) {
    int i = 0, j = 0;
    FILE *arquivo;
    arquivo = fopen(string, "r+");
    
    char delim[5] = " / ";
    char valor[NOME_TAM] = {0};
    fgets(valor, NOME_TAM, arquivo);

    char *nome = strtok(valor, delim), data[NOME_TAM] = {0};
    strcpy(data, nome);

    while(j < 2) {
        strcat(data, "/");
        if(j == 0) 
            nome = strtok(NULL, delim);
        if(j == 1) 
            nome = strtok(NULL, " ");
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
void aumentaLimite(Dados **vet, int *limite) {
    int max = 0, lim = *limite;
    while(max < (lim + 9)) {
        max += 10;
    }
    Dados *aux = realloc(*vet, sizeof(Dados) * lim);
    if(aux == NULL) {
        printf("Nao possivel aumentar o limite. Tente reiniciar o programa\n");
        return;
    }
    *limite = max;
    *vet = aux;
}
void puxarDadosBin(Dados *vet, int limite) {
    int i = 0;
    FILE *arquivo;
    arquivo = fopen("dados.bin", "rb+");

    if(arquivo == NULL) {
        printf("Nao possivel abrir o arquivo. Tente reiniciar o programa\n");
        
        while(i < limite) {
            strcpy(vet[i].nome, "00/00/0000\0");
            vet[i].dia = 0;
            vet[i].mes = 0;
            vet[i].ano = 0;
            i++;
        }
        fclose(arquivo);
        getchar();
        return;
    }

    while(i < limite && fread(&vet[i], sizeof(Dados), 1, arquivo)) {
        i++;
    }

    while(i < limite) {
        strcpy(vet[i].nome, "00/00/0000\0");
        vet[i].dia = 0;
        vet[i].mes = 0;
        vet[i].ano = 0;
        i++;
    }

    fclose(arquivo);
}