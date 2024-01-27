#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Uso: %s <n> <x>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int n = atoi(argv[1]);
  char x = argv[2][0];

  // Crear o adjuntar a la memoria compartida
  key_t key = ftok(".", 'S');
  int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("Error al obtener el identificador de la memoria compartida");
    exit(EXIT_FAILURE);
  }

  // Adjuntar la memoria compartida a un puntero
  char *shm_ptr = shmat(shmid, NULL, 0);
  if (shm_ptr == (char *)-1) {
    perror("Error al adjuntar la memoria compartida");
    exit(EXIT_FAILURE);
  }

  // Crear un proceso hijo
  pid_t pid = fork();

  if (pid == -1) {
    perror("Error en fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    // Proceso hijo
    usleep(500000);  // Retraso de 500,000 microsegundos (0.5 segundos)

    while (*shm_ptr != '\0') {
      if (*shm_ptr == x) {
        printf("Proceso hijo: Recibí '%c'\n", x);
      }
      shm_ptr++;
    }
  } else {
    // Proceso padre
    for (int i = 1; i <= SHM_SIZE; i++) {
      if (i % n == 0) {
        *shm_ptr = x;
        printf("Proceso padre: Envié '%c'\n", x);
      }
      shm_ptr++;
    }

    // Esperar a que el proceso hijo complete
    wait(NULL);

    // Desplegar el contenido de la memoria compartida
    printf("Contenido de la memoria compartida: %p\n", shmat(shmid, NULL, 0));

    // Desadjuntar la memoria compartida
    if (shmdt(shm_ptr) == -1) {
      perror("Error al desadjuntar la memoria compartida");
      exit(EXIT_FAILURE);
    }

    // Eliminar la memoria compartida
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
      perror("Error al eliminar la memoria compartida");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}
