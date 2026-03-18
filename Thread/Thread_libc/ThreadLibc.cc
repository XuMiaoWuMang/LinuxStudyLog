#include "Thread.hpp"

std::string toHex(pthread_t tid)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "0x%lx", tid);
    return std::string(buf);
}


int main()
{
    ThreadModule::Thread threads([](){
        while(true)
        {
            std::cout << "hello world" << std::endl;
            sleep(1);
        }
    });
    threads.start();
    std::cout << "新线程“" << threads.getName() << "”启动成功" << std::endl;
    sleep(3);
    threads.stop();
    std::cout << "新线程“" << threads.getName() << "”停止成功" << std::endl;
    sleep(3);
    threads.join();
    std::cout << "新线程“" << threads.getName() << "”退出成功" << std::endl;
    return 0;
}
