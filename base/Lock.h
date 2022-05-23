#ifndef BASE_LOCK_H
#define BASE_LOCK_H
#include "noncopyable.h"
#include "pthread.h"
#include "unistd.h"
#include "sys/syscall.h"
#include "assert.h"
#include "stdio.h"
namespace mymuduo
{
    class MutexLock : noncopyable
    {
    private:
        friend class MutexLockGuard;

        pthread_mutex_t mutex;
        pid_t holder;
        // 只能被MutexLockGuard调用
        void lock()
        {
            pthread_mutex_lock(&mutex);
            holder = ((long int)syscall(224));
        }
        // 只能被MutexLockGuard调用
        void unlock()
        {
            holder = 0;
            pthread_mutex_unlock(&mutex);
        }

    public:
        MutexLock() : holder(0) { pthread_mutex_init(&mutex, NULL); }
        ~MutexLock()
        {
            // 保证被解锁了
            assert(holder == 0);
            pthread_mutex_destroy(&mutex);
        }
        // 保证是在本线程内被调用
        bool isLockedByThisThread()
        {
            return holder == ((long int)syscall(224));
        }

        bool assertLocked()
        {
            assert(isLockedByThisThread());
        }
    };

    class MutexLockGuard
    {
    private:
        MutexLock &mutex;

    public:
        explicit MutexLockGuard(MutexLock &m) : mutex(m)
        {
            // printf("lock\n");
            mutex.lock();
        }
        ~MutexLockGuard()
        {
            // printf("unlock\n");
            mutex.unlock();
        }
    };

}

/* 防止出现如此用法：
MutexLockGuard(mutex);
遗漏了变量名，造成生成临时对象，然后马上释放，没有锁住临界区
*/ 
#define MutexLockGuard(x) static_assert(fasle, "missing mutex guard name")
#endif