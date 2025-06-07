#ifndef KAYAK_TYPE_LIST_HPP
#define KAYAK_TYPE_LIST_HPP

#include <kayak/type_description.hpp>

#include <tuple>

namespace kayak
{
template <described_type... Ts>
struct type_list {
  using description_tuple_type = std::tuple<type_description<Ts>...>;
  static constexpr auto descriptions = description_tuple_type{};
};

template <typename T>
struct is_type_list : std::false_type {};

template <described_type... Ts>
struct is_type_list<type_list<Ts...>> : std::true_type {};
} // namespace kayak

#endif
