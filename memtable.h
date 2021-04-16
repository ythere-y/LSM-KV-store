#ifndef MEMTABLE_H
#define MEMTABLE_H
#include <string>
#include <vector>
#include <algorithm>
#include "bloomfilter.h"
#include "skip_list.h"


struct Header{
    unsigned long long time_stamp;
    unsigned long long key_nums;
    unsigned long long key_min;
    unsigned long long key_max;
    Header(unsigned long long const _time){time_stamp = _time;}
    int Add(uint64_t key){
        key_max = std::max(key_max,key);
        key_min = std::min(key_min,key);
        key_nums++;
        return 1;
    }
};

class MemTable
{
private:
    Header *head;        //用于存放元数据
    BloomFilter *bf;     //布隆过滤器
    Skip_List *sl;       //索引区，用跳表实现
    std::vector<std::string> *vals;  //数据区，用于存储string

public:
    MemTable();
    ~MemTable();
    int insert(uint64_t key, const std::string &s);
    std::string search(uint64_t key);
    bool remove(uint64_t key);
    void reset();
};

#endif // MEMTABLE_H
