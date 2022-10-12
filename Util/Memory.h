#ifndef UTIL_MEMORY_H
#define UTIL_MEMORY_H

#include <cstddef>
#include <memory>
#include <memory_resource>
#include <type_traits>

namespace util::pmr
{
using Allocator = std::pmr::polymorphic_allocator<std::byte>;
struct Deleter
{
   Deleter() noexcept : m_pAllocator(nullptr) {}
   Deleter(const Deleter& rhs) noexcept = default;
   Deleter& operator=(const Deleter& rhs) noexcept = default;
   Deleter(Deleter&& rhs) noexcept                 = default;
   Deleter& operator=(Deleter&& rhs) noexcept = default;
   Deleter(Allocator* allocator) noexcept : m_pAllocator(allocator) {}
   void operator()(auto* p) noexcept
   {
      if (m_pAllocator)
         m_pAllocator->delete_object(p);
   }

private:
   Allocator* m_pAllocator;
};

template <typename T> using unique_ptr = std::unique_ptr<T, Deleter>;

template <typename T, typename... Params>
unique_ptr<T> make_unique(Allocator& alloc, Params&&... p) //todo: change order of params and rettype auto
{
   return unique_ptr<T>(alloc.new_object<T>(std::forward<Params>(p)...),
                        Deleter(&alloc));
}

template <typename T>
auto make_unique(T&& rhs, Allocator& alloc)
{
   using DType = std::decay_t<T>;
   return unique_ptr<DType>(alloc.new_object<DType>(std::forward<T>(rhs)),
                        Deleter(&alloc));
}

}   // namespace util::pmr

#endif