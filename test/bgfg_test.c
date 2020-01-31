#include <stdio.h>

int main() {
    int k = 0;
    while(1) {
        printf("%d\n", k);
        sleep(1);
        k++;
    }
    return 0;
}