#include "sstable.h"

SSTable::SSTable()
{

}


SSTable::SSTable(MemTable * m)
{
    head.nums = m->size();
    head.time_stamp = 0;
    SKNode_mem *p = m->sl->head->forward[0];
    uint32_t pos = 0;
    for (uint32_t i = 0 ;i < m->size();i++)
    {
        head.max = p->key > head.max ? p->key : head.max;
        head.min = p->key < head.min ? p->key : head.min;
        dict.push_back(Dict(p->key,pos));
        pos += p->offset->size();
        data += *(p->offset);
    }
}

