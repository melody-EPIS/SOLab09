#include <stdio.h>
#include <semaphore.h>
main() {
    sem_t my_semaphore;
    int value;
    sem_init(&my_semaphore, 0, 1);
    sem_getvalue(&my_semaphore, &value);
    printf("El valor inicial del semaforo es %d\n", value);
    sem_wait(&my_semaphore);
    sem_getvalue(&my_semaphore, &value);
    printf("El valor del semaforo después del wait es %d\n", value);
}