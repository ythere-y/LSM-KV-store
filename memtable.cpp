#include "memtable.h"

MemTable::MemTable()
{

}

int MemTable::insert(uint64_t key, const std::string &s){

    return 1;
}

std::string MemTable::search(uint64_t key){

        return "";

}

bool MemTable::remove(uint64_t key){
    return false;   //删除失败

}

