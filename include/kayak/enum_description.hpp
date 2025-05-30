#ifndef KAYAK_ENUM_DESCRIPTION_HPP
#define KAYAK_ENUM_DESCRIPTION_HPP

#include <algorithm>
#include <array>
#include <ranges>
#include <utility>

namespace kayak
{
template <typename T>
struct enum_description;

template <typename T>
struct is_string_view_enumerator_pair_array : std::false_type {};

template <typename T, std::size_t N>
struct is_string_view_enumerator_pair_array<
  std::array<std::pair<std::string_view, T>, N>> : std::true_type {};

template <typename T>
concept described_enum = is_string_view_enumerator_pair_array<std::remove_cv_t<
  decltype(enum_description<T>::string_view_enumerator_pairs)>>::value;

template <typename T>
constexpr auto to_underlying(T const e) noexcept
{
  return static_cast<std::underlying_type_t<T>>(e);
}

template <described_enum T>
struct described_enum_traits {
  using type = T;
  using description_type = enum_description<type>;

  static constexpr auto string_view_enumerator_pairs
    = description_type::string_view_enumerator_pairs;

  static constexpr auto size = string_view_enumerator_pairs.size();

  static constexpr auto enumerators = [] {
    auto result = std::array<type, size>{};
    std::ranges::copy(string_view_enumerator_pairs | std::views::values,
                      result.begin());
    return result;
  }();

  static constexpr auto enumerator_min = *std::ranges::min_element(enumerators);
  static constexpr auto enumerator_max = *std::ranges::max_element(enumerators);

  static constexpr auto is_contiguous = std::cmp_equal(
    to_underlying(enumerator_max) - to_underlying(enumerator_min) + 1,
    size);
};

template <typename T>
concept continguous_described_enum
  = described_enum<T> && described_enum_traits<T>::is_contiguous;

template <described_enum T>
constexpr auto to_string_view(T const e) -> std::string_view
{
  constexpr auto enumerator_string_view_pairs_sorted = [] {
    auto result = std::array<std::pair<T, std::string_view>,
                             described_enum_traits<T>::size>{};

    std::ranges::copy(described_enum_traits<T>::string_view_enumerator_pairs
                        | std::views::transform([&](auto const& pair) {
                            return std::pair{pair.second, pair.first};
                          }),
                      result.begin());

    std::ranges::sort(result, {}, &std::pair<T, std::string_view>::first);

    return result;
  }();

  auto const it
    = std::ranges::lower_bound(enumerator_string_view_pairs_sorted,
                               e,
                               {},
                               &std::pair<T, std::string_view>::first);

  if (it == enumerator_string_view_pairs_sorted.end() || it->first != e)
    throw std::out_of_range("no string representation for enumerator");

  return it->second;
}

template <continguous_described_enum T>
constexpr auto to_string_view(T const e) -> std::string_view
{
  constexpr auto to_index = [](T const e) {
    return to_underlying(e)
           - to_underlying(described_enum_traits<T>::enumerator_min);
  };

  constexpr auto string_views = [&] {
    auto result
      = std::array<std::string_view, described_enum_traits<T>::size>{};

    for (auto const& [name, enumerator] :
         described_enum_traits<T>::string_view_enumerator_pairs)
      result[to_index(enumerator)] = name;

    return result;
  }();

  return string_views.at(to_index(e));
}

template <described_enum T>
constexpr auto to_string(T const e) -> std::string
{
  return std::string{to_string_view(e)};
}

template <described_enum T>
constexpr auto from_string(std::string_view const s) -> T
{
  constexpr auto string_view_enumerator_pairs_sorted = [] {
    auto result = std::array<std::pair<std::string_view, T>,
                             described_enum_traits<T>::size>{};

    std::ranges::copy(described_enum_traits<T>::string_view_enumerator_pairs,
                      result.begin());

    std::ranges::sort(result, {}, &std::pair<std::string_view, T>::first);

    return result;
  }();

  auto const it
    = std::ranges::lower_bound(string_view_enumerator_pairs_sorted,
                               s,
                               {},
                               &std::pair<std::string_view, T>::first);

  if (it == string_view_enumerator_pairs_sorted.end() || it->first != s)
    throw std::out_of_range("no enumerator for string representation");

  return it->second;
}
} // namespace kayak

#endif
