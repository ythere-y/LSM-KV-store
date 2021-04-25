#pragma once

#include "kvstore_api.h"
#include "memtable.h"
#include "sstable.h"

struct Level{   //对于每个层级，直接存储的内容只是SSTable的haead
    std::vector<SSTable*> heads_in_level;      //存储这一层级中的head
    int size;
    Level():size(INT_MAX){}
    Level(int _size):size(_size){}
};

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
    MemTable *mem;

//    SSTable *ss;
    std::vector<Level*> levels;     //存储多个层级
    int SS_write(SSTable * tar,int level);
public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

    void read_ss_head();
};
