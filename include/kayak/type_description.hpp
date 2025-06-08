#ifndef KAYAK_TYPE_DESCRIPTION_HPP
#define KAYAK_TYPE_DESCRIPTION_HPP

#include <kayak/fixed_string.hpp>

namespace kayak
{
template <typename T>
struct type_description;

template <fixed_string Name, typename T>
struct type_description_impl {
  static constexpr auto name = Name;
  using type = T;
};

template <typename T, template <typename> typename Description>
concept described_type_impl
  = std::is_same_v<typename Description<T>::type, T>
    && is_fixed_string<std::remove_cv_t<decltype(Description<T>::name)>>::value;

template <typename T>
concept described_type = described_type_impl<T, type_description>;

template <described_type T>
static constexpr auto name_of = type_description<T>::name;

template <>
struct type_description<void> : type_description_impl<"void", void> {};

template <>
struct type_description<std::nullptr_t> :
  type_description_impl<"std::nullptr_t", std::nullptr_t> {};

template <>
struct type_description<bool> : type_description_impl<"bool", bool> {};

template <>
struct type_description<char> : type_description_impl<"char", char> {};

template <>
struct type_description<signed char> :
  type_description_impl<"signed char", signed char> {};

template <>
struct type_description<unsigned char> :
  type_description_impl<"unsigned char", unsigned char> {};

template <>
struct type_description<wchar_t> : type_description_impl<"wchar_t", wchar_t> {};

template <>
struct type_description<char16_t> :
  type_description_impl<"char16_t", char16_t> {};

template <>
struct type_description<char32_t> :
  type_description_impl<"char32_t", char32_t> {};

template <>
struct type_description<short int> :
  type_description_impl<"short int", short int> {};

template <>
struct type_description<unsigned short int> :
  type_description_impl<"unsigned short int", unsigned short int> {};

template <>
struct type_description<int> : type_description_impl<"int", int> {};

template <>
struct type_description<unsigned int> :
  type_description_impl<"unsigned int", unsigned int> {};

template <>
struct type_description<long int> :
  type_description_impl<"long int", long int> {};

template <>
struct type_description<unsigned long int> :
  type_description_impl<"unsigned long int", unsigned long int> {};

template <>
struct type_description<long long int> :
  type_description_impl<"long long int", long long int> {};

template <>
struct type_description<unsigned long long int> :
  type_description_impl<"unsigned long long int", unsigned long long int> {};

template <>
struct type_description<float> : type_description_impl<"float", float> {};

template <>
struct type_description<double> : type_description_impl<"double", double> {};

template <>
struct type_description<long double> :
  type_description_impl<"long double", long double> {};

// TODO: The east-const here might be controversial. Consider making this
// configurable.
template <described_type T>
struct type_description<T const> :
  type_description_impl<name_of<T> + " const", T const> {};

template <described_type T>
struct type_description<T volatile> :
  type_description_impl<name_of<T> + " volatile", T volatile> {};

template <described_type T>
struct type_description<T const volatile> :
  type_description_impl<name_of<T> + " const volatile", T const volatile> {};

template <described_type T>
struct type_description<T&> : type_description_impl<name_of<T> + "&", T&> {};

template <described_type T>
struct type_description<T&&> : type_description_impl<name_of<T> + "&&", T&&> {};

template <described_type T>
struct type_description<T*> : type_description_impl<name_of<T> + "*", T*> {};

template <described_type T>
struct type_description<T[]> : type_description_impl<name_of<T> + "[]", T[]> {};

template <described_type T, std::size_t N>
struct type_description<T[N]> :
  type_description_impl<name_of<T> + "[" + as_fixed_string<N> + "]", T[N]> {};
} // namespace kayak

#endif
