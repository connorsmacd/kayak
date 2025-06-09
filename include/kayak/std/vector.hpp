#ifndef KAYAK_STD_VECTOR_HPP
#define KAYAK_STD_VECTOR_HPP

#include <kayak/std/allocator.hpp>
#include <kayak/type_description.hpp>

#include <vector>

namespace kayak
{
template <typename T, typename Allocator>
struct type_description<std::vector<T, Allocator>> :
  type_description_impl<
    "std::vector<" + join(fixed_string{", "}, name_of<T>, name_of<Allocator>)
      + ">",
    std::vector<T, Allocator>> {};
} // namespace kayak

#endif // KAYAK_STD_VECTOR_HPP
