#include <kayak/std/string.hpp>
#include <kayak/struct_update.hpp>

#include <format>
#include <iostream>

using namespace std::string_literals;

struct user {
  bool active{};
  std::string username;
  std::string email;
  unsigned long long int sign_in_count{};
};

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
  auto const user0 = user{.active = true,
                          .username = "someusername123"s,
                          .email = "someone@example.com"s,
                          .sign_in_count = 1};

  auto const user1
    = kayak::struct_update(user0).with<"email">("another@example.com"s)();

  std::cout << std::format("{}\n", user1);
}
