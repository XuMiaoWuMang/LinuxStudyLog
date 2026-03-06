### 编译器优化与不优化，结果却截然不同

仔细阅读以下代码
```c
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
```

正常的结果如下：
```
i'm main, my pid is 12345
^Cchange flag: 0 -> 1
, my pid is 12345
程序正常退出
```

可是，以上结果，是因为在使用gcc编译源代码时，未使用任何优化选项，即默认的`-O0`选项。倘若修改gcc的编译选项为`-O1`，则结果如下：
```
i'm main, my pid is 12345
^Cchange flag: 0 -> 1
, my pid is 12345
^Cchange flag: 0 -> 1
, my pid is 12345
...
```

程序并没有正常退出，即使我们在`change`函数中修改了`flag`的值为1，也没有影响到`main`函数中的`while`循环。

#### 编译器优化导致的问题

在解释之前，我们需要注意一个关键点，在main函数中，及其调用过的函数中，有对flag做出任何修改的操作吗？答案显而易见，完全没有！！！change函数中里有？不对啊，main函数没有调用过change函数啊！

毫无疑问，优化导致的问题就在这，在编译器看来，这个判断条件从未被修改过，所以对于cpu而言，这个判断条件是一个常量，只需要第一次遇到的时候读取一次，将flag的值缓存在寄存器中，后续的判断都直接使用寄存器中的值，而不会去内存中读取，大大加快了程序的运行速度。

~~典型的硅基思维~~你可以想象一下，有这么一个仓库，它有两个管理员，一个负责日常排班，一个负责处理紧急情况。有一天，日常管理员发现，今天一天都没有货车来仓库拉货，所以他会将昨日的库存，当作今日剩余的库存，填在自己的工作表格中。刚刚好，同一天，紧急管理员接到通知，有军工企业急需这批库存，紧急管理员立即将这批库存从仓库中取出，交付企业，同时在自己的工作表格中减去拉走的库存。

一到月底对账，就必定出现问题！日常管理员就是优化过后的main函数，它的while循环的判断条件不是从内存读取flag的值，而是将第一次读取的值缓存起来，后续的判断都直接使用缓存的值。而紧急管理员就是change函数，OS收到信号后，就会调用change函数修改flag的值。

#### 解决方法
为了避免编译器优化导致的问题，我们可以使用`volatile`关键字修饰flag变量，告诉编译器，这个变量是不确定的，容易变化的，每次都要从内存中读取flag的值，而不是使用缓存的值。

> volatile: 易变关键字
>
> 注意：volatile关键字只能修饰变量，不能修饰函数。

修改后的代码如下：
```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile int flag = 0; // 加上volatile关键字

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
```

使用`-O1`选项编译以上代码，结果如下：
```
i'm main, my pid is 12345
^Cchange flag: 0 -> 1
, my pid is 12345
程序正常退出
```

加入这个关键字后，编译器就会每次都从内存中读取flag的值，而不是使用缓存的值，从而避免了优化导致的问题。

### 学后感

const, register和volatile等关键字的受众到底是谁啊？答：编译器。c语言通过gcc编译成汇编语言，而汇编语言中，是不存在这些关键字的！
