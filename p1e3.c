#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
  pid_t pid = fork();

  if (pid == -1) {
    perror("Error al crear el proceso hijo");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    // Proceso hijo
    printf("Mensaje del proceso hijo\n");
  } else {
    // Proceso padre
    while (1) {
        // Ciclo while infinito en el proceso padre
    }
  }

  return 0;
}
