#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    if (argc > MAXARG)
    {
        fprintf(2, "xargs: too many arguments\n");
        exit(1);
    }

    int num = 1;
    char *args[MAXARG] = {};
    args[0] = "echo";
    int args_start = 1;
    int args_num = 0;

    if (argc >= 2 && strcmp(argv[1], "-n") == 0)
        num = atoi(argv[2]), args_start = 3;

    for (int i = args_start; i < argc; i++)
        args[args_num++] = argv[i];

    char buf[512];
    int eof = num;
    int idx = args_num;
    int cnt = 0;
    int stat;
    
    while ((stat = read(0, &buf[cnt], sizeof(char))) > 0)
    {
        if (buf[cnt] == '\n' || buf[cnt] == ' ')
        {
            eof--;
            buf[cnt] = 0;
            args[idx] = buf;
            idx++;
            cnt = 0;
        }
        else
        {
            cnt++;
        }
        if (eof == 0)
        {
            if (cnt > 0)
            {
                buf[cnt] = '\0';
                args[idx] = buf;
                idx++;
                cnt = 0;
            }
            int pid = fork();
            if (pid < 0)
            {
                fprintf(2, "failed fork");
                exit(1);
            }
            if (pid == 0)
            {
                exec(args[0], args);
                fprintf(2, "failed exec");
                exit(1);
            }
            wait(0);
            eof = num;
            idx = args_num;
        }
    }

    exit(0);
}