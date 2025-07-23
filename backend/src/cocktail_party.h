#pragma once

#include "core.h"
#include "small_list.h"
#include "raw_string.h"

struct cocktail
{
  raw_string name;
  raw_string author_name;
  raw_string instructions;
};

lsResult get_cocktails(_Out_ small_list<std::tuple<size_t, raw_string>> &cocktailInfos);
lsResult get_cocktail(const size_t id, _Out_ cocktail &c);
lsResult add_cocktail(_Out_ size_t &id, _Out_ raw_string &name);
lsResult update_cocktail(const size_t id);
lsResult remove_cocktail(const size_t id);

