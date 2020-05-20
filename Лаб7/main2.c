#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    sem_t *sem2, *sem1;
    int file, number;
    
    //sem_unlink("sem1");
    
    if((sem1 = sem_open("sem1",O_RDWR | O_CREAT, 0777, 1)) == SEM_FAILED) {
        printf("error: sem_open");
        return 1;
    }
    if((sem2 = sem_open("sem2", O_RDWR | O_CREAT, 0777, 1)) == SEM_FAILED) {
        printf("error: sem_open");
        return 1;
    }
    
    while(1) {
        sem_wait(sem2);
        sem_wait(sem1);
        if((file = open("file", O_RDONLY)) < 0) {
            printf("Can't open file\n");
            exit(-1);
        }
        pthread_mutex_lock(&mutex);
        read(file, &number, 1);
        close(file);
        pthread_mutex_unlock(&mutex);
        sem_post(sem1);
        printf("Random number: %d got\n", number);
    }
    return 0;
}
