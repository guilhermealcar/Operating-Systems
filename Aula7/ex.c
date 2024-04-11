#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

int main(){
    // Cria um arq p compartilhar entre os processos
    int fd = open("shared_file.txt", O_CREAT | O_RDWR, 0644);
    if(fd == -1){
        perror("Erro ao criar arquivo compartilhado");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork(); // Processo filho

    if(pid < 0){
        perror("Erro ao criar o processo filho");
        exit(EXIT_FAILURE);
    } else if(pid == 0){
        // Escreve no arquivo compartilhado
        char *message = "Mensagem do filho\n";
        write(fd, message, strlen(message));
        printf("Filho: escreveu no arquivo compartilhado\n");

        // Executa um programa externo
        execl("/bin/echo", "echo", "Executando um programa externo", NULL);
        perror("Erro na execucao do programa externo");
        exit(EXIT_FAILURE);
    } else {    // Processo pai
        wait(NULL); // Espera o processo filho terminar

        // Le do arquivo compartilhado
        lseek(fd, 0, SEEK_SET);
        char buffer[100];
        read(fd, buffer, sizeof(buffer));
        printf("Pai: leu do arquivo compartilhado: %s", buffer);

        // Fecha o arquivo compartilhado
        close(fd);

        // Mata o processo filho
        if (kill(pid, SIGKILL) == -1){
            perror("Erro ao matar o processo filho");
            exit(EXIT_FAILURE);
        } else {
            printf("Pai: processo filho foi morto\n");
        }
    }

    return 0;
}