#pragma once
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <iostream>
#include <string>
#include <cstring>

#define MAXSIZE 4096
#define SHMPATH "."
#define SHMKEY 9527
class gshm
{

public:
    gshm() : _key(0), _shmid(-1), _addr(nullptr)
    {
    }
    void CreatShm(int mode)
    {
        _key = ftok(SHMPATH, SHMKEY);
        if (_key < 0)
        {
            std::cerr << "ftok error" << std::endl;
            return;
        }
        _shmid = shmget(_key, MAXSIZE, IPC_CREAT | IPC_EXCL | mode);
        if (_shmid < 0)
        {
            std::cerr << "shmget error" << std::endl;
            return;
        }
        std::cout << "shmid: " << _shmid << std::endl;
    }
    void OpenShm(int mode)
    {
        _key = ftok(SHMPATH, SHMKEY);
        if (_key < 0)
        {
            std::cerr << "ftok error" << std::endl;
            return;
        }
        _shmid = shmget(_key, MAXSIZE, IPC_CREAT | mode);
        if (_shmid < 0)
        {
            std::cerr << "shmget error" << std::endl;
            return;
        }
        std::cout << "shmid: " << _shmid << std::endl;
    }
    void AttachShm()
    {
        _addr = (char *)shmat(_shmid, nullptr, 0);
        std::cout << "attach done: " << std::endl;
    }
    void DetachShm()
    {
        if (_addr != nullptr)
            ::shmdt(_addr);
        std::cout << "detach done: " << std::endl;
    }
    void DeleteShm()
    {
        shmctl(_shmid, IPC_RMID, nullptr);
    }
    
    void *GetAddr()
    {
        return _addr;
    }
    int GetShmId()
    {
        return _shmid;
    }
    ~gshm()
    {
    }

private:
    key_t _key;
    int _shmid;
    char *_addr;
};