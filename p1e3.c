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
    for (int i = 1; i <= 4000000; ++i) {
        printf("Conteo en el proceso hijo: %d\n", i);
    }
  } else {
    // Proceso padre
    while (1) {
        // Ciclo while infinito en el proceso padre
    }
  }

  return 0;
}
