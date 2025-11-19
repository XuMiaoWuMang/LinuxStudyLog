#include "gfifo.hpp"

int main()
{
    // 1. 打开命名管道写入数据
    // int fd = open(gfifoName.c_str(), O_WRONLY);
    // if (fd == -1) {
    //     std::cerr << "Failed to open fifo" << std::endl;
    // }
    // else{
    //     std::cout << "Fifo opened successfully" << std::endl;
    // }
    myfifo.gfifoOpen(O_WRONLY);
    std::string buf;
    while (true)
    {
        getline(std::cin, buf);
        // int n = write(fd, buf.c_str(), sizeof(buf));
        // if (n == -1) {
        //     std::cerr << "Failed to write from fifo" << std::endl;
        //     break;
        // }
        // else if (n == 0) {
        //     std::cout << "Fifo closed" << std::endl;
        //     break;
        // }
        // else {
        //     std::cout << "write " << n << " bytes from fifo: " << buf << std::endl;
        // }
        int n = myfifo.gfifoWrite(buf.c_str());
        if (n == -1)
        {
            std::cerr << "Failed to write from fifo" << std::endl;
            exit(-1);
        }
        std::cout << "write " << n << " bytes from fifo: " << buf << std::endl;
    }

    // 2. 关闭命名管道
    myfifo.gfifoClose();
    return 0;
}