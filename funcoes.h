#ifndef FUNCOES_H
#define FUNCOES_H

// Biblioteca e Constantes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VET_LIM 10
#define NOME_TAM 15
#define MES_MAX 12

// Struct de Dados
typedef struct {
    char nome[NOME_TAM]; 
    int dia, mes, ano;
    double litragem;
} Dados;

// Funções do Saulo
void limpaTerminal();
static void limpa_buffer();
void input_s(const char *mensagem, char *x, size_t tamanho);

// Funções de Arquivo Normal
void mostrarMenu(int *opc);
void insereLote(Dados *vet, int *limite);
void excluirLote(Dados *vet, int limite);
void somatoriaLoteCSV(Dados *vet);
void listarLoteCSV(Dados *vet);
void opcaoMenu(Dados *vet, int opc, int limite, int *fim);

#endif