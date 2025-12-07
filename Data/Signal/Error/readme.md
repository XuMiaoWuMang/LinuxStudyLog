## 异常
什么是异常？

向空指针写入数据会触发异常，终端会输出`Segmentation fault`（段错误），程序会终止。操作符'/'的右边是0，也就是除了个0，也会触发异常，终端会输出`Floating point exception`（浮点异常），程序会终止。

本质是OS向进程发送了SIGSEGV信号（11）和SIGFPE号信号（8），进程收到信号后，会终止并输出相应的错误信息，以及一个`(core dumped)`,这个则是表示核心转储。

但是，云服务器和虚拟机一般都是设置核心文件大小为0来关闭核心转储功能，通过`ulimit -a`命令可以查看内容：
```shell
ulimit -a
-t: cpu time (seconds)              unlimited
-f: file size (blocks)              unlimited
-d: data seg size (kbytes)          unlimited
-s: stack size (kbytes)             8192
-c: core file size (blocks)         0
-m: resident set size (kbytes)      unlimited
-u: processes                       15126
-n: file descriptors                1048576
-l: locked-in-memory size (kbytes)  495052
-v: address space (kbytes)          unlimited
-x: file locks                      unlimited
-i: pending signals                 15126
-q: bytes in POSIX msg queues       819200
-e: max nice                        0
-r: max rt priority                 0
-N 15: rt cpu time (microseconds)   unlimited
```
> unlimited是无限制的意思。
比如上面那个`-c: core file size (blocks)         0`，表示核心文件大小为0。通过`ulimit -c 10240`命令可以设置核心文件大小为10MB，这样当程序异常终止时，会生成核心文件。在当前目录下会出现一个名字为core的文件。（如果内核版本比较低的话，就是core.1234，1234是进程ID） 

### 核心转储

⾸先解释什么是Core Dump（核心转储）。当⼀个进程要异常终⽌时,可以选择把进程的用户空间内存数据全部保存到磁盘上,⽂件名通常是core,这叫做Core Dump。

### 核心转储文件的用法

1. 核心转储文件是二进制文件，需要用调试器来查看，比如gdb。

```shell
$ gdb <可执行程序名>
(gdb) core-file <核心转储文件名>
```

2. 核心转储文件中包含了进程的内存数据，包括寄存器、堆栈、全局变量等。
3. 核心转储文件可以用来调试程序，找出程序崩溃的原因。

这样，我们就能快速定位出错的代码和原因。我们一般称之为事后调试。

### 如果是子进程core dump了呢？

一般主程序异常退出，会显示一个core dump