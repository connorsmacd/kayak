#include <kayak/type_description.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace kayak::literals;

TEST_CASE("type_description fundamental types", "[type_description]")
{
  STATIC_REQUIRE(kayak::name_of<void> == "void"_fs);
  STATIC_REQUIRE(kayak::name_of<std::nullptr_t> == "std::nullptr_t"_fs);
  STATIC_REQUIRE(kayak::name_of<bool> == "bool"_fs);
  STATIC_REQUIRE(kayak::name_of<char> == "char"_fs);
  STATIC_REQUIRE(kayak::name_of<signed char> == "signed char"_fs);
  STATIC_REQUIRE(kayak::name_of<unsigned char> == "unsigned char"_fs);
  STATIC_REQUIRE(kayak::name_of<wchar_t> == "wchar_t"_fs);
  STATIC_REQUIRE(kayak::name_of<char16_t> == "char16_t"_fs);
  STATIC_REQUIRE(kayak::name_of<char32_t> == "char32_t"_fs);
  STATIC_REQUIRE(kayak::name_of<short int> == "short int"_fs);
  STATIC_REQUIRE(kayak::name_of<unsigned short int> == "unsigned short int"_fs);
  STATIC_REQUIRE(kayak::name_of<int> == "int"_fs);
  STATIC_REQUIRE(kayak::name_of<unsigned int> == "unsigned int"_fs);
  STATIC_REQUIRE(kayak::name_of<long int> == "long int"_fs);
  STATIC_REQUIRE(kayak::name_of<unsigned long int> == "unsigned long int"_fs);
  STATIC_REQUIRE(kayak::name_of<long long int> == "long long int"_fs);
  STATIC_REQUIRE(kayak::name_of<unsigned long long int>
                 == "unsigned long long int"_fs);
  STATIC_REQUIRE(kayak::name_of<float> == "float"_fs);
  STATIC_REQUIRE(kayak::name_of<double> == "double"_fs);
  STATIC_REQUIRE(kayak::name_of<long double> == "long double"_fs);
}

TEST_CASE("type_description basic compound types", "[type_description]")
{
  STATIC_REQUIRE(kayak::name_of<int const> == "int const"_fs);
  STATIC_REQUIRE(kayak::name_of<int volatile> == "int volatile"_fs);
  STATIC_REQUIRE(kayak::name_of<int const volatile> == "int const volatile"_fs);

  STATIC_REQUIRE(kayak::name_of<int&> == "int&"_fs);
  STATIC_REQUIRE(kayak::name_of<int const&> == "int const&"_fs);

  STATIC_REQUIRE(kayak::name_of<int&&> == "int&&"_fs);

  STATIC_REQUIRE(kayak::name_of<int*> == "int*"_fs);
  STATIC_REQUIRE(kayak::name_of<int const*> == "int const*"_fs);
  STATIC_REQUIRE(kayak::name_of<int* const> == "int* const"_fs);
  STATIC_REQUIRE(kayak::name_of<int const* volatile* const volatile>
                 == "int const* volatile* const volatile"_fs);

  STATIC_REQUIRE(kayak::name_of<int[]> == "int[]"_fs);
}
