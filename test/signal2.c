#include <csapp.h>
#define N 5

void handler2(int sig) {
    int olderrno = errno;
    pid_t pid;

    while((pid = waitpid(-1, NULL, 0)) > 0) {
        printf("Handler reaped child %d\n", pid);
        //Sio_puts("Handler reaped child");
    }
    Sio_puts("aaaa\n");
    printf("%d\n", pid);
    if(errno != ECHILD)
        Sio_error("waitpid error");
    Sleep(1);
    errno = olderrno;
}

int main() {
    int i, n;
    char buf[MAXBUF];

    if(signal(SIGCHLD, handler2) == SIG_ERR)
        unix_error("signal error");

    for(i = 0; i < N; i++) {
        Sleep(10);
        if(Fork() == 0) {
            printf("Hello from child %d\n", (int)getpid());
            exit(0);
        }
    }

    if((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0)

        unix_error("read");

    printf("Parent processing input\n");
    while(1)
        ;

    exit(0);

}