#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 等效 bash 命令： ls -l | wc -l
                     v  ^
                     |  |
                     |  |
                     v  ^
            <--------    <---------
            |                      |
 子进程      --->======管道======    |
 父进程          ======管道====== --->
 */
int main()
{
    int pfds[2];    // pfds[0]：读取； pfds[1]：写入
    int fpid;
    if (0 != pipe(pfds))
    {
        perror("创建管道失败：");
        exit(EXIT_FAILURE);
    }
    fpid = fork();
    if (0 == fpid)  // 子进程
    {
        close(STDOUT_FILENO);           // 关闭标准输出（终端输出）
        dup2(pfds[1], STDOUT_FILENO);   // 重定向标准输出（终端输出）到‘子进程’的 管道 的’写入‘端
        close(pfds[0]);                 // 关闭’子进程‘管道的’输出端‘
        execlp("ls", "ls", "-l", NULL); // 执行’ls -l‘ bash命令（其产生的结果输出到管道的’写入‘端）
    }
    else    // 父进程
    {
        close(STDIN_FILENO);            // 关闭标注输入（终端输入）
        dup2(pfds[0], STDIN_FILENO);    // 重定向标准输入（终端输入）到’父进程‘的管道的’输出‘端
        close(pfds[1]);                 // 关闭’父进程‘管道的’写入‘端
        execlp("wc", "wc", "-l", NULL); // 执行’wc -l‘ bash命令（其输入来源于管道’输出‘端）
    }
    return 0;
}