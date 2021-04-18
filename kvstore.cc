#include "kvstore.h"
#include <string>
#include <iostream>

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
    mem = new MemTable(30);
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
    if (mem->insert(key,s)==-1){
        std::cout<<"failed to insert"<< std::endl;
    }
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    std::string get =  mem->search(key);
    return get;
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    if(mem->remove(key)==-1){
        std::cout<< "failed to insert deleted" << std::endl;
    }
    return true;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
    mem->reset();
}
