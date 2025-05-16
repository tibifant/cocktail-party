#pragma once

#include "core.h"
#include "local_list.h"
#include "raw_string.h"

constexpr size_t MaxCocktailCount = 64;

struct cocktail
{
  raw_string name;
  raw_string author_name;
  raw_string instructions;
};

lsResult add_cocktail();
lsResult update_cocktail(const size_t cocktail_id, const cocktail updated_cocktail);
lsResult remove_cocktail(const size_t cocktail_id);

lsResult get_cocktails(_Out_ local_list<cocktail, MaxCocktailCount> &cocktails); // TODO! (include idxs)

void generate_cocktail_name(_Out_ raw_string &name);
void generate_author(_Out_ raw_string &name);
void generate_instructions(_Out_ raw_string &instructions);
