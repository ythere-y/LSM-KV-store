#ifndef BITMAP_H
#define BITMAP_H

#include <stdio.h>
#include <cstring>

class BitMap{
private:
    char *bitmap;
    int size;
public:
    BitMap(){
        bitmap = nullptr;
        size = 0;
    }
    /*
     * 用size B的大小来初始化
     */
    BitMap(int size){
        bitmap = nullptr;
        bitmap = new char[size];
        if (bitmap == nullptr)
            printf("ErroR In BitMap Constractor!\n");
        else{
            memset(bitmap,0x0,size*sizeof(char));
            this->size = size;
        }
    }
    /*
     * 用size B的大小来初始化
     */
    int initBitMap(int size){
        bitmap = nullptr;
        bitmap = new char[size];
        if (bitmap == nullptr){
            printf("ErroR In BitMap Constractor!");
            return 0;
        }else {
            memset(bitmap,0x0,size*sizeof(char));
            this->size=size;
            return this->size;
        }
    }
    /*
     * 将index位设为1
     */
    int bitmapSet(int index){
        int addr = index/8; // 确定需要取出哪个字节块
        int addroffset = index % 8;
        unsigned char temp = 0x1 << addroffset;
        bitmap[addr] |= temp;
        return 1;
    }

    /*
     * 查询index位是否为1
     */
    bool bitmapGet(int index){
        int addr = index/8;
        int addroffset = index % 8;
        unsigned char temp = 0x1 << addroffset;
        return (bitmap[addr] & temp);
    }
};

#endif // BITMAP_H
