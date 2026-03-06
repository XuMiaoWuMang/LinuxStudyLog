在终端下，直接按下键盘上的Ctrl + C，即可终止程序运行。但是，只能中止前台程序，如`./process`运行的程序，而后台程序无法中止，即`./process &`。

但是，后台程序依旧会输出内容到屏幕上，此时可以在终端中输入`fg 1`,即可将后台程序移至前台，然后按下Ctrl + C即可终止程序运行。

如果不想后台程序输出影响终端，又希望后台程序能正常运行，要么直接修改后台程序的源代码，重新编译运行，要么使用`nohup`命令，即`nohup ./process &`，将后台程序运行在nohup模式下，这样后台程序运行时，不会输出内容到屏幕上，而是输出到nohup.out文件中。

## 信号

信号是进程间通信的一种方式，用于通知进程发生了某种事件。例如，当用户按下Ctrl + C时，操作系统会向进程发送一个SIGINT信号，告诉进程用户希望终止它。进程可以捕获这个信号，并执行相应的操作。

所有信号的宏定义都在`<signal.h>`头文件中，可以通过`kill -l`命令查看所有信号。

```bash
$kill -l
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL       5) SIGTRAP
 6) SIGABRT      7) SIGBUS       8) SIGFPE       9) SIGKILL     10) SIGUSR1
11) SIGSEGV     12) SIGUSR2     13) SIGPIPE     14) SIGALRM     15) SIGTERM
16) SIGSTKFLT   17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP
21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ
26) SIGVTALRM   27) SIGPROF     28) SIGWINCH    29) SIGIO       30) SIGPWR
31) SIGSYS      34) SIGRTMIN    35) SIGRTMIN+1  36) SIGRTMIN+2  37) SIGRTMIN+3
38) SIGRTMIN+4  39) SIGRTMIN+5  40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8
43) SIGRTMIN+9  44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12 47) SIGRTMIN+13
48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14 51) SIGRTMAX-13 52) SIGRTMAX-12
53) SIGRTMAX-11 54) SIGRTMAX-10 55) SIGRTMAX-9  56) SIGRTMAX-8  57) SIGRTMAX-7
58) SIGRTMAX-6  59) SIGRTMAX-5  60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2
63) SIGRTMAX-1  64) SIGRTMAX
```
也许你会疑惑，为什么会有这么多信号，而且这些信号的名字都这么长，计算机难道搞了和字符串表，出错了就一个个比对，然后输出并处理吗？

当然不是，信号本质其实就是一个整型数字，采用的是位图的方式来保存的。上面的信号前面都有一个编号，意思是第几位为1来表示的，比如SIGINT就是第2位为1，SIGQUIT就是第3位为1，以此类推。


### 信号维护

OS(操作系统)会在PCB(进程控制块)中维护一个信号集，用于记录当前进程所接收到的信号。信号集是一个位图，每一位代表一个信号，如果该位为1，则表示该信号已经被进程接收，如果该位为0，则表示该信号还没有被进程接收。
> 对于进程来讲，信号不应该用接收来描述，而应该用写入来描述。

在进程中存在一个函数指针数组来保存对应的信号处理函数，当进程接收到信号时，OS会根据信号类型，找到对应的处理函数，并执行该函数。
***
### 信号捕捉与处理
对于信号的处理总共有三种大致处理方法:1、忽略信号，2、用户自定义操作，3、执行默认操作。

用户自定义操作可以使用`signal()`函数，该函数接受两个参数，第一个参数是要处理的信号类型，第二个参数是处理函数的指针。处理函数的原型为`void handler(int sig)`，其中sig参数就是接收到的信号类型。处理函数的函数名可以自定义。因为宏定义的原因，所以signal函数的第一个参数既可以以SIG开头，也可以直接使用数字。

```c++
#include <iostream>
#include <signal.h>

void signal_handler(int signum) {
    if( signum == SIGINT) {
        std::cout << "Interrupt signal (" << signum << ") received.\n"; 
    }
}
int main() {
    signal(SIGINT, signal_handler); 
    while(true) {
        std::cout << "Hello World!" << std::endl;
        sleep(1);
    }
}
```

以上代码的执行结果如下:

```bash
$./sig
Hello World!
Hello World!
^CInterrupt signal (2) received.
Hello World!
```

细心的朋友一定发现了，在上述代码中，signal函数被安排在了循环之外！

这是因为signal函数只能被调用一次，如果被多次调用，那么只有最后一次调用的处理函数会生效。因此，signal函数应该被放在程序开始的地方，而不是放在循环中。

如果程序一直都没有捕捉到对应的信号，那么直到程序结束，对应的signal_handler函数都不会被调用!

然而，并不是所有信号都能进行捕捉，然后执行自定义操作，比如SIGKILL信号，该信号不能被捕捉，也不能被忽略，只能执行默认操作，即终止进程。

### 信号的产生方式

信号一共有四种产生方式：
1. 键盘
2. 指令
3. 系统调用
4. 软件条件产生
5. 异常

#### 键盘

键盘一共可以产生四种信号，分别是：

1. ctrl + c：SIGINT（2）
2. ctrl + \：SIGQUIT（3）
3. ctrl + d：EOF（4）
4. ctrl + z：SIGTSTP（20）

#### 指令

在linux的终端中，可以使用kill命令来向进程发送信号，格式为`kill -信号类型 pid`，例如`kill -9 5414`，表示向进程ID为5414的进程发送SIGINT信号。

```bash
$ ./sig &
[1] 5414
Hello World!
Hello World!
Hello World!
Hello World!
Hello World!
Hello World!
kill -9 5414
[1]+  Killed                  ./sig
```


#### 系统调用

C++中，可以使用`kill()`函数来向进程发送信号，格式为`kill(pid, signum)`，其中pid为进程ID，signum为信号类型。例如`kill(5414, SIGINT)`，表示向进程ID为5414的进程发送SIGINT信号。

头文件：`<sys/types.h>`和`<signal.h>`。

还存在`raise(signum)`函数，该函数接受一个参数，表示要发送的信号类型，例如`raise(SIGINT)`，表示向当前进程发送SIGINT信号。

> 如果是`raise(9)`，那么就可以说**它自杀了**。

头文件：`<signal.h>`。

还有一个函数，`abort(signum)`。会给调用进程发送一个信号，默认是`SIGABRT`也就是6号信号，该信号会导致该进程异常终止，并产生core dump文件。

头文件：`<signal.h>`。

#### 软件条件产生

软件条件产生信号，一般是通过软件条件来触发信号的产生，例如：

1. 管道破裂：当进程向一个已经关闭的管道写入数据时，操作系统会向进程发送SIGPIPE信号。
2. alarm定时器到期，操作系统会向进程发送SIGALRM信号。

#### 异常

最经典的两个：
1. 野指针
2. 除0

会分别产生SIGSEGV（11号）和SIGFPE（8号）信号。

请一定要在信号处理函数中写好退出代码，否则程序将不会正常退出，直接卡死。

> 比如除0异常，是因为cpu的某个标志位记录了程序出现异常，os会捕捉到这个异常，然后向进程发送SIGFPE信号，但是cpu不是你一个进程的，不存在上下文这种东西，你用完了我用，我用完了他用，所以这个标志位就一直在，os有一直能检测到异常，就一直向程序发送SIGFPE信号，被进程捕捉了，但是进程没有退出代码，于是就这样一直循环往复。

> 同理，野指针也是一样，在进行页表转化的时候，野指针转换成立了无权限或者不存在的地址，CPU就知道了出现了异常，然后向进程发送SIGSEGV信号，然后进程捕捉到，但是没有退出代码，标志位一直存在，然后向进程发送SIGSEGV信号，然后被进程捕捉，循环往复。
