#include "sstable.h"

SSTable::SSTable()
{

}


SSTable::SSTable(MemTable * m,uint64_t &_time)
{
    head.time_stamp = _time;
    head.nums = m->size();
//    head.time_stamp = 0;
    SKNode_mem *p = m->sl->head->forward[0];
    uint32_t pos = 0;
    for (uint32_t i = 0 ;i < m->size();i++)
    {
        head.max = p->key > head.max ? p->key : head.max;
        head.min = p->key < head.min ? p->key : head.min;
        dict.push_back(Dict(p->key,pos));
        blfter.Add(p->key);
        pos += p->offset->size();
        data += *(p->offset);
    }
}

void SSTable::reset(){
    head.time_stamp = 0;
    head.nums = 0;
    head.max = LONG_LONG_MIN;
    head.min = LONG_LONG_MAX;
    dict.resize(0);
    data = "";
    return;
}

