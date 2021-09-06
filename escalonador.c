#include "filaCircular.h"
#include "process.h"

#define DISCO 83432
#define FITA 83433
#define IMPRESSORA 83434

typedef struct escalonador {
    filaCircular filaAltaPrioridade;
    filaCircular filaBaixaPrioridade;
    int pidProcessando;
    int tempoProcessador;
    int tempoQuantum;

    filaCircular filaDisco;
    int tempoDisco;
    int tempoTotalDisco;    
    filaCircular filaFita;
    int tempoFita;
    int tempoTotalFita;
    filaCircular filaImpressora;
    int tempoImpressora;
    int tempoTotalImpressora;
} Escalonador;

void inicializaEscalonador (Escalonador* escalonador, int num_max_processos, int tempoIODisco, int tempoIOFita, int tempoIOImpressora, int tempoQuantum) {
    inicializaFilaCircular(&(escalonador->filaDisco), num_max_processos);
    inicializaFilaCircular(&(escalonador->filaFita), num_max_processos);
    inicializaFilaCircular(&(escalonador->filaImpressora), num_max_processos);
    inicializaFilaCircular(&(escalonador->filaAltaPrioridade), num_max_processos);
    inicializaFilaCircular(&(escalonador->filaBaixaPrioridade), num_max_processos);
    escalonador->tempoTotalDisco = tempoIODisco;
    escalonador->tempoTotalFita = tempoIOFita;
    escalonador->tempoTotalImpressora = tempoIOImpressora;
    escalonador->tempoQuantum = tempoQuantum;
    escalonador->tempoDisco = 0;
    escalonador->tempoFita = 0;
    escalonador->tempoImpressora = 0;
    escalonador->tempoProcessador = 0;
    escalonador->pidProcessando = 0;

}

void retornoDoIO (Escalonador* escalonador, int IO) {
    if (IO == DISCO) {
        entraNaFila(&(escalonador->filaBaixaPrioridade), saiDaFila(&(escalonador->filaDisco)));
    }
    if (IO == FITA) {
        entraNaFila(&(escalonador->filaAltaPrioridade), saiDaFila(&(escalonador->filaFita)));
    }
    if (IO == IMPRESSORA) {
        entraNaFila(&(escalonador->filaAltaPrioridade), saiDaFila(&(escalonador->filaImpressora)));
    }
}

void processaIO (Escalonador* escalonador) {
    if ((escalonador->filaDisco.emUso == 0) && !(estaVazio(escalonador->filaDisco))) {
        escalonador->filaDisco.emUso = 1;
    }

    if (escalonador->filaDisco.emUso == 1) {
        escalonador->tempoDisco += 1;
        if (escalonador->tempoDisco > escalonador->tempoTotalDisco) {
            retornoDoIO(escalonador, DISCO);
            escalonador->tempoDisco = 0;
            if (estaVazio(escalonador->filaDisco)) {
                escalonador->filaDisco.emUso = 0;
            }
        }
    }

    if ((escalonador->filaFita.emUso == 0) && !(estaVazio(escalonador->filaFita))) {
        escalonador->filaFita.emUso = 1;
    }

    if (escalonador->filaFita.emUso == 1) {
        escalonador->tempoFita += 1;
        if (escalonador->tempoFita > escalonador->tempoTotalFita) {
            retornoDoIO(escalonador, FITA);
            escalonador->tempoFita = 0;
            if (estaVazio(escalonador->filaFita)) {
                escalonador->filaFita.emUso = 0;
            }
        }
    }

    if ((escalonador->filaImpressora.emUso == 0) && !(estaVazio(escalonador->filaImpressora))) {
        escalonador->filaImpressora.emUso = 1;
    }

    if (escalonador->filaImpressora.emUso == 1) {
        escalonador->tempoImpressora += 1;
        if (escalonador->tempoImpressora > escalonador->tempoTotalImpressora) {
            retornoDoIO(escalonador, IMPRESSORA);
            escalonador->tempoImpressora = 0;
            if (estaVazio(escalonador->filaImpressora)) {
                escalonador->filaImpressora.emUso = 0;
            }
        }
    }

}

void printaTodasFilas (Escalonador escalonador) {

    if (!estaVazio(escalonador.filaAltaPrioridade)){
        printw ("\nFila de Alta prioridade: ");
        imprimeFila (escalonador.filaAltaPrioridade);
    }
    else {
        printw ("\nFila de Alta prioridade: \n");
    }
    if (!estaVazio(escalonador.filaBaixaPrioridade)){
        printw ("Fila de Baixa prioridade: ");
        imprimeFila (escalonador.filaBaixaPrioridade);
    }
    else {
        printw ("Fila de Baixa prioridade: \n");
    }
    if (!estaVazio(escalonador.filaDisco)){
        printw ("Fila do Disco: ");
        imprimeFila (escalonador.filaDisco);
    }
    else {
        printw ("Fila do Disco: \n");
    }
    if (!estaVazio(escalonador.filaFita)){
        printw ("Fila da Fita: ");
        imprimeFila (escalonador.filaFita);
    }
    else {
        printw ("Fila da Fita: \n");
    }
    if (!estaVazio(escalonador.filaImpressora)){
        printw ("Fila da Impressora: ");
        imprimeFila (escalonador.filaImpressora);
    }
    else {
        printw ("Fila da Impressora: \n");
    }
}

void processa(Escalonador *escalonador, Processo* processos, int tempo) {

    int indice;

    if (escalonador->pidProcessando == 0) {
        escalonador->pidProcessando = saiDaFila(&(escalonador->filaAltaPrioridade));
        escalonador->tempoProcessador += 1;

        indice = getProcessoByPID(processos, escalonador->pidProcessando);

        processos[indice].status = 1;
        processos[indice].programCounter += 1;
    } else {
        escalonador->tempoProcessador = escalonador->tempoProcessador + 1;

        indice = getProcessoByPID(processos, escalonador->pidProcessando);
        
        if (processos[indice].status == 1) {
            processos[indice].programCounter += 1;
        }

        int i;
        for (i = 0; i < processos[indice].numeroDisco; i++) {
            if ((processos[indice].programCounter == processos[indice].requisicoesDisco[i]) && (processos[indice].status == 1)) {
                processos[indice].status = 0;
                processos[indice].prioridade = 2;
                escalonador->tempoProcessador = escalonador->tempoQuantum;
                entraNaFila(&(escalonador->filaDisco), processos[indice].pid);
            }
        }
        for (i = 0; i < processos[indice].numeroFita; i++) {
            if ((processos[indice].programCounter == processos[indice].requisicoesFita[i]) && (processos[indice].status == 1)) {
                processos[indice].status = 0;
                processos[indice].prioridade = 1;
                escalonador->tempoProcessador = escalonador->tempoQuantum;
                entraNaFila(&(escalonador->filaFita), processos[indice].pid);
            }
        }
        for (i = 0; i < processos[indice].numeroImpressora; i++) {
            if ((processos[indice].programCounter == processos[indice].requisicoesImpressora[i]) && (processos[indice].status == 1)) {
                processos[indice].status = 0;
                processos[indice].prioridade = 1;
                escalonador->tempoProcessador = escalonador->tempoQuantum;
                entraNaFila(&(escalonador->filaImpressora), processos[indice].pid);
            }
        }
        
        if (processos[indice].programCounter == processos[indice].servico) {
            processos[indice].fim = tempo + 1;
            processos[indice].status = 0;
        }

        if (escalonador->tempoProcessador >= escalonador->tempoQuantum) {
            escalonador->tempoProcessador = 0;
            if (processos[indice].status == 1) {
                processos[indice].prioridade = 2;
                entraNaFila(&(escalonador->filaBaixaPrioridade), processos[indice].pid);
            }
            processos[indice].status = 0;
            if (!estaVazio(escalonador->filaAltaPrioridade)) {
                indice = getProcessoByPID(processos, saiDaFila(&(escalonador->filaAltaPrioridade)));
            } else {
                indice = getProcessoByPID(processos, saiDaFila(&(escalonador->filaBaixaPrioridade)));
            }
            escalonador->pidProcessando = processos[indice].pid;
            processos[indice].status = 1;
        }
    }
}

void processoNovo(Escalonador *escalonador, int PID) {
    entraNaFila(&(escalonador->filaAltaPrioridade), PID);
}
