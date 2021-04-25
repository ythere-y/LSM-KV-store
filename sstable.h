#ifndef SSTABLE_H
#define SSTABLE_H
#include <stdio.h>
#include "memtable.h"
#include "bloomfilter.h"
#include <fstream>
#include <iostream>
struct Header{
    uint64_t time_stamp = 0;
    uint64_t nums = 0;
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
//    std::string data;
    void write_to_file();
public:
    SSTable();
    SSTable(MemTable *m,uint64_t &_time);
    SSTable(std::string & s);
    void reset();
};

class SSTable_Head
{
public:
    Header head;
    BloomFilter blfter;
    std::vector<Dict> dict;
    SSTable_Head(){}
//    SSTable_Head(MemTable*m,uint64_t &_time);
};

#endif // SSTABLE_H
