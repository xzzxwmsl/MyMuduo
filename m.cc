#include "pthread.h"
#include "stdio.h"
#include "unistd.h"
#include "base/Lock.h"
mymuduo::MutexLock lock;

void *f(void *arg)
{
    int i = *(int *)(arg);
    {
        mymuduo::MutexLockGuard mlg(lock);
        printf("Hello %d\n", i);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t p, q;
    pthread_create(&p, NULL, f, new int(-1));
    pthread_create(&q, NULL, f, new int(7));
    pthread_join(p, NULL);
    pthread_join(q, NULL);
    printf("OVER\n");
}