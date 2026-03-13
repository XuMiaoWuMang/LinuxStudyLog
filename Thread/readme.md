## 线程

线程是操作系统调度的最小单位，一个进程可以包含多个线程。

优点是：

- 线程切换的成本低
- 线程之间共享进程的资源，如内存、文件描述符等
- 线程可以独立执行，互不干扰

缺点是：

- 线程的创建和销毁成本高
- 健壮性差，一个线程崩溃可能会导致整个进程崩溃
- 线程之间的同步和通信需要额外的机制，如锁、条件变量等

> 友情提醒：如果在linux中使用pthread库来创建线程，需要链接pthread库，如gcc -o test test.c **-lpthread**。
>> c++11中引入了thread库，我们可以使用thread库来创建线程，但本质上还是使用pthread库来实现的。

### 线程的基础操作

线程的基础操作包括：

- 创建线程
- 等待线程结束
- 取消线程
- 分离线程


### 线程的使用

在介绍线程的使用之前，我们需要先知道，在linux中，线程一般使用pthread库来创建和管理线程，而创建线程使用的pthread\_create函数不是系统调用，而是pthread库中的库函数。

在linux中，不存在线程的系统调用，因为对于操作系统来说，没有线程的概念，只有轻量级进程（LWP）的概念，所以linux只会提高轻量级进程的接口，如vfork等。

> 进程一般简称PCB，轻量级进程简称LWP，线程简称TCB。
>
> 值得一提的是，在fork和vfork函数中，都是使用系统调用clone来创建进程或者轻量级进程，只需要控制clone函数的flags参数即可。

所以在linux中，线程并不是操作系统所定义的概念，而是用户层面的概念，我们一般称其为用户级线程，但是在windows中，线程是操作系统所定义的概念，我们一般称其为内核级线程，它是真的有TCB，而不是像linux中那样，通过LWP来模拟线程。

值得一提的是，对于OS来说，时间片轮转的对象是PCB，而不是LWP，所以线程是瓜分PCB的时间片的，每个线程都有自己的时间片，当时间片用完时，就会切换到其他线程。

```c
#include <pthread.h>

int pthread_create(pthread_t *restrict thread,
                    const pthread_attr_t *restrict attr,
                    void *(*start_routine)(void *),
                    void *restrict arg);
```

参数：

- thread：指向线程ID的指针，用于返回新创建的线程ID
- attr：指向线程属性的指针，用于设置线程的属性，如栈大小、调度策略等
- start_routine：线程的入口函数，新创建的线程会从这个函数开始执行
- arg：传递给线程入口函数的参数

返回值：

- 成功：0
- 失败：非0值

> 参数thread是线程ID，和LWP的ID是不同的，由于linux操作系统内不存在线程的概念，所以管理线程的其实是pthread库。
>
> 也许你不理解，库函数为什么能管理线程这种看上去高大上的东西，但是，你一定就遇到过库管理概念的情况，那就是文件，即struct FILE。


c++的thread库是对pthread库的封装，使用方式可以使用lamdba表达式，也可以使用普通函数。

```c++
#include <thread>

std::thread t([](){
    while(true)
    {
        std::cout << "我是新线程，我的名字是： new thread" << std::endl;
        std::cout << "新线程的ID是： " << std::this_thread::get_id() << std::endl;
        sleep(1);
    }
});
```

在pthread_create函数中，我们可以传递一个参数arg，用于传递给线程入口函数的参数。这个参数的类型是`void *`，所以我们可以传递任意类型的参数。**任意**，意味着我们可以传递数字、字符串、乃至数组和类！！！

### 线程的退出

就像是父子进程之间，父进程需要回收子进程一样，线程之间也需要回收线程，且最后的回收都是主线程来回收，注意，并不是说所有线程都要由主线程来回收，只是说回收最后一个子线程的操作，必须由主线程来执行。并且，回收线程不止是为了释放资源，还是为了获得线程执行的结果。

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

参数：

- thread：要回收的线程ID
- retval：指向指针的指针，用于返回线程的退出状态

返回值：

- 成功：0
- 失败：非0值

> 调用pthread_join函数的线程会阻塞，直到指定的线程thread执行完毕。
>
> 如果thread线程已经执行完毕，那么pthread_join函数会立即返回。
>
> 如果retval不是NULL，那么线程thread的退出状态会被存储在retval指向的内存中。

在新线程的入口函数中，return的结果就是线程的返回值，主线程通过pthread_join函数来获取这个返回值，同时线程也就退出了。

线程当然不止return这种方式退出，还可以使用pthread_exit函数来退出线程。

```c
#include <pthread.h>

void pthread_exit(void *retval);
```

参数：

- retval：线程的返回值

返回值：无

> 调用pthread_exit函数会使当前线程退出，并且将retval作为线程的返回值。
>
> 线程退出后，其退出状态会被存储在retval指向的内存中，主线程可以通过pthread_join函数来获取这个退出状态。



### 线程的传参和返回值

我们常说线程像进程一样，每个线程都有独立的栈空间，页表等。但是线程之间传递参数和返回值的方式比进程简单多了。

#### 一.使用pthread_join(pthread_t thread, void **retval)

retval是一个指向指针的指针，可用于接收线程的退出状态或着线程的返回值。

使用方式：`((int *)retval)`，请注意，这里的int是线程返回值的类型，你需要根据实际情况来替换。

#### 二.使用临时变量或着全局变量

在线程创建之前，使用数组或者类的地址（你也可以把int* 变量的地址作为pthread_create的最后一个参数传进去），这样就不必在pthread_join函数中使用retval参数了。使用的时候也只需要`*`一下。

使用方式：

```c++
int ret = 0;
int* ret_p = &ret;

pthread_create(&tid1, nullptr, Task1, (void **)&ret_p);

pthread_join(tid1, nullptr);

std::cout << "线程tid1已回收, 结果为:  " << *ret_p << std::endl;
// std::cout << "线程tid1已回收, 结果为:  " << ret << std::endl; // 这样也行
```

实际上，参数可以是你申请的堆空间的地址，也可以是栈空间的地址，在新线程中，可以通过参数访问和修改这些变量，在新线程退出后，主线程也可以通过这些地址来获取这些变量。

就像是我把装着苹果的盘子交给了你，你把盘子里的苹果换成梨，然后把盘子给我，盘子还是那个盘子，但是苹果却变成了梨。线程也是如此，主线程将需要保存运算的变量的地址给新线程，新线程在执行完毕后，将结果存储在这个地址中，主线程通过这个地址来获取计算结果。

#### 三.使用pthread_cancel(pthread_t thread)

pthread_cancel函数可以用来取消一个线程的执行。

```c
#include <pthread.h>

int pthread_cancel(pthread_t thread);
```

参数：

- thread：要取消的线程ID

返回值：

- 成功：0
- 失败：非0值

> 调用pthread_cancel函数会向线程thread发送取消请求，线程thread会在合适的时机退出。
>
> 如果线程thread已经退出，那么pthread_cancel函数会立即返回。
>
> 如果线程thread没有响应取消请求，那么pthread_cancel函数会返回一个非0值。

但是！！！即使pthread_cancel函数已经执行了，依旧需要pthread_join函数来回收线程，否则线程就会成为僵尸线程。返回值为PTHREAD_CANCELED（即-1）。

### 线程分离

在多执行流的情况下，主线程是最后退出的，在项目中，主线程一般都是不退出的，除非用户手动退出

默认情况下，新创建的线程是joinable的，线程退出后，需要对其进⾏pthread_join操作，否则⽆法释放资源，从⽽造成系统泄漏。但是如果不关⼼线程的返回值，pthread_join是⼀种负担，这个时候，我们可以告诉系统，当线程退出时，⾃动释放线程资源。

```c
int pthread_detach(pthread_t thread);
```

参数：

- thread：要分离的线程ID

返回值：

- 成功：0
- 失败：非0值

> 调用pthread_detach函数会将线程thread设置为detached状态，线程thread退出后，系统会⾃动释放线程的资源，⽆需调⽤pthread_join函数。
>
> 如果线程thread已经退出，那么pthread_detach函数会立即返回。
>
> 如果线程thread不是joinable状态，那么pthread_detach函数会返回一个非0值。 