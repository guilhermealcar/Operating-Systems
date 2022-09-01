#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid;

    pid = fork();

    if(pid < 0)
        printf("Erro na criação do processo\n");
    else if(pid == 0){
        printf("Processo filho Pid = %d\n", getpid());
        execl("/bin/ls", "ln", "-l", NULL);
    }else
        printf("Processo pai Pid = %d\n", getpid());

    return 0;
}