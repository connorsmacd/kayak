# kayak

A C++20 reflection library with minimal macro usage.

**NOTE: Kayak is likely to change substantially as it is under active development.**

## Features

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

  std::cout << std::format("{}\n", s);
}
```

Prints:

```text
{i: 42, d: 3.14, c: x, b: true}
```

### Struct Update

We often want to create a new struct instance from an existing instance that keeps most of the members the same, but changes some. In C++, this is can be difficult to express elegantly:

```c++
using namespace std::string_literals;

struct user {
  bool active{};
  std::string username;
  std::string email;
  unsigned long long int sign_in_count{};
};

int main()
{
  auto const user1 = user{.active = true,
                          .username = "someusername123"s,
                          .email = "someone@example.com"s,
                          .sign_in_count = 1};

  // Succinct, but instance is not const.
  {
    auto user2 = user1;
    user2.email = "another@example.com"s;
  }

  // Instance is const, but verbose and breaks if new members are added.
  {
    auto const user2 = user{.active = user1.active,
                            .username = user1.username,
                            .email = "another@example.com"s,
                            .sign_in_count = user1.sign_in_count};
  }

  // Instance is const and solution is future-proof, but verbose.
  {
    auto const user2 = [&] {
      auto result = user1;
      result.email = "another@example.com"s;
      return result;
    }();
  }
}
```

Rust, and other languages, have language support for this kind of operation. Rust calls this "Struct Update Syntax":

```rust
struct User {
    active: bool,
    username: String,
    email: String,
    sign_in_count: u64,
}

fn main() {
    let user1 = User {
        active: true,
        username: String::from("someusername123"),
        email: String::from("someone@example.com"),
        sign_in_count: 1,
    };

    let user2 = User {
        email: String::from("another@example.com"),
        ..user1
    };
}
```

We can achieve something similar to this syntax using `kayak::struct_update`:

```c++
#include <kayak/std/string.hpp>
#include <kayak/struct_update.hpp>

// ...

namespace kayak
{
template <>
struct struct_description<user> {
  using members = member_list<{"active", &user::active},
                              {"username", &user::username},
                              {"email", &user::email},
                              {"sign_in_count", &user::sign_in_count}>;
};
} // namespace kayak

int main()
{
  auto const user1 = user{.active = true,
                          .username = "someusername123"s,
                          .email = "someone@example.com"s,
                          .sign_in_count = 1};

  auto const user2
    = kayak::struct_update(user1).with<"email">("another@example.com"s)();

  std::cout << std::format("{}\n", user2);
}
```

Prints:

```text
{active: true, username: someusername123, email: another@example.com, sign_in_count: 1}
```
