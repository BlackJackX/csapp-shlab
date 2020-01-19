/*
测试waitpid会不会影响signalhandler
*/
#include <csapp.h>
#define N 5

void handler2(int sig) {
    int olderrno = errno;
    pid_t pid;

    printf("aaaaaa\n");

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


    if(Fork() == 0) {
        printf("Hello from child %d\n", (int)getpid());
        exit(0);
    }
    
    //waitpid(-1, NULL, 0);

    if((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0)

        unix_error("read");

    printf("Parent processing input\n");
    while(1)
        ;

    exit(0);

}