#include <stdlib.h>
#include <stdbool.h>

typedef struct filaCircular {

    int frente;
    int atras;
    int tamanho;
    int emUso;
    int* pids;

} filaCircular;

void inicializaFilaCircular(filaCircular* fila, int max_processos) {
    int i;
    fila->frente = -1;
    fila->atras = -1;
    fila->tamanho = max_processos;
    fila->pids = (int*)malloc(fila->tamanho*sizeof(int));
    fila->emUso = 0;
}

bool estaCheio(filaCircular fila) {
    if ((fila.frente == fila.atras + 1) || (fila.frente == 0 && fila.atras == fila.tamanho - 1)) return true;
    return false;
}

bool estaVazio(filaCircular fila) {
    if (fila.frente == -1) return true;
    return false;
}

void entraNaFila(filaCircular* fila, int processoPID) {
    if (!estaCheio(*fila)) {
        if (fila->frente == -1) fila->frente = 0;
        fila->atras = (fila->atras + 1) % fila->tamanho;
        fila->pids[fila->atras] = processoPID;
    }
}

int saiDaFila(filaCircular* fila) {
    if (!estaVazio(*fila)) {
        int retorno = fila->pids[fila->frente];
        if (fila->frente == fila->atras) {
            fila->frente = -1;
            fila->atras = -1;
        }
        else {
            fila->frente = (fila->frente + 1) % fila->tamanho;
        }
        return retorno;
    }
}

void imprimeFila(filaCircular fila) {
    int i = 0;
    for (i = fila.frente; i != (fila.atras + 1); i = ((i + 1) % (fila.tamanho + 1))) {
        printw ("%d ", fila.pids[i]);
        refresh();
    }
    printw ("\n");
}
