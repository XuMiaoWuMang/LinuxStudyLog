#include "gfifo.hpp"
int main()
{
    // 1. 创建命名管道
    // int n = mkfifo(gfifoName.c_str(), gmode);
    // if (n == -1) {
    //     std::cerr << "Failed to create fifo" << std::endl;
    //     return 1;
    // }
    // else{
    //     std::cout << "Fifo created successfully" << std::endl;
    // }
    myfifo.gfifoCreate(gmode);

    // 2. 打开命名管道读取数据
    // int fd = open(gfifoName.c_str(), O_RDONLY);
    // if (fd == -1) {
    //     std::cerr << "Failed to open fifo" << std::endl;
    // }
    // else{
    //     std::cout << "Fifo opened successfully" << std::endl;
    // }
    myfifo.gfifoOpen(O_RDONLY);

    std::string buf;
    while (true)
    {
        // int n = read(fd, buf, gsize);
        // if (n == -1) {
        //     std::cerr << "Failed to read from fifo" << std::endl;
        //     break;
        // }
        // else if (n == 0) {
        //     std::cout << "Fifo closed" << std::endl;
        //     break;
        // }
        // else {
        //     buf[n] = '\0';
        //     std::cout << "Read " << n << " bytes from fifo: " << buf << std::endl;
        // }
        int n = myfifo.gfifoRead(&buf);
        if (n == 0)
        {
                std::cout << "fifo is empty" << std::endl;

            break;
        }
        else
        {
            std::cout << "read " << n << " bytes from fifo: " << buf << std::endl;
        }
        sleep(1);
    }

    // 3. 关闭命名管道
    // unlink(gfifoName.c_str());
    myfifo.gfifoClose();
    return 0;
}