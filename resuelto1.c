#include <semaphore.h>
main() {
    sem_t my_semaphore;
    int rc;
    rc = sem_init(&my_semaphore, 0, 10);
    rc = sem_destroy(&my_semaphore);
}
