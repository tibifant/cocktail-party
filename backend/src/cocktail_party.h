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

void generate_cocktail_name(_Out_ raw_string &name);
void generate_author(_Out_ raw_string &name);
void generate_instructions(_Out_ raw_string &instructions);

lsResult add_cocktail();
lsResult update_cocktail(const size_t cocktail_id, const cocktail updated_cocktail);
lsResult remove_cocktail(const size_t cocktail_id);

lsResult get_cocktails(_Out_ small_list<cocktail> &cocktails);
