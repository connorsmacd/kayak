#ifndef KAYAK_STD_STRING_HPP
#define KAYAK_STD_STRING_HPP

#include <kayak/std/allocator.hpp>

#include <string>

namespace kayak
{
template <typename CharT>
struct type_description<std::char_traits<CharT>> :
  type_description_impl<"std::char_traits<" + name_of<CharT> + ">",
                        std::char_traits<CharT>> {};

template <typename CharT, typename Traits, typename Allocator>
struct type_description<std::basic_string<CharT, Traits, Allocator>> :
  type_description_impl<"std::basic_string<" + name_of<CharT> + ", "
                          + name_of<Traits> + ", " + name_of<Allocator> + ">",
                        std::basic_string<CharT, Traits, Allocator>> {};
} // namespace kayak

#endif
