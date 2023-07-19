#include <iostream>
#include <memory.h>
#include <memory>

namespace { //=================================================================

namespace _1 {

struct CacheLine{
    uint32_t lineOffset:6;
    uint32_t lineAddress:8;
    uint32_t tag:18;
};

void updateCacheLine(void* address, CacheLine* cache)
{
    uint64_t value = (uint64_t)address;
    cache->tag = (value & 0xffffc000) >> 14;
    cache->lineAddress = (value & 0x3fc0) >> 6;
    cache->lineOffset = value & 0x3f;
}

void test()
{
    auto s = std::make_shared<uint32_t>();
    CacheLine cache;
    updateCacheLine(s.get(), &cache);

    std::cout << std::hex;
    std::cout << "pointer: " << s.get() << "\n";
    std::cout << "tag: " << cache.tag << ", address: " << cache.lineAddress << ", offset: " << cache.lineOffset;
    std::cout << std::endl;
}

} //_1 --------------------------------------------------------------

} //===========================================================================
void test_cacheline()
{
    _1::test();
}
