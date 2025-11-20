#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <stdio.h>

const std::string gfifoName = "./fifo";
const int gmode = 0600;
const int gsize = 1024;
/**
 * @brief gfifo类，用于实现命名管道(FIFO)的创建、打开、读写、关闭等操作
 */
class gfifo
{
public:
    /**
     * @brief 构造函数，使用默认的管道名称
     */
    gfifo() : fifoname(gfifoName) {}
    /**
     * @brief 构造函数，使用指定的管道名称
     * @param name 管道名称
     */
    gfifo(const char *name) : fifoname(name) {}
    /**
     * @brief 创建命名管道
     * @param mode 管道的创建权限模式
     */
    void gfifoCreate(int mode)
    {
        // create fifo
        int fd = mkfifo(fifoname.c_str(), mode);
        if (fd == -1)
        {
            std::cerr << "mkfifo error" << std::endl;
            exit(1);
        }
        else
        {
            std::cout << "mkfifo success" << std::endl;
        }
    }
    /**
     * @brief 打开已存在的命名管道
     * @param flag 打开标志，如O_RDONLY, O_WRONLY等
     */
    void gfifoOpen(int flag)
    {
        // open fifo
        _fd = open(fifoname.c_str(), flag);
        if (_fd == -1)
        {
            std::cerr << "open fifo error" << std::endl;
            exit(1);
        }
        else
        {
            std::cout << "open fifo success" << std::endl;
        }
    }
    /**
     * @brief 向管道写入数据
     * @param buf 要写入的数据字符串
     * @return 返回实际写入的字节数
     */
    int gfifoWrite(const std::string &buf)
    {
        return write(_fd, buf.c_str(), buf.size());
    }
    /**
     * @brief 从管道读取数据
     * @param buf 用于存储读取数据的字符串指针
     * @return 返回实际读取的字节数
     */
    /**
     * 从全局FIFO读取数据
     * @param buf 用于存储读取数据的字符串指针
     * @return 返回实际读取的字节数，如果出错则程序退出并返回-1
     */
    int gfifoRead(std::string *buf)
    {
        char message[gsize];               // 定义临时字符数组用于存储读取的数据
        int n = read(_fd, message, gsize); // 从文件描述符_fd读取最多gsize字节的数据到message中
        if (n == -1)                       // 如果读取失败
        {
            std::cerr << "read error" << std::endl; // 输出错误信息
            exit(-1);                               // 退出程序，返回-1
        }
        else // 读取成功
        {
            message[n] = '\0'; // 在读取的数据末尾添加字符串结束符
            *buf = message;    // 将读取的数据赋值给传入的字符串指针
        }

        return n; // 返回读取的字节数
    }
    /**
     * @brief 关闭管道并删除命名管道文件
     */
    void gfifoClose()
    {
        close(_fd); // 关闭管道
    }
    void gfifoUnlink()
    {
        gfifoClose();
        int ret = unlink(fifoname.c_str()); // 删除命名管道文件
        if (ret == -1)
        {
            std::cerr << "unlink error" << std::endl;
            exit(-1);
        }
        else
        {
            std::cout << "unlink success" << std::endl;
        }
    }

private:
    std::string fifoname; // 命名管道名称
    int _fd;              // 文件描述符
};

gfifo myfifo;