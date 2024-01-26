#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int i;

  for (i = 0; i < 4; ++i) {
    pid_t pid = fork();

    if (pid == 0) {
      // Código ejecutado por el proceso hijo
      printf("Proceso hijo en la iteración %d, PID: %d, PPID: %d\n", i + 1, getpid(), getppid());
      // El proceso hijo sale del bucle para evitar crear más procesos
      break;
    } else if (pid < 0) {
      // Manejo de error al crear el proceso hijo
      fprintf(stderr, "Error al crear el proceso hijo en la iteración %d\n", i + 1);
      return 1;
    }

    // El proceso padre espera a que el hijo termine
    wait(NULL);
  }
  return 0;
}
