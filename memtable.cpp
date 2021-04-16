#include "memtable.h"

MemTable::MemTable()
{

}

int MemTable::insert(uint64_t key, const std::string &s){
    if (bf->Contains(key)){
        SKNode*get = sl->searchNode(key);   //找到key值点
        get->offset = &s;           //数据替换
        return 1;
    }
    head->Add(key);
    bf->Add(key);
    sl->insertNode(key,s);
    vals->push_back(s);
    return 1;
}

std::string MemTable::search(uint64_t key){
    if (key < head->key_min || key > head->key_max)
        return "";
    if (!bf->Contains(key))
        return "";
    SKNode* get = sl->searchNode(key);
    return *get->offset;
}

bool MemTable::remove(uint64_t key){
    if (key < head->key_min || key>head->key_max)
        return false;   //删除失败
    if (!bf->Contains(key))
        return false;   //删除失败

}

