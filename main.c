#include "funcoes.h"
#include "arquivo.h"

int main(int argc, char *argv[]) {
    Dados *vetor = malloc(sizeof(Dados) * VET_LIM);
    int fimLoop = 0, opcao = 0, limite = VET_LIM;

    validarArquivo(argv[1], &opcao, &limite);
    
    if(limite > VET_LIM) {
        aumentaLimite(&vetor, &limite);
    }

    if(argv[1] != NULL) {
        puxarDados(vetor, argv[1], limite);
    }

    if(argv[1] == NULL) {
        puxarDadosBin(vetor, limite);
    }

    while(fimLoop == 0) {
        limpaTerminal();
        mostrarMenu(&opcao);
        opcaoMenu(vetor, opcao, limite, &fimLoop);
    }
    return 0;
}