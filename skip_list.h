#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include <cstdio>
#include <cstdlib>
#include <vector>
struct SKNode{
    long long key;
    std::vector<SKNode*> forward;
    SKNode(){
        key = 0;
    }
    SKNode(const long long &tar){
        key = tar;
    };
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
    SKNode*searchNode(const long long &key);
    int insertNode(const long long &key);
    int deleteNode(const long long &key);
    void printList();
};

#endif // SKIP_LIST_H
