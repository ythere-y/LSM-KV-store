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
    std::string data;
    for (uint32_t i = 0 ;i < m->size();i++)
    {
        head.max = p->key > head.max ? p->key : head.max;
        head.min = p->key < head.min ? p->key : head.min;
        dict.push_back(Dict(p->key,pos));
        blfter.Add(p->key);
        pos += p->offset->size();
        data += *(p->offset);
    }
    write_to_file();
}
void SSTable::write_to_file(){
    std::string write_to = "123456789012345678901234567890";
    std::cout<< "now start the fileout\n" << std::endl;
    std::ofstream outFile("out_test.sst",std::ios::out|std::ios::binary);
    std::vector<std::string>test;

//    outFile.write((char*)&(head),32);
//    outFile.write((char*)&(blfter.bitmap.bitmap),10240);
//    outFile.write((char*)&(dict),head.nums*12);
    outFile.write((char*)&(write_to),sizeof(write_to));
//    for (uint64_t i = 0; i < head.nums;i++){
//        outFile.write((char*)&(dict[i].key),8);
//        outFile.write((char*)&(dict[i].offset),4);
//    }
    outFile.close();
    std::cout<<"now the out end\n";

    std::cout<<"\nnow start the read\n";
    std::ifstream inFile("out_test.sst",std::ios::in|std::ios::binary);
    if (!inFile)
    {
        std::cout<<"read error\n";
        return;
    }
    SSTable *read_test = new SSTable;
    std::string ptr;
    inFile.read((char*)&(ptr),32+10240);
//    inFile.read((char*)&(read_test->head),32);
//    inFile.read((char*)&(read_test->blfter.bitmap.bitmap),10240);
//    inFile.read((char*)&(read_test->dict),read_test->head.nums*12);
    std::string read_from;
    uint64_t show_out = 0;
    inFile.read((char*)&(show_out),8);
//    inFile.read((char*)&(read_from),sizeof(read_from));
//    for (uint64_t i = 0 ;i < head.nums; i++){
//        Dict add;

//        inFile.read((char*)&(add.key),8);
//        inFile.read((char*)&(add.offset),4);
//        read_test->dict.push_back(add);
//    }
    inFile.close();
    return ;
}

void SSTable::reset(){
    head.time_stamp = 0;
    head.nums = 0;
    head.max = LONG_LONG_MIN;
    head.min = LONG_LONG_MAX;
    dict.resize(0);
//    data = "";
    return;
}

//SSTable_Head::SSTable_Head(MemTable *m,uint64_t &_time){
//    head.time_stamp = _time;
//    head.nums = m->size();
//    SKNode_mem *p = m->sl->head->forward[0];
//    uint32_t pos = 0;
//    for (uint32_t i = 0; i < m->size(); i++){
//        head.max =
//    }
//}
