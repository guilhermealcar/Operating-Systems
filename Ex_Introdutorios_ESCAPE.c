/*Estudar (usar o man) e executar o código usando a função (em conformidade com a API POSIX  write()).
Crie uma biblioteca para implementar alguns métodos de entrada e saída de dados, seguindo as seguintes especificações:

DICA: Para ver mais sobre teclas de escape ->ESCAPE*/

#include <unistd.h> //biblioteca do write
#include <signal.h>
#include <stdio.h>
#include <string.h>


void myprintf(int x,int y, char* msg){
  int size = strlen(msg);
  char msg2[16];
  strcpy(msg2, "\033[2B\033[6C");

  msg2[2]= x + '0';
  msg2[6]= y + '0';

  write(1, msg2, 15);
	write(1, msg, 10);

}

int main(){
  int x, y;
  char *msg;

  myprintf(1,3,"Olá");

  return 0;
}
