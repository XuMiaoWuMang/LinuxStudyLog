#include <iostream>
#include <signal.h>

void handler(int sig)
{
    std::cout << "Caught signal " << sig << std::endl;
}

int main()
{
    // signal(SIGSEGV, handler);
    // // 野指针会触发段错误
    // int *p = nullptr;
    // *p = 10;
    // signal(SIGFPE, handler);
    if (fork() == 0)
    {
        int a = 10;
        a /= 0;
        return 0;
    }
    int a = 10;
    a /= 0;
    return 0;
}