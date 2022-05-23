#ifndef BASE_NONCOPYABLE_H
#define BASE_NONCOPYABLE_H
// 定义了禁止对象的拷贝
class noncopyable
{
private:
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;

public:
    noncopyable() = default;
    ~noncopyable() = default;
};
#endif