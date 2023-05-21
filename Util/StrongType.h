#ifndef UTIL_STRONG_TYPE_H
#define UTIL_STRONG_TYPE_H

#include <fmt/format.h>

namespace util
{
template <typename T, typename Tag, template <typename> class... Skills>
struct StrongType : private Skills<StrongType<T, Tag, Skills...> >...
{
   using value_type = T;
   explicit StrongType(const T& val) : val_(val) {}
   T& get() { return val_; }
   const T& get() const { return val_; }

private:
   T val_;
};

template <typename Derived> struct Addable
{
   friend Derived& operator+=(Derived& lhs, const Derived& rhs)
   {
      lhs.get() += rhs.get();
      return lhs;
   }
   friend Derived operator+(const Derived& lhs, const Derived& rhs)
   {
      return Derived{lhs.get() + rhs.get()};
   }
};
template <typename Derived> struct Substractable
{
   friend Derived& operator-=(Derived& lhs, const Derived& rhs)
   {
      lhs.get() -= rhs.get();
      return lhs;
   }
   friend Derived operator-(const Derived& lhs, const Derived& rhs)
   {
      return Derived{lhs.get() - rhs.get()};
   }
};
template <typename Derived> struct Multipliable
{
   friend Derived& operator*=(Derived& lhs, const Derived& rhs)
   {
      lhs.get() *= rhs.get();
      return lhs;
   }
   friend Derived operator*(const Derived& lhs, const Derived& rhs)
   {
      return Derived{lhs.get() * rhs.get()};
   }
};
template <typename Derived> struct Dividable
{
   friend Derived& operator/=(Derived& lhs, const Derived& rhs)
   {
      lhs.get() /= rhs.get();
      return lhs;
   }
   friend Derived operator/(const Derived& lhs, const Derived& rhs)
   {
      return Derived{lhs.get() / rhs.get()};
   }
};
template <typename Derived> struct Printable
{
   friend std::ostream& operator<<(std::ostream& os, const Derived& d)
   {
      os << d.get();
      return os;
   }
};
template <typename Derived> struct Swappable
{
   friend void swap(Derived& lhs, Derived& rhs) noexcept
   {
      using std::swap;   // Enable ADL
      swap(lhs.get(), rhs.get());
   }
};

}   // namespace util

#define MAKE_FMT_CONVERTIBLE(StrongType)                                      \
   template <>                                                                \
struct fmt::formatter<StrongType>                                             \
       : formatter<StrongType::value_type>{auto format(StrongType c,          \
                                                       format_context & ctx){ \
             return formatter<StrongType::value_type>::format(c.get(), ctx);  \
   }                                                                          \
   }                                                                          \
   ;

#endif