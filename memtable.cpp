#include "memtable.h"
const std::string D_str = "~DELETED~";

MemTable::MemTable():memory_remaining(0),memory_usage(0),len(0){}
MemTable::MemTable(uint32_t size):memory_remaining(size),memory_usage(0),len(0){}

int MemTable::insert(long long  key, const std::string &s){
    uint32_t length = s.size();
    if (length + 1 > memory_remaining)
        return -1;
    // 空间不足，插入失败,返回-1
    sl->insertNode(key,s);
    len ++;
    memory_remaining -= (length+1);
    memory_usage += (length + 1);
    return 1;
}

std::string MemTable::search(long long  key){
    SKNode_mem* get = sl->searchNode(key);
    if (get == nullptr || *get->offset == D_str )
        return "";
    return *get->offset;
}

int MemTable::remove(long long  key){
    uint32_t length = sl->deleteNode(key);
    if (length){//在这个里面删除成功，维护长度
        len--;
        memory_remaining += length;
        memory_usage -= length;
    }

    if (insert(key,D_str) == -1)
        return -1;  //特殊标识，标识插入deleted时满
    else
        return 1;
}

void MemTable::reset(){
    memory_remaining = memory_remaining+memory_usage;
    memory_usage = 0;
    len = 0;
    delete sl;
    sl = new Skip_List_mem;
}
