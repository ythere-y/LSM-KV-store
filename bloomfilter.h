#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H
#include "BitMap.h"

class BloomFilter
{
private:
    BitMap bitmap;
    int length;
public:
    BloomFilter(){
        bitmap.initBitMap(0);
        this->length = 0;
    };
    BloomFilter(int length){
        bitmap.initBitMap(length);
        this->length = length;
    }
    ~BloomFilter(){};
    void Add(const long long &tar);
    bool Contains(const long long &tar);
};

#endif // BLOOMFILTER_H
