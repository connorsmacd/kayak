#ifndef KAYAK_STD_OPTIONAL_HPP
#define KAYAK_STD_OPTIONAL_HPP

#include <kayak/type_description.hpp>

#include <optional>

namespace kayak
{
template <typename T>
struct type_description<std::optional<T>>
  : type_description_impl<"std::optional<" + name_of<T> + ">", std::optional<T>>
{
};
}

#endif
