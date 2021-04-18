#include "skip_list_mem.h"
#include <stack>
#include <iostream>
Skip_List_mem::Skip_List_mem()
{
    head = new SKNode_mem(-LONG_LONG_MAX);
    end = new SKNode_mem(LONG_LONG_MAX);
    listLevel = 0;
    len = 0;
    head->forward.push_back(end);
}

Skip_List_mem::~Skip_List_mem(){
    delete head;
}

SKNode_mem* Skip_List_mem::searchNode(const unsigned long long &key){
    SKNode_mem * cur = head;
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

int Skip_List_mem::insertNode(const unsigned long long &key,const std::string &s){
    std::stack<SKNode_mem*> store;
    SKNode_mem *cur = head;
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
    len ++;
    //成功循环之后，store中必然会有listlevel个node
    //开始插入，随机计算次结点的高度
    SKNode_mem * pre;
    SKNode_mem * add = new SKNode_mem(key,s);
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
uint32_t Skip_List_mem::deleteNode(const unsigned long long &key){
    if (len == 0)
        return 0;
    std::stack<SKNode_mem*> store;
    SKNode_mem *cur = head;
    for (int i =  listLevel; i>=0; i--){
        while(cur->forward[i]->key!=LONG_LONG_MAX && cur->forward[i]->key < key)
            cur=cur->forward[i];
        if (cur->forward[i]->key == key){    //找到了一样的值
            store.push(cur);
        }
    }
    if (store.empty())
        return 0;

    len --;
    int cur_level = 0;
    SKNode_mem *del = store.top()->forward[cur_level];//删除目标点
    SKNode_mem *change;
    uint32_t res_len = del->offset->size();
    while (!store.empty()) {
        change = store.top();
        change->forward[cur_level] = change->forward[cur_level]->forward[cur_level];
        store.pop();
        cur_level ++;
    }
    delete del;
    return res_len;   //删除成功，返回被删除的string的长度
}


void Skip_List_mem::printList(){
    SKNode_mem * cur;
    SKNode_mem * down;
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
