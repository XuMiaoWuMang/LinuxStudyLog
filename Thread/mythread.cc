// ------------------------------ 不同语言的线程 ---------------------------------
// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>
// #include <thread>

// std::string toHex(pthread_t tid)
// {
//     char buf[64];
//     snprintf(buf, sizeof(buf), "0x%lx", tid);
//     return std::string(buf);
// }

// void *routine(void *arg)
// {
//     std::string thread_name = static_cast<const char *>(arg);

//     while(true)
//     {
//         std::cout << thread_name << ", name = " << thread_name << ", ";
//         std::cout << ", tid = " << toHex(pthread_self()) << std::endl;
//         sleep(1);
//     }
//     return nullptr;
// }

// int main()
// {
//     // 使用pthread_create创建线程
//     pthread_t tid;
//     int n = pthread_create(&tid, nullptr, routine, (void *)"thread-1");
//     if(n != 0)
//     {
//         std::cout << "pthread_create error" << std::endl;
//     }

//     // while(true)
//     // {
//     //     std::cout << "I am main thread" << std::endl;
//     //     sleep(1);
//     // }

//     // 使用std::thread创建线程
//     // std::thread t([](){
//     //     while(true)
//     //     {
//     //         std::cout << "我是新线程，我的名字是： new thread" << std::endl;
//     //         std::cout << "新线程的ID是： " << std::this_thread::get_id() << std::endl;
//     //         sleep(1);
//     //     }
//     // });
    
//     while(true)
//     {
//         std::cout << "I am main thread" << std::endl;
//         sleep(1);
//     }
// }

// ------------------------------ 多线程 ---------------------------------
// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>
// #include <thread>

// std::string toHex(pthread_t tid)
// {
//     char buf[64];
//     snprintf(buf, sizeof(buf), "0x%lx", tid);
//     return std::string(buf);
// }
// // 被重入了！！！
// void *routine(void *arg)
// {
//     std::string thread_name = static_cast<const char *>(arg);

//     while(true)
//     {
//         std::cout << "我是新线程，我的名字是： " << thread_name << ", 我的ID是： " << toHex(pthread_self()) << std::endl;
//         sleep(1);
//     }
//     return nullptr;
// }

// int main()
// {
//     // 使用pthread_create创建线程
//     pthread_t tid1;
//     int n = pthread_create(&tid1, nullptr, routine, (void *)"thread-1");
//     if(n != 0)
//     {
//         std::cout << "pthread_create error" << std::endl;
//     }

//     pthread_t tid2;
//     n = pthread_create(&tid2, nullptr, routine, (void *)"thread-2");
//     if(n != 0)
//     {
//         std::cout << "pthread_create error" << std::endl;
//     }

//     pthread_t tid3;
//     n = pthread_create(&tid3, nullptr, routine, (void *)"thread-3");
//     if(n != 0)
//     {
//         std::cout << "pthread_create error" << std::endl;
//     }

//     pthread_t tid4;
//     n = pthread_create(&tid4, nullptr, routine, (void *)"thread-4");
//     if(n != 0)
//     {
//         std::cout << "pthread_create error" << std::endl;
//     }

//     while(true)
//     {
//         std::cout << "I am main thread" << std::endl;
//         sleep(1);
//     }
// }


// ------------------------------ 线程回收 ---------------------------------

// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>
// #include <thread>

// std::string toHex(pthread_t tid)
// {
//     char buf[64];
//     snprintf(buf, sizeof(buf), "0x%lx", tid);
//     return std::string(buf);
// }
// // 被重入了！！！
// void *routine(void *arg)
// {
//     std::string thread_name = static_cast<const char *>(arg);

//     while(true)
//     {
//         std::cout << "我是新线程，我的名字是： " << thread_name << ", 我的ID是： " << toHex(pthread_self()) << std::endl;
//         sleep(1);
//         break;
//     }
//     return nullptr;
// }

// int main(){
//     // 使用pthread_create创建线程
//     pthread_t tid1;
//     int n = pthread_create(&tid1, nullptr, routine, (void *)"thread-1");
//     if(n != 0)
//     {
//         std::cout << "pthread_create error" << std::endl;
//     }

//     // 等待线程tid1结束
//     n = pthread_join(tid1, nullptr);
//     if(n != 0)
//     {
//         std::cout << "pthread_join error" << std::endl;
//     }
//     std::cout << "线程tid1已回收" << std::endl;

//     return 0;
// }

// ------------------------------ 线程参数传递 ---------------------------------

// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>
// #include <thread>

// class ThreadTask
// {
// public:
//     ThreadTask(const std::string &name, int x, int y) : _name(name), _x(x), _y(y) {}

//     int run()
//     {
//         return _x + _y;
//     }

//     std::string Name() const { return _name; }
//     ~ThreadTask() {}

// private:
//     std::string _name;
//     int _x;
//     int _y;
// };

// std::string toHex(pthread_t tid)
// {
//     char buf[64];
//     snprintf(buf, sizeof(buf), "0x%lx", tid);
//     return std::string(buf);
// }
// // 被重入了！！！
// void *routine(void *arg)
// {
//     ThreadTask *Task = static_cast<ThreadTask *>(arg);

//     while(true)
//     {
//         std::cout << "我是新线程，我的名字是： " << Task->Name() << ", 我的ID是： " << toHex(pthread_self()) << std::endl;
//         sleep(1);
//         std::cout << "我是新线程，我的计算结果是： " << Task->run() << std::endl;
//         break;
//     }
//     return nullptr;
// }

// int main(){
//     // 使用pthread_create创建线程
//     pthread_t tid1;
//     ThreadTask *Task = new ThreadTask("thread-1", 10, 20);
//     int n = pthread_create(&tid1, nullptr, routine, (void *)Task);
//     if(n != 0)
//     {
//         std::cout << "pthread_create error" << std::endl;
//     }

//     // 等待线程tid1结束
//     n = pthread_join(tid1, nullptr);
//     if(n != 0)
//     {
//         std::cout << "pthread_join error" << std::endl;
//     }
//     std::cout << "线程tid1已回收" << std::endl;

//     return 0;
// }


// ------------------------------ 线程沟通 ---------------------------------

// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>
// #include <thread>

// class ThreadTask
// {
// public:
//     ThreadTask(const std::string &name, int x, int y) : _name(name), _x(x), _y(y) {}

//     void run()
//     {
//         _ret = _x + _y;
//     }

//     int Ret() const { return _ret; }
//     std::string Name() const { return _name; }
//     ~ThreadTask() {}

// private:
//     std::string _name;
//     int _x;
//     int _y;
//     int _ret;
// };

// std::string toHex(pthread_t tid)
// {
//     char buf[64];
//     snprintf(buf, sizeof(buf), "0x%lx", tid);
//     return std::string(buf);
// }

// void *routine(void *arg)
// {
//     ThreadTask *Task = static_cast<ThreadTask *>(arg);

//     while(true)
//     {
//         std::cout << "我是新线程，我的名字是： " << Task->Name() << ", 我的ID是： " << toHex(pthread_self()) << std::endl;
//         sleep(1);
//         Task->run();
//         std::cout << Task->Name() << "运算结束" << std::endl;
//         break;
//     }
//     return (void *)Task; 
// }

// int main(){
//     // 使用pthread_create创建线程
//     pthread_t tid1;
//     ThreadTask *Task1 = new ThreadTask("thread-1", 10, 20);
//     int n = pthread_create(&tid1, nullptr, routine, (void *)Task1);
//     if(n != 0)
//     {
//         std::cout << "pthread_create error" << std::endl;
//     }
//     pthread_t tid2;
//     ThreadTask *Task2 = new ThreadTask("thread-2", 100, 120);
//     n = pthread_create(&tid2, nullptr, routine, (void *)Task2);
//     if(n != 0)
//     {
//         std::cout << "pthread_create error" << std::endl;
//     }

//     // 等待线程tid1结束
//     ThreadTask *ret1; // 用于接收线程返回值
//     n = pthread_join(tid1, (void **)&ret1); // ret1获取线程返回值
//     if(n != 0)
//     {
//         std::cout << "pthread_join error" << std::endl;
//     }
//     std::cout << "线程tid1已回收, 结果为:  " << ret1->Ret() << std::endl;
//     // 等待线程tid2结束
//     n = pthread_join(tid2, nullptr); // Task2获取线程返回值
//     if(n != 0)
//     {
//         std::cout << "pthread_join error" << std::endl;
//     }
//     std::cout << "线程tid2已回收, 结果为:  " << Task2->Ret() << std::endl; // 为什么不把ret1和Task2定义成全局变量？
//     // 答：在流程上，当线程没有退出时，意为着线程的任务没有结束，获取不到线程的返回值，也自然不会使用，在此期间，pthread_join会阻塞。
//     // 因此，当你在pthread_join之后使用ret1或者Task2时，意味着线程已经结束了，你可以安全地使用它们。不必使用死循环来一直检测线程是否结束，返回值是否被修改。
//     // 因此，我们不必特殊处理ret1和Task2，直接使用即可。在使用上，大大简化了代码，也不需要程序员额外操心。
//     // 等待线程tid3结束

//     return 0;
// }

// ------------------------------ 线程退出 ---------------------------------
// #include <typeindex>
// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>
// #include <thread>

// class ThreadTask
// {
// public:
//     ThreadTask(const std::string &name, int x, int y) : _name(name), _x(x), _y(y) {}

//     void run()
//     {
//         _ret = _x + _y;
//     }

//     int Ret() const { return _ret; }
//     std::string Name() const { return _name; }
//     ~ThreadTask() {}

// private:
//     std::string _name;
//     int _x;
//     int _y;
//     int _ret;
// };

// std::string toHex(pthread_t tid)
// {
//     char buf[64];
//     snprintf(buf, sizeof(buf), "0x%lx", tid);
//     return std::string(buf);
// }

// void *routine3(void *arg)
// {
//     ThreadTask *Task = static_cast<ThreadTask *>(arg);

//     while(true)
//     {
//         // std::cout << "我是新线程，我的名字是： " << Task->Name() << ", 我的ID是： " << toHex(pthread_self()) << std::endl;
//         sleep(3);
//         Task->run();
//         // std::cout << Task->Name() << "运算结束" << std::endl;
//         break;
//     }
//     // pthread_exit((void*)100); // 线程退出方式2: 线程通过pthread_exit退出
//     return (void *)10; // 线程退出方式1: 线程的入口函数通过return退出
// }
// void *routine2(void *arg)
// {
//     ThreadTask *Task = static_cast<ThreadTask *>(arg);

//     while(true)
//     {
//         // std::cout << "我是新线程，我的名字是： " << Task->Name() << ", 我的ID是： " << toHex(pthread_self()) << std::endl;
//         sleep(3);
//         Task->run();
//         // std::cout << Task->Name() << "运算结束" << std::endl;
//         break;
//     }
//     pthread_exit((void*)100); // 线程退出方式2: 线程通过pthread_exit退出
//     return 0;
// }
// void *routine1(void *arg)
// {
//     ThreadTask *Task = static_cast<ThreadTask *>(arg);
//     while(true)
//     {
//         std::cout << "我是新线程，我的名字是： " << Task->Name() << std::endl;
//         sleep(10);
//         // break;
//     }
// }
// int main(){
//     // 使用pthread_create创建线程
//     pthread_t tid1;
//     ThreadTask *Task1 = new ThreadTask("thread-1", 10, 20);
//     int n = pthread_create(&tid1, nullptr, routine1, (void *)Task1);
//     pthread_t tid2;
//     ThreadTask *Task2 = new ThreadTask("thread-2", 10, 20);
//     n = pthread_create(&tid2, nullptr, routine2, (void *)Task2);
//     pthread_t tid3;
//     ThreadTask *Task3 = new ThreadTask("thread-3", 10, 20);
//     n = pthread_create(&tid3, nullptr, routine3, (void *)Task3);
    
//     sleep(5);
//     pthread_cancel(tid1); 
//     // 等待线程tid1结束
//     void *ret; // 用于接收线程返回值
//     pthread_join(tid1, &ret);
//     std::cout << "线程tid1已回收, 结果为:  " << Task1->Ret() << ", 返回值为: " << (long long)ret << std::endl; // 线程退出方式3: 主线程通过pthread_cancel退出（不推荐）
//     // 等待线程tid2结束
//     pthread_join(tid2, &ret); 
//     std::cout << "线程tid2已回收, 结果为:  " << Task2->Ret() << ", 返回值为: " << (long long)ret << std::endl;
//     // 等待线程tid3结束
//     pthread_join(tid3, &ret); 
//     std::cout << "线程tid3已回收, 结果为:  " << Task3->Ret() << ", 返回值为: " << (long long)ret << std::endl;



//     return 0;
// }

// ------------------------------ 线程分离 ---------------------------------

#include <typeindex>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <thread>

class ThreadTask
{
public:
    ThreadTask(const std::string &name, int x, int y) : _name(name), _x(x), _y(y) {}

    void run()
    {
        _ret = _x + _y;
    }

    int Ret() const { return _ret; }
    std::string Name() const { return _name; }
    ~ThreadTask() {}

private:
    std::string _name;
    int _x;
    int _y;
    int _ret;
};

std::string toHex(pthread_t tid)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "0x%lx", tid);
    return std::string(buf);
}

void *routine1(void *arg)
{
    // pthread_detach(pthread_self());
    ThreadTask *Task = static_cast<ThreadTask *>(arg);

    while(true)
    {
        std::cout << "我是新线程，我的名字是： " << Task->Name() << ", 我的ID是： " << toHex(pthread_self()) << std::endl;
        Task->run();
        sleep(1);
        // std::cout << Task->Name() << "运算结束" << std::endl;
        // break;
    }
    // return (void *)10; 
}

int main(){
    pthread_t tid1;
    ThreadTask *Task1 = new ThreadTask("thread-1", 10, 20);
    int n = pthread_create(&tid1, nullptr, routine1, (void *)Task1);
    // pthread_detach(tid1);


    sleep(3);
    // pthread_cancel(tid1); 
    // 等待线程tid1结束
    void *ret; // 用于接收线程返回值
    n = pthread_join(tid1, &ret);
    std::cout << "线程tid1已回收, 结果为:  " << Task1->Ret() << ", 返回值为: " << (long long)ret;
    std::cout << ", n: " << n << std::endl; 

    return 0;
}

