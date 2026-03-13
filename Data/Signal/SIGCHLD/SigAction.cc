#include <iostream>
#include <unistd.h>
#include <signal.h>

void handler(int signo)
{
    std::cout << "get a signal: " << signo << ", pid = " << getpid() << std::endl;
}

int main()
{
    signal(SIGCHLD, handler);

    pid_t pid = fork();
    if(pid < 0)
    {
        std::cerr << "fork error" << std::endl;
    }
    else if(pid == 0)
    {
        // 子进程
        std::cout << "child process, pid = " << getpid() << std::endl;
        sleep(3);
        exit(0);
    }
    else
    {
        while(true)
        {
            sleep(1);
        }
    }
    return 0;
}