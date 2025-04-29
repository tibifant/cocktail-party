#pragma once

#include "local_list.h"

constexpr size_t MaxNameLength = 256;
constexpr size_t MaxCocktailCount = 64;

struct ingridient
{
  char name[MaxNameLength];
  uint8_t amount;
  char unit[MaxNameLength];
};

constexpr size_t MaxIngridients = 16;

struct cocktail
{
  char name[MaxNameLength];
  local_list<ingridient, MaxIngridients> ingridients;
  char author_surname[MaxNameLength];
  char author_lastname[MaxNameLength];
};

lsResult generate_cocktail();
lsResult update_cocktail(const size_t cocktail_id, const cocktail updated_cocktail);
lsResult remove_cocktail(const size_t cocktail_id);

lsResult get_cocktails(_Out_ local_list<cocktail, MaxCocktailCount> &cocktails);
