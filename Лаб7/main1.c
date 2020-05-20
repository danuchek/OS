#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<string.h>
#include<semaphore.h>
#include<pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    sem_t *sem2, *sem1;
    int file, number;
    char inputSymbols[10];

    if((sem1 = sem_open("sem1", O_RDWR | O_CREAT, 0777, 1)) == SEM_FAILED) {
        printf("error: sem_open");
        return 1;
    }

    if((sem2 = sem_open("sem2", O_RDWR | O_CREAT, 0777, 1)) == SEM_FAILED) {
        printf("error: sem_open");
        return 1;
    }

//     sem_unlink("sem2");

    while(1) {
        read(0, inputSymbols, sizeof(inputSymbols));
        number = rand() % 10;
        sem_wait(sem1);
        if((file = open("file", O_WRONLY | O_CREAT, 0777)) < 0) {
            printf("Can't open file\n");
            exit(-1);
        }
        pthread_mutex_lock(&mutex);
        write(file, &number, sizeof(number));
        close(file);
        pthread_mutex_unlock(&mutex);
        printf("Random number: %d sent\n", number);
        sem_post(sem1);
        sem_post(sem2);
    }
    return 0;
}
