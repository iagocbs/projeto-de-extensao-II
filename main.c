#include "funcoes.h"
#include "arquivo.h"

int main(int argc, char *argv[]) {
    Dados vetor[VET_LIM] = {0};
    int fimLoop = 0, opcao = 0, limite = VET_LIM;

    if(argv[1] != NULL) {
        validarArquivo(argv[1], &opcao);
        puxarDados(vetor, argv[1], limite);
    }

    while(fimLoop == 0) {
        limpaTerminal();
        mostrarMenu(&opcao);
        opcaoMenu(vetor, opcao, limite, &fimLoop);
    }
    return 0;
}