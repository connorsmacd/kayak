#ifndef KAYAK_STD_ALLOCATOR_HPP
#define KAYAK_STD_ALLOCATOR_HPP

#include <kayak/type_description.hpp>

#include <memory>

namespace kayak
{
template <typename T>
struct type_description<std::allocator<T>>
  : type_description_impl<"std::allocator<" + name_of<T> + ">",
                          std::allocator<T>>
{
};
}

#endif
