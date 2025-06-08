#ifndef KAYAK_STRUCT_UPDATE_HPP
#define KAYAK_STRUCT_UPDATE_HPP

#include <kayak/struct_description.hpp>

namespace kayak
{
template <typename T>
struct member_object_pointer_traits;

template <typename Member, typename Class>
struct member_object_pointer_traits<Member(Class::*)> {
  using member_type = Member;
};

namespace detail
{
template <described_struct T,
          fixed_string Name,
          std::size_t I = 0,
          bool IsMember
          = std::get<I>(struct_description<T>::members::tuple).name == Name>
struct member_by_name_traits;

template <described_struct T, fixed_string Name, std::size_t I>
struct member_by_name_traits<T, Name, I, true> {
  static constexpr auto member_description
    = std::get<I>(struct_description<T>::members::tuple);
  static constexpr auto member = member_description.member;

  using type = member_object_pointer_traits<
    std::remove_cv_t<decltype(member)>>::member_type;
};

template <described_struct T, fixed_string Name, std::size_t I>
struct member_by_name_traits<T, Name, I, false> :
  member_by_name_traits<T, Name, I + 1> {};
} // namespace detail

template <described_struct T, fixed_string Name>
static constexpr auto struct_has_member
  = struct_description<T>::members::template contains<Name>;


template <described_struct T>
class struct_update {
public:
  using struct_type = T;

  explicit constexpr struct_update(struct_type s) : s_{std::move(s)} {}

  template <fixed_string Name, typename T>
  constexpr auto with(T&&) -> struct_update&
  {
    static_assert(false, "invalid member name");
  }

  template <fixed_string Name, typename T>
    requires(struct_has_member<struct_type, Name>)
  constexpr auto with(T&& new_value) -> struct_update<struct_type>&
  {
    std::invoke(detail::member_by_name_traits<struct_type, Name>::member, s_)
      = std::forward<T>(new_value);

    return *this;
  }

  constexpr auto operator()() const -> struct_type { return s_; }

private:
  struct_type s_;
};
} // namespace kayak

#endif
