#pragma once

#include "core.h"
#include "small_list.h"
#include "raw_string.h"

struct cocktail
{
  raw_string title;
  raw_string author_name;
  raw_string instructions;
};

struct cocktail_info
{
  size_t id;
  raw_string title;
};

lsResult get_cocktails(_Out_ small_list<cocktail_info> &list);
lsResult get_cocktail(const size_t id, _Out_ cocktail *pCocktail);
lsResult add_cocktail(_Out_ size_t &id);
lsResult update_cocktail(const size_t id);
lsResult remove_cocktail(const size_t id);
