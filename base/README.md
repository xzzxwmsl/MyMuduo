# lock.h

## Condition

如果一个类要同时使用条件锁和互斥锁，那么需要注意声明顺序应该是：互斥锁先于条件锁（条件锁需要使用互斥锁）,比如

```cpp
class Counter
{
private:
    mymuduo::MutexLock mutex;
    mymuduo::Condition cond;
    int cnt;

public:
    Counter(int c) : mutex(), cond(mutex), cnt(c) {}
};
```  
