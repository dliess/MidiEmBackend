#include <spdlog/spdlog.h>

#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <map>


struct NoteEvent
{
    int note;
    float velocity;
};

int main()
{
    spdlog::set_level(spdlog::level::trace);

    print_alloc oom{"Out of Memory", std::pmr::null_memory_resource()};

    std::byte stackBuf[20048];
    std::pmr::monotonic_buffer_resource mb_rsrc(stackBuf, sizeof stackBuf, &oom);
    print_alloc print_mb_rsrc("Monotonic", &mb_rsrc);

    std::pmr::unsynchronized_pool_resource unsync_pool({20, 0}, &print_mb_rsrc);
    print_alloc pool("Pool", &unsync_pool);

    std::pmr::map<int, NoteEvent> map(&pool);
    map[23] = NoteEvent{55, 1.0};
    spdlog::trace("------------------");
    map[24] = NoteEvent{55, 1.0};
    spdlog::trace("---Going to erase -------");
    map.erase(24);
    spdlog::trace("------------------");
    for (int i = 0; i < 1024; ++i)
    {
        map[i] = NoteEvent{55, 1.0};   
    }
    spdlog::trace("------------------");

    spdlog::trace("------------------");
}