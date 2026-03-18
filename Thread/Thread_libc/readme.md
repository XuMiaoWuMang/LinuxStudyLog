# 模拟实现线程库文档

## 1. 项目概述

本项目基于 pthread 库，实现了一个面向对象的线程封装库，提供了更加简洁、易用的线程操作接口。通过 C++ 的面向对象特性，将线程的创建、启动、停止、加入和退出等操作封装在 `Thread` 类中，简化了线程编程的复杂度。

## 2. 核心设计

### 2.1 命名空间与类型定义

```cpp
namespace ThreadModule{
    using func_t = std::function<void()>;
    static int num_threads = 0;
    enum class TSTATUS{
        NEW,
        RUNNING,
        STOP
    };
    // ...
}
```

- **命名空间**：使用 `ThreadModule` 命名空间避免命名冲突
- **函数类型**：使用 `std::function<void()>` 作为线程函数类型，支持各种可调用对象
- **线程计数器**：静态变量 `num_threads` 用于自动生成线程名称
- **线程状态**：使用枚举类 `TSTATUS` 定义线程的三种状态：`NEW`（新建）、`RUNNING`（运行中）、`STOP`（已停止）

### 2.2 Thread 类设计

#### 2.2.1 私有成员变量

| 成员变量 | 类型 | 说明 |
|---------|------|------|
| `_tid` | `pthread_t` | 线程 ID |
| `_pid` | `pid_t` | 进程 ID |
| `_name` | `std::string` | 线程名称 |
| `_joinable` | `bool` | 线程是否可 join |
| `_func` | `func_t` | 线程执行函数 |
| `_status` | `TSTATUS` | 线程状态 |

#### 2.2.2 构造函数

```cpp
Thread(func_t func):
    _joinable(true),
    _func(func),
    _status(TSTATUS::NEW)
{
    _pid = getpid();
    _tid = pthread_self();
    _name = "Thread-" + std::to_string(num_threads++);
}
```

- **参数**：接受一个 `func_t` 类型的可调用对象
- **初始化**：
  - 设置 `_joinable` 为 `true`
  - 保存线程函数 `_func`
  - 初始状态为 `TSTATUS::NEW`
  - 获取当前进程 ID
  - 获取当前线程 ID
  - 生成线程名称（格式：Thread-数字）

#### 2.2.3 核心方法

| 方法 | 说明 | 实现细节 |
|------|------|---------
| `start()` | 启动线程 | 检查状态是否为 `NEW`，调用 `pthread_create` 创建线程，更新状态为 `RUNNING` |
| `join()` | 等待线程结束 | 检查线程是否可 join，调用 `pthread_join` 等待线程结束，更新状态为 `STOP` |
| `stop()` | 停止线程 | 检查状态是否为 `RUNNING`，调用 `pthread_cancel` 取消线程，更新状态为 `STOP` |
| `detach()` | 分离线程 | 检查线程是否可 join，调用 `pthread_detach` 分离线程，设置 `_joinable` 为 `false` |
| `isJoinable()` | 检查线程是否可 join | 返回 `_joinable` 的值 |
| `getName()` | 获取线程名称 | 返回 `_name` 的值 |

#### 2.2.4 静态运行方法

```cpp
static void *Runtime(void *arg)
{
    Thread *thread = static_cast<Thread *>(arg);
    thread->_func();
    return nullptr;
}
```

- **作用**：作为 pthread 的回调函数，用于执行用户指定的线程函数
- **实现**：将 `void*` 类型的参数转换为 `Thread*`，然后调用线程的 `_func()` 方法

## 3. 实现细节

### 3.1 线程创建与启动

1. **线程创建**：通过 `start()` 方法调用 `pthread_create` 函数创建线程
2. **参数传递**：将 `this` 指针作为参数传递给 `Runtime` 静态方法
3. **状态管理**：创建成功后将状态更新为 `RUNNING`，失败则恢复为 `NEW`

### 3.2 线程停止与退出

1. **线程停止**：通过 `stop()` 方法调用 `pthread_cancel` 函数取消线程
2. **线程等待**：通过 `join()` 方法调用 `pthread_join` 函数等待线程结束
3. **状态更新**：线程停止或结束后将状态更新为 `STOP`

### 3.3 线程分离

通过 `detach()` 方法调用 `pthread_detach` 函数将线程设置为分离状态，这样线程结束后会自动释放资源，不需要调用 `join()` 方法。

## 4. 使用示例

### 4.1 基本使用

```cpp
#include "Thread.hpp"

int main()
{
    Thread::Thread threads([](){
        while(true)
        {
            std::cout << "hello world" << std::endl;
            sleep(1);
        }
    });
    threads.start();
    std::cout << "新线程“" << threads.getName() << "”启动成功" << std::endl;
    sleep(3);
    threads.stop();
    std::cout << "新线程“" << threads.getName() << "”停止成功" << std::endl;
    sleep(3);
    threads.join();
    std::cout << "新线程“" << threads.getName() << "”退出成功" << std::endl;
    return 0;
}
```

### 4.2 运行流程

1. **创建线程**：使用 lambda 表达式创建一个线程对象
2. **启动线程**：调用 `start()` 方法启动线程，线程开始执行 lambda 表达式中的代码
3. **停止线程**：3 秒后调用 `stop()` 方法停止线程
4. **等待线程**：再等待 3 秒后调用 `join()` 方法等待线程结束
5. **程序退出**：主线程退出

## 5. 技术特点与优势

### 5.1 面向对象设计

- **封装性**：将 pthread 库的复杂操作封装在 `Thread` 类中
- **易用性**：提供简洁的方法接口，如 `start()`、`join()`、`stop()` 等
- **安全性**：通过状态管理和参数检查，提高线程操作的安全性

### 5.2 功能完整性

- **线程创建**：支持通过可调用对象创建线程
- **线程控制**：支持启动、停止、等待和分离操作
- **状态管理**：通过枚举类型管理线程的不同状态
- **线程命名**：自动生成线程名称，方便调试和识别

### 5.3 扩展性

- **可调用对象支持**：使用 `std::function<void()>` 作为线程函数类型，支持函数、函数对象、lambda 表达式等
- **状态扩展**：可以根据需要扩展线程状态
- **方法扩展**：可以根据需要添加更多线程操作方法

## 6. 代码优化建议

1. **异常处理**：
   - 添加异常处理机制，捕获 pthread 函数可能抛出的异常
   - 提供更加友好的错误信息

2. **资源管理**：
   - 考虑使用 RAII 原则管理线程资源
   - 确保线程在析构时能够正确清理

3. **线程池**：
   - 可以考虑实现线程池功能，提高线程的复用性
   - 支持任务队列和线程数量控制

4. **线程局部存储**：
   - 添加线程局部存储支持，方便线程间数据隔离

5. **线程同步**：
   - 提供内置的互斥锁、条件变量等同步原语
   - 支持更加复杂的线程同步场景

## 7. 总结

本项目成功实现了一个基于 pthread 库的面向对象线程封装库，通过简洁的接口和完整的功能，大大简化了线程编程的复杂度。该实现具有以下特点：

1. **简洁易用**：提供了直观的方法接口，如 `start()`、`join()`、`stop()` 等
2. **功能完整**：支持线程的创建、启动、停止、等待和分离等操作
3. **状态管理**：通过枚举类型清晰地管理线程的不同状态
4. **面向对象**：利用 C++ 的面向对象特性，提供了更加优雅的线程编程方式

该线程库可以作为学习线程编程的工具，也可以在实际项目中使用，为多线程编程提供了便利。

## 8. 输入输出示例

#### 输入输出示例
输入：
```cpp
Thread::Thread threads([](){
    while(true)
    {
        std::cout << "hello world" << std::endl;
        sleep(1);
    }
});
threads.start();
std::cout << "新线程“" << threads.getName() << "”启动成功" << std::endl;
sleep(3);
threads.stop();
std::cout << "新线程“" << threads.getName() << "”停止成功" << std::endl;
sleep(3);
threads.join();
std::cout << "新线程“" << threads.getName() << "”退出成功" << std::endl;
```

输出：
```
新线程“Thread-0”启动成功
hello world
hello world
hello world
新线程“Thread-0”停止成功
新线程“Thread-0”退出成功
```

## 9. 结论

本模拟实现的线程库通过面向对象的设计，成功封装了 pthread 库的功能，提供了更加简洁、易用的线程操作接口。它不仅可以作为学习线程编程的工具，也可以在实际项目中使用，为多线程编程提供了便利。

通过本项目的实现，我们可以看到：
1. 面向对象设计可以有效地封装底层库的复杂性
2. 状态管理对于线程操作的重要性
3. 如何通过 C++ 的特性（如 `std::function`、lambda 表达式等）提高代码的可读性和易用性

该实现为我们理解线程的工作原理和使用方法提供了一个很好的参考，也为进一步扩展和优化线程库打下了基础。