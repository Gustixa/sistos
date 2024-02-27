#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

// Recursos a compartir
#define RECURSO_1 1
#define RECURSO_2 2

// Procesos
#define PROCESO_1 1
#define PROCESO_2 2

// Variables para almacenar los recursos
bool recurso_1_libre = true; 
bool recurso_2_libre = true; 

// Función que simula el proceso 1
void *proceso_1(void *arg) {
    while (1) {
        // Intenta adquirir el recurso 1
        if (recurso_1_libre) {
            recurso_1_libre = false;
            printf("Proceso 1 adquiere recurso 1\n");
        } else {
            // Si no puede adquirir el recurso 1, intenta adquirir el recurso 2
            if (recurso_2_libre) {
                recurso_2_libre = false;
                printf("Proceso 1 adquiere recurso 2\n");
            } else {
                // Si no puede adquirir ninguno de los recursos, se bloquea
                printf("Proceso 1 se bloquea\n");
                sleep(1);
            }
        }
    }
    return NULL;
}

// Función que simula el proceso 2
void *proceso_2(void *arg) {
    while (1) {
        // Intenta adquirir el recurso 2
        if (recurso_2_libre) {
            recurso_2_libre = false;
            printf("Proceso 2 adquiere recurso 2\n");
        } else {
            // Si no puede adquirir el recurso 2, intenta adquirir el recurso 1
            if (recurso_1_libre) {
                recurso_1_libre = false;
                printf("Proceso 2 adquiere recurso 1\n");
            } else {
                // Si no puede adquirir ninguno de los recursos, se bloquea
                printf("Proceso 2 se bloquea\n");
                sleep(1);
            }
        }
    }
    return NULL;
}

int main(void) {
    pthread_t thread_1, thread_2;

    // Inicializacion threads
    if (pthread_create(&thread_1, NULL, proceso_1, NULL) != 0) {
        perror("Error creating thread 1");
        return 1;
    }

    if (pthread_create(&thread_2, NULL, proceso_2, NULL) != 0) {
        perror("Error creating thread 2");
        return 1;
    }

    // Esperar a que los threads terminen
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    return 0;
}
