#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 9
#define M 9
#define NUM_THREADS 3

// Arreglo bidimensional global
int sudoku[N][M];

// Estructura para pasar argumentos a la función del hilo
typedef struct {
  int start;          // Índice de inicio para la validación
  int end;            // Índice de fin para la validación
  char *threadName;   // Nombre del hilo para la identificación
} ThreadArgs;

/**
 * Función que valida una fila de Sudoku.
 *
 * @param args Puntero a una estructura ThreadArgs con los argumentos.
 * @return NULL.
 */
void *validateRow(void *args) {
  ThreadArgs *threadArgs = (ThreadArgs *)args;
  int start = threadArgs->start;
  int end = threadArgs->end;
  char *threadName = threadArgs->threadName;

  #pragma omp parallel for private(col)
  for (int row = start; row < end; row++) {
    for (int col = 0; col < M; col++) {
      if (sudoku[row][col] < 1 || sudoku[row][col] > 9) {
        printf("%s: Error en la fila %d\n", threadName, row);
        return NULL;
      }
    }
  }

  printf("%s: Validación de filas completa\n", threadName);
  return NULL;
}

/**
 * Función que valida una columna de Sudoku.
 *
 * @param args Puntero a una estructura ThreadArgs con los argumentos.
 * @return NULL.
 */
void *validateColumn(void *args) {
  ThreadArgs *threadArgs = (ThreadArgs *)args;
  int start = threadArgs->start;
  int end = threadArgs->end;
  char *threadName = threadArgs->threadName;

  #pragma omp parallel for private(row)
  for (int col = start; col < end; col++) {
    for (int row = 0; row < N; row++) {
      if (sudoku[row][col] < 1 || sudoku[row][col] > 9) {
        printf("%s: Error en la columna %d\n", threadName, col);
        return NULL;
      }
    }
  }

  printf("%s: Validación de columnas completa\n", threadName);
  return NULL;
}

/**
 * Función que valida un subarreglo de 3x3 de Sudoku.
 *
 * @param args Puntero a una estructura ThreadArgs con los argumentos.
 * @return NULL.
 */
void *validateSubarray(void *args) {
  ThreadArgs *threadArgs = (ThreadArgs *)args;
  int start = threadArgs->start;
  int end = threadArgs->end;
  char *threadName = threadArgs->threadName;

  #pragma omp parallel for private(x, y)
  for (int i = start; i < end; i += 3) {
    for (int j = 0; j < M; j += 3) {
      int subarray[3][3] = {
        {sudoku[i][j], sudoku[i][j + 1], sudoku[i][j + 2]},
        {sudoku[i + 1][j], sudoku[i + 1][j + 1], sudoku[i + 1][j + 2]},
        {sudoku[i + 2][j], sudoku[i + 2][j + 1], sudoku[i + 2][j + 2]}
      };

      #pragma omp parallel for private(x, y)
      for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
          if (subarray[x][y] < 1 || subarray[x][y] > 9) {
            printf("%s: Error en el subarreglo de %d, %d\n", threadName, i, j);
            return NULL;
          }
        }
      }
    }
  }

  printf("%s: Validación de subarreglos completa\n", threadName);
  return NULL;
}

/**
 * Función principal.
 *
 * @return 0 si el programa se ejecuta con éxito.
 */
int main() {
    omp_set_num_threads(1);
    // Abrimos el archivo de solución
    FILE *file = fopen("sudoku", "r");
    if (file == NULL) {
    perror("Error al abrir el archivo");
    return 1;
    }

    // Mapear el archivo a la memoria
    char *fileContent = (char *)malloc(sizeof(char) * 81);
    fread(fileContent, sizeof(char), 81, file);

    // Contenido del archivo a la grilla
    for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
        sudoku[i][j] = fileContent[i * M + j] - '0';
    }
    }

    // Imprimir la matriz por consola
    printf("Matriz Sudoku:\n");
    for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
        printf("%d ", sudoku[i][j]);
    }
    printf("\n");
    }

    // Cerrar el archivo
    fclose(file);
    free(fileContent);

    // Hilos para validar en paralelo
    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];

    // Argumentos para cada hilo
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].threadName = (char *)malloc(20);
        sprintf(args[i].threadName, "Thread %d", i + 1);
    }

    // Argumentos para validar las filas
    args[0].start = 0;
    args[0].end = N;

    // Argumentos para validar las columnas
    args[1].start = 0;
    args[1].end = M;

    // Argumentos para validar los subarreglos
    args[2].start = 0;
    args[2].end = N;
    pid_t pid = getpid();

    if(fork() == 0) {
    char pid_str[10];
    sprintf(pid_str, "%d", getppid());
    execlp("ps", "ps", "-p", pid_str, "-lLf", NULL);
    }

    pthread_t thread;
    pthread_create(&thread, NULL, validateColumn, (void*)&args[1]);

    pthread_join(thread, NULL);

    printf("ID del thread: %ld\n", (long)syscall(SYS_gettid));

    int status;
    waitpid(-1, &status, 0);

    // Validación de filas

    if(fork() == 0) {
    char pid_str[10];
    sprintf(pid_str, "%d", getppid());
    execlp("ps", "ps", "-p", pid_str, "-lLf", NULL);
    }

    printf("Sudoku resuelto\n");

    // Imprimir si es valido

    pthread_join(threads[2], NULL);

    int status2;
    waitpid(-1, &status2, 0);

    return 0;
}
