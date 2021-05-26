#ifndef BITMAP_H
#define BITMAP_H

#include <stdio.h>
#include <cstring>
class BitMap{
private:
    const int size = 10240;
public:
    char *bitmap;
    BitMap(){
        bitmap = new char[size];
        memset(bitmap,0x0,size*sizeof(char));
    }
    /*
     * 将index位设为1
     */
    int bitmapSet(unsigned int index){
        index = index % (size*8);
        unsigned int addr = index/8; // 确定需要取出哪个字节块
        unsigned int addroffset = index % 8;
        unsigned char temp = 0x1 << addroffset;
        bitmap[addr] |= temp;
        return 1;
    }

    /*
     * 查询index位是否为1
     */
    bool bitmapGet(unsigned int index){
        index = index % (size * 8);
        unsigned int addr = index/8;
        unsigned int addroffset = index % 8;
        unsigned char temp = 0x1 << addroffset;
        return (bitmap[addr] & temp);
    }
};

#endif // BITMAP_H
