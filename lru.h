#pragma once
#include <unordered_map>
#include <list>
#include <cstdio>
#include <utility>
#include "infoPack.h"

class LRU
{
private:
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> index;
    std::list<std::pair<int, int>> cache;
    unsigned capacity;
    void debugShowElem();
    bool debugFlag = false;

public:
    // 传入缓存最大容量
    LRU(unsigned preset_capacity) { capacity = preset_capacity; }
    // 传入要使用的页
    infoPack usePage(int id);
    // Debug 才用到，每使用一次页就输出一次当前页
    void setDebug() { debugFlag = !debugFlag; }
};

