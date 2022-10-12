#ifndef UTIL_MEMORY_H
#define UTIL_MEMORY_H

#include <cstddef>
#include <memory>
#include <memory_resource>
#include <type_traits>

namespace util::pmr
{

using DefaultAllocator = std::pmr::polymorphic_allocator<std::byte>;
struct Deleter
{
   Deleter() noexcept : m_pMemoryResource(nullptr) {}
   Deleter(const Deleter& rhs) noexcept = default;
   Deleter& operator=(const Deleter& rhs) noexcept = default;
   Deleter(Deleter&& rhs) noexcept                 = default;
   Deleter& operator=(Deleter&& rhs) noexcept = default;
   template<typename Allocator>
   Deleter(Allocator& allocator) noexcept : m_pMemoryResource(allocator.resource()) {}
   void operator()(auto* p) noexcept
   {
      if (m_pMemoryResource)
         DefaultAllocator(m_pMemoryResource).delete_object(p);
   }

private:
   std::pmr::memory_resource* m_pMemoryResource;
};

template <typename T> using unique_ptr = std::unique_ptr<T, Deleter>;

template <typename T, typename Allocator, typename... Params>
auto make_unique(Params&&... p, const Allocator& alloc)
{
   return unique_ptr<T>(DefaultAllocator(alloc.resource()).new_object<T>(std::forward<Params>(p)...),
                        Deleter(alloc));
}

template <typename T, typename Allocator>
auto make_unique(T&& rhs, const Allocator& alloc)
{
   using DType = std::decay_t<T>;
   return unique_ptr<DType>(DefaultAllocator(alloc.resource()).new_object<DType>(std::forward<T>(rhs)),
                        Deleter(alloc));
}

}   // namespace util::pmr

#endif