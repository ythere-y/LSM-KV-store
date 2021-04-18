#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

struct SKNode{
    uint64_t key;
    uint32_t offset;
    std::vector<SKNode*> forward;
    SKNode(){
        key = 0;
    }
    SKNode(const uint64_t &tar){
        key = tar;
    };
    SKNode(const uint64_t &tar, const uint32_t _offset){
        key = tar;
        offset = _offset;
    }
};

class Skip_List
{
private:
    SKNode *head;
    SKNode *end;
    int listLevel;
public:
    Skip_List();
    ~Skip_List();
    SKNode*searchNode(const unsigned long long &key);
    int insertNode(const unsigned long long &key,const uint32_t &s);
    int deleteNode(const unsigned long long &key);
    void printList();
};

#endif // SKIP_LIST_H
