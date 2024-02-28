#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_RECURSOS 3
#define NUM_PROCESOS 5

int disponibles[NUM_RECURSOS];
int maximo[NUM_PROCESOS][NUM_RECURSOS];
int asignado[NUM_PROCESOS][NUM_RECURSOS];
int necesidad[NUM_PROCESOS][NUM_RECURSOS];

bool solicitud_segura(int proceso, int solicitud[NUM_RECURSOS]) {
    // Verificar si la solicitud es válida
    for (int i = 0; i < NUM_RECURSOS; i++) {
        if (solicitud[i] > necesidad[proceso][i] || solicitud[i] > disponibles[i]) {
            return false;
        }
    }

    // Simular asignación de recursos al proceso
    for (int i = 0; i < NUM_RECURSOS; i++) {
        disponibles[i] -= solicitud[i];
        asignado[proceso][i] += solicitud[i];
        necesidad[proceso][i] -= solicitud[i];
    }

    // Verificar si el sistema sigue siendo seguro después de la asignación
    bool proceso_finalizado[NUM_PROCESOS] = {false};
    int secuencia_ejecucion[NUM_PROCESOS];
    int contador_finalizados = 0;

    while (contador_finalizados < NUM_PROCESOS) {
        bool ejecucion_encontrada = false;

        for (int i = 0; i < NUM_PROCESOS; i++) {
            if (!proceso_finalizado[i]) {
                // Verificar si el proceso puede finalizar
                bool recursos_suficientes = true;
                for (int j = 0; j < NUM_RECURSOS; j++) {
                    if (necesidad[i][j] > disponibles[j]) {
                        recursos_suficientes = false;
                        break;
                    }
                }

                if (recursos_suficientes) {
                    // Simular la liberación de recursos
                    for (int j = 0; j < NUM_RECURSOS; j++) {
                        disponibles[j] += asignado[i][j];
                    }

                    // Marcar el proceso como finalizado
                    proceso_finalizado[i] = true;
                    secuencia_ejecucion[contador_finalizados++] = i;
                    ejecucion_encontrada = true;
                }
            }
        }

        if (!ejecucion_encontrada) {
            // No se encontró un proceso seguro, deshacer asignaciones y salir
            for (int i = 0; i < NUM_PROCESOS; i++) {
                if (!proceso_finalizado[i]) {
                    for (int j = 0; j < NUM_RECURSOS; j++) {
                        disponibles[j] += asignado[i][j];
                    }
                }
            }

            return false;
        }
    }

    // La solicitud es segura, imprimir la secuencia de ejecución
    printf("Solicitud segura para el proceso %d. Secuencia de ejecución: ", proceso);
    for (int i = 0; i < NUM_PROCESOS; i++) {
        printf("%d ", secuencia_ejecucion[i]);
    }
    printf("\n");

    return true;
}

int main() {
    // Inicializar recursos disponibles
    for (int i = 0; i < NUM_RECURSOS; i++) {
        disponibles[i] = rand() % 10 + 1;
    }

    // Inicializar matrices maximo y asignado
    for (int i = 0; i < NUM_PROCESOS; i++) {
        for (int j = 0; j < NUM_RECURSOS; j++) {
            maximo[i][j] = rand() % (disponibles[j] + 1);
            asignado[i][j] = rand() % (maximo[i][j] + 1);
            necesidad[i][j] = maximo[i][j] - asignado[i][j];
        }
    }

    // Imprimir estado inicial
    printf("Recursos disponibles: ");
    for (int i = 0; i < NUM_RECURSOS; i++) {
        printf("%d ", disponibles[i]);
    }
    printf("\n\n");

    printf("Matriz Maximo:\n");
    for (int i = 0; i < NUM_PROCESOS; i++) {
        for (int j = 0; j < NUM_RECURSOS; j++) {
            printf("%d ", maximo[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Matriz Asignado:\n");
    for (int i = 0; i < NUM_PROCESOS; i++) {
        for (int j = 0; j < NUM_RECURSOS; j++) {
            printf("%d ", asignado[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Simular solicitudes y liberación de recursos
    for (int i = 0; i < NUM_PROCESOS; i++) {
        int solicitud[NUM_RECURSOS];
        for (int j = 0; j < NUM_RECURSOS; j++) {
            solicitud[j] = rand() % (necesidad[i][j] + 1);
        }

        printf("Proceso %d solicita recursos: ", i);
        for (int j = 0; j < NUM_RECURSOS; j++) {
            printf("%d ", solicitud[j]);
        }
        printf("\n");

        if (!solicitud_segura(i, solicitud)) {
            printf("La solicitud para el proceso %d no es segura. Abortando.\n", i);
            break;
        }

        // Simular liberación de recursos después de un tiempo
        sleep(1);

        int liberacion[NUM_RECURSOS];
        for (int j = 0; j < NUM_RECURSOS; j++) {
            liberacion[j] = rand() % (asignado[i][j] + 1);
        }

        printf("Proceso %d libera recursos: ", i);
        for (int j = 0; j < NUM_RECURSOS; j++) {
            printf("%d ", liberacion[j]);
        }
        printf("\n");

        // Liberar recursos
        for (int j = 0; j < NUM_RECURSOS; j++) {
            asignado[i][j] -= liberacion[j];
            necesidad[i][j] += liberacion[j];
            disponibles[j] += liberacion[j];
        }

        // Imprimir estado después de la liberación
        printf("Recursos disponibles: ");
        for (int j = 0; j < NUM_RECURSOS; j++) {
            printf("%d ", disponibles[j]);
        }
        printf("\n\n");

        printf("Matriz Asignado:\n");
        for (int k = 0; k < NUM_PROCESOS; k++) {
            for (int j = 0; j < NUM_RECURSOS; j++) {
                printf("%d ", asignado[k][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    return 0;
}
