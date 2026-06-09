# Projeto de Extensão II - Saulo 
> Projeto para registo da coleta de óleo, via documentos .txt / .csv 

Esse projeto tem como propósito criar um sistema, onde o cliente pode subir os dados de arquivos Texto (.txt), e o sistema salva os dados, transformando eles em planilhas (.csv).

---

## Sumário
1. [Introdução](https://github.com/iagocbs/projeto-de-extensao-II/master/README.md#)
    > Explicação do Projeto + Struct Usada
2. [Arquivos Usados](https://github.com/iagocbs/projeto-de-extensao-II/master/README.md#)
    > Listagem de Cada Arquivo com o Uso Deles
3. [Funções Principais](https://github.com/iagocbs/projeto-de-extensao-II/master/README.md#)
    > Listagem das Funções de Cada Arquivo
4. [Arquivos Txt Aceitos](https://github.com/iagocbs/projeto-de-extensao-II/master/README.md#)
    > Modelos de Arquivo Txt Aceitos
5. [Arquivos Criados](https://github.com/iagocbs/projeto-de-extensao-II/master/README.md#)
    > Todos os Arquivos Criados pelo Programa
6. [Créditos](https://github.com/iagocbs/projeto-de-extensao-II/master/README.md#)
    > Participantes do Projeto

---

## **1. Introdução**

O projeto foi idealizado pelo Prof. Me. Saulo Savio Leite Santos, onde nessa atividade buscamos transformar uma problemática, a contabilização da litragem doada de óleo e salvamento de dados, em um sistema de fácil compreensão e uso do usuário.

Nesse projeto, temos como uso base da Struct '**Dados**', onde a mesma tem como o recebimento:
- Nome do Arquivo (Vetor de Caracteres / Strings);
- Dia / Mês / Ano (Números Inteiros);
- Litragem Doada (Números Reais com Dobro de Precisão).

Segue o exemplo abaixo:

```c
typedef struct {
    char nome[NOME_MAX];
    int dia, mes, ano;
    double litragem;
} Dados;
```
---

## **2. Arquivos Usados**

Ao todo foi utilizado 5 arquivos como base, neste projeto:
1. _main.c_ -> Arquivo Principal
2. _funcoes.h_ -> Cabeçalho das Funções Padrões (Prototipagem + Bibliotecas / Constantes)
3. _funcoes.c_ -> Funções Padrões Completas
4. _arquivo.h_ -> Cabeçalho das Funções de Arquivo (Prototipagem)
5. _arquivo.c_ -> Funções de Arquivo Completas

Para que consiga rodar o programa sem erros, recomendamos a compilação no terminal do seguinte comando:
`gcc main.c funcoes.c arquivo.c -o {nome_do_executavel}`

---

## **3. Funções Principais**

Neste tópico separei apenas as funções principais, com suas informações resumidas, para melhor entendimento:
- Mostrar Menu / Menu Opções;
- Inserir Novo Lote;
- Excluir Lote;
- Somatoria do Mês (CSV);
- Listagem de Doações (CSV);

Sendo elas:

1. **Mostrar Menu / Menu Opções**
```c
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

```

Nesta função, fazemos a exibição de cada opção no menu, e depois é passado para função de opções que segue em cada situação mapeada.

-

2. **Inserir Novo Lote**
```c
void insereLote(Dados *vet, int *limite) {
    // Declaração de variáveis
    char valor[VET_LIM] = {0}; 
    int i = 0, j = 0;

    // Verifica qual o próximo endereço que pode ser preenchido
    while(vet[i].dia != 0) {
        i++;
    }

    // Puxa o nome do Arquivo
    input_s("Digite o nome do arquivo (Ex: 20_06_2026.txt): ", valor, NOME_TAM);

    // Abre o Arquivo de Texto Aberto +  Binário
    FILE *arquivo;
    arquivo = fopen(valor, "r+");

    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo digitado");
        return;
    }

    FILE *binario;
    binario = fopen("dados.bin", "ab");

    // Define as variáveis para tratativa dos dados no arquivo texto
    char delim[5] = "_.";
    char *nome = strtok(valor, delim), data[15];
    strcpy(data, nome);

    while(j < 2) {
        strcat(data, "/");
        nome = strtok(NULL, delim);
        strcat(data, nome);
        j++;
    }

    // Retorna ao Byte 0 do Arquivo
    rewind(arquivo);

    // Segue em Looping para preenchimento de dados, até finalizar os dados + insere no Binário
    while(i < limite && fscanf(arquivo, "%d/%d/%d %lf", &vet[i].dia, &vet[i].mes, &vet[i].ano, &vet[i].litragem) == 4) {
        strcpy(vet[i].nome, data);
        fwrite(&vet[i], sizeof(Dados), 1, binario);
        i++;
    }

    // Finaliza os arquivos abertos
    fclose(arquivo);
    fclose(binario);
}
```

Nesta função sendo um pouco mais complicada, ela vai basicamente puxar o nome do arquivo que o cliente digitar, o sistema vai tentar abrir o arquivo para leitura.
Puxando o nome, e voltando ao início para puxar os dados de forma completa e adicionando no `dados.bin`, depois finalizando os arquivos abertos

-

3. **Excluir Lote**

```c
void excluirLote(Dados *vet, int limite) {
    // Define as variaveis da função
    char valor[NOME_TAM] = {0};
    int i = 0, j = 0;

    // Puxa os dados do nome, para excluir os dados
    input_s("Digite a data dos arquivos a serem excluidos [Ex: 09/06/2026]: ", valor, NOME_TAM);
    
    // Troca os dados atual, para o backup.bin
    rename("dados.bin", "backup.bin");

    // Abre novo arquivo
    FILE *arquivo;
    arquivo = fopen("dados.bin", "ab+");

    // Escreve no arquivo, todos os dados que forem diferentes do nome / data digitado
    while(i < limite && strcmp(vet[i].nome, "\0")) {
        if(!(strcmp(vet[i].nome, valor))) {
            i++;
            continue;
        }
        fwrite(&vet[i], sizeof(Dados), 1, arquivo);
        i++;
    }

    // Retorna ao Byte 0 do Arquivo
    rewind(arquivo);

    // Puxa os novos dados e limpa os demais não preenchidos
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

    // Finaliza o arquivo
    fclose(arquivo);
}
```

Essa função apesar de grande, apenas tem como base, puxar o nome digitado ao cliente, comparando com o nome dos dados Vetor excluindo eles indiretamente, e depois salvando no novo `dados.bin`!

-

4. **Somatoria do Mês (CSV)**

```c
void somatoriaLoteCSV(Dados *vet) {
    // Define as variaveis da função
    int i = 0, k = 0, limite = VET_LIM;
    double litragem_mes[MES_MAX] = {0};

    // Definição do Arquivo com o Cabeçalho da Planilha
    FILE *planilha;
    planilha = fopen("somatoria_mes.csv", "w+");
    fprintf(planilha, "Mes;Litragem Total;\n");    

    // Abre uma repetição, para validar se o mês é igual ao i para somatória
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

    // Preenchimento dos dados na planilha e exibição na tela!
    printf("+--------------------------------+\n");
    while(k < MES_MAX) {
        fprintf(planilha, "%d;%.2lf;\n", (k+1), litragem_mes[k]);
        printf("| Mes: %d | ", (k+1));
        printf("Litragem: %.2lf |\n", litragem_mes[k]);
        k++;
    }
    printf("+--------------------------------+\n");
    
    // Finalizando a planilha
    fclose(planilha);
}
```

Nesta função, fazemos a somatória da litragem adicionada dos meses iguais, e depois demonstramos na tela e criamos a planilha

-

5. **Listagem de Doações (CSV)**

```c
void listarLoteCSV(Dados *vet) {
    // Declaração das Variaveis
    int i = 0, limite = VET_LIM;

    // Definição da Planilha Sendo Aberta +  Cabeçalho
    FILE *planilha;
    planilha = fopen("listagem_lote.csv", "w+");
    fprintf(planilha, "Arquivo;Litragem;\n");

    // Exibição na tela + Salvando na Planilha
    printf("+---------------------------------+\n");
    while(i < limite && vet[i].dia != 0) {
        fprintf(planilha, "%s;%.2lf;\n", vet[i].nome, vet[i].litragem);        
        printf("| Arquivo: %s | ", vet[i].nome);
        printf("Litragem: %.2lf |\n", vet[i].litragem);
        i++;
    }
    printf("+---------------------------------+\n");

    // Finalizando o arquivo
    fclose(planilha);
}
```

Na última função, sendo a mais simples, mostramos todos os dados preenchidos com valores, a serem mostrados na planilha e na tela ao usuário.

---

## **4. Arquivos Txt Aceitos**

Nesse projeto, foi estipulado que o arquivo TXT tivesse o padrão:
- Nome: {data_separado_com_underline}.txt | Ex: `09_06_2026.txt`
- Dados: {data_separado_por_barra} {litragem_separado_por_ponto} | Ex: 09/06/2026 9.9

Ficando, por exemplo:
```txt
*Titulo*: 09_06_2026.txt

09/06/2026 9.9
09/06/2026 0.3
09/06/2026 1.2
09/06/2026 8.5
```

---

## **5. Arquivos Criados**

No final do uso do programa, podem ser gerados 4 arquivos, sendo eles:
- _dados.bin_ -> Arquivo Binário, criado para uso dos dados após a exclusão
- _backup.bin_ -> Arquivo Binário, criado para caso o novo `dados.bin` tenha erro na abertura
- *somatoria_mes.csv* -> Arquivo de Planilha, onde contabiliza o valor de óleo doado em determinado mês
- *listagem_lote.csv* -> Arquivo de Planilha, gerado com todos os dados preenchido no Vetor / Array do Programa

---

## **6. Créditos**

| Participante   | Registro de Aluno   |
| :------------: | :-----------------: |
| Gabrielli V.   | 04725-100           |
| Iago C.        | 04725-011           |
| Marcos P.      | 04725-067           |
| Nicolle S.     | 04725-051           |