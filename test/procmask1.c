/*
本机fork先执行父再执行子导致问题
(添加显示等待信号循环)
*/
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
        while(isfull(jobs)) ;

        if((pid = Fork()) == 0) {
            Execve("/bin/date", argv, NULL);
        }
        Sigprocmask(SIG_BLOCK, &mask_all, NULL);
        addjob(jobs, pid, 1, "/bin/date");
        Sigprocmask(SIG_SETMASK, &prev_one, NULL);        
    }
    exit(0);
}

