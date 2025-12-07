#include "gfifo.hpp"
#include "shmMemory.hpp"
int main()
{
    // key_t key = ftok(SHMPATH, SHMKEY);
    // int shmid = shmget(key, MAXSIZE, IPC_CREAT | IPC_EXCL | 0666);
    // if (shmid < 0)
    // {
    //     std::cerr << "shmget error" << std::endl;
    // }else{
    //     std::cout << "shmget success" << std::endl;
    // }
    gshm shmmemory;
    shmmemory.CreatShm(0666);

    // char *shmaddr = (char *)shmat(shmid, nullptr, 0);
    // if (shmaddr == (char *)-1)
    // {
    //     std::cerr << "shmat error" << std::endl;
    // }else{
    //     std::cout << "shmat success" << std::endl;
    // }
    shmmemory.AttachShm();

    gfifo fifoToRead("./fifoServer");
    fifoToRead.gfifoCreate(gmode);
    fifoToRead.gfifoOpen(O_RDONLY);

    std::string tmp;

    while (true)
    {
        fifoToRead.gfifoRead(&tmp);
        if (tmp.size() == 0)
        {
            std::cout << "Client close !!! " << std::endl;
            break;
        }

        // std::cout << "shmid: " << shmid << ", 大小为: "<< strlen(shmaddr) <<", 内容为: " << shmaddr << std::endl;
        std::cout << "shmid: " << shmmemory.GetShmId()
                << ", 大小为: " << strlen((char *)shmmemory.GetAddr()) 
                << ", 内容为: " << shmmemory.GetAddr() << std::endl;
        sleep(1);
    }

    fifoToRead.gfifoUnlink();

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