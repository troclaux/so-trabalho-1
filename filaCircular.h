#include "filaCircular.c"

struct filaCircular;

void inicializaFilaCircular(filaCircular* fila, int max_processos);
bool estaCheio(filaCircular fila);
bool estaVazio(filaCircular fila);
void entraNaFila(filaCircular* fila, int processoPID);
int saiDaFila(filaCircular* fila);
