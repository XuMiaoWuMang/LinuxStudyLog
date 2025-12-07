#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

// 父进程读取、子进程写入
int main()
{
    // 1.创建管道
    int fds[2] = {0};
    int n = pipe(fds);

    if (n != 0)
    {
        std::cerr << "pipe error" << std::endl;
    }

    // 2.创建子进程
    pid_t pid = fork();
    if(pid < 0)
    {
        std::cerr << "fork error" << std::endl;
    }
    else if(pid == 0)
    {
        // 子进程
        // 3.关闭不需要的fd
        ::close(fds[0]);
        int cnt = 0;
        while(true)
        {
            std::string str = "hello world, i'm child process, i'm ";
            str += std::to_string(getpid());
            str += " ,cnt = " + std::to_string(cnt++);

            ::write(fds[1], str.c_str(), str.size());
            sleep(1);
        }


        ::close(fds[1]);
        exit(0);
    }
    else
    {
        // 父进程
        // 3.关闭不需要的fd
        ::close(fds[1]);

        char buffer[1024] = {0};
        while(true)
        {
            size_t n = ::read(fds[0], buffer, sizeof(buffer) - 1);

            if(n > 0) buffer[n] = '\0';
            std::cout << "child process --> father process, message: " << buffer << std::endl;
        }



        ::close(fds[0]);

        pid_t ret = waitpid(pid, nullptr, 0);
        std::cout << "father wait child success" << std::endl;
    }
    return 0;
}
//匿名管道只能用于具有亲缘关系的进程之间通信