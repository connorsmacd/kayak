#ifndef KAYAK_FIXED_STRING_HPP
#define KAYAK_FIXED_STRING_HPP

#include <cstddef>

namespace kayak
{
template <std::size_t L>
struct fixed_string {
  char data[L + 1]{};
};

template <std::size_t N>
fixed_string(char const (&c_str)[N]) -> fixed_string<N - 1>;
} // namespace kayak

#endif
