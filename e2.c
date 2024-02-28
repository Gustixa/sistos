#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define IZQUIERDA (filosofo + NUM_FILOSOFOS - 1) % NUM_FILOSOFOS
#define DERECHA (filosofo + 1) % NUM_FILOSOFOS

pthread_t filosofos[NUM_FILOSOFOS];
sem_t tenedores[NUM_FILOSOFOS];
sem_t mutex; // Semáforo para garantizar exclusión mutua en la sección crítica

void *filosofo_func(void *arg) {
    int filosofo = *(int *)arg;

    while (1) {
        printf("Filósofo %d está pensando.\n", filosofo);

        // Esperar para tomar el tenedor izquierdo
        sem_wait(&tenedores[IZQUIERDA]);

        // Esperar para tomar el tenedor derecho
        sem_wait(&tenedores[DERECHA]);

        // Filósofo está comiendo
        printf("Filósofo %d está comiendo.\n", filosofo);
        sleep(rand() % 3 + 1); // Simula el tiempo que tarda en comer

        // Soltar los tenedores
        sem_post(&tenedores[IZQUIERDA]);
        sem_post(&tenedores[DERECHA]);

        // Filósofo termina de comer y vuelve a pensar
        printf("Filósofo %d termina de comer y vuelve a pensar.\n", filosofo);
        sleep(rand() % 3 + 1); // Simula el tiempo que tarda en pensar
    }
}

int main() {
    srand(time(NULL));

    // Inicializar semáforos
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_init(&tenedores[i], 0, 1);
    }

    // Crear hilos para los filósofos
    int filosofo_ids[NUM_FILOSOFOS];
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        filosofo_ids[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo_func, (void *)&filosofo_ids[i]);
    }

    // Esperar a que los hilos terminen (esto nunca debería ocurrir)
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(filosofos[i], NULL);
    }

    // Destruir semáforos
    sem_destroy(&mutex);
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_destroy(&tenedores[i]);
    }

    return 0;
}
