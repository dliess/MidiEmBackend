#ifndef UTIL_VECTOR_INDEX_IN_RANGE_H
#define UTIL_VECTOR_INDEX_IN_RANGE_H

namespace util
{

template<typename Container>
bool vector_index_in_range(int index, const Container& container)
{
   return (index >= 0) && (index < container.size());
}

} // namespace util

#endif