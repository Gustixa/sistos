#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Uso: %s origen.txt destino.txt \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Abrir el archivo de origen para lectura
  int fd_origen = open(argv[1], O_RDONLY);
  if (fd_origen == -1) {
    perror("Error al abrir el archivo de origen");
    exit(EXIT_FAILURE);
  }

  // Crear o truncar el archivo de destino para escritura
  int fd_destino = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd_destino == -1) {
    perror("Error al abrir/crear el archivo de destino");
    close(fd_origen);
    exit(EXIT_FAILURE);
  }

  // Leer del archivo de origen y escribir en el archivo de destino
  char buffer[BUFFER_SIZE];
  ssize_t bytes_leidos, bytes_escritos;

  while ((bytes_leidos = read(fd_origen, buffer, sizeof(buffer))) > 0) {
    bytes_escritos = write(fd_destino, buffer, bytes_leidos);
    if (bytes_escritos != bytes_leidos) {
      perror("Error al escribir en el archivo de destino");
      close(fd_origen);
      close(fd_destino);
      exit(EXIT_FAILURE);
    }
  }

  if (bytes_leidos == -1) {
    perror("Error al leer el archivo de origen");
    close(fd_origen);
    close(fd_destino);
    exit(EXIT_FAILURE);
  }

  // Cerrar los archivos
  close(fd_origen);
  close(fd_destino);

  printf("Archivo copiado exitosamente de %s a %s\n", argv[1], argv[2]);

  return 0;
}