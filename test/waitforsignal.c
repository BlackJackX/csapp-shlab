/*
while体内空循环浪费cpu
放pause,当while-pause之间来信号接收不到pid改变
放sleep,可能浪费sleep的时间
*/

#include "csapp.h"

/* Values for the HOW argument to `sigprocmask'.  */
#define	SIG_BLOCK     0		 /* Block signals.  */
#define	SIG_UNBLOCK   1		 /* Unblock signals.  */
#define	SIG_SETMASK   2		 /* Set the set of blocked signals.  */

volatile sig_atomic_t pid;

void sigchld_handler(int s) {
    int olderrno = errno;
    pid = waitpid(-1, NULL, 0);
    errno = olderrno;
}

void sigint_handler(int s) {

}

int main() {
    sigset_t mask, prev;
    Signal(SIGCHLD, sigchld_handler);
    Signal(SIGINT, sigint_handler);
    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);

    while(1) {
        Sigprocmask(SIG_BLOCK, &mask, &prev);
        if(Fork() == 0)
            exit(0);
        
        pid = 0;
        Sigprocmask(SIG_SETMASK, &prev, NULL);

        while(!pid) ;

        printf(".");
    }
    exit(0);
}