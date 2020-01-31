#include <csapp.h>

int main() {
    int pid;
    if((pid = Fork()) == 0) {
        printf("111111\n");
    }
    else {
        printf("33333\n");
        sleep(5);
        printf("22222\n");
    }
    return 0;
}