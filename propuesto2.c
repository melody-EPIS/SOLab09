#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // Número de filósofos
#define MAX_COMIDAS 3 // Número máximo de veces que cada filósofo puede comer

sem_t tenedores[N];
sem_t contador; // Semáforo contador para controlar el número de filósofos comiendo

void pensar(int filosofo) {
    printf("Filósofo %d está pensando...\n", filosofo);
    sleep(rand() % 3);
}

void comer(int filosofo) {
    printf("Filósofo %d está comiendo...\n", filosofo);
    sleep(rand() % 3);
}

void *filosofo(void *arg) {
    int filosofo = *(int *)arg;
    int comidas = 0;

    while (comidas < MAX_COMIDAS) {
        pensar(filosofo);

        // Tomar tenedor izquierdo
        sem_wait(&tenedores[filosofo]);
        printf("Filósofo %d tomó el tenedor izquierdo\n", filosofo);

        // Tomar tenedor derecho (evitar deadlock usando sem_trywait)
        if (sem_trywait(&tenedores[(filosofo + 1) % N]) == 0) {
            printf("Filósofo %d tomó el tenedor derecho\n", filosofo);
            comer(filosofo);
            sem_post(&tenedores[filosofo]);
            sem_post(&tenedores[(filosofo + 1) % N]);
            comidas++;
        } else {
            // Si no pudo tomar el tenedor derecho, suelta el izquierdo y lo intenta más tarde
            printf("Filósofo %d no pudo tomar el tenedor derecho, soltando el izquierdo\n", filosofo);
            sem_post(&tenedores[filosofo]);
        }
    }

    printf("Filósofo %d ha terminado de comer.\n", filosofo);

    // Decrementar el contador de filósofos comiendo
    sem_wait(&contador);
    printf("Filósofo %d ha salido de la mesa.\n", filosofo);
    sem_post(&contador);

    pthread_exit(NULL);
}

int main() {
    pthread_t filosofos[N];
    int ids[N];

    sem_init(&contador, 0, N - 1); // Inicializar el semáforo contador

    for (int i = 0; i < N; i++) {
        sem_init(&tenedores[i], 0, 1); // Inicializar cada semáforo tenedor con valor 1 (disponible)
        ids[i] = i;
    }

    // Crear los hilos de los filósofos
    for (int i = 0; i < N; i++) {
        pthread_create(&filosofos[i], NULL, filosofo, &ids[i]);
    }

    // Esperar a que terminen los hilos
    for (int i = 0; i < N; i++) {
        pthread_join(filosofos[i], NULL);
    }

    // Destruir los semáforos
    sem_destroy(&contador);
    for (int i = 0; i < N; i++) {
        sem_destroy(&tenedores[i]);
    }

    return 0;
}
