#ifndef KAYAK_STD_ARRAY_HPP
#define KAYAK_STD_ARRAY_HPP

#include <kayak/type_description.hpp>

#include <array>

namespace kayak
{
template <typename T, std::size_t N>
struct type_description<std::array<T, N>> :
  type_description_impl<"std::array<"
                          + join(", ", name_of<T>, as_fixed_string<N>) + ">",
                        std::array<T, N>> {};
} // namespace kayak

#endif
