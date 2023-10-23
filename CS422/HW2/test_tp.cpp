#include "target_predictor.hpp"

void check_cache()
{
    Cache cache = Cache(7, 4);
    std::cout << std::hex;
    CADDRINT addr = 0x12345678;
    CADDRINT target = 0x000fff00;
    cache.show_index(addr);
    Target tg = cache.get_target(addr);
    cache.show_index(addr);
    cache.update_target(addr, target);
    cache.show_index(addr);
    addr = 0x22345678;
    target = 0x000eee00;
    tg = cache.get_target(addr);
    cache.show_index(addr);
    cache.update_target(addr, target);
    cache.show_index(addr);
    addr = 0x32345678;
    target = 0x000ddd00;
    tg = cache.get_target(addr);
    cache.show_index(addr);
    cache.update_target(addr, target);
    cache.show_index(addr);
    addr = 0x42345678;
    target = 0x000ccc00;
    tg = cache.get_target(addr);
    cache.show_index(addr);
    cache.update_target(addr, target);
    cache.show_index(addr);
    addr = 0x52345678;
    target = 0x000bbb00;
    tg = cache.get_target(addr);
    cache.show_index(addr);
    cache.update_target(addr, target);
    cache.show_index(addr);
}

int main()
{
    BTB btb = BTB();
    CADDRINT addr;
    CADDRINT target;
    addr = 0x12345600;
    target = 0x1;
    btb.predict(addr, target);
    btb.cache_block->show_index(addr);
    addr = 0x22345600;
    target = 0x2;
    btb.predict(addr, target);
    btb.cache_block->show_index(addr);
    addr = 0x12345600;
    target = 0x1;
    btb.predict(addr, target);
    btb.cache_block->show_index(addr);
    return 0;
}
