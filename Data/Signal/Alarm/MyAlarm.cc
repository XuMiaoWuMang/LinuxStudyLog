#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <functional>
#include <vector>

using func_t = std::function<void()>;
int count = 0;
std::vector<func_t> callbacks;

void handler(int signo)
{
    count++;
    for (auto &callback : callbacks)
    {
        callback();
    }
    std::cout << "count: " << count << std::endl;
    alarm(1);// 一定要重新设置，否则只会执行一次
}
int main()
{
    callbacks.push_back([]()
                        { std::cout << "打开日志log.txt" << std::endl; });
    callbacks.push_back([]()
                        { std::cout << "写入日志log.txt" << std::endl; });
    callbacks.push_back([]()
                        { std::cout << "保存日志log.txt" << std::endl; });
    callbacks.push_back([]()
                        { std::cout << "关闭日志log.txt" << std::endl; });

    alarm(1);
    signal(SIGALRM, handler);
 
    while (1){}
}