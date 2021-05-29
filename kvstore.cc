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
    mem = new MemTable(mem_size);
    time_stamp_label = 0;
    levels.push_back(new Level);
//    init_SSTables(dir);
}

void KVStore::init_SSTables(const std::string &dir){
    std::vector<std::string> dir_list;
    std::vector<std::string> file_list;
    utils::scanDir("data",dir_list);
    for (auto dir_p : dir_list)
    {
        utils::scanDir(dir.c_str(),file_list);
        for (auto file_p : file_list)
        {
            printf("init by the file [%s]\n",file_p.c_str());
        }
    }

//    long hFile = 0, in_hFile = 0;
//    struct _finddata_t fileinfo, in_fileinfo;
//    std::string p;
//    std::string path = dir;
//    std::string tmp_name;
//    uint32_t level = 0;
//    uint32_t id = 0;
//    int count = 0;
//    //遍历data下所有level目录
//    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(),&fileinfo))!=-1)
//    {
//        do{
////            printf("%s\n",fileinfo.name);
//            if ((fileinfo.attrib & _A_SUBDIR))
//            {
//                //找到真实的level目录
//                if (strcmp(fileinfo.name,".")!= 0 && strcmp(fileinfo.name,"..")!= 0)
//                {
//                    level = level_to_int(fileinfo.name);

//                    //进入单个level目录，进入下一层
//                    std::string tmp_path = p.assign(path).append("/").append(fileinfo.name);
//                    if ((in_hFile = _findfirst(p.assign(tmp_path).append("/*").c_str(),&in_fileinfo))!=1){
//                        do{
//                            //找到正真的.sst文件
//                            if (strcmp(in_fileinfo.name,".")!= 0 && strcmp(in_fileinfo.name,"..")!=0)
//                            {
//                                //尝试打开文件，开始读取，读到的数据用来初始化一个add
//                                id = id_to_int(in_fileinfo.name);
//                                tmp_name = p.assign(tmp_path).append("/").append(in_fileinfo.name);
//                                SSTable *add = new SSTable;
//                                time_stamp_label = add->head.time_stamp > time_stamp_label ? add->head.time_stamp : time_stamp_label;
//                                std::ifstream inFile(tmp_name,std::ios::in|std::ios::binary);
//                                if (inFile){
//                                    add->read_from_file(inFile,level,id);
//                                    //如果没有对应的层级，则先准备好
//                                    while (level>= levels.size())
//                                        levels.push_back(new Level);
//                                    levels[level]->heads_in_level.push_back(add);
//                                    count ++;
//                                }else
//                                    printf("fail to open file %s\n",tmp_name.c_str());
//                                inFile.close();
//                                printf("read %s OK\n",tmp_name.c_str());
//                            }
//                            fflush(stdout);
//                        }while(_findnext(in_hFile,&in_fileinfo)==0);
//                    }
//                }
//            }
//        }while(_findnext(hFile,&fileinfo)==0);
//    }

//    if (count)
//        printf("success to read %d files to init SSTables\n",count);
//    else
//        printf("no files to read for initations\n");
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
    if (mem->insert(key,s)==-1){                //如果插入成功，则返回0，插入失败则返回-1，进入特殊处理
        uint64_t next_time = time_stamp_label++;
        SSTable * add = new SSTable(mem,next_time,0);  //根据MEMTable创建一个新的SSTabel 的level为0
        levels[0]->heads_in_level.push_back(add);       //把这个新的直接放到level_0里面
        recombination();
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
    std::string res = "";
    //先看看能不能在mem中直接找到
    res = mem->search(key);
    if (strcmp(res.c_str(),""))//如果mem查找结果不为
    {
        if (!strcmp(res.c_str(),D_str.c_str()))
            return "";
        else
            return res;
    }
    else
    {
//        if (key == 0)
//            printf("********* in get section fail to find in mem\n");

//        if (!levels[0]->heads_in_level.size())
//            return "";
//        if(key == 0)
//            printf("********get there\n");
        fflush(stdout);
        //再去遍历level
        //level-0特殊处理，要考虑时间戳
        std::string tmp_res("");

        for (auto table:levels[0]->heads_in_level)
        {
            //时间戳由小到大，大的可以覆盖小的
            if (!table)
                break;
            tmp_res = table->search(key);
            if (strcmp(tmp_res.c_str(),""))//找到了有用的数据
            {   //进行覆盖
                res = tmp_res;
            }
        }
        //最后看res是否有数据，如果有，则直接返回
        if (strcmp(res.c_str(),"")) //如果不是空
        {
            if (!strcmp(res.c_str(),D_str.c_str())) //如果结果被删除
            {
                return "";
            }else return res;
        }

        for (auto level = levels.begin()+1; level!= levels.end(); level++)
        {
            //遍历level中的table
            for (auto table:(*level)->heads_in_level)
            {
                res = table->search(key);
                if (!strcmp(res.c_str(),D_str.c_str()))
                    return "";
                else if (strcmp(res.c_str(),""))
                    return res;
            }
        }
        //遍历完成都没有找到res，默认返回""空字符串
    }
    return "";
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    std::string res_get = get(key);
    if (!strcmp(res_get.c_str(),""))   //如果查找结果为空，就直接返回失败
        return false;
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
    //接下来删除所有目录
    std::string p;
    std::vector<std::string> dir_list;
    utils::scanDir("data",dir_list);
    for (auto dir : dir_list)
    {
        utils::rmdir(dir.c_str());
    }
    return;
}
/** 检查level的数量，并且进行重组 */
void KVStore::recombination(){
    if (levels[0]->heads_in_level.size() > 2)//如果第一层多余2个，发动重组
    {
        std::vector<SSTable*> recombineList;    //需要合并的列表
        long long tmp_min = LONG_LONG_MAX;
        long long tmp_max = LONG_LONG_MIN;
        //第0层的全都加入
        for(auto table: levels[0]->heads_in_level)
        {
            recombineList.push_back(table);
            tmp_min = table->head.min < tmp_min ? table->head.min : tmp_min;
            tmp_max = table->head.max > tmp_max ? table->head.max : tmp_max;
        }
        levels[0]->heads_in_level.clear();      // level-0的table全部清除
        if (cur_level>=1)//如果有第1层
        {
            for (auto table = levels[1]->heads_in_level.begin(); table!= levels[1]->heads_in_level.end(); table++)
            {
                //如果范围无交集，直接跳过
                if ((*table)->head.min > tmp_max || (*table)->head.max < tmp_min)
                    continue;
                else{
                    recombineList.push_back(*table);//加入需要合并的列表
                    levels[1]->heads_in_level.erase(table); //从原来的level中删除
                }
            }
        }
        /** 归并操作，将一个List中的所有SSTable进行归并，并再次分解为2M一个的SSTable */
        compaction(recombineList,0);
    }
}

void KVStore::compaction(std::vector<SSTable *> &tar_list, const uint32_t level){
    int last_level = 0;             // 表示是最后一层的合并的标志，默认为0
    if (level  == cur_level)    // 目前是在最后一层进行的合并
    {
        levels.push_back(new Level);    //加入新的一层
        cur_level ++;
        last_level = 1;
    }
    MemTable * com_mem = new MemTable(mem_size);
    std::vector<std::vector<Dict>::iterator> iter_list;
    std::vector<SSTable *> comp_table;
    std::set<int> num_list;
    int len = tar_list.size();
    uint64_t time;
    uint64_t key_min = ULLONG_MAX;
    int i_min = 0;

    /** 专用检查迭代器是否到末尾的函数，及时用这个函数来更新num_list*/
    auto checkEnd = [&](){
        std::vector<int> delete_list;       // 一言不合就用vector
        for (auto tail : num_list)
        {
            if (iter_list[tail] == tar_list[tail]->dict.end())
                delete_list.push_back(tail);
        }
        for (auto del_index : delete_list)
        {
            num_list.erase(del_index);
        }
    };
    // 填充下标的vector
    for (int i = 0; i < len; i++)
        num_list.insert(i);
    // 准备好迭代器
    for (auto table: tar_list)
        iter_list.push_back((*table).dict.begin());
    // 归并的循环
    while(!num_list.empty())
    {
        key_min = ULLONG_MAX;
        //遍历待选集合，找一个最小的key
        for (auto i : num_list)
        {
            uint64_t tmp = (*iter_list[i]).key;
            if (tmp == key_min) //如果有出现相同的key，跳过时间戳小的,采用时间戳大的
            {
                if (tar_list[i]->head.time_stamp < tar_list[i_min]->head.time_stamp)
                {
                    iter_list[i]++;
                    checkEnd();
                }else
                {
                    iter_list[i_min]++;
                    checkEnd();
                    i_min = i;
                }
            }else if (tmp < key_min)
            {
                key_min = tmp;
                i_min = i;
            }
        }

        std::string tmp_val = tar_list[i_min]->search(key_min); //去对应的table中找到string

        if (key_min == 0)
        {
            printf("in compaction, the key = %lu, the val = %s\n",key_min, tmp_val.c_str());
        }
//        printf("insert : key = %lu, val = %s",key_min,tmp_val.c_str());
        if (last_level && !strcmp(tmp_val.c_str(),"~DELETED~"))     // 如果是最后一层的操作，且检查到了delete标志，则跳过insert操作
        {
        }else if(com_mem->insert(key_min,tmp_val)==-1)                       //插入这个找到的对
        {   //如果插入导致溢满
//            printf("create new sstable in compaction\n");
            time = time_stamp_label++;
            // 用time作为文件id，就可放到同一个level下
            SSTable * add = new SSTable(com_mem,time,level+1);
            levels[level+1]->heads_in_level.push_back(add);
            com_mem->reset();
            com_mem->insert(key_min,tmp_val);
        }
        iter_list[i_min]++; //对应的迭代器后移一个格子
        checkEnd();
        //最后所有下标从集合中删除，循环结束
    }
    // 处理剩下的未满的一个MEMTable
    if (com_mem->len)
    {
        time = time_stamp_label++;
        SSTable * last_one = new SSTable(com_mem,time,level+1);
        levels[level+1]->heads_in_level.push_back(last_one);
    }
    /** 至此，前面的合并完成，收拾残局 */
    // 原来的table彻底没用了，进行清除
    for (auto table : tar_list)
    {
        table->reset(); //顺带删除原有文件
        delete table;
    }
    delete com_mem;

    // 准备下一层的迭代
    int next_len = levels[level+1]->heads_in_level.size() - (1 << (level+2));   //计算下一层多出来的table的数量
    if (next_len <= 0)  //如果没有多出来的table，则不需要进行迭代
    {return;}
    else {              //有多出来的table，需要找出其中时间戳最小的len个
        // 用来存储下一次迭代需要的队列list;
        std::vector<SSTable*> next_table_list;
        // 因为初始化时，SSTable就严格按照时间戳递增的顺序排列,所以直接取前len个就可以
        std::vector<SSTable*>::iterator iter_list = levels[level+1]->heads_in_level.begin();
        if (cur_level >= level+2)//如果存在下层,则需要维护一个范围，方便从下层取的table
        {
            long long next_tmp_min = LONG_LONG_MAX;
            long long next_tmp_max = LONG_LONG_MIN;
            // 加入上层的几个table
            for (int i = 0; i < next_len; i++){
                // 加入下次迭代的列表中
                SSTable*cur = *iter_list;
                // 更新最大值和最小值
                next_tmp_min = cur->head.min < next_tmp_min ? cur->head.min : next_tmp_min;
                next_tmp_max = cur->head.max > next_tmp_max ? cur->head.max : next_tmp_max;
                next_table_list.push_back(cur);
                // 之后从这一层中删除
                levels[level+1]->heads_in_level.erase(iter_list);
            }
            // 加入下层与范围有交集的table
            for (auto table = levels[level+2]->heads_in_level.begin(); table!=levels[level+2]->heads_in_level.end(); table++)
            {
                if ((*table)->head.min > next_tmp_max || (*table)->head.max < next_tmp_min)
                    continue;   // 逃过一杰
                else {
                    // 加入列表
                    next_table_list.push_back(*table);
                    // 在原来的位置删除
                    levels[level+2]->heads_in_level.erase(table);
                }
            }
        }else{  //没有下层，则可以省去范围的维护
            // 加入上层的几个table
            for (int i = 0; i < next_len; i++){
                // 加入下次迭代的列表中
                SSTable * cur = *iter_list;
                next_table_list.push_back(cur);
                // 之后从这一层中删除
                levels[level+1]->heads_in_level.erase(iter_list);
            }
        }
//        for (auto table : levels[level+1]->heads_in_level)
//        {
//            table_map.insert(std::pair<uint64_t,SSTable*>(table->head.time_stamp,table));
//        }
//        // 从map中取前len个放到next list中
//        long long next_tmp_min = LONG_LONG_MAX;
//        long long next_tmp_max = LONG_LONG_MIN;
//        std::map<uint64_t,SSTable*>::iterator iter_map = table_map.begin();
//        for (int i = 0; i < len; i ++)
//        {
//            SSTable * cur = iter_map->second;
//            next_tmp_min = cur->head.min < next_tmp_min ? cur->head.min : next_tmp_min;
//            next_tmp_max = cur->head.max > next_tmp_max ? cur->head.max : next_tmp_max;
//            next_table_list.push_back(cur);
//            iter_map++;
//            // 确定一个之后，将它从原来所处的level层删除
//            for(std::vector<SSTable*>::iterator it = levels[level+1]->heads_in_level.begin(); it != levels[level+1]->heads_in_level.end(); it++){
//                if (*it == cur)
//                    levels[level+1]->heads_in_level.erase(it);
//                break;
//            }
//        }

        // 调用递归
        compaction(next_table_list,level+1);
    }
    return;
}
