#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <ncurses.h>

//--- Premissa 1: limite máximo de processos criados ---//
const int NUM_MAX_PROCESSOS = 255;

#include "escalonador.h"

//--- Premissa 2: valor da fatia de tempo dada aos processos em execução ---//
int QUANTUM = 4;

//--- Premissa 4: tempos de duração de cada tipo de I/O (disco, fita magnética e impressora) ---//
int TEMPO_IO_DISCO = 3;
int TEMPO_IO_FITA = 5;
int TEMPO_IO_IMPRESSORA = 10;

//--- Contadores ---//
int contador_de_processos = 0;                              //Contador incremental para passar diferentes PIDs para os processos
int unidade_de_tempo = 0;                                   //Quantificação do tempo

//--- Processamento por unidade de tempo ---//
void processamento(Escalonador* escalonador, Processo* processos) {

    processaIO(escalonador);
    processa(escalonador, processos, unidade_de_tempo);
    
    unidade_de_tempo++;                                     //Incremento do contador de tempo
}

//--- Main ---//
int main () {
    int numeroProcessosUsuario; //Pede para o usuário o número de processos a serem criados
    char valoresPadrao, valoresPadraoTemp;

    initscr();
    start_color();
    refresh();

	Processo processos[NUM_MAX_PROCESSOS];

    printw("\nDigite o numero de processos: ");
    refresh();
    scanw("%d", &numeroProcessosUsuario);
    printw("\nTotal de processos = %d\n", numeroProcessosUsuario);
    refresh();

    printw("\nQUANTUM: %d", QUANTUM);
    printw("\nTempo de IO em Disco: %d", TEMPO_IO_DISCO);
    printw("\nTempo de IO em Fita: %d", TEMPO_IO_FITA);
    printw("\nTempo de IO em Impressora: %d", TEMPO_IO_IMPRESSORA);
    printw("\nDeseja alterar estes valores padrões (y/N)? ");
    scanw("%c", &valoresPadrao);
    valoresPadraoTemp = tolower(valoresPadrao);
    if ('y' == valoresPadraoTemp) {
        printw("\nDigite o valor para o QUANTUM: ");
        scanw("%c", &QUANTUM);
        printw("Digite o Tempo de IO em Disco: ");
        scanw("%c", &TEMPO_IO_DISCO);
        printw("Digite o Tempo de IO em Fita: ");
        scanw("%c", &TEMPO_IO_FITA);
        printw("Digite o Tempo de IO em Impressora: ");
        scanw("%c", &TEMPO_IO_IMPRESSORA);
    }

    Escalonador escalonador;
    inicializaEscalonador(&escalonador, NUM_MAX_PROCESSOS, TEMPO_IO_DISCO, TEMPO_IO_FITA, TEMPO_IO_IMPRESSORA, QUANTUM);

    int i;
    for (i = 0; i < numeroProcessosUsuario; i++) {
        inicializaProcesso(&processos[i], &contador_de_processos, NULL, unidade_de_tempo);
    }

    //Loop da execução dos processos
    while(1) {

        for (i = 0; i < numeroProcessosUsuario; i++){
            if (processos[i].inicio == unidade_de_tempo) {
                processoNovo(&escalonador, processos[i].pid);
            }
        }

        processamento(&escalonador, processos);
        
        clear();
        init_pair(50, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(50));
        printw("Tempo = %d u.t.\n", unidade_de_tempo);
        refresh();
        init_pair(5, COLOR_BLACK, COLOR_WHITE);
        attron(COLOR_PAIR(5));
        printw("QUANTUM: %i\n", QUANTUM);
        printw("Tempo de IO em Disco: %d\n", TEMPO_IO_DISCO);
        printw("Tempo de IO em Fita: %d\n", TEMPO_IO_FITA);
        printw("Tempo de IO em Impressora: %d\n", TEMPO_IO_IMPRESSORA);
        refresh();
        printaProcessos(processos, numeroProcessosUsuario);
        refresh();
        attron(COLOR_PAIR(2));
        printaTodasFilas(escalonador);

        refresh();
        getch();
    }

	return 0;
}
