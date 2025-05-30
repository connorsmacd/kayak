#ifndef KAYAK_TEST_ASSERT_HPP
#define KAYAK_TEST_ASSERT_HPP

#ifndef NDEBUG
#  undef NDEBUG
#  include <cassert>
#  define NDEBUG
#else
#  include <cassert>
#endif

#endif
