#ifndef UTIL_FIXED_SIZE_STRING_H
#define UTIL_FIXED_SIZE_STRING_H

#include <array>
#include <cstddef>
#include <algorithm>
#include <string_view>
#include <string>

namespace util
{
template <std::size_t N>
class FixedSizeString
{
private:
    std::array<char, N> m_data{};
    std::size_t m_strLen = 0;
public:
    FixedSizeString() = default;
    FixedSizeString(const char* str) noexcept
    {
      for (std::size_t i = 0; i < N; ++i)
      {
          if (str[i] == '\0' || i == N-1)
          {
              m_data[i] = '\0';
              m_strLen = i-1;
              return;
          }
          m_data[i] = str[i];
          m_strLen = i;
      }
    }

    FixedSizeString(const FixedSizeString& other) noexcept = default;
    // {
    //     m_strLen = other.m_strLen;
    //     std::copy_n(other.m_data.begin(), other.m_strLen + 1, m_data.begin());
    // }
    explicit FixedSizeString(std::string_view strView) noexcept
    {
      m_strLen = std::min(strView.size(), N-1);
      std::copy_n(strView.begin(), m_strLen, m_data.begin());
      m_data[m_strLen] = '\0';
    }
    explicit FixedSizeString(const std::string& str) noexcept : 
         FixedSizeString(std::string_view(str))
    {
    }

    FixedSizeString& operator=(const FixedSizeString& other) noexcept = default;
    // {
    //     m_strLen = other.m_strLen;
    //     std::copy_n(other.m_data.begin(), other.m_strLen + 1, m_data.begin());
    //     return *this;
    // }

    FixedSizeString& operator=(const char* str) noexcept
    {
         for (std::size_t i = 0; i < N; ++i)
         {
               if (str[i] == '\0' || i == N-1)
               {
                  m_data[i] = '\0';
                  m_strLen = i-1;
                  return *this;
               }
               m_data[i] = str[i];
               m_strLen = i;
         }
        return *this;
    }

    [[nodiscard]] const char* c_str() const noexcept { return m_data.data(); }

    [[nodiscard]] std::size_t size() const noexcept { return m_strLen; }

    [[nodiscard]] bool operator==(const FixedSizeString& other) const noexcept
    {
      if (m_strLen != other.m_strLen)
      {
          return false;
      }
      return std::equal(m_data.begin(), m_data.begin() + m_strLen, 
                        other.m_data.begin());
    }

    [[nodiscard]] bool operator==(const char* str) const noexcept
    {
        return std::equal(m_data.begin(), m_data.begin() + m_strLen + 1, str);
    }

    [[nodiscard]] bool operator!=(const FixedSizeString& other) const noexcept
    {
        return !(*this == other);
    }

    [[nodiscard]] bool operator!=(const char* str) const noexcept
    {
        return !(*this == str);
    }

    operator std::string_view() const noexcept
    {
        return std::string_view(m_data.data(), m_strLen);
    }
};
} // namespace util

#endif 
