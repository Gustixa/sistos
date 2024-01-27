#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  // Crear dos procesos hijos
  pid_t pid1 = fork();

  if (pid1 == -1) {
    perror("Error en fork");
    exit(EXIT_FAILURE);
  }

  if (pid1 == 0) {
    // Primer proceso hijo
    execl("./ipc", "ipc", "3", "A", NULL);
    perror("Error al ejecutar execl para el primer hijo");
    exit(EXIT_FAILURE);
  } else {
    // Proceso padre
    pid_t pid2 = fork();

    if (pid2 == -1) {
      perror("Error en fork");
      exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
      // Segundo proceso hijo
      execl("./ipc", "ipc", "2", "B", NULL);
      perror("Error al ejecutar execl para el segundo hijo");
      exit(EXIT_FAILURE);
    } else {
      // Proceso padre espera a ambos hijos
      wait(NULL);
      wait(NULL);
    }
  }

  return 0;
}
