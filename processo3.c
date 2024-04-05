#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    int pid, status;
    pid = fork();

    if(pid == -1){  // fork falhou
        perror("fork falhou!\n");
        exit(-1);
    }
    else if(pid == 0){  // Este é o processo filho

        printf("processo pai pid: %d\t pid filho: %d\t", getppid(), getpid());
        pid = fork();
        if (pid == 0){
            printf("pid neto: %d\n", getpid());
        }
        while(1);
        exit(130);
    }
    else{   // Este é o processo pai
        wait(&status);
        if(WIFEXITED(status))
            printf("Processo filho encerrou normalmente\n\n");
        if(WIFSIGNALED(status))
            printf("Processo filho encerrou por kill\n\n");

        printf("Processo filho encerrou com codigo exit = %d\n\n", WEXITSTATUS(status));

        exit(0);
    }

}