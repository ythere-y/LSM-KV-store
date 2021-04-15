#include "bloomfilter.h"
#include "MurmurHash3.h"

void BloomFilter::Add(const long long &tar){
    unsigned int hash_res[4] = {0};
    MurmurHash3_x64_128(&tar,sizeof(tar),1,hash_res);
    for (int hash_in : hash_res)
        bitmap.bitmapSet(hash_in);
}

bool BloomFilter::Contains(const long long &tar){
    unsigned int hash_res[4] = {0};
    MurmurHash3_x64_128(&tar, sizeof(tar),1,hash_res);
    for (int hash_in : hash_res)
        if (!bitmap.bitmapGet(hash_in))
            return false;
    return true;
}


