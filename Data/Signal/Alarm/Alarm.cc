// #include <iostream>
// #include <sys/types.h>
// #include <signal.h>
// #include <unistd.h>

// int main()
// {
//     alarm(1);
//     int count = 0;
//     while(1)
//     {
//         count++;
//         printf("count = %d\n", count);
//     }
//     return 0;
// }
// 可以统计一秒内count能进行多少次累加，并输出。count = 121914

#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int count = 0;

void alarm_handler(int signo)
{
    std::cout << "get signal: "<< signo << ", ";
    std::cout << "count = " << count << std::endl;
    exit(signo);
}
int main()
{
    alarm(1);
    signal(SIGALRM, alarm_handler);
    while(1)
    {
        count++;
    }
    return 0;
}
// 可以测试一秒内count执行多少次累加，并输出。count = 584510807