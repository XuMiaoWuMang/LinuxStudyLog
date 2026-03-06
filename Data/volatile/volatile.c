#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int flag = 0;

void change(int signo) // 信号捕捉执行流
{
    (void)signo;

    flag = 1;
    printf("change flag: 0 -> 1\n, my pid is %d\n", getpid());
}

int main() // 主执行流
{
    printf("i'm main, my pid is %d\n", getpid());

    signal(2, change);

    while(!flag); // 在main函数中，我们从未修改过flag的值
    printf("程序正常退出\n");
}