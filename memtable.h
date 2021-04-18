#ifndef MEMTABLE_H
#define MEMTABLE_H
#include <string>
#include <vector>
#include <algorithm>
#include "bloomfilter.h"
#include "skip_list_mem.h"


class MemTable
{
private:
    Skip_List_mem *sl;       //索引区，用跳表实现

public:
    MemTable();
    ~MemTable();
    int insert(uint64_t key, const std::string &s);
    std::string search(uint64_t key);
    bool remove(uint64_t key);
    void reset();
};

#endif // MEMTABLE_H
