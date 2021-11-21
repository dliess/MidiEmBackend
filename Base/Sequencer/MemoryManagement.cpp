#include <spdlog/spdlog.h>

#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory_resource>

// thanks to Rahil Baber
// Prints if new/delete gets used.
class print_alloc : public std::pmr::memory_resource {
 public:
  print_alloc(std::string name, std::pmr::memory_resource* upstream)
      : m_name(std::move(name)), m_upstream(upstream) {
    assert(upstream);
  }

 private:
  std::string m_name;
  std::pmr::memory_resource* m_upstream;

  void* do_allocate(std::size_t bytes, std::size_t alignment) override {
    spdlog::trace("[{} (alloc)] Size: {} Alignment: {} ...", m_name, bytes,
                  alignment);
    auto result = m_upstream->allocate(bytes, alignment);
    spdlog::trace("[{} (alloc)] ... Address: {}", m_name, result);
    return result;
  }

  std::string format_destroyed_bytes(std::byte* p, const std::size_t size) {
    std::string result = "";
    bool in_string = false;

    auto format_char = [](bool& in_string, const char c, const char next) {
      auto format_byte = [](const char byte) {
        return fmt::format(" {:02x}", static_cast<unsigned char>(byte));
      };

      if (std::isprint(static_cast<int>(c))) {
        if (!in_string) {
          if (std::isprint(static_cast<int>(next))) {
            in_string = true;
            return fmt::format(" \"{}", c);
          } else {
            return format_byte(c);
          }
        } else {
          return std::string(1, c);
        }
      } else {
        if (in_string) {
          in_string = false;
          return '"' + format_byte(c);
        }
        return format_byte(c);
      }
    };

    std::size_t pos = 0;
    for (; pos < std::min(size - 1, static_cast<std::size_t>(32)); ++pos) {
      result += format_char(in_string, static_cast<char>(p[pos]),
                            static_cast<char>(p[pos + 1]));
    }
    result += format_char(in_string, static_cast<char>(p[pos]), 0);
    if (in_string) {
      result += '"';
    }
    if (pos < (size - 1)) {
      result += " <truncated...>";
    }
    return result;
  }

  void do_deallocate(void* p, std::size_t bytes,
                     std::size_t alignment) override {
    spdlog::trace(
        "[{} (dealloc)] Address: {} Dealloc Size: {} Alignment: {} Data: {}",
        m_name, p, bytes, alignment,
        format_destroyed_bytes(static_cast<std::byte*>(p), bytes));
    m_upstream->deallocate(p, bytes, alignment);
  }

  bool do_is_equal(
      const std::pmr::memory_resource& other) const noexcept override {
    return this == &other;
  }
};


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