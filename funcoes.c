#include "funcoes.h"
#include "arquivo.h"

void limpaTerminal() {
    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #endif
}

static void limpa_buffer() {
    int lixo;
    while ((lixo = getchar()) != '\n' && lixo != '\r' && lixo != EOF);
}

void input_s(const char *mensagem, char *x, size_t tamanho) {	
    do {
        printf("%s", mensagem);
        if (fgets(x, (int)tamanho, stdin) == NULL) {
            x[0] = '\0';
            break;
        }
        x[strcspn(x, "\n")] = '\0';
    } while (strlen(x) == 0);
    if (strlen(x) == tamanho - 1) limpa_buffer();    
}

void mostrarMenu(int *opc) {
    printf("+----------- Menu ----------+\n");
    printf("| 1- Inserir Lote           |\n");
    printf("| 2- Excluir Lote           |\n");
    printf("| 3- Somatorio mensal (csv) |\n");
    printf("| 4- Listagem arquivo (csv) |\n");
    printf("| 5- Encerrar               |\n");
    printf("+---------------------------+\n");
    printf("Selecione uma opcao [1-5]: ");
    scanf("%d", opc);
    getchar();
}

void insereLote(Dados *vet, int *limite) {
    char valor[VET_LIM] = {0}; // 15 caracteres
    int i = 0, j = 0;

    while(vet[i].dia != 0) {
        i++;
    }

    input_s("Digite o nome do arquivo (Ex: 20_06_2026.txt): ", valor, NOME_TAM);

    FILE *arquivo;
    arquivo = fopen(valor, "r+");

    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo digitado");
        return;
    }

    FILE *binario;
    binario = fopen("dados.bin", "ab");

    char delim[5] = "_.";
    char *nome = strtok(valor, delim), data[15];
    strcpy(data, nome);

    while(j < 2) {
        strcat(data, "/");
        nome = strtok(NULL, delim);
        strcat(data, nome);
        j++;
    }

    rewind(arquivo);
    
    while(i < *limite && fscanf(arquivo, "%d/%d/%d %lf", &vet[i].dia, &vet[i].mes, &vet[i].ano, &vet[i].litragem) == 4) {
        strcpy(vet[i].nome, data);
        fwrite(&vet[i], sizeof(Dados), 1, binario);
        i++;
    }

    fclose(arquivo);
    fclose(binario);
}

void excluirLote(Dados *vet, int limite) {
    char valor[15] = {0};
    int i = 0, j = 0;

    input_s("Digite a data dos arquivos a serem excluidos [Ex: 09/06/2026]: ", valor, NOME_TAM);
    
    rename("dados.bin", "backup.bin");

    FILE *arquivo;
    arquivo = fopen("dados.bin", "ab+");

    while(i < limite && strcmp(vet[i].nome, "\0")) {
        if(!(strcmp(vet[i].nome, valor))) {
            i++;
            continue;
        }
        fwrite(&vet[i], sizeof(Dados), 1, arquivo);
        i++;
    }

    rewind(arquivo);

    while(j < limite && fread(&vet[j], sizeof(Dados), 1, arquivo) != EOF) 
        j++;
    
    while(j < limite) {
        strcpy(vet[j].nome, "00/00/0000");
        vet[j].dia = 0;
        vet[j].mes = 0;
        vet[j].ano = 0;
        vet[j].litragem = 0;
        j++;
    }

    fclose(arquivo);
}

void somatoriaLoteCSV(Dados *vet) {
    int i = 0, k = 0, limite = VET_LIM;
    double litragem_mes[MES_MAX] = {0};

    FILE *planilha;
    planilha = fopen("somatoria_mes.csv", "w+");
    fprintf(planilha, "Mes;Litragem Total;\n");    

    while(i < MES_MAX) {
        int j = 0;
        while(j < limite && (vet[i].litragem != 0)) {
            if((vet[j].mes - 1) == i) {
                litragem_mes[i] += vet[j].litragem;
            }
            j++;
        }
        i++;
    }

    printf("+--------------------------------+\n");
    while(k < MES_MAX) {
        fprintf(planilha, "%d;%.2lf;\n", (k+1), litragem_mes[k]);
        printf("| Mes: %d | ", (k+1));
        printf("Litragem: %.2lf |\n", litragem_mes[k]);
        k++;
    }
    printf("+--------------------------------+\n");
    fclose(planilha);
}

void listarLoteCSV(Dados *vet) {
    int i = 0, limite = VET_LIM;

    FILE *planilha;
    planilha = fopen("listagem_lote.csv", "w+");
    fprintf(planilha, "Arquivo;Litragem;\n");

    printf("+---------------------------------+\n");
    while(i < limite && vet[i].dia != 0) {
        fprintf(planilha, "%s;%.2lf;\n", vet[i].nome, vet[i].litragem);        
        printf("| Arquivo: %s | ", vet[i].nome);
        printf("Litragem: %.2lf |\n", vet[i].litragem);
        i++;
    }
    printf("+---------------------------------+\n");
    fclose(planilha);
}

void opcaoMenu(Dados *vet, int opc, int limite, int *fim) {
    switch(opc) {
        case 1:
            insereLote(vet, &limite);
            break;
        case 2:
            excluirLote(vet, limite);
            break;
        case 3:
            somatoriaLoteCSV(vet);
            break;
        case 4:
            listarLoteCSV(vet);
            break;
        case 5:
            (*fim)++;
            break;
        default:
            printf("Valor digitado e incorreto! Tente novamente...\n");
            break;
    }
    getchar();
}