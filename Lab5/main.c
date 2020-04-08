#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>


int main(int argc, const char * argv[]) {
    size_t sizeRead;
    char string[200];
    int i = 1;
    char* str = NULL;
    sizeRead = read(0, string, 200);
    
    if(sizeRead <= 0){
        printf("Can\'t read.\n");
        return(-1);
    };
    
    int file1 = open("file1", O_WRONLY | O_TRUNC | O_CREAT, 0777);
    int file2 = open("file2", O_WRONLY | O_TRUNC | O_CREAT, 0777);
    str = strtok(string, "\n");
    
    while (str != NULL) {
        if(i%2 == 0) {
            char newStr[16];
            snprintf(newStr, sizeof(str), "%d.", i);
            write(file1, newStr, 2);
            write(file1, str, strlen(str));
            write(file1, "\n", 1);
        } else {
            char newStr[16];
            snprintf(newStr, sizeof(newStr), "%d", i);
            write(file2, newStr, 2);
            write(file2, str, strlen(str));
            write(file2, "\n", 1);
        }
        printf("%d. %s\n", i, str);
        i += 1;
        str= strtok(NULL, "\n");
    }
    
    close(file1);
    close(file2);
    return 0;
}
