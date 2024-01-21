#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
  pid_t f = fork();

  if (f == -1) {
    perror("Error en la llamada al sistema fork");
    exit(EXIT_FAILURE);
  } 
  
  if(f == 0) {
    execl("./part1", "./part1", (char*)NULL);
    perror("Error en la llamada del sistema execl");
    exit(EXIT_FAILURE);
  } else {
    printf("Nombre del proces padre: %d\n", (int)getpid());
    wait(NULL);
    printf("Proces hijo terminado\n");
    
  }
  return 0;
}