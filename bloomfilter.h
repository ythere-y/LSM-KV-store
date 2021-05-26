#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H
#include "BitMap.h"

class BloomFilter
{
public:
    BitMap bitmap;
    BloomFilter(){};
    ~BloomFilter(){};
    void Add(const long long &tar);
    bool Contains(const long long &tar);
};

#endif // BLOOMFILTER_H
