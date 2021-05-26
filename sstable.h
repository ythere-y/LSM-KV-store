#ifndef SSTABLE_H
#define SSTABLE_H
#include <stdio.h>
#include "memtable.h"
#include "bloomfilter.h"
#include <fstream>
#include <iostream>
#include <string>
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
    uint32_t file_id = 0;
    uint32_t file_level = 0;
    Header head;
    BloomFilter blfter;
    std::vector<Dict> dict;
//    std::string data;
    void write_to_file(std::ofstream &outFile, std::string & str_data);
    void read_from_file(std::ifstream &file,const uint32_t level,const uint32_t id);
    std::string read_from_file_by_key(std::ifstream &inFile, const uint64_t key);
    std::vector<Dict>::iterator BinarySearch(const uint64_t key);


public:
    SSTable();
    SSTable(MemTable *m,uint64_t &_time,uint32_t level, uint32_t id);
    SSTable(std::string & s);
    void reset();
    std::string search(long long key);
};



#endif // SSTABLE_H
