#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }

     int fd[2][2] = {}; // 0 from parent to children
                       // 1 for children to parent

    for (int i = 0; i < 2; i++)
    {
        int stat = pipe(fd[i]);
        if (stat < 0)
        {
            fprintf(2, "pipe failed\n");
            exit(1);
        }
    }

    int pid = fork();
    if (pid < 0)
    {
        fprintf(2, "fork failed\n");
        exit(1);
    }

   

    if (pid == 0)
    {
        close(fd[1][0]);
        close(fd[0][1]);
        int n = -1;
        int stat = read(fd[0][0], &n, sizeof(n));
        if (stat < 0)
        {
            fprintf(2, "read failed\n");
            exit(1);
        }
        wait(0);
        fprintf(1, "%d: received ping\n", getpid());
        n = 2333333;
        stat = write(fd[1][1], &n, sizeof(n));
        if (stat < 0)
        {
            fprintf(2, "write failed\n");
            exit(1);
        }
        close(fd[0][0]);
        close(fd[1][1]);
        exit(0);
    }
    else
    {
        close(fd[0][0]);
        close(fd[1][1]);
        int n = 114514;
        int stat = write(fd[0][1], &n, sizeof(n));
        if (stat < 0)
        {
            fprintf(2, "write failed\n");
            exit(1);
        }
        //fprintf(1, "%d: sent %d\n", pid, n);
        wait(0);
        stat = read(fd[1][0], &n, sizeof(n));
        if (stat < 0)
        {
            fprintf(2, "read failed\n");
            exit(1);
        }
        fprintf(1, "%d: received pong\n", getpid());
        close(fd[0][1]);
        close(fd[1][0]);
        wait(0);
        exit(0);
        fprintf(1, "%d: received pong\n", pid);
    }

    exit(0);
}