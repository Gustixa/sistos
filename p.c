#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define N 9
#define M 9

// Declaramos el arreglo bidimensional globalmente
int sudoku[N][M];

// Función para revisar que todos los números del uno al nueve estén en una columna
void validateColumn(int col) {
  for (int i = 0; i < N; i++) {
    if (sudoku[i][col] < 1 || sudoku[i][col] > 9) {
      printf("Error en la columna %d\n", col);
      return;
    }
  }
}

// Función para revisar que todos los números del uno al nueve estén en una fila
void validateRow(int row) {
  for (int col = 0; col < M; col++) {
    if (sudoku[row][col] < 1 || sudoku[row][col] > 9) {
      printf("Error en la fila %d\n", row);
      return;
    }
  }
}

// Función para revisar que todos los números del uno al nueve estén en un subarreglo de 3x3
void validateSubarray(int row, int col) {
  int subarray[3][3] = {
    {sudoku[row][col], sudoku[row][col + 1], sudoku[row][col + 2]},
    {sudoku[row + 1][col], sudoku[row + 1][col + 1], sudoku[row + 1][col + 2]},
    {sudoku[row + 2][col], sudoku[row + 2][col + 1], sudoku[row + 2][col + 2]}
  };

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (subarray[i][j] < 1 || subarray[i][j] > 9) {
        printf("Error en el subarreglo de %d, %d\n", row, col);
        return;
      }
    }
  }
}

int main() {
  // Abrimos el archivo de solución
  FILE *file = fopen("sudoku", "r");
  if (file == NULL) {
    perror("Error al abrir el archivo");
    return 1;
  }

  // Mapeamos el archivo a la memoria
  char *fileContent = (char *)mmap(NULL, sizeof(char) * 81, PROT_READ, MAP_PRIVATE, fileno(file), 0);
  if (fileContent == MAP_FAILED) {
    perror("Error mapeando el archivo a memoria");
    fclose(file);
    return 1;
  }

  // Copiamos el contenido del archivo a la grilla
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      sudoku[i][j] = *(fileContent + i * M + j) - '0';
    }
  }

  // Imprimimos la matriz por consola
  printf("Matriz Sudoku:\n");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      printf("%d ", sudoku[i][j]);
    }
    printf("\n");
  }

  // Cerramos el archivo
  fclose(file);

  // Desmapeamos la memoria
  munmap(fileContent, sizeof(char) * 81);

  // Revisamos cada fila
  for (int i = 0; i < N; i++) {
    validateRow(i);
  }

  // Revisamos cada columna
  for (int col = 0; col < M; col++) {
    validateColumn(col);
  }

  // Revisamos cada subarreglo de 3x3
  for (int row = 0; row < N; row += 3) {
    for (int col = 0; col < M; col += 3) {
      validateSubarray(row, col);
    }
  }

  return 0;
}
