#include "Thread.hpp"
#include <vector>
#define NUM 4

static int ticketnum = 1000;
using namespace ThreadModule;

void Ticket()
{
    while(true)
    {
        if(ticketnum > 0)
        {
            usleep(1000);

            // 抢票
            printf("get a new ticket, id: %d\n", ticketnum--);
        }else
        {
            printf("no ticket\n");
            break;
        }
    }
}

int main()
{
    std::vector<Thread> threads;

    for(int i = 0; i < NUM; i++)
    {
        threads.emplace_back(Ticket);
    }

    for(int i = 0; i < NUM; i++)
    {
        threads[i].start();
    }

    while(ticketnum > 0); // 等待抢票完成

    for(int i = 0; i < NUM; i++)
    {
        threads[i].join();
    }
    printf("ticketnum: %d\n", ticketnum);
}
