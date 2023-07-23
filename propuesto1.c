#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#define SUMSIZE 100
#define BUFSIZE 8
int buffer[BUFSIZE];
sem_t llenos;
sem_t vacios;
static void *productor(void *arg1) {
    int i, entra, producir;
    entra=0;
    for (i=1;i<=SUMSIZE;i++){
        while((producir = getchar())=='\n');
        sem_wait(&vacios);
        buffer[entra] = producir;
        entra = (entra + 1) % BUFSIZE;
        printf("Productor -->\t %c\n",producir);
        sem_post(&llenos);
    }
    return NULL;
}

static void *consumidor(void *arg2) {
    int i, sale, consumir;
    sale=0;
    for(i=1;i<=SUMSIZE;i++){
        sem_wait(&llenos);
        sleep(1); // el consumo se demora 1 segundo sem_wait(&llenos);
        consumir = buffer[sale];
        sale = (sale + 1) % BUFSIZE;
        printf(" \t%c\tConsumidor -->\n",consumir);
        sem_post(&vacios);
    }
    return NULL;
}

main() {
    pthread_t prodtid;
    pthread_t constid;
    // semáforos contadores para controlar los espacios en el almacén
    sem_init(&llenos,0, 0);
    sem_init(&vacios,0, BUFSIZE);
    pthread_create(&prodtid, NULL, productor, NULL);
    //pthread_create(&constid, NULL, consumidor, NULL);
    pthread_join(prodtid, NULL);
    pthread_join(constid, NULL);
    printf("termidado\n");
}
