#include "cocktail_party.h"

#include "pool.h"
#include <mutex>

#include <mutex>

pool<cocktail> _Cocktails;
static std::mutex _ThreadLock;

//////////////////////////////////////////////////////////////////////////

static const char *Units[] =
{ "g", "mg", "kg", "cg", "dg", "hg", "dag", "t", "oz", "lb", "st", "long ton", "short ton", "L", "mL", "m\xc2\xb3", "cm\xc2\xb3", "dm\xc2\xb3", "hL", "dL", "cL", "gal", "qt", "pt", "gi", "min", "bbl", "tsp", "tbsp", "c", "pt", "qt", "gal", "fl oz", "in\xc2\xb3", "ft\xc2\xb3", "yd\xc2\xb3", "whole", "bowl(s)" };

static const char *AuthorFirstnames[] = { "Nelson", "Igor", "Evgeniy", "Adriana", "Gary", "Moses", "Agnes", "Cesar", "Xiaoli", "Usha", "Alfredo", "Isaac", "Luiz", "Josephine", "Krishna", "Michael", "Juan", "Anna", "Mary", "Jean", "Robert", "Daniel", "Luis", "Carlos", "James", "Antonio", "Joseph", "Hui", "Elena", "Francisco", "Carmen", "Ghulam", "Barbara", "Angel", "Vladimir", "G\xc3\xbcnther", "Kai", "Rainer", "Hans", "Jesus", "Donald", "Caren", "Captain", "Doctor", "Professor", "Mister", "Miss", "Lady", "Sir", "Richard", "Dolly", "Angela", "Marie", "Sarah", "Frank", "Harry", "Giga", "Madonna", "Jan-Phillip", "Annegret", "Alonso", "Horst", "Stina", "Uschi", "Manfred", "Albert", "Celine", "Taylor" };

static const char *AuthorLastnames[] = { "Wang", "Walker", "Clark", "Lewis", "Hosen", "Diarra", "Franco", "Moyo", "Watson", "Hughes", "Gomez", "Schneider", "Fischer", "Petrov", "Meyer", "Weber", "Thomas", "Ferrari", "Lauch", "Trump", "Stalin", "Jinping", "Jong Un", "Putin", "Nero", "the Hun", "Pinochet", "Honecker", "Napoleon", "Lenin", "Castro", "Mussolini", "Piranha", "Lewis", "Merkel", "Curie", "Connor", "Cena", "Duck", "Sinatra", "Styles", "Chad", "Krampf-Kabrackenbauer", "Mozart", "Spears", "Dion", "Swift", "Newton", "Einstein" };

static const char *IngridientPrefixes[] = { "Aged", "Amber", "Black", "Blonde", "Bold", "Bourbon", "Brut", "Caramel", "Cherry", "Citrus", "Creamy", "Dark", "Dry", "Elderflower", "Extra", "Fruity", "Golden", "Green", "Honey", "Ice", "Jamaican", "Lemon", "Light", "Lime", "Mango", "Mellow", "Mint", "Moscow", "Navy", "Old", "Orange", "Peach", "Pineapple", "Raspberry", "Red", "Rum", "Smooth", "Spiced", "Strawberry", "Sweet", "Tequila", "Tropical", "Vanilla", "Vermouth", "Vodka", "White", "Whiskey", "Wild", "Winter", "Young", "French", "Beach", "Hot", "Indonesian", "Alaska", "Florida", "Barbados", "Mallorca", "Flavor", "Fidji", "Tiki", "Caribbean", "Mulled" };

static const char *NonBeverageAttributes[] = { "horned", "organic", "pickled", "juicy", "wild", "spiced", "nutty", "spicy", "peeled", "ground", "frozen", "caramelized" };

static const char *NonBeverages[] = { "Apple", "Banana", "Orange", "Grape", "Lemon", "Lime", "Cherry", "Peach", "Pear", "Pineapple", "Plum", "Strawberry", "Blueberry", "Raspberry", "Blackberry", "Watermelon", "Cantaloupe", "Honeydew", "Kiwi", "Mango", "Papaya", "Guava", "Passion fruit", "Dragon fruit", "Lychee", "Rambutan", "Durian", "Jackfruit", "Starfruit", "Coconut", "Starfruit", "Soursop", "Cherimoya", "Sapote", "Longan", "Mangosteen", "Pitaya", "Acerola", "Tamarind", "Breadfruit", "Grapefruit", "Tangerine", "Clementine", "Mandarin", "Pomelo", "Ugli fruit", "Bergamot", "Kumquat", "Yuzu", "Calamondin", "Cranberry", "Gooseberry", "Boysenberry", "Loganberry", "Mulberry", "Elderberry", "Huckleberry", "Salal berry", "Cloudberry", "Sea buckthorn", "Apricot", "Nectarine", "Plumcot", "Aprium", "Peento", "Sloe", "Damson", "Mirabelle", "Greengage", "Salak", "Melon", "Casaba", "Canary", "Crenshaw", "Galia", "Honeydew", "Santa Claus", "Snake", "Sugar Baby", "Tigger", "Akee", "Atemoya", "Custard apple", "Feijoa", "Raisin", "Prune", "Apricot", "Fig", "Date", "Cranberry", "Apple", "Pear", "Mango", "Pineapple", "Carrot", "Potato", "Tomato", "Cucumber", "Lettuce", "Spinach", "Broccoli", "Cauliflower", "Cabbage", "Bell pepper", "Onion", "Garlic", "Zucchini", "Eggplant", "Green bean", "Pea", "Corn", "Radish", "Celery", "Beet", "Kale", "Swiss chard", "Collard greens", "Arugula", "Bok choy", "Endive", "Escarole", "Fennel", "Mustard greens", "Turnip greens", "Sweet potato", "Parsnip", "Turnip", "Rutabaga", "Ginger", "Horseradish", "Jicama", "Daikon", "Burdock", "Celeriac", "Brussels sprouts", "Kale", "Cabbage", "Broccoli rabe", "Kohlrabi", "Radicchio", "Watercress", "Butternut squash", "Acorn squash", "Spaghetti squash", "Pumpkin", "Delicata squash", "Hubbard squash", "Turban squash", "Leek", "Shallot", "Chive", "Green onion", "Ramps", "Elephant garlic", "Tomatillo", "Poblano pepper", "Jalape\xc3\xb1o pepper", "Habanero pepper", "Eggplant", "Ground cherry", "Okra", "Artichoke", "Fiddlehead ferns", "Nopales", "Kohlrabi", "Daikon", "Jicama", "Lotus root", "Bitter melon", "Chinese eggplant", "Button mushroom", "Cremini mushroom", "Portobello mushroom", "Shiitake mushroom", "Oyster mushroom", "Enoki mushroom", "Maitake mushroom", "Chanterelle mushroom", "Morel mushroom", "Truffle", "Basil", "Parsley", "Cilantro", "Dill", "Rosemary", "Thyme", "Oregano", "Sage", "Tarragon", "Chives", "Snow pea", "Sugar snap pea", "Fava bean", "Lima bean", "Black-eyed pea", "Chickpea", "Lentil", "Almond", "Cashew", "Hazelnut", "Peanut", "Pecan", "Pistachio", "Walnut", "Chestnut", "Macadamia", "Pine nut", "Apple Pie", "Cupcake", "Caramel", "Honey", "Raisin", "Vanilla", "Pepper", "Salt", "Black Pepper", "Garlic Powder", "Onion Powder", "Paprika", "Cumin", "Coriander", "Turmeric", "Chili Powder", "Oregano", "Basil", "Thyme", "Rosemary", "Sage", "Cinnamon", "Nutmeg", "Cloves", "Allspice", "Ginger", "Mustard Powder", "Cardamom", "Fennel", "Star Anise", "Anise", "Fenugreek", "Mustard", "Cayenne", "Red Pepper Flakes", "White Pepper", "Green Peppercorns", "Dill", "Parsley", "Mint", "Lemon Grass", "Lemon Zest", "Lime Zest" };

static const char *Beverages[] = { "Water", "Coffee", "Tea", "Milk", "Buttermilk", "Juice", "Soda", "Cola", "Ginger Ale", "Water", "Lemonade", "Iced tea", "Hot chocolate", "Smoothie", "Hot cocoa", "Chai", "Horchata", "Kombucha", "Matcha", "Mate", "Beer", "Wine", "Champagne", "Vodka", "Gin", "Rum", "Tequila", "Whiskey", "Bourbon", "Scotch", "Brandy", "Cognac", "Absinthe", "Mezcal", "Sake", "Pisco", "Ouzo", "Raki", "Soju", "Baijiu", "Poire Williams", "Liqueur", "Armagnac", "Calvados", "Triple Sec", "Ros\xc3\xa9", "Soy Sauce", "Terriyaki", "White Wine", "Red Wine", "Prosecco", "Sherry", "Port Wine", "Merlot", "Cider", "Jenever", "Shochu", "Moonshine", "Korn", "Maotai", "Grappa", "Himbeergeist", "Kefir", "Ayran", "Schnapps", "Syrup", "Cointreau", "Limoncello", "Umeshu", "Grand Manier", "Pastis", "Amaretto", "Mist" };

static const char *CocktailAttributesSingle[] = { "on Ice", "on the Rocks" };

static const char *CocktailAttributesPrefix[] = { "\xc3\xa1 la", "de" };

static const char *Instructions[] = { "Measure", "Peel", "Slice", "Infuse", "Ground", "Blend", "Grate", "Half", "Drink", "Eat", "Mash", "Stir", "Chop", "Shake", "Dice", "Cook", "Reduce", "Boil", "Puree", "Mix", "Toss", "Grill", "Saut\xc3\xa9", "Roast", "Steam", "Deglaze", "Smoke", "Toast", "Juice", "Zest", "Whisk", "Open", "Caramelize" };

static const char *SingleInstructions[] = { "Cool to absolute Zero.", "Mix with Ice 3.", "Bake for 20 minutes.", "Bring to a boil.", "Reduce to half.", "Deglaze the pan.", "Add a pinch of Salt.", "Shake the mixture.", "Let it sit for 1 hour.", "Cool down to room temperature.", "Refrigerate over night.", "Precool the glass." };

static const char *InstructionAdverbs[] = { "with", "using", "atop", "in front of", "in" };

static const char *CookingUtensils[] = { "Pan", "Oven", "Mixer", "Shaker", "Knive", "Grater", "Stove", "Whisk", "Blender", "Spoon", "Fork", "Glass", "Corkscrew", "Can Openener", "Cutting Board", "Skillet", "Baking Sheet", "Pie Dish" };

static const char *InstructionEndings[] = { "Serve cold.", "Serve with Lime Juice.", "Serve immediately!", "Immediately serve in a Mug.", "Enjoy!", "Pour into a Martini Glass and serve.", "Garnish with an Olive and serve.", "Serve while stirring thoroughly.", "Serve on Ice.", "Serve warm.", "Serve on a warm summer night.", "Enjoy with friends!", "Bon Appetit!", "Decorate with a Cocktail Umbrella and serve." };

lsResult generate_cocktail_title(_Out_ raw_string &title, rand_seed &seed)
{
  lsResult result = lsR_Success;

  constexpr size_t ChanceIngridientAttribute = 40;
  constexpr size_t ChanceCocktailAttribute = 10;

  uint64_t rnd = lsGetRand(seed);

  const size_t idxIngridientPrefixA = rnd % LS_ARRAYSIZE(IngridientPrefixes);
  rnd >>= 5;
  const size_t randChanceIngridientAttribute = rnd % 100;
  rnd >>= 5;
  const size_t idxIngridientAttribute = rnd % LS_ARRAYSIZE(NonBeverageAttributes);
  rnd >>= 5;
  const size_t idxNonBeverage = rnd % LS_ARRAYSIZE(NonBeverages);
  rnd >>= 5;
  const size_t idxIngridientPrefixB = rnd % LS_ARRAYSIZE(IngridientPrefixes);
  rnd >>= 5;
  const size_t idxBeverage = rnd % LS_ARRAYSIZE(Beverages);
  rnd >>= 5;
  const size_t randChanceCocktailAttribute = rnd % 100;

  // choose random prefix
  LS_ERROR_CHECK(string_append(title, IngridientPrefixes[idxIngridientPrefixA]));
  LS_ERROR_CHECK(string_append(title, " "));

  // choose attribute with chance
  if (randChanceIngridientAttribute < ChanceIngridientAttribute)
  {
    LS_ERROR_CHECK(string_append(title, NonBeverageAttributes[idxIngridientAttribute]));
    LS_ERROR_CHECK(string_append(title, " "));
  }

  // choose random non beverage ingridient
  LS_ERROR_CHECK(string_append(title, NonBeverages[idxNonBeverage]));
  LS_ERROR_CHECK(string_append(title, " "));

  // choose random prefix
  LS_ERROR_CHECK(string_append(title, IngridientPrefixes[idxIngridientPrefixB]));
  LS_ERROR_CHECK(string_append(title, " "));

  // choose random beverage
  LS_ERROR_CHECK(string_append(title, Beverages[idxBeverage]));

  // choose either single or prefix attribute with chance (if prefix -> choose ingridient)
  if (randChanceCocktailAttribute < ChanceCocktailAttribute)
  {
    rnd >>= 5;
    const bool chanceCocktailAttributeSingle = rnd & 1ULL;

    LS_ERROR_CHECK(string_append(title, " "));

    if (chanceCocktailAttributeSingle)
    {
      const size_t idxCocktailAttribute = rnd % LS_ARRAYSIZE(CocktailAttributesSingle);
      LS_ERROR_CHECK(string_append(title, CocktailAttributesSingle[idxCocktailAttribute]));
    }
    else
    {
      const size_t idxCocktailAttribute = rnd % LS_ARRAYSIZE(CocktailAttributesPrefix);
      rnd >>= 5;
      const size_t idxNonBeverageB = rnd % LS_ARRAYSIZE(NonBeverages);

      LS_ERROR_CHECK(string_append(title, CocktailAttributesPrefix[idxCocktailAttribute]));
      LS_ERROR_CHECK(string_append(title, " "));
      LS_ERROR_CHECK(string_append(title, NonBeverages[idxNonBeverageB]));
    }
  }

epilogue:
  return result;
}

lsResult generate_author(_Out_ raw_string &name, rand_seed &seed)
{
  lsResult result = lsR_Success;

  constexpr uint8_t ChanceSecondName = 35;

  uint64_t rnd = lsGetRand(seed);
  const size_t idxFirstName = rnd % LS_ARRAYSIZE(AuthorFirstnames);
  rnd >>= 5;
  const size_t randChanceSecondName = rnd % 100;
  rnd >>= 5;
  const size_t idxSecondName = rnd % LS_ARRAYSIZE(AuthorFirstnames);
  rnd >>= 5;
  const size_t idxLastName = rnd % LS_ARRAYSIZE(AuthorLastnames);

  // choose random first name
  LS_ERROR_CHECK(string_append(name, AuthorFirstnames[idxFirstName]));
  LS_ERROR_CHECK(string_append(name, " "));

  // choose second name with chance
  if (randChanceSecondName < ChanceSecondName)
  {
    LS_ERROR_CHECK(string_append(name, AuthorFirstnames[idxSecondName]));
    LS_ERROR_CHECK(string_append(name, " "));
  }

  // choose random lastname
  LS_ERROR_CHECK(string_append(name, AuthorLastnames[idxLastName]));

epilogue:
  return result;
}

lsResult generate_measurement(_Out_ raw_string &text, rand_seed &seed)
{
  lsResult result = lsR_Success;

  constexpr size_t MaxAmount = 200;

  uint64_t rnd = lsGetRand(seed);

  const size_t amount = rnd % MaxAmount;
  rnd >>= 5;

  const size_t idxUnit = rnd % LS_ARRAYSIZE(Units);
  rnd >>= 5;

  // amount
  LS_ERROR_CHECK(string_append(text, sformat(amount)));
  LS_ERROR_CHECK(string_append(text, " "));

  // unit
  LS_ERROR_CHECK(string_append(text, Units[idxUnit]));

epilogue:
  return result;
}

lsResult generate_ingridient(_Out_ raw_string &text, rand_seed &seed)
{
  lsResult result = lsR_Success;

  constexpr size_t BeverageChance = 40;
  constexpr size_t NonBeverageAttributesChance = 10;

  uint64_t rnd = lsGetRand(seed);

  const size_t chanceBeverage = rnd % 100;
  rnd >>= 5;

  // choose ingridient
  if (chanceBeverage < BeverageChance)
  {
    const size_t idxBeverage = rnd % LS_ARRAYSIZE(Beverages);

    LS_ERROR_CHECK(string_append(text, Beverages[idxBeverage]));
  }
  else
  {
    const size_t chanceNonBeverageAttribute = rnd % 100;
    rnd >>= 5;

    const size_t idxNonBeverageAttributes = rnd % LS_ARRAYSIZE(NonBeverageAttributes);
    rnd >>= 5;

    const size_t idxNonBeverage = rnd % LS_ARRAYSIZE(NonBeverages);

    // choose prefix with chance
    if (chanceNonBeverageAttribute < NonBeverageAttributesChance)
    {
      LS_ERROR_CHECK(string_append(text, NonBeverageAttributes[idxNonBeverageAttributes]));
      LS_ERROR_CHECK(string_append(text, " "));
    }

    LS_ERROR_CHECK(string_append(text, NonBeverages[idxNonBeverage]));
  }

epilogue:
  return result;
}

lsResult generate_instructions(_Out_ raw_string &instructions, rand_seed &seed)
{
  lsResult result = lsR_Success;

  constexpr size_t MaxInstructions = 16;
  constexpr size_t SingleInstructionChance = 30;
  constexpr size_t MeasurementChance = 25;
  constexpr size_t AdverbChance = 40;

  uint64_t rnd = lsGetRand(seed);

  const size_t instructionCount = (rnd % (MaxInstructions - 1)) + 1;
  rnd >>= 5;

  const size_t idxEndInstruction = rnd % LS_ARRAYSIZE(InstructionEndings);

  for (size_t i = 0; i < instructionCount; i++)
  {
    uint64_t rndInner = lsGetRand(seed);

    const size_t chanceSingleInstuction = rndInner % 100;
    rndInner >>= 5;

    // choose single instruction with chance
    if (chanceSingleInstuction < SingleInstructionChance)
    {
      const size_t idxSingleInstruction = rndInner % LS_ARRAYSIZE(SingleInstructions);

      LS_ERROR_CHECK(string_append(instructions, SingleInstructions[idxSingleInstruction]));
    }
    else
    {
      const size_t idxInstruction = rndInner % LS_ARRAYSIZE(Instructions);
      rndInner >>= 5;

      // choose instruction
      LS_ERROR_CHECK(string_append(instructions, Instructions[idxInstruction]));
      LS_ERROR_CHECK(string_append(instructions, " "));

      // choose ingridient
      const size_t chanceMeasurement = rndInner % 100;
      rndInner >>= 5;

      if (chanceMeasurement > MeasurementChance)
      {
        generate_measurement(instructions, seed);
        LS_ERROR_CHECK(string_append(instructions, " "));
      }
      else
      {
        LS_ERROR_CHECK(string_append(instructions, "the "));
      }

      generate_ingridient(instructions, seed);

      const size_t chanceAdverb = rndInner % 100;
      rndInner >>= 5;

      if (chanceAdverb < AdverbChance)
      {
        const size_t idxAdverb = rndInner % LS_ARRAYSIZE(InstructionAdverbs);
        rndInner >>= 5;

        const size_t idxUtensil = rndInner % LS_ARRAYSIZE(CookingUtensils);
        rndInner >>= 5;

        // choose adverb with chance
        LS_ERROR_CHECK(string_append(instructions, " "));
        LS_ERROR_CHECK(string_append(instructions, InstructionAdverbs[idxAdverb]));
        LS_ERROR_CHECK(string_append(instructions, " the "));

        // choose utensil
        LS_ERROR_CHECK(string_append(instructions, CookingUtensils[idxUtensil]));
      }

      LS_ERROR_CHECK(string_append(instructions, "."));
    }

    LS_ERROR_CHECK(string_append(instructions, "\n"));
  }

  LS_ERROR_CHECK(string_append(instructions, InstructionEndings[idxEndInstruction]));

epilogue:
  return result;
}

lsResult generate_cocktail(_Out_ cocktail &ret, rand_seed &seed)
{
  lsResult result = lsR_Success;

  LS_ERROR_CHECK(generate_cocktail_title(ret.title, seed));
  LS_ERROR_CHECK(generate_author(ret.author, seed));
  LS_ERROR_CHECK(generate_instructions(ret.instructions, seed));

epilogue:
  return result;
}

// TODO: error handling for string_append

//////////////////////////////////////////////////////////////////////////

lsResult get_cocktails(_Out_ small_list<cocktail_info> &list)
{
  lsResult result = lsR_Success;

  list_clear(&list);

  // Scope Lock.
  {
    std::scoped_lock lock(_ThreadLock);

    for (const auto &c : _Cocktails)
    {
      raw_string title;
      LS_ERROR_CHECK(string_append(title, c.pItem->title));
      LS_ERROR_CHECK(list_add(&list, cocktail_info(c.index, std::move(title)))); 
    }
  }

epilogue:
  return result;
}

lsResult get_cocktail(const size_t id, _Out_ cocktail **pCocktail)
{
  lsResult result = lsR_Success;

  lsAssert(*pCocktail == nullptr);

  // Scope Lock.
  {
    std::scoped_lock lock(_ThreadLock);
    
    LS_ERROR_CHECK(pool_get_safe(&_Cocktails, id, pCocktail));
  }

epilogue:
  return result;
}

lsResult add_cocktail(_Out_ size_t &id, rand_seed &seed)
{
  lsResult result = lsR_Success;

  cocktail c;
  LS_ERROR_CHECK(generate_cocktail(c, seed));

  // Scope Lock.
  {
    std::scoped_lock lock(_ThreadLock);

    LS_ERROR_CHECK(pool_add(&_Cocktails, std::move(c), &id));
  }

epilogue:
  return result;
}

lsResult update_cocktail(const size_t id, rand_seed &seed)
{
  lsResult result = lsR_Success;
  
  raw_string i;
  generate_instructions(i, seed);

  // Scope Lock.
  {
    std::scoped_lock lock(_ThreadLock);

    LS_ERROR_IF(!pool_has(_Cocktails, id), lsR_InvalidParameter);
    
    cocktail *pCocktail = nullptr;
    LS_ERROR_CHECK(pool_get_safe(&_Cocktails, id, &pCocktail));
    pCocktail->instructions = std::move(i);
  }

epilogue:
  return result;
}

lsResult remove_cocktail(const size_t id)
{
  lsResult result = lsR_Success;

  // Scope Lock.
  {
    std::scoped_lock lock(_ThreadLock);

    LS_ERROR_CHECK(pool_remove_safe(&_Cocktails, id));
  }

epilogue:
  return result;
}
