#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Array para armazenar os descritores de arquivo dos pipes
    int pipes[3][2];

    // Criar os 3 pipes
    for (int i = 0; i < 3; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Iterar para criar os 4 processos
    for (int i = 0; i < 4; i++) {
        int pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Processo filho
            // Fechar os lados dos pipes que não serão usados por este processo
            for (int j = 0; j < 3; j++) {
                if (j != i - 1 && j != i) {
                    close(pipes[j][0]); // Fechar lado de leitura
                    close(pipes[j][1]); // Fechar lado de escrita
                }
            }

            if (i == 3) { // Quarto processo
                char *msg = "Mensagem do quarto processo";
                write(pipes[i - 1][1], msg, strlen(msg) + 1);
            } else { // Outros processos
                char buffer[1024];
                read(pipes[i][0], buffer, sizeof(buffer));
                printf("Processo %d recebeu: %s\n", i, buffer);
            }

            exit(EXIT_SUCCESS);
        }
    }

    // Fechar os lados dos pipes no processo pai que não serão usados
    for (int i = 0; i < 3; i++) {
        close(pipes[i][0]); // Fechar lado de leitura
        close(pipes[i][1]); // Fechar lado de escrita
    }

    return 0;
}
