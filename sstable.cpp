#include "sstable.h"

SSTable::SSTable()
{

}

/*
 * 用一个Memtable来初始化SSTable,需要记录的是header、布隆过滤器、索引区，stringdata不记录，直接写入文件
 */
SSTable::SSTable( MemTable * m,uint64_t &_time,uint32_t level, uint32_t id)
{
//    printf("get the args : time = %llu, level = %d, id = %d\n",_time,level,id);
    file_id = id;
    file_level = level;
    head.time_stamp = _time;    //记录时间戳
    head.nums = m->size();      //数量记载
    SKNode_mem *p = m->sl->head->forward[0];    //第一个
    std::string str_data;
    char combine_str[20000];
    uint32_t pos = 0;
    long long tmp_max = LONG_LONG_MIN;
    long long tmp_min = LONG_LONG_MAX;
    while(p->key != LONG_LONG_MAX)
    {
        //更新最大值
        tmp_max = p->key > tmp_max ? p->key : tmp_max;
        //更新最小值
        tmp_min = p->key < tmp_min ? p->key : tmp_min;
        Dict add_dict = Dict(p->key,pos);
        dict.push_back(add_dict);                   //索引更新
        blfter.Add(p->key);                                 //布隆过滤器同步更新
        pos += p->offset.size();                           //索引点向后移动
        strcat(combine_str,p->offset.c_str());

//        if (p->key == LONG_LONG_MAX)
//            break;
        p = p->forward[0];
  }
    head.max = tmp_max;
    head.min = tmp_min;
    str_data = combine_str;
    //生成一个SSTable之后就写入到硬盘
    char tmp_buf[80];
    sprintf(tmp_buf,"data/level-%d/%d.sst",level,id);
    std::string filename(tmp_buf);
    std::ofstream outFile(filename,std::ios::out|std::ios::binary);
    write_to_file(outFile, str_data);
    outFile.close();
}
void SSTable::write_to_file(std::ofstream &outFile, std::string & str_data){
    char buff[10240];
    //写header
    outFile.write((char*)&(head),32);
    //写布隆过滤器
    memcpy(buff,blfter.bitmap.bitmap,10240);
    outFile.write(buff,10240);
//    printf("test of the yuan bitmap :\n");
//    printf("test 1 : %d\n",blfter.Contains(1));
//    printf("test 3 : %d\n",blfter.Contains(3));
//    printf("test 5 : %d\n",blfter.Contains(5));
    //写索引区
    outFile.write((char*)&(dict),head.nums*12);
//    printf("the dict is\n");
//    for (unsigned int i = 0; i < head.nums; i++){
//        printf("{%llu,%u};",dict[i].key,dict[i].offset);
//    }
//    printf("\n");
    //写字符串内容
    outFile.write(str_data.c_str(),str_data.size());
//    printf("the string to write is :\n");
//    std::cout<<str_data<<std::endl;
//    outFile.close();

//    std::cout<<"now the out end\n";

//    //读测试开始
//    {
//    std::cout<<"now start the read\n";
//    std::ifstream inFile("out_test.sst",std::ios::in|std::ios::binary);
//    if (!inFile)
//    {
//        std::cout<<"fail to open file\n";
//        return;
//    }
//    SSTable *read_test = new SSTable;
//    //读header
//    inFile.read((char*)&(read_test->head),32);
//    //读布隆过滤器
//    inFile.read((char*)&(buff),10240);
//    memcpy(read_test->blfter.bitmap.bitmap, buff,10240);
//    printf("test of the new bitmap :\n");
//    printf("test 1 : %d\n",read_test->blfter.Contains(1));
//    printf("test 3 : %d\n",read_test->blfter.Contains(3));
//    printf("test 5 : %d\n",read_test->blfter.Contains(5));
//    //读索引区
//    inFile.read((char*)&(read_test->dict),read_test->head.nums*12);
//    printf("the dict get is\n");
//    for (unsigned int i = 0; i < head.nums; i++){
//        printf("{%llu,%u};",read_test->dict[i].key,read_test->dict[i].offset);
//    }
//    printf("\n");
//    //读字符串数据
//    char read_from[12];
//    inFile.seekg(11,std::ios::cur);
//    inFile>>read_from;
//    read_from[11] = '\0';
//    printf("the string get is : \n");
//    std::cout << read_from<<std::endl;
//    read_test->data = read_from;
//    printf("the data get is :\n");
//    std::cout<<read_test->data<<std::endl;

//    inFile.close();
//    fflush(stdout);
//    }
    return ;
}
/*
 * 从一个文件流中读取数据填充SSTable
 */
void SSTable::read_from_file(std::ifstream &file,const uint32_t level,const uint32_t id){
    file_level = level;
    file_id  = id;
    char buff[10240];
    printf("ready to read from file to fill the ssTable, the level is %d; the id is %d\n",file_level,file_id);
    //读header
    file.read((char*)&(head),32);
    //读布隆过滤器
    file.read((char*)&(buff),10240);
    memcpy(blfter.bitmap.bitmap, buff,10240);
    //读索引区
    file.read((char*)&(dict),head.nums*12);
    fflush(stdout);
    return ;
}
/*
 * 在已经有基础SSTable的情况下查询某个key对应的val
 */
std::string SSTable::read_from_file_by_key(std::ifstream & inFile, const uint64_t key){
    std::vector<Dict>::iterator iter = BinarySearch(key);
//    std::ifstream inFile("out_test.sst",std::ios::in|std::ios::binary);
    int offset_start  = 0,offset_end = 0,length = 0;
    if (iter == dict.end())    //如果没有找到，就自动退出
    {
        return "";
    }
    offset_start = (*iter).offset;//确定要找的数据对应的偏移量
    if ((++iter) != dict.end())   //如果不是最后一个
    {
        offset_end = (*iter).offset;   //可以找到结束点位
    }else{                              //是最后一个
        inFile.seekg(0,std::ios::end);
        offset_end = inFile.tellg();
    }
    int gap = offset_end-offset_start;
    length = gap > 0 ? gap : 0;
    inFile.seekg(32+10240+head.nums*12+offset_start,std::ios::beg);//依次跳过header、布隆过滤器、索引，再加上偏移量
    char read_from[length+1];
    inFile.read(read_from,length);
    read_from[length] = '\0';


//    printf("the data get is :\n");
//    std::cout<<read_from<<std::endl;

    std::string res(read_from);

    inFile.close();
    fflush(stdout);
    return res;
}
/*
 * 使用二分搜索法找key,
 * 成功则返回对应的迭代器，失败则返回end()
 */
std::vector<Dict>::iterator SSTable::BinarySearch(const uint64_t key){
    auto left = dict.begin();
    auto right = dict.end();
    auto mid = left + (right-left)/2;
    while(left != right && (*mid).key != key)
    {
        if (key<(*mid).key)
            right = mid;
        else
            left = mid+1;
        mid = left + (right-left)/2;
    }
    if ((*mid).key == key)
        return mid;
    else
        return dict.end();
}

/* 重置一个SSTable */
void SSTable::reset(){
    head.time_stamp = 0;
    head.nums = 0;
    head.max = LONG_LONG_MIN;
    head.min = LONG_LONG_MAX;
    dict.resize(0);
    return;
}
std::string SSTable::search(long long key){
    if (key < head.min || key > head.max)
        return "";
    if (!blfter.Contains(key))
        return "";
    std::string res;
    char buf[80];

    sprintf(buf,"data/level-%d/%d.sst",file_level,file_id);
    std::string filename(buf);
    std::ifstream inFile(filename.c_str(),std::ios::in|std::ios::binary);
    if (!inFile)
    {
        printf("Open file failed !\n");
        return "";
    }
    res = read_from_file_by_key(inFile,key);
    return res;
}
