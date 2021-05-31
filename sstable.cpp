#include "sstable.h"

SSTable::SSTable()
{

}

/*
 * 用一个Memtable来初始化SSTable,需要记录的是header、布隆过滤器、索引区，stringdata不记录，直接写入文件
 */
SSTable::SSTable( MemTable * m,uint64_t &_time,uint32_t level, std::string &path):dir(path)
{
//    printf("get the args : time = %llu, level = %d, id = %d\n",_time,level,id);

    file_id = _time;
    file_level = level;
    head.time_stamp = _time;    //记录时间戳
    head.nums = m->size();      //数量记载
    SKNode_mem *p = m->sl->head->forward[0];    //第一个
    std::string str_data;
    uint32_t pos = 0;
    long long tmp_max = LONG_LONG_MIN;
    long long tmp_min = LONG_LONG_MAX;

    while(p->key != LONG_LONG_MAX)
    {
        //更新最大值
        tmp_max = p->key > tmp_max ? p->key : tmp_max;
        //更新最小值
        tmp_min = p->key < tmp_min ? p->key : tmp_min;

        pos += p->offset.size();                            //索引点向后移动
        Dict add_dict = Dict(p->key,pos);
        dict.push_back(add_dict);                           //索引更新，索引记录的是终点
        blfter.Add(p->key);                                 //布隆过滤器同步更新
        str_data += p->offset.c_str();
//        if (p->key == 0)
//        {
//            printf("in trans secton key = 0 ");
//            printf("offset = %d ",pos);
//            printf("data = %s ",p->offset.c_str());
//            printf("\n");
//            printf("in trans section the dic get is {%llu,%d}\n",dict[0].key,dict[0].offset);
//            printf("in trans section the data is : %s\n",str_data.c_str());
//            printf("in trans section the nums is %llu\n",head.nums);
//            printf("in trans section the level = %d, the id = %llu\n",file_level,file_id);

//        }
        if (p->key == LONG_LONG_MAX)
            break;
        p = p->forward[0];
  }
    head.max = tmp_max;
    head.min = tmp_min;
//    str_data = combine_str;
    //生成一个SSTable之后就写入到硬盘
    char tmp_buf[80];
    char tmp_dir_buf[80];
    fflush(stdout);


    sprintf(tmp_dir_buf,"%s/level-%d",dir.c_str(),level);
    sprintf(tmp_buf,"%s/level-%d/%llu.sst",dir.c_str(),level,file_id);

    std::string filename(tmp_buf);
    std::string dirname(tmp_dir_buf);
    // 先检查文件目录是否存在，如果不存在就创建一个
    if (!utils::dirExists(dirname.c_str()))
    {
        utils::mkdir(dirname.c_str());
    }
    std::ofstream outFile(filename,std::ios::out|std::ios::binary);
    write_to_file(outFile, str_data);

    outFile.close();
}
void SSTable::write_to_file(std::ofstream &outFile, std::string & str_data){
    char buff[10240];
    ///写header
    outFile.write((char*)&(head),32);
    ///写布隆过滤器h
    memcpy(buff,blfter.bitmap.bitmap,10240);
    outFile.write(buff,10240);
    ///写索引区
    for (uint32_t i = 0 ;i < dict.size(); i ++)
    {
        outFile.write((char*)&(dict[i].key),8);
        outFile.write((char*)&(dict[i].offset),4);
    }

    ///写字符串内容
    outFile.write(str_data.c_str(),str_data.size());

/*
    int len = str_data.size();
    printf("in write section the size is %d\n",len);

//    //读测试开始
//    {
    std::cout<<"in write section, now start the read\n";
    std::ifstream inFile("data/level-0/0.sst",std::ios::in|std::ios::binary);
    if (!inFile)
    {
        std::cout<<"fail to open file\n";
        return;
    }
    SSTable *read_test = new SSTable;
    //读header
//    inFile.read((char*)&(read_test->head),32);
    //读布隆过滤器
//    inFile.read((char*)&(buff),10240);
//    memcpy(read_test->blfter.bitmap.bitmap, buff,10240);
//    printf("test of the new bitmap :\n");
//    printf("test 1 : %d\n",read_test->blfter.Contains(1));
//    printf("test 3 : %d\n",read_test->blfter.Contains(3));
//    printf("test 5 : %d\n",read_test->blfter.Contains(5));
    printf("in write section the head is : %llu, %llu, %lld, %lld\n",read_test->head.time_stamp,read_test->head.nums,read_test->head.min, read_test->head.max);
    ///读索引区
//    inFile.seekg(0,std::ios::cur);
//    std::vector<Dict> read_dict;
//    for (int i = 0; i < dict.size(); i ++)
//    {
//        uint64_t tmp_key = 0;
//        uint32_t tmp_offset = 0;
//        inFile.read((char*)&tmp_key,8);
//        inFile.read((char*)&tmp_offset,4);
//        printf("{%lld,%d};",tmp_key,tmp_offset);
//        Dict add = Dict(tmp_key,tmp_offset);
//        read_dict.push_back(add);
//    }
//    printf("in the read test section the dict size is : %llu\n",read_dict.size());

//    inFile.read((char*)&read_dict,dict.size()*12);
//    inFile.read((char*)&(dict_test),8);
//    printf("in read test section; the first is {%lld,%d}\n",read_dict[0].key,read_dict[0].offset);

//    inFile.seekg(4,std::ios::cur);
//    inFile.read((char*)&(read_test->dict),read_test->head.nums*12);

//    for (unsigned int i = 0 ;i < read_test->dict.size(); i++){
//    for (unsigned int i = 0; i < head.nums; i++){
//        printf("{%llu,%u};",read_test->dict[i].key,read_test->dict[i].offset);
//    }
//    printf("\n");
    /// 读字符串数据
    if (!inFile)
        return ;
    uint32_t start = 2059435;
    uint32_t end = 2061465;
    uint32_t length = end-start;
    char read_from[length+1];

    inFile.seekg(32+start,std::ios::beg);
    inFile.read(read_from,end-start);
    read_from[length] = '\0';
    printf("in write section the read test is %s\n",read_from);
    inFile.close();
//    inFile.close();
//    fflush(stdout);
//    }
*/
    return ;
}
/*
 * 从一个文件流中读取数据填充SSTable
 */
void SSTable::read_from_file(std::ifstream &file,const uint32_t level,const uint32_t id){
    file_level = level;
    file_id  = id;
    char buff[10240];
    printf("in read section ready to read from file to fill the ssTable, the level is %d; the id is %llu\n",file_level,file_id);
    ///读header
    file.read((char*)&(head),32);
    ///读布隆过滤器
    file.read((char*)&(buff),10240);
    memcpy(blfter.bitmap.bitmap, buff,10240);
    ///读索引区
    int len = head.nums;
    dict = std::vector<Dict>(len);
    for (int i = 0; i < len; i++)
    {
        file.read((char*)&(dict[i].key),8);
        file.read((char*)&(dict[i].offset),4);
    }
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
    offset_end = (*iter).offset;    //确定要找的数据对应的终点

    if (iter != dict.begin())     //如果不是第一个
    {
        offset_start = (*(iter-1)).offset;   //可以找到结束点位
    }else{                              //是第一个
        offset_start = 0;
    }
    int gap = offset_end-offset_start;
    length = gap > 0 ? gap : 0;
    inFile.seekg(32+10240+head.nums*12+offset_start,std::ios::beg);//依次跳过header、布隆过滤器、索引，再加上偏移量
    char read_from[length+1];
    memset(read_from,0,sizeof(char)*(length+1));
    inFile.read(read_from,length);
    read_from[length] = '\0';

//    printf("the data get is :\n");
//    std::cout<<read_from<<std::endl;

    std::string res(read_from);
//    if (key == 0)
//        printf("in read section the read_from = %s\n",read_from);
//    if (key == 0)
//        printf("in read seciont the res = %s\n",res.c_str());

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
    //删除对应的文件
    char buf[80];
    sprintf(buf,"data/level-%d/%llu.sst",file_level,file_id);
//    printf("try to delete file : %s \n",buf);
    utils::rmfile(buf);
    return;
}
std::string SSTable::search(long long key){
    if (key < head.min || key > head.max)
        return "";
    if (!blfter.Contains(key))
        return "";
    std::string res = "";
    char buf[80];
    memset(buf,0,sizeof(char)*80);

    sprintf(buf,"data/level-%d/%llu.sst",file_level,file_id);
    std::string filename(buf);
    std::ifstream inFile(filename.c_str(),std::ios::in|std::ios::binary);
    if (!inFile)
    {
        printf("Open file failed ! level = %d, id = %llu\n",file_level,file_id);
        return "";
    }
    res = read_from_file_by_key(inFile,key);
    return res;
}
