#ifndef ARENA_H
#define ARENA_H

#include <vector>
#include <cstdint>
#include <string>
static const unsigned int kBlockSize = 4096; // 定义了一个内存块的大小
// 代码源自leveldb/util/arena.h
// Arena类定义
class Arena {
    // 把成员变量提到前面注释，否则好多函数没法注释了
private:
    // Arena管理的内存是由一个个的内存块组成，每个内存块大小由常数kBlockSize定义,当申请的内存超过一个阈值时
    // 触发合并条件。否则多次申请的内存会复用一个内存块,只是每次使用的内存块上的不同地址。
    char* alloc_ptr_;                   // 指向最新内存块剩余空间的首地址
    std::vector<char*> blocks_;         // 所有申请的内存块都通过std::vector管理
    uint32_t memory_remaining_;         // 记录最新内存块还有多少剩余空间可用
    uint32_t memory_usage_;             // 内存使用量统计，采用原子指针实现，其实就是原子操作整型数据
public:
    // 构造函数，因为初始化的时候没有内存块，指针为空，剩余可用的大小为0
    Arena()  : memory_usage_(0) {
        alloc_ptr_ = NULL;
        memory_remaining_ = 0;
    }
    // 析构函数
    ~Arena() {
        // 释放所有的内存块
        for (size_t i = 0; i < blocks_.size(); i++) {
            delete[] blocks_[i];    //这是一个char数组，要用delete[]来删除
        }
    }
    // 申请一个bytes大小的内存空间
    int insert(const std::string &s){
        unsigned int len = s.size();
        if (len+1 > memory_remaining_)
            return -1;  //余量不足，返回-1，提示
        for (unsigned int i = 0 ; i < len ;i++){
            *alloc_ptr_ = s[i];
            alloc_ptr_ ++;
        }
        *alloc_ptr_ = '\0';
        alloc_ptr_ ++;

        return 1;
    }

    char* Allocate(size_t bytes) {
        assert(bytes > 0);
        // 看看最新申请的内存块中剩余空间是否够用？
        if (bytes <= alloc_bytes_remaining_) {
            // 如果够用，那就在最新的内存块中拨相应大小的内存出去，操作很简单，指针偏移，剩余空间减去申请量
            char* result = alloc_ptr_;
            alloc_ptr_ += bytes;
            alloc_bytes_remaining_ -= bytes;
            return result;
        }
        // 最新的内存块剩余空间不够那就只能重新申请内存块了，下面会对AllocateFallback()进行详细说明
        return AllocateFallback(bytes);
    }
    // 相比于Allocate()，申请地址要对齐，其实我们用new()/malloc()申请的内存一般都是8字节对齐的
    // 所以有些情况是需要按照地址对齐方式申请内存的。
    char* AllocateAligned(size_t bytes) {
        // 默认是8字节对齐，当然如果64位以上的指针那就按照系统提供的方式对齐
        const int align = (sizeof(void*) > 8) ? sizeof(void*) : 8;
        // 对齐长度必须是2的指数值，比如8、16、32
        assert((align & (align-1)) == 0);
        // 获取当前最新内存块可用空间的首地址现对于对齐地址偏移了多少
        size_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr_) & (align-1);
        // 因为分配的地址要对齐，所以要计算出来要将当前可用地址再偏移多少才能对齐
        size_t slop = (current_mod == 0 ? 0 : align - current_mod);
        // 所以实际申请的内存大小要加上对齐偏移量
        size_t needed = bytes + slop;
        char* result;
        // 下面的部分和Allocate()一样了。
        if (needed <= alloc_bytes_remaining_) {
            result = alloc_ptr_ + slop; // 要把地址偏移到对齐的地方
            alloc_ptr_ += needed;
            alloc_bytes_remaining_ -= needed;
        } else {
            // 因为申请新的内存块的首地址肯定是对齐的，所以大小就要调整回来了
            result = AllocateFallback(bytes);
        }
        assert((reinterpret_cast<uintptr_t>(result) & (align-1)) == 0);
        return result;
    }
    // 获取内存使用量，还记得MemTable.ApproximateMemoryUsage()么？就是通过这个函数实现的
    // 上面的各种申请函数貌似我们没有看到memory_usage_的统计，其实这个统计是在申请内存块的时候统计的
    size_t MemoryUsage() const {
        return reinterpret_cast<uintptr_t>(memory_usage_.NoBarrier_Load());
    }

private:
    // 禁止拷贝构造函数和赋值操作
    Arena(const Arena&);
    void operator=(const Arena&);
};

#endif // ARENA_H
