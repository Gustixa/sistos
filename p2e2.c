/**
 * Programa 2, ejercicio 2
*/
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    clock_t start, end;
    double elapsed_time;

    start = clock();

    pid_t pid = fork();

    if (pid == 0) {
        // Proceso hijo
        pid_t child_pid = fork();

        if (child_pid == 0) {
            // Proceso bisnieto
            for (int i = 0; i < 1000000; ++i) {}
        } else {
            // Proceso hijo
            for (int i = 0; i < 1000000; ++i) {}
            wait(NULL);
        }
    } else {
        // Proceso padre
        wait(NULL);
        end = clock();
        elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Tiempo transcurrido (concurrente): %f segundos\n", elapsed_time);
    }

    return 0;
}
