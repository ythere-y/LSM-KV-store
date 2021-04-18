#ifndef MEMTABLE_H
#define MEMTABLE_H
#include <string>
#include <vector>
#include <algorithm>
#include "bloomfilter.h"
#include "skip_list_mem.h"


class MemTable
{
//private:
public:
    Skip_List_mem *sl = new Skip_List_mem;       //索引区，用跳表实现
    uint32_t memory_remaining;
    uint32_t memory_usage;
    uint32_t len;

public:
    MemTable();
    MemTable(uint32_t size);
    ~MemTable();
    int insert(long long key, const std::string &s);
    std::string search(long long key);
    bool remove(long long key);
    uint32_t size(){return len;}
    void reset();
};

#endif // MEMTABLE_H
