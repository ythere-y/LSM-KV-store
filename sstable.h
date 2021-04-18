#ifndef SSTABLE_H
#define SSTABLE_H
#include <stdio.h>
#include "memtable.h"
#include "bloomfilter.h"

struct Header{
    uint64_t time_stamp;
    uint64_t nums;
    long long max = LONG_LONG_MIN;
    long long min = LONG_LONG_MAX;
    Header(){}
    Header(uint64_t,uint64_t,long long, long long);
};
struct Dict{
    uint64_t key;
    uint32_t offset;
    Dict(){}
    Dict(uint64_t _key,uint32_t _offset):key(_key),offset(_offset){};
};

class SSTable
{
public:
    Header head;
    BloomFilter blfter;
    std::vector<Dict> dict;
    std::string data;
public:
    SSTable();
    SSTable(MemTable *m);
    SSTable(std::string & s);
};

#endif // SSTABLE_H
