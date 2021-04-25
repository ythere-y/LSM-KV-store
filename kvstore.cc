#include "kvstore.h"
#include <string>
#include <iostream>

const std::string D_str = "~DELETED~";

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
    mem = new MemTable(30);
    levels.push_back(new Level);
}

int KVStore::SS_write(SSTable* tar,int level){
    std::ofstream outFile("test.sst",std::ios::out|std::ios::binary);
    std::vector<std::string>test;

    outFile.write((char*)&(tar->head.time_stamp),sizeof(uint64_t));
    outFile.write((char*)&(tar->head.nums),8);
    outFile.write((char*)&(tar->head.max),8);
    outFile.write((char*)&(tar->head.min),8);
    outFile.write((char*)&(*tar->blfter.bitmap.bitmap),10240);
    for(uint64_t i = 0 ; i < tar->head.nums;i++){
        outFile.write((char*)&(tar->dict[i].key),8);
        outFile.write((char*)&(tar->dict[i].offset),4);
    }
//    outFile.write(tar->data.c_str(),sizeof(tar->data));
    outFile.close();
    return 0;
}

void KVStore::read_ss_head(){
    std::ifstream inFile("test.sst",std::ios::in|std::ios::binary);//二进制读方式打开
    if (!inFile){
        std::cout<<"error" <<std::endl;
        return;
    }
    SSTable_Head * test = new SSTable_Head;
    inFile.read((char*)&(test->head.time_stamp),8);
    inFile.read((char*)&(test->head.nums),8);
    inFile.read((char*)&(test->head.max),8);
    inFile.read((char*)&(test->head.min),8);
    inFile.read((char*)&(*test->blfter.bitmap.bitmap),10240);
    if (test->blfter.Contains(2))
        std::cout<< "copy OK~!@" << std::endl;
    else
        std::cout<<"copy not ~~"<< std::endl;

    for(uint64_t i = 0 ; i < test->head.nums;i++){
        Dict add;

        inFile.read((char*)&(add.key),8);
        inFile.read((char*)&(add.offset),4);
        test->dict.push_back(add);
    }
    std::string read_test;
    std::memset(&read_test,'\0',8);
    inFile.read((char*)&read_test,8);
    inFile.close();
}

KVStore::~KVStore()
{
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
    uint64_t time = 0;
    if (mem->insert(key,s)==-1){                //如果插入成功，则返回0，插入失败则返回-1，进入特殊处理
        std::cout<<"failed to insert"<< std::endl;  //输出失败提示
//        SSTable_Head * add = new SSTable_Head(mem,time);  //创建一个新的mem,SSTabel 的level为0
        SSTable * add = new SSTable(mem,time);  //创建一个新的mem,SSTabel 的level为0
        levels[0]->heads_in_level.push_back(add);       //把这个新的直接放到level_0里面
        mem->reset();                           //mem重置
        put(key,s);                             //重新put一次
    }
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    return "";
//    std::string get =  mem->search(key);
//    if (get == ""){
//        for (auto iter : levels[0]->all_ss){
//            if (iter->head.min <= key && iter->head.max >= key && iter->blfter.Contains(key)){
//                //确保在这个区块
//                // TODO:读取这个SSTable的数据区
////                read_ss()
//                return "get";
//            }
//        }
//        return get;
//    }
//    return get;
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    mem->remove(key);
    put(key,D_str);
    return true;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
    return;
//    for(auto iter : levels[0]->all_ss){
//        iter->reset();
//    }
//    mem->reset();
}

