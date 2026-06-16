#ifndef ARQUIVO_H
#define ARQUIVO_H

// Inclusão da Outra Biblioteca
#include "funcoes.h"

// Funções de Arquivos
void validarArquivo(char *string, int *opc, int *limite);
void puxarDados(Dados *vet, char *string, int limite);
void aumentaLimite(Dados **vet, int *limite);
void puxarDadosBin(Dados *vet, int limite);

#endif