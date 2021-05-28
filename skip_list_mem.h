#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
//#include <io.h>
#include <string.h>

struct SKNode_mem{
    long long key;
    std::string offset;
    std::vector<SKNode_mem*> forward;
    SKNode_mem(){
        key = 0;
    }
    SKNode_mem(const long long &tar){
        key = tar;
    };
    SKNode_mem(const long long &tar,const std::string &s){
        key = tar;
        offset = s;
    }
};

class Skip_List_mem
{
//private:
public:
    SKNode_mem *head;
    SKNode_mem *end;
    int listLevel;
    int len;
public:
    Skip_List_mem();
    ~Skip_List_mem();
    std::string searchNode(const long long &key);
    int insertNode(const long long &key,const std::string &s);
    uint32_t deleteNode(const long long &key);
    void printList();
    uint32_t size(){return this->len;};
};

#endif // SKIP_LIST_H
