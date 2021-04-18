#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H
#include "BitMap.h"

class BloomFilter
{
private:
    int length;
public:
    BitMap bitmap;
    BloomFilter(){
        this->length = 0;
    };
    BloomFilter(int length){
        this->length = length;
    }
    ~BloomFilter(){};
    void Add(const long long &tar);
    bool Contains(const long long &tar);
};

#endif // BLOOMFILTER_H
