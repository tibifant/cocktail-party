#pragma once

#include "core.h"

struct raw_string
{
  char *text = nullptr;
  size_t bytes = 0;
  size_t capacity = 0;

  inline raw_string() = default;
  inline raw_string(const raw_string &) = delete;
  inline raw_string &operator =(const raw_string &) = delete;
  inline raw_string(raw_string &&);
  inline raw_string &operator =(raw_string &&);
  inline ~raw_string();
};

lsResult string_reserve(raw_string &str, const size_t bytes);
lsResult string_append(raw_string &str, const char *text, const size_t bytes);

inline lsResult string_append(raw_string &str, const raw_string &other)
{
  return string_append(str, other.text, other.bytes);
}

template <typename T>
inline lsResult string_append(raw_string &str, T text)
{
  return string_append(str, text, lsStringLength(text));
}

void string_destroy(raw_string &str);

inline raw_string::raw_string(raw_string &&mv) : text(mv.text), bytes(mv.bytes), capacity(mv.capacity)
{
  mv.text = nullptr;
  mv.bytes = 0;
  mv.capacity = 0;
}

inline raw_string &raw_string::operator=(raw_string &&mv)
{
  std::swap(text, mv.text);
  std::swap(bytes, mv.bytes);
  std::swap(capacity, mv.capacity);

  lsFreePtr(&mv.text);
  mv.bytes = mv.capacity = 0;

  return *this;
}

inline raw_string::~raw_string()
{
  string_destroy(*this);
}
