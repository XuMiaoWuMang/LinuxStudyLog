
### OS如何处理短时间内连续收到多个信号

```c++
#include <iostream>
#include <unistd.h>
#include <signal.h>

void handler(int signo)
{
    static int cnt = 0;
    cnt++;
    while(1){
        std::cout << "get a signal: " << signo << ", cnt = " << cnt << std::endl;
        sleep(1);
    }
    exit(1);
}

int main()
{
    struct sigaction act, oact;
    act.sa_handler = handler;

    sigaction(2, &act, &oact);
    
    while(1)
    {
        pause();
    }

    return 0;
}
```
以上代码，运行结果如下：
```
^Cget a signal: 2, cnt = 1
^Cget a signal: 2, cnt = 1
^Cget a signal: 2, cnt = 1
...
```

具体流程：
1. 主函数中，`sigaction(2, &act, &oact);` 表示将信号`2`（即`SIGINT`）的处理函数设置为`handler`。
2. 执行到循环时，`pause()` 函数会使进程进入睡眠状态，直到收到一个信号。
3. 当程序收到信号`2`（即`SIGINT`）时，会调用`handler`函数。
4. `handler`函数中，每执行一次`handler`函数，`cnt`变量会加1，且会打印出信号号和`cnt`的值。
5. 由于`handler`函数中包含一个死循环，所以当收到信号`2`（即`SIGINT`）时，会一直打印出信号和`cnt`的值，直到进程被强制终止。
6. 但是，多次按下`Ctrl+C`，只有第一次会触发`handler`函数，后续的按下`Ctrl+C`，进程会继续运行，不会触发`handler`函数。
> 因为操作系统在执行信号处理函数(即`handler`函数)的时候，会阻塞该信号，后续的`SIGINT`信号将不会递达，直到`handler`函数执行完毕。

### 如何查看信号阻塞集
在上述代码中添加`PrintBlock()`函数，用于打印当前信号阻塞集。
```c++
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
```

使用`sigprocmask()`函数可以获取当前信号阻塞集。使用`sigismember()`函数可以判断一个信号是否在阻塞集中。
#### 自定义阻塞集
可以使用`sigprocmask()`函数自定义信号阻塞集。

举例：
```c++
sigset_t set;
sigemptyset(&set); // 清空信号阻塞集
sigaddset(&set, 2); // 阻塞信号2（SIGINT）
sigprocmask(SIG_BLOCK, &set, NULL); // 将信号2添加到阻塞集中
```

事实上，在开头的`main()`函数中，`sigaction(2, &act, &oact);` 表示将信号`2`（即`SIGINT`）的处理函数设置为`handler`,这是因为在上一行`act.sa_handler = handler;` 中，将`handler`函数赋值给了`act.sa_handler`。同理，也只需要修改`act.sa_mask`，就可以自定义对应的阻塞集。

比如将开头的例子里的`main()`函数修改为：
```c++
int main()
{
    struct sigaction act, oact;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask); // 清空信号阻塞集
    sigaddset(&act.sa_mask, 2);
    sigaddset(&act.sa_mask, 3);
    sigaddset(&act.sa_mask, 4);
    sigaddset(&act.sa_mask, 5);
    sigaction(2, &act, &oact);
    
    while(1)
    {
        PrintBlock();
        pause();
    }

    return 0;
}
```
> 别忘了在handler函数的循环里调用`PrintBlock()`函数，用于打印当前信号阻塞集。

那么程序运行结果如下：
```
block set: 0000000000000000000000000000000
^Cget a signal: 2, cnt = 1
block set: 0000000000000000000000000111010
get a signal: 2, cnt = 1
block set: 0000000000000000000000000111010
get a signal: 2, cnt = 1
...
```
> 请不要同时阻塞2、3号信号，否则你将无法使用键盘直接结束进程！！！
>> 注意，4号信号只是结束输入流，不会结束进程。
>>
>> 虽然20号信号(ctrl+z)可以暂停进程，但是程序只是陷入沉睡，资源依旧在占用！

### 查看pending信号集
在上述代码中添加`PrintPending()`函数，用于打印当前pending信号集。
```c++
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
```

再在`handler`函数中调用`PrintPending()`函数，用于打印当前pending信号集。

运行结果如下：
```
^Cget a signal: 2, cnt = 1
pending set: 0000000000000000000000000000000
^Cget a signal: 2, cnt = 2
pending set: 0000000000000000000000000000000
^Cget a signal: 2, cnt = 3
pending set: 0000000000000000000000000000000
...
```

可以看到pending信号集为空，说明没有pending信号。即在调用`handler`函数前，就已经将信号对应的pending置0。

> 如果pending是在`handler`函数结束后置0的，那么在`handler`函数中，2号信号对应的pending就应该为1，而结果为0，说明在`handler`函数执行前，2号信号对应的pending就已经被置0了。


### 重入

#### 重入定义

简单来说，当一个函数被两个或两个以上的执行流同时调用了，称为重入。

如果程序在这种情况下，导致了程序的行为不可预测，甚至会导致程序崩溃，那么就称这个程序是不可重入的。反之，如果程序在这种情况下，行为是可预测的，结果是正常的，那么就称这个程序是可重入的。