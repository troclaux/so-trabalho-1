#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int MAX_REQUISICOES = 2;            //Máximo de requisições por tipo de I/O
int seed = 1337;

//--- Processo ---//
typedef struct process {

    int pid;                        //Identificador do processo
    int ppid;                       //Identificador do processo pai
    int inicio;                     //Momento em que o processo foi iniciado
    int fim;                        //Momento em que finaliza
    int prioridade;                 //Nível de prioridade do processo na fila de espera
    int status;                     //Identificador de status do processo
    int programCounter;             //Contador do programa
    int servico;                    //Tempo total de serviço para término do processo
    int numeroDisco;
    int numeroFita;
    int numeroImpressora;
    int *requisicoesDisco;          //Array com as requisições de I/O para o disco com tempo onde é feita a requisição
    int *requisicoesFita;           //Array com as requisições de I/O para a fita magnética com o tempo onde é feita a requisição
    int *requisicoesImpressora;     //Array com as requisições de I/O para a impressora com o tempo onde é feita a requisição

} Processo;


//--- Função de inicialização de um processo sem processo pai ---//
void inicializaProcesso (Processo* iniciado, int* contador_de_processos, Processo* pai, int tempo) {
    srand((unsigned)((time(NULL)*seed)));  				//Seed para gerador de números pseudo-aleatórios
    seed = rand()* *contador_de_processos;

    iniciado->pid = 100 + *contador_de_processos;			//Pega um ID que vem de um contador incremental da máquina
    *contador_de_processos += 1;
    iniciado->prioridade = 1;                   			//Guarda prioridade e começa como alta prioridade
    iniciado->status = 0;                       			//Inicializa como "not running state"
    iniciado->programCounter = 1;               			//Apontador de próxima instrução apontaria para primeira instrução
    iniciado->servico = (rand() % 30) + 3 * (MAX_REQUISICOES - 1);	//Tempo de serviço aleatório com tempo mínimo para suportar o máximo de requisições
    iniciado->inicio = rand() % 255;
    if (pai == NULL) {
        iniciado->ppid = 1;
    } else {
        iniciado->ppid = pai->pid; //Se não existe pai, processo pai é o init
    }
    iniciado->numeroDisco = rand() % (MAX_REQUISICOES + 1);        				//Numero de requisições I/O do processo
    iniciado->requisicoesDisco = (int*)malloc(sizeof(int) * iniciado->numeroDisco);             //Define tamanho do array
    iniciado->numeroFita = rand() % (MAX_REQUISICOES + 1);         				//Numero de requisições I/O do processo
    iniciado->requisicoesFita = (int*)malloc(sizeof(int) * iniciado->numeroFita);               //Define tamanho do array
    iniciado->numeroImpressora = rand() % (MAX_REQUISICOES + 1);   				//Numero de requisições I/O do processo
    iniciado->requisicoesImpressora = (int*)malloc(sizeof(int) * iniciado->numeroImpressora);   //Define tamanho do array

    int *checaRepetido = (int*)malloc(sizeof(int)*3*(MAX_REQUISICOES - 1) + 1);         	//Array com tempos de requisições que ja saíram
    int indice = 0;

    int i;
    int j;
    int random;
    int flag = 0;										//Flag para auxiliar a não adicionar múltiplas requisições I/O no mesmo quantum 

    //--- Atribuição aleatória de tempos da requisições de DISCO ---//
    for (i = 0; i < iniciado->numeroDisco; i++) {
        random = 2 + (rand() % (iniciado->servico - 2));
        for (j = 0; j < indice; j++) {
            if (random == checaRepetido[j]) {
                flag = 1;
                i--;
                break;
            }
        }
        if (flag == 0) {
            iniciado->requisicoesDisco[i] = random;
            checaRepetido[indice++] = random;
        }
        flag = 0;
    }

    //--- Atribuição aleatória de tempos da requisições de FITA ---//
    for (i = 0; i < iniciado->numeroFita; i++) {
        random = 2 + (rand() % (iniciado->servico - 2));
        for (j = 0; j < indice; j++) {
            if (random == checaRepetido[j]) {
                flag = 1;
                i--;
                break;
            }
        }
        if (flag == 0) {
            iniciado->requisicoesFita[i] = random;
            checaRepetido[indice++] = random;
        }
        flag = 0;
    }

    //--- Atribuição aleatória de tempos da requisições de IMPRESSORA ---//
    for (i = 0; i < iniciado->numeroImpressora; i++) {
        random = 2 +(rand() % (iniciado->servico - 2));
        for (j = 0; j < indice; j++) {
            if (random == checaRepetido[j]) {
                flag = 1;
                i--;
                break;
            }
        }
        if (flag == 0) {
            iniciado->requisicoesImpressora[i] = random;
            checaRepetido[indice++] = random;
        }
        flag = 0;
    }
}


void printaProcessos(Processo processo[], int numeroTotal) {
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw ("Numero de processos: %d\n", numeroTotal);
    attron(COLOR_PAIR(2));
    printw ("PID\tPPID\tPrioridade\tInicio\tFim\tStatus\tPC\tServico\t\tDisco\t\tFita\t\tImpressora\n");
    attron(COLOR_PAIR(3));
    int i;
    for (i = 0; i < numeroTotal; i++) {
        printw ("%d\t", processo[i].pid);
        printw ("%d\t", processo[i].ppid);
        printw ("%d\t\t", processo[i].prioridade);
        printw ("%d\t", processo[i].inicio);
        printw ("%d\t", processo[i].fim);
        printw ("%d\t", processo[i].status);
        printw ("%d\t", processo[i].programCounter);
        printw ("%d\t\t", processo[i].servico);
        int j;
        for (j = 0; j < processo[i].numeroDisco; j++) {
            printw("%d", processo[i].requisicoesDisco[j]);
            if (j != (processo[i].numeroDisco - 1)) {
                printw(",");
            }
        }
        printw("\t\t");
        for (j = 0; j < processo[i].numeroFita; j++) {
            printw("%d", processo[i].requisicoesFita[j]);
            if (j != (processo[i].numeroFita - 1)) {
                printw(",");
            }
        }
        printw("\t\t");
        for (j = 0; j < processo[i].numeroImpressora; j++) {
            printw("%d", processo[i].requisicoesImpressora[j]);
            if (j != (processo[i].numeroImpressora - 1)) {
                printw(",");
            }
        }
        printw ("\n");
    }
}


int getProcessoByPID (Processo* processos, int PID) {
    int i;
    for (i = 0; i< NUM_MAX_PROCESSOS; i++) {
        if (processos[i].pid == PID) {
            return i;
        }
    }
}
