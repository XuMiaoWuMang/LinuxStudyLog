#ifndef __THREAD_HPP__
#define __THREAD_HPP__
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <functional>
namespace ThreadModule{
    using func_t = std::function<void()>;
    static int num_threads = 0;
    enum class TSTATUS{
        NEW,
        RUNNING,
        STOP
    };
    class Thread{

        static void *Runtime(void *arg)
        {
            Thread *thread = static_cast<Thread *>(arg);
            thread->_func();
            return nullptr;
        }
    public:
        Thread(func_t func):
            _joinable(true),
            _func(func),
            _status(TSTATUS::NEW)
        {
            _pid = getpid();
            _tid = pthread_self();
            _name = "Thread-" + std::to_string(num_threads++);
        }

        bool start()
        {
            if(_status != TSTATUS::NEW)
            {
                return false;
            }
            _status = TSTATUS::RUNNING;
            int ret = ::pthread_create(&_tid, nullptr, Runtime, this);
            if(ret != 0)
            {
                _status = TSTATUS::NEW;
                return false;
            }
            return true;
        }

        bool join()
        {
            if(_joinable)
            {
                int ret = ::pthread_join(_tid, nullptr);
                if(ret != 0)
                {
                    return false;
                }
                _status = TSTATUS::STOP;
                return true;
            }
            
            return false;
        }

        bool stop()
        {
            if(_status == TSTATUS::RUNNING)
            {
                ::pthread_cancel(_tid);
                _status = TSTATUS::STOP;
                return true;
            }
            return false;
        }

        bool detach()
        {
            if(_joinable)
            {
                int ret = ::pthread_detach(_tid);
                if(ret != 0)
                {
                    return false;
                }
                _joinable = false;
                return true;
            }
            return false;
        }
        bool isJoinable() const
        {
            return _joinable;
        }

        std::string getName() const
        {
            return _name;
        }
        ~Thread() = default;
    private:
        pthread_t _tid;
        pid_t _pid;
        std::string _name;
        bool _joinable;
        func_t _func;
        TSTATUS _status;
    };
}



#endif
