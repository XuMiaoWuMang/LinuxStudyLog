// 信号测试
// #include <iostream>
// #include <unistd.h>
// #include <signal.h>

// void handler(int signo)
// {
//     static int cnt = 0;
//     cnt++;
//     while(1){
//         std::cout << "get a signal: " << signo << ", cnt = " << cnt << std::endl;
//         sleep(1);
//     }
//     exit(1);
// }

// int main()
// {
//     struct sigaction act, oact;
//     act.sa_handler = handler;

//     sigaction(2, &act, &oact);
    
//     while(1)
//     {
//         pause();
//     }

//     return 0;
// }

// 信号阻塞集打印测试
// #include <iostream>
// #include <unistd.h>
// #include <signal.h>

// void PrintBlock()
// {
//     sigset_t set,oset;
//     sigemptyset(&set);
//     sigemptyset(&oset);
//     sigprocmask(SIG_BLOCK, &set, &oset);
//     std::cout << "block set: ";
//     for(int i = 31; i > 0; i--)
//     {
//         if(sigismember(&oset, i))
//         {
//             std::cout << "1";
//         }else{
//             std::cout << "0";
//         }
//     }
//     std::cout << std::endl;
// }

// void handler(int signo)
// {
//     static int cnt = 0;
//     cnt++;
//     while(1){
//         std::cout << "get a signal: " << signo << ", cnt = " << cnt << std::endl;
//         PrintBlock();
//         sleep(1);
//     }
//     exit(1);
// }

// int main()
// {
//     struct sigaction act, oact;
//     act.sa_handler = handler;
//     sigemptyset(&act.sa_mask); // 清空信号阻塞集
//     sigaddset(&act.sa_mask, 2);
//     sigaddset(&act.sa_mask, 3);
//     sigaddset(&act.sa_mask, 5);
//     sigaddset(&act.sa_mask, 6);
//     sigaction(2, &act, &oact);
    
//     while(1)
//     {
//         PrintBlock();
//         pause();
//     }

//     return 0;
// }

// handler结束时，清空阻塞集
// #include <iostream>
// #include <unistd.h>
// #include <signal.h>

// void PrintBlock()
// {
//     sigset_t set,oset;
//     sigemptyset(&set);
//     sigemptyset(&oset);
//     sigprocmask(SIG_BLOCK, &set, &oset);
//     std::cout << "block set: ";
//     for(int i = 31; i > 0; i--)
//     {
//         if(sigismember(&oset, i))
//         {
//             std::cout << "1";
//         }else{
//             std::cout << "0";
//         }
//     }
//     std::cout << std::endl;
// }

// void handler(int signo)
// {
//     static int cnt = 0;
//     cnt++;
//     while(1){
//         std::cout << "get a signal: " << signo << ", cnt = " << cnt << std::endl;
//         PrintBlock();
//         // sleep(1);
//         break;
//     }
//     // exit(1);
// }

// int main()
// {
//     struct sigaction act, oact;
//     act.sa_handler = handler;
//     sigemptyset(&act.sa_mask); // 清空信号阻塞集
//     sigaddset(&act.sa_mask, 2);
//     sigaddset(&act.sa_mask, 3);
//     sigaddset(&act.sa_mask, 5);
//     sigaddset(&act.sa_mask, 6);
//     sigaction(2, &act, &oact);
    
//     while(1)
//     {
//         PrintBlock();
//         pause();
//     }

//     return 0;
// }

// 打印pending信号集
#include <iostream>
#include <unistd.h>
#include <signal.h>

void PrintBlock()
{
    sigset_t set,oset;
    sigemptyset(&set);
    sigemptyset(&oset);
    sigprocmask(SIG_BLOCK, &set, &oset);
    std::cout << "block set: ";
    for(int i = 31; i > 0; i--)
    {
        if(sigismember(&oset, i))
        {
            std::cout << "1";
        }else{
            std::cout << "0";
        }
    }
    std::cout << std::endl;
}

void PrintPending()
{
    sigset_t set;
    ::sigpending(&set);
    std::cout << "pending set: ";
    for(int i = 31; i > 0; i--)
    {
        if(sigismember(&set, i))
        {
            std::cout << "1";
        }else{
            std::cout << "0";
        }
    }
    std::cout << std::endl;
}

void handler(int signo)
{
    static int cnt = 0;
    cnt++;
    while(1){
        std::cout << "get a signal: " << signo << ", cnt = " << cnt << std::endl;
        // PrintBlock();
        PrintPending();
        // sleep(1);
        break;
    }
    // exit(1);
}

int main()
{
    struct sigaction act, oact;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask); // 清空信号阻塞集
    sigaddset(&act.sa_mask, 2);
    sigaddset(&act.sa_mask, 3);
    sigaddset(&act.sa_mask, 5);
    sigaddset(&act.sa_mask, 6);
    sigaction(2, &act, &oact);
    
    while(1)
    {
        // PrintBlock();
        pause();
    }

    return 0;
}
