#include "memtable.h"

MemTable::MemTable():memory_remaining(0),memory_usage(0),len(0){}
MemTable::MemTable(uint32_t size):memory_remaining(size),memory_usage(0),len(0){}

int MemTable::insert(uint64_t key, const std::string &s){
    uint32_t length = s.size();
    if (length + 1 > memory_remaining)
        return -1;
    // 空间不足，插入失败
    sl->insertNode(key,s);
    len ++;
    memory_remaining -= (length+1);
    memory_usage += (length + 1);
    return 1;
}

std::string MemTable::search(uint64_t key){
    SKNode_mem* get = sl->searchNode(key);
    return *get->offset;
}

bool MemTable::remove(uint64_t key){
    uint32_t length = sl->deleteNode(key);
    if (length){
        len--;
        memory_remaining += length;
        memory_usage -= length;
        return true;
    }
    return false;
}

