#include "lru.h"
#include <utility>
#include <cstdio>

void LRU::debugShowElem()
{
	std::printf("\n============\n( ");
	for (auto elem : cache)
	{
		std::printf("%d ", elem);
	}
	std::printf(")\n============\n");
}

infoPack LRU::usePage(int id)
{
    infoPack flagPack;
	auto target = index.find(id);
	if (target != index.end())
	{
		// ƥ��
		// ������ҳ�ƶ�������
		cache.push_front(*(target->second));
		cache.erase(target->second);
		target->second = cache.begin();


		if (debugFlag)
		{
			debugShowElem();
		}
        flagPack.lackFlag = 0;
		// һ������������ cache.size() <= capacity
		// �����е�����һ�����ᳬ���������
        // ���������ִ������Ĳ���
	}
    else
    {
        // ��ƥ��
        // ������뵽 HashMap ��
        cache.push_front(id);
        std::pair<int, std::list<int>::iterator> tempPair(id, cache.begin());
        index.emplace(tempPair);
        if (cache.size() > capacity)
        {
            // �����е������������������
            target = index.find(cache.back());
            cache.pop_back();
            index.erase(target);
        }


        if (debugFlag)
        {
            debugShowElem();
        }
        flagPack.lackFlag = 1;
    }
    flagPack.cache = cache;
    return flagPack;
}
