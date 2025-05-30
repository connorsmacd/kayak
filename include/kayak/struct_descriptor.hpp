#ifndef KAYAK_STRUCT_DESCRIPTOR_HPP
#define KAYAK_STRUCT_DESCRIPTOR_HPP

#include <kayak/fixed_string.hpp>

#include <tuple>
#include <type_traits>

namespace kayak
{
template <typename T>
concept member_ptr = std::is_member_pointer_v<T>;

template <std::size_t N, member_ptr M>
struct member_descriptor {
  fixed_string<N> name;
  M member;
};

template <std::size_t N, member_ptr M>
member_descriptor(char const (&name)[N], M member) -> member_descriptor<N, M>;

template <member_descriptor... Ms>
struct member_list {
  static constexpr auto tuple = std::tuple{Ms...};
};

template <typename T>
struct struct_descriptor;

template <typename T>
struct is_member_list : std::false_type {};

template <member_descriptor... Ms>
struct is_member_list<member_list<Ms...>> : std::true_type {};

template <typename T>
concept described_struct
  = is_member_list<typename struct_descriptor<T>::members>::value;
} // namespace kayak

#endif
