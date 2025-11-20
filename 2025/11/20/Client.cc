#include "gfifo.hpp"
#include "shmMemory.hpp"
int main()
{
    // key_t key = ftok(SHMPATH, SHMKEY);
    // int shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666);
    // if (shmid < 0)
    // {
    //     std::cerr << "shmget error" << std::endl;
    //     exit(-1);
    // }
    // else
    // {
    //     std::cout << "shmget success" << std::endl;
    // }
    gshm shmmemory;
    shmmemory.OpenShm(0666);

    // char *shmaddr = (char *)shmat(shmid, nullptr, 0);
    // if (shmaddr == (char *)-1)
    // {
    //     std::cerr << "shmat error" << std::endl;
    // }
    // else
    // {
    //     std::cout << "shmat success" << std::endl;
    // }
    shmmemory.AttachShm();

    gfifo fifoToWrite("./fifoServer");
    fifoToWrite.gfifoOpen(O_WRONLY);

    std::string buf;
    std::string tmp;
    // srand(time(NULL));
    while (true)
    {
        std::cout << "Please input sonething: ";
        getline(std::cin, buf);
        // strcpy(shmaddr, buf.c_str());
        strcpy((char*)shmmemory.GetAddr(), buf.c_str());
        fifoToWrite.gfifoWrite("1");
    }

    fifoToWrite.gfifoClose();

    // shmdt(shmaddr);
    shmmemory.DetachShm();
    // int n = shmctl(shmid, IPC_RMID, nullptr);
    // if (n < 0)
    // {
    //     std::cerr << "shmctl error" << std::endl;
    // }
    // shmaddr = nullptr;
    shmmemory.DeleteShm();

    return 0;
}