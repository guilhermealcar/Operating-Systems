/*Elabore um programa que a cada 5 segundos exiba na tela a mensagem "dentro de um loop".
Quando o usuáro digitar CTRL_C o programa deverá exibir uma mensagem "saindo..." e sair do programa.
OBS: Não use nada que não seja da API POSX.*/

#include <unistd.h> //biblioteca do write
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


void exemplo_alarme(int signum){

    write(1, "\n\ndentro de um loop\n\n",25);


}

void exemplo_interrupcao(int signum){


    write(1, "\n\nsaindo...\n\n",20);


}

int main()
{
    int i=1;

    //signal(SIGTSTP, exemplo_tstp);

    do{
      if(signal(SIGINT, exemplo_interrupcao)){
        i=0;
      }

      signal(SIGALRM, exemplo_alarme);
      i=1;
      alarm(5);
      pause(); // faz com que o processo (ou thread) hiberne até que um sinal,
      // que encerre o processo ou cause a invocação de uma função de captura de sinal, seja entregue.
    } while (i!=0);


    return 0;


}
