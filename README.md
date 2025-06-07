# kayak

A C++20 reflection library with minimal macro usage.

**NOTE: Kayak is likely to change substantially as it is under active development.**

## Example Usage

### Describing Structs

```c++
#include <kayak/struct_description.hpp>

#include <format>
#include <iostream>

struct sample_struct {
  int i{};
  double d{};
  char c{};
  bool b{};
};

template <>
struct kayak::struct_description<sample_struct> {
  using members = kayak::member_list<{"i", &sample_struct::i},
                                     {"d", &sample_struct::d},
                                     {"c", &sample_struct::c},
                                     {"b", &sample_struct::b}>;
};

int main()
{
  static constexpr auto s
    = sample_struct{.i = 42, .d = 3.14, .c = 'x', .b = true};

  std::cout << std::format("{}", s) << '\n';
}
```

Prints:

```text
{i: 42, d: 3.14, c: x, b: true}
```
