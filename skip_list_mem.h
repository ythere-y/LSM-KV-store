#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

struct SKNode{
    uint64_t key;
    const std::string * offset;
    std::vector<SKNode*> forward;
    SKNode(){
        key = 0;
    }
    SKNode(const uint64_t &tar){
        key = tar;
    };
    SKNode(const uint64_t &tar, const std::string &s){
        key = tar;
        offset = &s;
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
    int insertNode(const unsigned long long &key,const std::string &s);
    int deleteNode(const unsigned long long &key);
    void printList();
};

#endif // SKIP_LIST_H
