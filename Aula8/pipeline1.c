#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int pid = fork();
    int fd[2];
    pipe(fd);   // Creates pipe

    if (pid > 0) {
        char *msg = "Sem pai";
        write(fd[1], msg, strlen(msg));
    } else if (pid == 0) {
        char buf[1024];
        read(fd[0], buf, 1024);
        printf("Msg %s\n", buf);
    } else {
        printf("Error occurred during fork()\n");
        return 1;
    }

    return 0;
}
