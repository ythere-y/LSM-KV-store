#pragma once

#include "kvstore_api.h"

#include "memtable.h"
#include "sstable.h"
#include <set>
#include <queue>
#include <map>
//#include <io.h>
//#include <direct.h>
struct Level{   //对于每个层级，直接存储的内容只是SSTable的haead
    std::vector<SSTable*> heads_in_level;      //存储这一层级中的head
    int size;
    Level():size(INT_MAX){}
    Level(int _size):size(_size){}
};

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
    const uint32_t mem_size =  2*1024*1024;
//    const uint32_t mem_size = 2*1024*8;
//    const uint32_t mem_size = 32+10240+40;
    std::string dir = "";
    uint64_t time_stamp_label = 0;      // 全局时间标签，总是标志着下一个table的time_stamp
    MemTable *mem;
    std::vector<Level*> levels;     //存储多个层级
    uint32_t cur_level = 0;         //记录目前层级数量

    uint32_t level_to_int(std::string level_name);
    uint32_t id_to_int(std::string id_name);
    void dirname_to_num(std::string &dirname,std::string &filename,uint32_t &level, uint32_t &id);

//    SSTable *ss;
    void recombination();
    void compaction(std::vector<SSTable*> & tar_list,const uint32_t level);
public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

    void init_SSTables();
};
