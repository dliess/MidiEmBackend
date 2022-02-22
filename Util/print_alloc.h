#ifndef UTIL_PRINT_ALLOC_H
#define UTIL_PRINT_ALLOC_H

#include "spdlog/spdlog.h"
#include <memory_resource>
#include <cassert>

namespace util 
{
// thanks to Rahil Baber
// Prints if new/delete gets used.
class PrintAlloc : public std::pmr::memory_resource {
 public:
  PrintAlloc(std::string name, std::pmr::memory_resource* upstream)
      : m_name(std::move(name)), m_upstream(upstream) {
    assert(upstream);
  }

 private:
  std::string m_name;
  std::pmr::memory_resource* m_upstream;

  void* do_allocate(std::size_t bytes, std::size_t alignment) override {
    spdlog::info( "[{} (alloc)] Size: {} Alignment: {} ...", m_name, bytes,
                  alignment);
    auto result = m_upstream->allocate(bytes, alignment);
    spdlog::info( "[{} (alloc)] ... Address: {}", m_name, result);
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
    spdlog::info( 
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

} // namespace util
#endif