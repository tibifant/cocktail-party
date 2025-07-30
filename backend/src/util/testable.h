#pragma once

#include "core.h"

struct _testable_init { size_t instanceId; };

typedef lsResult(*testable_func)();
_testable_init register_testable(const char *name, testable_func func);
lsResult run_testables();

#ifdef _MSC_VER
#define _MSVC_PRAGMA(...) __pragma(__VA_ARGS__)
#else
#define _MSVC_PRAGMA(...)
#endif

#define DEFINE_TESTABLE(name) \
  lsResult test_ ## name(); \
  struct test_ ## name ## obj \
  { _testable_init __test_init_; \
    inline test_ ## name ## obj() : __test_init_(register_testable(#name, & test_ ## name)) { } \
  }; \
  _MSVC_PRAGMA(comment(linker, "/include:__test__" #name "__ref")) \
  extern "C" auto __test__ ## name ## __ref = test_ ## name ## obj(); \
  lsResult test_ ## name()

template <int64_t n>
struct register_testable_files { static void handle(); };

template <>
struct register_testable_files<-1> { inline static void handle() {} };

#define REGISTER_TESTABLE_FILE(n) template <> struct register_testable_files<n>{ static void handle(); };\
   void register_testable_files<n>::handle() { register_testable_files<n - 1>::handle(); }

constexpr int64_t testable_file_count = 0; // <-- INCREMENT, when new tests are added.

template <typename T>
inline void testable_print_value_of_type(const T &v)
{
  constexpr size_t max_size = 16;

  for (size_t i = 0; i < max_size && i < sizeof(T); i++)
  {
    uint8_t byte;
    memcpy(&byte, reinterpret_cast<const uint8_t *>(&v) + i, sizeof(byte));
    print(' ', FX(Min(2), Fill0)(byte));
  }

  if constexpr (sizeof(T) > max_size)
    print(" ...");
}

inline void testable_print_value_of_type(const unsigned char v) { print(v, " | 0x", FX()(v)); }
inline void testable_print_value_of_type(const signed char v) { print(v, " | 0x", FX()(v)); }
inline void testable_print_value_of_type(const char v) { print('\'', v, "' | 0x", FX()(v)); }
inline void testable_print_value_of_type(const unsigned short v) { print(v, " | 0x", FX()(v)); }
inline void testable_print_value_of_type(const signed short v) { print(v, " | 0x", FX()(v)); }
inline void testable_print_value_of_type(const unsigned int v) { print(v, " | 0x", FX()(v)); }
inline void testable_print_value_of_type(const signed int v) { print(v, " | 0x", FX()(v)); }
inline void testable_print_value_of_type(const unsigned long v) { print(v, " | 0x", FX()(v)); }
inline void testable_print_value_of_type(const signed long v) { print(v, " | 0x", FX()(v)); }
inline void testable_print_value_of_type(const unsigned long long v) { print(v, " | 0x", FX()(v)); }
inline void testable_print_value_of_type(const signed long long v) { print(v, " | 0x", FX()(v)); }
inline void testable_print_value_of_type(const char *v) { print(v); }

#ifdef _MSC_VER
inline void testable_print_value_of_type(const wchar_t *v) { print(v); }
#endif

inline void testable_print_value_of_type(const float_t v) { print(FF(AllFrac)(v), " | ", FF(Exp)(v)); }
inline void testable_print_value_of_type(const double_t v) { print(FD(AllFrac)(v), " | ", FD(Exp)(v)); }
inline void testable_print_value_of_type(const bool v) { print(v ? "true" : "false"); }

inline void testable_print_value_of_type(const lsResult v) { print(lsResult_to_string(v)); }

template <typename T>
inline void testable_print_value(const T &v, const char *desc)
{
  lsSetConsoleColor(lsCC_DarkCyan, lsCC_Black);
  print("\tvalue '", FS(desc, Max(36), Min(36)), "': ");
  lsSetConsoleColor(lsCC_BrightCyan, lsCC_Black);
  testable_print_value_of_type(v);
  lsSetConsoleColor(lsCC_DarkCyan, lsCC_Black);
  print("\t ('", typeid(T).name(), "' at 0x", FX()(reinterpret_cast<size_t>(&v)), ")\n");
  lsResetConsoleColor();
}

#define TESTABLE_RETURN_SUCCESS() do { result = lsR_Success; goto epilogue; } while (0)
#define TESTABLE_RETURN_FAILURE() do { result = lsR_Failure; goto epilogue; } while (0)

#define TESTABLE_FAIL() \
  do \
  { print_error_line("Test Failed\n at ", LS_FUNCTION_NAME, "\n in File '" __FILE__ "' Line " LS_STRINGIFY_VALUE(__LINE__) ".\n"); \
    __debugbreak(); \
      TESTABLE_RETURN_FAILURE(); \
  } while(0)

#define TESTABLE_ASSERT_EQUAL(a, b) \
  do \
  { const auto &a_ = (a); \
    const auto &b_ = (b); \
    \
    if (!(a_ == b_)) \
    { print_error_line("Test Failed\n on '" #a " == " #b "'\n at ", LS_FUNCTION_NAME, "\n in File '" __FILE__ "' Line " LS_STRINGIFY_VALUE(__LINE__) ".\n"); \
      testable_print_value(a_, #a); \
      testable_print_value(b_, #b); \
      __debugbreak(); \
      TESTABLE_RETURN_FAILURE(); \
    } \
  } while(0)

#define TESTABLE_ASSERT_TRUE(a) TESTABLE_ASSERT_EQUAL(a, true)
#define TESTABLE_ASSERT_FALSE(a) TESTABLE_ASSERT_EQUAL(a, false)

#define TESTABLE_ASSERT_NOT_EQUAL(a, b) \
  do \
  { const auto &a_ = (a); \
    const auto &b_ = (b); \
    \
    if (!(a_ != b_)) \
    { print_error_line("Test Failed\n on '" #a " != " #b "'\n at ", LS_FUNCTION_NAME, "\n in File '" __FILE__ "' Line " LS_STRINGIFY_VALUE(__LINE__) ".\n"); \
      testable_print_value(a_, #a); \
      testable_print_value(b_, #b); \
      __debugbreak(); \
      TESTABLE_RETURN_FAILURE(); \
    } \
  } while(0)

#define TESTABLE_ASSERT_SUCCESS(functionCall) \
  do \
  { const lsResult __result = (functionCall); \
    \
    if (LS_FAILED(__result)) \
    { print_error_line("Test Failed on 'LS_FAILED(" #functionCall ")'\n with Result ", lsResult_to_string(__result), " (0x", FX()(__result), ")\n at ", LS_FUNCTION_NAME, "\n in File '" __FILE__ "' Line " LS_STRINGIFY_VALUE(__LINE__) ".\n"); \
      __debugbreak(); \
      TESTABLE_RETURN_FAILURE(); \
    } \
  } while(0)

#define TESTABLE_ASSERT_FAILURE(functionCall) \
  do \
  { const lsResult __result = (functionCall); \
    \
    if (LS_SUCCESS(__result)) \
    { print_error_line("Test Failed on 'LS_SUCCESS(" #functionCall ")'\n with Result ", lsResult_to_string(__result), " (0x", FX()(__result), ")\n at ", LS_FUNCTION_NAME, "\n in File '" __FILE__ "' Line " LS_STRINGIFY_VALUE(__LINE__) ".\n"); \
      __debugbreak(); \
      TESTABLE_RETURN_FAILURE(); \
    } \
  } while(0)

inline bool testable_FloatInRange(const double_t expected, const double_t value, const double_t variance)
{
  return variance >= lsAbs(value - expected);
}

inline bool testable_FloatEquals(const double_t expected, const double_t value)
{
  return testable_FloatInRange(expected, value, 4.0 * (double_t)lsMax(lsSmallest<float_t>(), lsSmallest<float_t>((float_t)lsAbs(expected)) + lsSmallest<float_t>((float_t)lsAbs(value))));
}

#define TESTABLE_FLOAT_IN_RANGE(expected, value, variance) testable_FloatInRange((expected), (value), (variance))
#define TESTABLE_FLOAT_EQUALS(expected, value) testable_FloatEquals((expected), (value))
#define TESTABLE_ASSERT_FLOAT_EQUAL(a, b) \
  do \
  { const auto &a_ = (a); \
    const auto &b_ = (b); \
    \
    if (!(testable_FloatEquals(a_, b_))) \
    { print_error_line("Test Failed\n on '" #a " != " #b "'\n at ", LS_FUNCTION_NAME, "\n in File '" __FILE__ "' Line " LS_STRINGIFY_VALUE(__LINE__) ".\n"); \
      testable_print_value(a_, #a); \
      testable_print_value(b_, #b); \
      __debugbreak(); \
      TESTABLE_RETURN_FAILURE(); \
    } \
  } while(0)

#define TESTABLE_ASSERT_FLOAT_IN_RANGE(a, b, variance) \
  do \
  { const auto &a_ = (a); \
    const auto &b_ = (b); \
    \
    if (!(testable_FloatInRange(a_, b_, variance))) \
    { print_error_line("Test Failed\n on '" #a " != " #b "' (with range '" #variance "')\n at ", LS_FUNCTION_NAME, "\n in File '" __FILE__ "' Line " LS_STRINGIFY_VALUE(__LINE__) ".\n"); \
      testable_print_value(a_, #a); \
      testable_print_value(b_, #b); \
      __debugbreak(); \
      TESTABLE_RETURN_FAILURE(); \
    } \
  } while(0)
