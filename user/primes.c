#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int, int) __attribute__((noreturn));

void sieve(int rd_fd, int maxn)
{
    int base = -1;

    int stat = read(rd_fd, &base, sizeof(base));

    if (stat <= 0)
    {
        close(rd_fd);
        exit(0);
    }

    fprintf(1, "prime %d\n", base);
    if (base > maxn)
    {
        close(rd_fd);
        exit(0);
    }

    int fd[2];
    stat = pipe(fd);
    if (stat < 0)
    {
        fprintf(2, "pipe failed\n");
        close(fd[0]);
        close(fd[1]);
        exit(1);
    }

    int pid = fork();
    if (pid < 0)
    {
        fprintf(2, "fork failed\n");
        close(rd_fd);
        close(fd[0]);
        close(fd[1]);
        exit(1);
    }

    if (pid == 0)
    {
        close(fd[1]);
        close(rd_fd);
        sieve(fd[0], maxn);
    }
    else
    {
        close(fd[0]);
        int num = 0;
        while (read(rd_fd, &num, sizeof(num)) > 0)
        {
            if (num % base != 0)
                write(fd[1], &num, sizeof(num));
        }
        close(rd_fd);
        close(fd[1]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    int fd[2];
    int stat = pipe(fd);
    if (stat < 0)
    {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0)
    {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    int maxn = 280;
    if (pid == 0)
    {
        close(fd[1]);
        sieve(fd[0], maxn);
    }
    else
    {
        close(fd[0]);
        for (int i = 2; i < maxn; i++)
        {
            stat = write(fd[1], &i, sizeof(i));
            if (stat < 0)
            {
                fprintf(2, "write failed\n");
                exit(1);
            }
        }
        close(fd[1]);
        wait(0);
        exit(0);
    }
}