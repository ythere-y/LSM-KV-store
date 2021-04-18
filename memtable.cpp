#include "memtable.h"

MemTable::MemTable()
{

}

int MemTable::insert(uint64_t key, const std::string &s){

    sl->insertNode(key,s);
    return 1;
}

std::string MemTable::search(uint64_t key){
    SKNode_mem* get = sl->searchNode(key);
    return *get->offset;
}

bool MemTable::remove(uint64_t key){
    return sl->deleteNode(key);
}

