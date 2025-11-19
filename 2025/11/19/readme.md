完成不同程序通过命名管道进行通讯的功能，并将功能集成。要求如下：

1. 创建一个命名管道，命名为“./myfifo”，用于不同程序之间的通信。
2. 编写两个程序，Client程序作为写端，向命名管道中写入数据；Server程序作为读端，从命名管道中读取数据。

编译Client和Server程序。
```bash
make
```
清理编译生成的文件。
```bash
make clean
```
若Server程序异常退出，请一定使用`unlink fifo`清理fifo文件，否则Server程序会因为无法正常启动而异常退出。