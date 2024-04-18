#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>

int main(int argc, char *argv[]){
    key_t k = ftok("/sys", 'X');
    int shmid = shmget(k, 1024, 0644 | IPC_CREAT);
    char *data;
    data = (char*)shmat(shmid, (void *)0, 0);

    if ((argc > 1) && !strcmp(argv[1], "apagar")){
        shmdt(data);
        shmctl(shmid, IPC_RMID, 0);
        printf("Deletando seguimento... \n\n");
        exit(0);
    } if (argc > 1){
        strncpy(data, argv[1], 1024);
    } else {
        printf("Conteudo do seguimento: \"%s\"\n", data);
    }

    return 0;
}