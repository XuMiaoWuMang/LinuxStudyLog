#include <typeindex>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <thread>

std::string toHex(pthread_t tid)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "0x%lx", tid);
    return std::string(buf);
}

void *start(void *arg)
{
    std::string name = static_cast<const char*>(arg);
    sleep(1);
    while(true)
    {
        std::cout << "I'm a new thread, my name is :" << name << ", thread id is: " << toHex(pthread_self()) << std::endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, start, (void*)"thread-1");
    std::cout << "I'm a new thread, my name is : main, thread id is " << toHex(pthread_self()) << " create thread: " << toHex(tid) << std::endl;
    

    pthread_join(tid, nullptr);
    return 0;
}
