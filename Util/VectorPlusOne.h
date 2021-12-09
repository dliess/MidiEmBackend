#ifndef VECTOR_PLUS_ONE_H
#define VECTOR_PLUS_ONE_H

#include <vector>

namespace util
{

template <typename T> class VectorPlusOne
{
public:
   using ContainerT      = std::vector<T>;
   using value_type      = typename ContainerT::value_type;
   using reference       = value_type&;
   using const_reference = const value_type&;
   using size_type       = std::size_t;
   using iterator        = typename ContainerT::iterator;
   using const_iterator  = typename ContainerT::const_iterator;

   VectorPlusOne() = default;
   VectorPlusOne(size_t size) : m_data(size + 1) {}
   [[nodiscard]] const_reference operator[](int i) const noexcept { return m_data[i + 1]; }
   [[nodiscard]] reference operator[](int i) noexcept { return m_data[i + 1]; }
   [[nodiscard]] const_reference at(int i) const { return m_data.at(i + 1); }
   [[nodiscard]] reference at(int i) { return m_data.at(i + 1); }

   [[nodiscard]] size_type size() const noexcept { return std::distance(m_data.begin(), m_data.end()); };

   [[nodiscard]] iterator begin() noexcept { return m_data.begin(); }
   [[nodiscard]] iterator end() noexcept { return m_data.end(); }
   [[nodiscard]] const_iterator begin() const noexcept { return m_data.begin(); }
   [[nodiscard]] const_iterator end() const noexcept { return m_data.end(); }

   

private:
   ContainerT m_data;
};

}   // namespace util

#endif