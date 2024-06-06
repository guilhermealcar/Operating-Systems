#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int tempo;
    int chegada;
    int tempoFinalizacao;
    int tempoEspera;
    int prioridade;
} tipoProcesso;


void lerArquivo(tipoProcesso processo[20], int *maxProcessos, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Não foi possível abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }

    *maxProcessos = 0;
    char linha[1024]; // Increase size if needed
    int values[20];
    
    // Read header
    fgets(linha, sizeof(linha), arquivo);
    
    // Read 'Tempo'
    fgets(linha, sizeof(linha), arquivo);
    sscanf(linha, "Tempo\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
           &processo[0].tempo, &processo[1].tempo, &processo[2].tempo, &processo[3].tempo, &processo[4].tempo,
           &processo[5].tempo, &processo[6].tempo, &processo[7].tempo, &processo[8].tempo, &processo[9].tempo);

    // Read 'Chegada'
    fgets(linha, sizeof(linha), arquivo);
    sscanf(linha, "Chegada\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
           &processo[0].chegada, &processo[1].chegada, &processo[2].chegada, &processo[3].chegada, &processo[4].chegada,
           &processo[5].chegada, &processo[6].chegada, &processo[7].chegada, &processo[8].chegada, &processo[9].chegada);

    // Read 'Prioridade'
    fgets(linha, sizeof(linha), arquivo);
    sscanf(linha, "Prioridade\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
           &processo[0].prioridade, &processo[1].prioridade, &processo[2].prioridade, &processo[3].prioridade, &processo[4].prioridade,
           &processo[5].prioridade, &processo[6].prioridade, &processo[7].prioridade, &processo[8].prioridade, &processo[9].prioridade);

    *maxProcessos = 10; // You specified 10 processes A to J
    fclose(arquivo);
}


void simpleBubbleSort(tipoProcesso processo[20], int maxProcessos) {
    for (int i = 0; i < maxProcessos - 1; i++) {
        for (int j = 0; j < maxProcessos - i - 1; j++) {
            if (processo[j].chegada > processo[j + 1].chegada) {
                tipoProcesso temp = processo[j];
                processo[j] = processo[j + 1];
                processo[j + 1] = temp;
            }
        }
    }
}

void printProcessos(int maxProcessos, tipoProcesso processo[20], const char* preMsg) {
    printf("%s\n", preMsg);
    for (int i = 0; i < maxProcessos; i++) {
        printf("Processo %c:\tChegada: %i\tTempo: %i\n", 'A' + i, processo[i].chegada, processo[i].tempo);
    }
}

void SJFPreemptivo(tipoProcesso processo[20], int maxProcessos, int tempoMax) {
    if (tempoMax == 0) {
        printf("No work to schedule.\n");
        return;
    }

    printf("\nSJF Preemptivo:\n");
    int *linhaDoTempo = (int *) malloc(tempoMax * sizeof(int));
    if (!linhaDoTempo) {
        printf("Failed to allocate memory for timeline.\n");
        return;
    }

    memset(linhaDoTempo, -1, tempoMax * sizeof(int)); // Initialize all to -1 for idle

    int tempoAtual = 0;
    int processosRestantes = maxProcessos;
    int processoMenorTempo = -1;

    while (processosRestantes > 0) {
        int menorTempo = 99999;
        processoMenorTempo = -1; // Reset on each iteration

        for (int i = 0; i < maxProcessos; i++) {
            if (processo[i].chegada <= tempoAtual && processo[i].tempo > 0) {
                if (processo[i].tempo < menorTempo) {
                    menorTempo = processo[i].tempo;
                    processoMenorTempo = i;
                }
            }
        }

        if (processoMenorTempo == -1) {
            if (tempoAtual < tempoMax) {
                linhaDoTempo[tempoAtual] = -1; // Idle
            }
            tempoAtual++;
            continue;
        }

        if (tempoAtual < tempoMax) {
            linhaDoTempo[tempoAtual] = processoMenorTempo;
            processo[processoMenorTempo].tempo--;
            if (processo[processoMenorTempo].tempo == 0) {
                processosRestantes--;
                processo[processoMenorTempo].tempoFinalizacao = tempoAtual + 1;
            }
        }

        tempoAtual++;
    }

    printf("\nLINHA DO TEMPO:\n");
    for (int i = 0; i < tempoMax; i++) {
        if (linhaDoTempo[i] == -1) {
            printf("[tempo:%d] idle\n", i);
        } else {
            printf("[tempo:%d] Processo %c\n", i, 'A' + linhaDoTempo[i]);
        }
    }

    // Calculate average times
    float tempoResposta_total = 0;
    float tempoEspera_total = 0;

    for (int i = 0; i < maxProcessos; i++) {
        if (processo[i].tempoFinalizacao > 0) { // Only include processed ones
            tempoResposta_total += processo[i].tempoFinalizacao;
            tempoEspera_total += processo[i].tempoFinalizacao - processo[i].chegada - processo[i].tempo;
        }
    }

    float tempoResposta_medio = tempoResposta_total / maxProcessos;
    float tempoEspera_medio = tempoEspera_total / maxProcessos;
    printf("\nTempo médio de resposta dos processos: %.2f t.u.\n", tempoResposta_medio);
    printf("\nTempo médio de espera dos processos: %.2f t.u.\n", tempoEspera_medio);

    free(linhaDoTempo);
}

void RoundRobin(tipoProcesso processo[20], int maxProcessos, int quantum) {
    printf("\nRound-Robin com quantum %d:\n", quantum);

    int tempoAtual = 0;
    int processosRestantes = maxProcessos;
    int index = 0;

    while (processosRestantes > 0) {
        if (processo[index].chegada <= tempoAtual && processo[index].tempo > 0) {
            int tempoExecucao = processo[index].tempo < quantum ? processo[index].tempo : quantum;
            processo[index].tempo -= tempoExecucao;
            tempoAtual += tempoExecucao;
            processo[index].tempoFinalizacao += tempoExecucao;

            if (processo[index].tempo == 0) {
                processosRestantes--;
            }
        } else {
            tempoAtual++;
        }
        index = (index + 1) % maxProcessos;
    }

    // Calcular tempos médios
    float tempoResposta_total = 0;
    float tempoEspera_total = 0;

    for (int i = 0; i < maxProcessos; i++) {
        tempoResposta_total += processo[i].tempoFinalizacao;
        tempoEspera_total += processo[i].tempoFinalizacao - processo[i].chegada - processo[i].tempo;
    }

    float tempoResposta_medio = tempoResposta_total / maxProcessos;
    float tempoEspera_medio = tempoEspera_total / maxProcessos;
    printf("\nTempo médio de resposta dos processos: %.2f t.u.\n", tempoResposta_medio);
    printf("\nTempo médio de espera dos processos: %.2f t.u.\n", tempoEspera_medio);
}

void PrioridadePreemptiva(tipoProcesso processo[20], int maxProcessos, int tempoMax) {
    if (tempoMax == 0) {
        printf("No work to schedule.\n");
        return;
    }

    printf("\nPrioridade Preemptiva:\n");
    int *linhaDoTempo = (int *) malloc(tempoMax * sizeof(int));
    if (!linhaDoTempo) {
        printf("Failed to allocate memory for timeline.\n");
        return;
    }

    memset(linhaDoTempo, -1, tempoMax * sizeof(int)); // Initialize all to -1 for idle

    int tempoAtual = 0;
    int processosRestantes = maxProcessos;

    while (processosRestantes > 0) {
        int maiorPrioridade = 99999;
        int processoPrioridade = -1; // Reset on each iteration

        for (int i = 0; i < maxProcessos; i++) {
            if (processo[i].chegada <= tempoAtual && processo[i].tempo > 0) {
                if (processo[i].prioridade < maiorPrioridade) {
                    maiorPrioridade = processo[i].prioridade;
                    processoPrioridade = i;
                }
            }
        }

        if (processoPrioridade == -1) {
            if (tempoAtual < tempoMax) {
                linhaDoTempo[tempoAtual] = -1; // Idle
            }
            tempoAtual++;
            continue;
        }

        if (tempoAtual < tempoMax) {
            linhaDoTempo[tempoAtual] = processoPrioridade;
            processo[processoPrioridade].tempo--;
            if (processo[processoPrioridade].tempo == 0) {
                processosRestantes--;
                processo[processoPrioridade].tempoFinalizacao = tempoAtual + 1;
            }
        }

        tempoAtual++;
    }

    printf("\nLINHA DO TEMPO:\n");
    for (int i = 0; i < tempoMax; i++) {
        if (linhaDoTempo[i] == -1) {
            printf("[tempo:%d] idle\n", i);
        } else {
            printf("[tempo:%d] Processo %c\n", i, 'A' + linhaDoTempo[i]);
        }
    }

    free(linhaDoTempo);
}

int main() {
    tipoProcesso processo[20];
    int maxProcessos;
    const char *nomeArquivo = "teste.csv";
    lerArquivo(processo, &maxProcessos, nomeArquivo);
    simpleBubbleSort(processo, maxProcessos);
    printProcessos(maxProcessos, processo, "Processos Carregados:");

    // Preencher tempo máximo considerando a duração de todos os processos
    int tempoMax = 0;
    for (int i = 0; i < maxProcessos; i++) {
        tempoMax += processo[i].tempo;
    }

    // Simula SJF Preemptivo
    SJFPreemptivo(processo, maxProcessos, tempoMax);

    // Resetar dados dos processos após simulação do SJF
    lerArquivo(processo, &maxProcessos, nomeArquivo);

    // Simula Round Robin
    int quantum = 4;
    RoundRobin(processo, maxProcessos, quantum);

    return 0;
}