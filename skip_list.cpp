#include "skip_list.h"
#include <stack>
#include <iostream>
Skip_List::Skip_List()
{
    head = new SKNode(-LONG_LONG_MAX);
    end = new SKNode(LONG_LONG_MAX);
    listLevel = 0;
    head->forward.push_back(end);
}

Skip_List::~Skip_List(){
    delete head;
}

SKNode* Skip_List::searchNode(const long long &key){
    SKNode * cur = head;
    for (int i = listLevel;i>=0;i--){
        while(cur->forward[i]->key != LONG_LONG_MAX && cur->forward[i]->key < key)
            cur = cur->forward[i];
    }
    cur = cur->forward[0];
    if (cur->key == key)
    {
        return cur;
    }
    return nullptr;
}

int Skip_List::insertNode(const long long &key, std::string *s){
    std::stack<SKNode*> store;
    SKNode *cur = head;
    for (int i = listLevel; i>=0 ;i--){
        while(cur->forward[i]->key != LONG_LONG_MAX && cur->forward[i]->key < key){
            cur = cur->forward[i];
        }
        if (cur->forward[i]->key == key){   //如果找到一样的，就放弃
            return 1;
        }else{      //没有找到，就暂时将这一层入栈
            store.push(cur);
        }
        //循环后i--，自动进入下一层
    }
    //成功循环之后，store中必然会有listlevel个node
    //开始插入，随机计算次结点的高度
    SKNode * pre;
    SKNode * add = new SKNode(key,s);
    int cur_level = 0;//从最下层开始
    do{
        pre = store.top();
        store.pop();    //取出栈中第一个
        add->forward.push_back(pre->forward[cur_level]);
        pre->forward[cur_level] = add;  //在该层插入成功
        cur_level++;
    }while(!store.empty() && (rand()%2));   //二分之一的概率上升一层
    if (cur_level == listLevel+1){        //如果目前升到了最高层
        if (rand()%2)   //有概率再升一层
        {
            listLevel++;
            head->forward.push_back(add);
            add->forward.push_back(end);
        }

    }
    return 1;
}
int Skip_List::deleteNode(const long long &key){
    std::stack<SKNode*> store;
    SKNode *cur = head;
    for (int i =  listLevel; i>=0; i--){
        while(cur->forward[i]->key!=LONG_LONG_MAX && cur->forward[i]->key < key)
            cur=cur->forward[i];
        if (cur->forward[i]->key == key){    //找到了一样的值
            store.push(cur);
        }
    }
    if (store.empty())
        return 0;
    int cur_level = 0;
    SKNode *del = store.top()->forward[cur_level];//删除目标点
    SKNode *change;
    while (!store.empty()) {
        change = store.top();
        change->forward[cur_level] = change->forward[cur_level]->forward[cur_level];
        store.pop();
        cur_level ++;
    }
    delete del;
    return 1;   //删除成功，返回1
}


void Skip_List::printList(){
    SKNode * cur = head;
    SKNode * down = head;
    for (int i = listLevel; i >= 0; i --){
        cur = head;
        down = head;
        while(cur->key != LONG_LONG_MAX){
            while(cur->key != down->key){
                down = down->forward[0];
                std::cout<< "\t";
            }
            std::cout<<cur->key<<',';
            cur = cur->forward[i];
        }
        std::cout << "\n";
    }
}
