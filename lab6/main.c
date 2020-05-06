#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>

int fd1[2], fd12[2];

void signalCalled() {
    printf("Signal was called\n");
    char time[100];
    int seconds;
    
    close(fd1[1]);
    
    read(fd1[0], &seconds, sizeof(seconds));
    printf("Child %d read: %d seconds\n", getpid(), seconds);

    close(fd1[0]);
    close(fd12[0]);
    
    int h = seconds / 3600;
    int m = (seconds / 60) % 60;
    int s = seconds % 60;
    sprintf(time, "%d %d %d", h, m, s);

    printf("Child %d write formatted time = %s\n", getpid(), time);
    write(fd12[1], &time, sizeof(time));

    close(fd12[1]);

    printf("Child %d exit\n", getpid());
}

int main() {
    int res;
    int seconds;

    if (pipe(fd1) < 0 || pipe(fd12) < 0) {
        printf("Can't create pipe\n");
        return -1;
    }

    res = fork();
    if (res < 0) {
        printf("Can't fork child\n");
        return -1;
    }
    else if (res > 0) {
        char num2[100];
        printf("Time: \n");
        scanf("%d", &seconds);
        
        close(fd1[0]);
        printf("Parent %d write: ", getpid());
        
        printf("%d\n", seconds);
        write(fd1[1], &seconds, sizeof(seconds));
        close(fd1[1]);

        kill(res, SIGUSR1);
        wait(NULL);

        close(fd12[1]);
        read(fd12[0], num2, strlen(num2));
        close(fd12[0]);

        printf("Parent %d read: ", getpid());
        printf("%s\n", num2);
        printf("Parent %d exit\n", getpid());
    } else {
        (void)signal(SIGUSR1, signalCalled);
        pause();
    }
}
