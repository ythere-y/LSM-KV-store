#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

struct SKNode_mem{
    uint64_t key;
    const std::string * offset;
    std::vector<SKNode_mem*> forward;
    SKNode_mem(){
        key = 0;
    }
    SKNode_mem(const uint64_t &tar){
        key = tar;
    };
    SKNode_mem(const uint64_t &tar, const std::string &s){
        key = tar;
        offset = &s;
    }
};

class Skip_List_mem
{
private:
    SKNode_mem *head;
    SKNode_mem *end;
    int listLevel;
public:
    Skip_List_mem();
    ~Skip_List_mem();
    SKNode_mem*searchNode(const unsigned long long &key);
    int insertNode(const unsigned long long &key,const std::string &s);
    int deleteNode(const unsigned long long &key);
    void printList();
};

#endif // SKIP_LIST_H
