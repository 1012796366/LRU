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
		// 匹配
		// 将所用页移动到顶端
		cache.push_front(*(target->second));
		cache.erase(target->second);
		target->second = cache.begin();

		if (debugFlag)
		{
			debugShowElem();
		}
        flagPack.lackFlag = 0;
		// 一经擦除，必有 cache.size() <= capacity
		// 缓存中的容量一定不会超过最大容量
        // 因此无需再执行下面的擦除
	}
    else
    {
        // 不匹配
        // 将其插入到 HashMap 中
        // cache.push_front(std::make_pair<int, int>(id, ));
        cache.push_front(std::make_pair(id, (cache.size() == capacity ? cache.back().second : cache.size())));
        std::pair<int, std::list<std::pair<int, int>>::iterator> tempPair(id, cache.begin());
        index.emplace(tempPair);
        if (cache.size() > capacity)
        {
            // 缓存中的容量超过了最大容量
            target = index.find(cache.back().first);
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
