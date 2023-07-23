#include <stdio.h>
#include <semaphore.h>
main() {
    sem_t my_semaphore;
    int value;
    sem_init(&my_semaphore, 0, 10);
    sem_getvalue(&my_semaphore, &value);
    printf("El valor inicial del semaforo es %d\n", value);
    sem_post(&my_semaphore);
    sem_getvalue(&my_semaphore, &value);
    printf("El valor del semaforo después del post es %d\n", value);
}