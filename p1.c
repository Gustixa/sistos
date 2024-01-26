/**
 * Programa 1 del ejercicio 1
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  int i;
  for (i = 0; i < 4; ++i) {
    pid_t pid = fork();

    if (pid == 0) {
      // Proceso hijo, codigo
      printf("Proces hijo %d, PID: %d, PPID: %d\n", i + 1, getpid(), getppid());
      break;
    } else if (pid < 0) {
      // Manejo de posibles errores al crear el proceso hijo
      fprintf(stderr, "Error al crear el proceso hijo %d\n", i+1);
      return 1;
    }
  }
  // Proceso padre
  if (i == 4) {
    for (i = 0; i < 4; ++i) {
      wait(NULL);
    }
  }
  return 0;
}