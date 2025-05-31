#ifndef KAYAK_STRUCT_DESCRIPTION_HPP
#define KAYAK_STRUCT_DESCRIPTION_HPP

#include <kayak/fixed_string.hpp>

#include <format>
#include <tuple>
#include <type_traits>

namespace kayak
{
template <typename T>
concept member_ptr = std::is_member_pointer_v<T>;

template <std::size_t L, member_ptr M>
struct member_description {
  fixed_string<L> name;
  M member;
};

template <std::size_t N, member_ptr M>
member_description(char const (&name)[N], M member)
  -> member_description<N - 1, M>;

template <member_description... Ms>
struct member_list {
  static constexpr auto size = sizeof...(Ms);
  static constexpr auto tuple = std::tuple{Ms...};
};

template <typename T>
struct struct_description;

template <typename T>
struct is_member_list : std::false_type {};

template <member_description... Ms>
struct is_member_list<member_list<Ms...>> : std::true_type {};

template <typename T>
concept described_struct = is_member_list<
  typename struct_description<std::remove_cv_t<T>>::members>::value;

template <described_struct T>
void for_each_member(auto&& visitor, T& s)
{
  return std::apply(
    [&](auto const&... descriptions) {
      (visitor(as_string_view(descriptions.name),
               std::invoke(descriptions.member, s)),
       ...);
    },
    struct_description<std::remove_cv_t<T>>::members::tuple);
}

template <typename T>
struct type_tag {
  using type = T;
};

template <typename... Ts>
struct type_list {
  using tag_tuple_type = std::tuple<type_tag<Ts>...>;
  static constexpr auto types = tag_tuple_type{};
};

template <typename T>
struct is_type_list : std::false_type {};

template <typename... Ts>
struct is_type_list<type_list<Ts...>> : std::true_type {};

template <typename T>
concept described_struct_with_bases
  = described_struct<T>
    && is_type_list<
      typename struct_description<std::remove_cv_t<T>>::bases>::value;

template <described_struct_with_bases T>
void for_each_base(auto&& visitor, T& s)
{
  using bases = typename struct_description<std::remove_cv_t<T>>::bases;

  std::apply(
    // TODO: Make const-agnostic
    [&](auto const&... base_tags) {
      (visitor(static_cast<std::conditional_t<
                 std::is_const_v<T>,
                 typename std::remove_cvref_t<decltype(base_tags)>::type const&,
                 typename std::remove_cvref_t<decltype(base_tags)>::type&>>(s)),
       ...);
    },
    bases::types);
}

template <typename... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

template <described_struct_with_bases T>
void for_each_member_recurse_bases(auto&& visitor, T& s)
{
  for_each_member(visitor, s);

  for_each_base(overload{[&](described_struct_with_bases auto& base) {
                           for_each_member_recurse_bases(visitor, base);
                         },
                         [&](described_struct auto& base) {
                           for_each_member(visitor, base);
                         }},
                s);
}
} // namespace kayak

template <kayak::described_struct T>
struct std::formatter<T, char> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx)
  {
    // TODO: Consider per-member format specifiers
    return ctx.begin();
  }

  template <typename FormatContext>
  constexpr auto format(T const& s, FormatContext& ctx) const
  {
    auto out = ctx.out();

    std::format_to(out, "{{");

    kayak::for_each_member(
      [&, i = std::size_t{0}](std::string_view const name,
                              auto const& value) mutable {
        out = std::format_to(out, "{}: {}", name, value);

        if (++i < kayak::struct_description<T>::members::size)
          out = std::format_to(out, ", ");
      },
      s);

    std::format_to(out, "}}");

    return out;
  }
};

#endif
