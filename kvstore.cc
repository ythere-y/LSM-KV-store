#include "kvstore.h"
#include <string>
#include <iostream>

const std::string D_str = "~DELETED~";

uint32_t KVStore::level_to_int(std::string level_name){
    int len = level_name.size();
    uint32_t res = 0;
    for (int i = 6; i < len; i ++)
    {
        res *= 10;
        res += level_name[i]-'0';
    }
    return res;
}
uint32_t KVStore::id_to_int(std::string id_name){
    int len = id_name.size();
    uint32_t res = 0;
    for (int i = 0; i < len-4; i++)
    {
        res *= 10;
        res += id_name[i]-'0';
    }
    return res;
}

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
    mem = new MemTable(30);
    levels.push_back(new Level);
}

int KVStore::SS_write(SSTable* tar,int level){
    std::ofstream outFile("test.sst",std::ios::out|std::ios::binary);
    std::vector<std::string>test;

    outFile.write((char*)&(tar->head.time_stamp),sizeof(uint64_t));
    outFile.write((char*)&(tar->head.nums),8);
    outFile.write((char*)&(tar->head.max),8);
    outFile.write((char*)&(tar->head.min),8);
    outFile.write((char*)&(*tar->blfter.bitmap.bitmap),10240);
    for(uint64_t i = 0 ; i < tar->head.nums;i++){
        outFile.write((char*)&(tar->dict[i].key),8);
        outFile.write((char*)&(tar->dict[i].offset),4);
    }
//    outFile.write(tar->data.c_str(),sizeof(tar->data));
    outFile.close();
    return 0;
}
void KVStore::init_SSTables(){
    long hFile = 0, in_hFile = 0;
    struct _finddata_t fileinfo, in_fileinfo;
    std::string p;
    std::string path = "data";
    std::string tmp_name;
    uint32_t level = 0;
    uint32_t id = 0;
    int count = 0;
    //遍历data下所有level目录
    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(),&fileinfo))!=-1)
    {
        do{
//            printf("%s\n",fileinfo.name);
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                //找到真实的level目录
                if (strcmp(fileinfo.name,".")!= 0 && strcmp(fileinfo.name,"..")!= 0)
                {
                    level = level_to_int(fileinfo.name);

                    //进入单个level目录，进入下一层
                    std::string tmp_path = p.assign(path).append("/").append(fileinfo.name);
                    if ((in_hFile = _findfirst(p.assign(tmp_path).append("/*").c_str(),&in_fileinfo))!=1){
                        do{
                            //找到正真的.sst文件
                            if (strcmp(in_fileinfo.name,".")!= 0 && strcmp(in_fileinfo.name,"..")!=0)
                            {
                                //尝试打开文件，开始读取，读到的数据用来初始化一个add
                                id = id_to_int(in_fileinfo.name);
                                tmp_name = p.assign(tmp_path).append("/").append(in_fileinfo.name);
                                SSTable *add = new SSTable;
                                std::ifstream inFile(tmp_name,std::ios::in|std::ios::binary);
                                if (inFile){
                                    add->read_from_file(inFile,level,id);
                                    //如果没有对应的层级，则先准备好
                                    while (level>= levels.size())
                                        levels.push_back(new Level);
                                    levels[level]->heads_in_level.push_back(add);
                                    count ++;
                                }else
                                    printf("fail to open file %s\n",tmp_name.c_str());
                                inFile.close();
                                printf("read %s OK\n",tmp_name.c_str());
                            }
                            fflush(stdout);
                        }while(_findnext(in_hFile,&in_fileinfo)==0);
                    }
                }
            }
        }while(_findnext(hFile,&fileinfo)==0);
    }
    if (count)
        printf("success to read %d files to init SSTables\n",count);
    else
        printf("no files to read for initations\n");
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
    uint64_t time = 0;
    if (mem->insert(key,s)==-1){                //如果插入成功，则返回0，插入失败则返回-1，进入特殊处理
        printf("the key %lu insert to a new memtable\n",key);
        int name_id = levels[0]->heads_in_level.size();
        uint32_t level =0;
        SSTable * add = new SSTable(mem,time,level,name_id);  //根据MEMTable创建一个新的SSTabel 的level为0
        levels[0]->heads_in_level.push_back(add);       //把这个新的直接放到level_0里面
        mem->reset();                           //mem重置
        put(key,s);                             //重新put一次
    }
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    std::string res;
    //先看看能不能在mem中直接找到
    res = mem->search(key);
    if (res == "" || !strcmp(res.c_str(),D_str.c_str()))
    {
        return "";
    }
    //再去遍历level
    for (auto level : levels)
    {
        //遍历level中的table
        for (auto table:level->heads_in_level)
        {
            res = table->search(key);
            if (!strcmp(res.c_str(),D_str.c_str()))
                return "";
            else if (res != "")
                return res;
        }
    }
    //遍历完成都没有找到res，默认返回""空字符串
    return "";
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    mem->remove(key);
    put(key,D_str);
    return true;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
    //删除内存中的数据
    mem->reset();
    for (auto level:levels)
    {
        for (auto table:level->heads_in_level)
        {
            table->reset();
        }
    }
    std::string p;
    std::string path = "data";
    long hFile = 0;
    long in_hFile = 0;
    struct _finddata_t fileinfo;
    struct _finddata_t in_fileinfo;
    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(),&fileinfo))!=-1)
    {
        do{
//            printf("%s\n",fileinfo.name);
            if (fileinfo.attrib & _A_SUBDIR){
                if (strcmp(fileinfo.name,".")!=0 && strcmp(fileinfo.name,"..")!=0)
                {
                    std::string tmp_path = p.assign(path).append("/").append(fileinfo.name);
                    if ((in_hFile = _findfirst(p.assign(tmp_path).append("/*").c_str(),&in_fileinfo))!=1)
                    {
                        do{
                            if (strcmp(in_fileinfo.name,".") != 0 && strcmp(in_fileinfo.name,"..")!=0)
                            {

                                if (!remove(p.assign(tmp_path).append("/").append(in_fileinfo.name).c_str()))
                                    printf("remove %s OK\n",p.assign(tmp_path).append("/").append(in_fileinfo.name).c_str());
                                else
                                    printf("fail to remove %s \n",p.assign(tmp_path).append("/").append(in_fileinfo.name).c_str());
                            }
                        }while(_findnext(in_hFile,&in_fileinfo)==0);
                    }
                }
            }
            fflush(stdout);
        }while(_findnext(hFile,&fileinfo)==0);
    }
    return;
}

