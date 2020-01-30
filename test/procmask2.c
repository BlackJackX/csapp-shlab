#include "csapp.h"
#include "jobs.h"

/* Values for the HOW argument to `sigprocmask'.  */
#define	SIG_BLOCK     0		 /* Block signals.  */
#define	SIG_UNBLOCK   1		 /* Unblock signals.  */
#define	SIG_SETMASK   2		 /* Set the set of blocked signals.  */
#define MAXJOBS 10

struct job_t jobs[MAXJOBS]; /* The job list */

void handler(int sig) {
    int olderrno = errno;
    sigset_t mask_all, prev_all;
    pid_t pid;
    
    Sigfillset(&mask_all);
    while((pid = waitpid(-1, NULL, 0)) > 0) {
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        deletejob(jobs, pid);
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    if(errno != ECHILD)
        Sio_error("waitpid error");
    errno = olderrno;
    
}

int main(int argc, char **argv) {
    int pid;
    sigset_t mask_all, mask_one, prev_one;

    Sigfillset(&mask_all);
    Sigemptyset(&mask_one);
    Sigaddset(&mask_one, SIGCHLD);
    Signal(SIGCHLD, handler);
    initjobs(jobs);
    int t = 100;
    while(t--) {
        Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);       //屏蔽exit中断
        if((pid = Fork()) == 0) {
            Sigprocmask(SIG_SETMASK, &prev_one, NULL);      //解除exit中断
            Execve("/bin/date", argv, NULL);
        }
        Sigprocmask(SIG_BLOCK, &mask_all, NULL);            //屏蔽所有中断
        addjob(jobs, pid, 1, "/bin/date");
        Sigprocmask(SIG_SETMASK, &prev_one, NULL);          //恢复原中断
    }
    exit(0);
}