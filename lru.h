#pragma once
#include <unordered_map>
#include <list>
#include <cstdio>
#include "infoPack.h"

class LRU
{
private:
	std::unordered_map<int, std::list<int>::iterator> index;
	std::list<int> cache;
	unsigned capacity;
	void debugShowElem();
	bool debugFlag = false;

public:
	// ���뻺���������
	LRU(unsigned preset_capacity) { capacity = preset_capacity; }
	// ����Ҫʹ�õ�ҳ
    infoPack usePage(int id);
	// Debug ���õ���ÿʹ��һ��ҳ�����һ�ε�ǰҳ
	void setDebug() { debugFlag = !debugFlag; }
};
