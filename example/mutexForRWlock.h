#include "../base/Lock.h"
#include "stdio.h"
#include "map"
#include "vector"
#include "memory"
class CustomerData
{
private:
    using Entry = std::pair<std::string, int>;    // stock-时间
    using EntryList = std::vector<Entry>;         // 不同stock的最小交易间隔,已经排好序
    using Map = std::map<std::string, EntryList>; // 用户名-EntryList
    using MapPtr = std::shared_ptr<Map>;

    void update(const std::string &customer, const EntryList &ent);
    // 在EntryList中寻找股票
    static int findEntry(const EntryList &en, const std::string &stock);

    // 获取数据,对应读锁
    MapPtr getData()
    {
        mymuduo::MutexLockGuard lock(mutex);
        // 这里返回一个shared_ptr副本,在读的话,那么当前shared_ptr的use_count大于1
        return data;
    }
    mutable mymuduo::MutexLock mutex;
    MapPtr data;

public:
    CustomerData() : data(new Map), mutex() {}

    int query(const std::string &customer, const std::string &stock);
};

int CustomerData::query(const std::string &customer, const std::string &stock)
{
    MapPtr temp = getData();
    Map::const_iterator it = temp->find(customer);
    return (it != temp->end()) ? findEntry(it->second, stock) : -1;
}

void CustomerData::update(const std::string &customer, const EntryList &ent)
{
    mymuduo::MutexLockGuard lock(mutex);
    if (!data.unique())
    {
        MapPtr newData = std::make_shared<Map>(*data);
        data.swap(newData);
    }
    assert(data.unique());
    (*data)[customer] = ent;
}