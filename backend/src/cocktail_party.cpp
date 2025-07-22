#include "cocktail_party.h"

#include "pool.h"
#include <mutex>

#include <mutex>

pool<cocktail> _Cocktails;
static std::mutex _ThreadLock;

//////////////////////////////////////////////////////////////////////////

static const char *Units[] =
{ "g", "mg", "kg", "cg", "dg", "hg", "dag", "t", "oz", "lb", "st", "long ton", "short ton", "L", "mL", "m³", "cm³", "dm³", "hL", "dL", "cL", "gal", "qt", "pt", "gi", "min", "bbl", "tsp", "tbsp", "c", "pt", "qt", "gal", "fl oz", "in³", "ft³", "yd³", "whole", "bowl(s)" };

static const char *AuthorFirstnames[] = { "Nelson", "Igor", "Evgeniy", "Adriana", "Gary", "Moses", "Agnes", "Cesar", "Xiaoli", "Usha", "Alfredo", "Isaac", "Luiz", "Josephine", "Krishna", "Michael", "Juan", "Anna", "Mary", "Jean", "Robert", "Daniel", "Luis", "Carlos", "James", "Antonio", "Joseph", "Hui", "Elena", "Francisco", "Carmen", "Ghulam", "Barbara", "Angel", "Vladimir", "Günther", "Kai", "Rainer", "Hans", "Jesus", "Donald", "Caren", "Captain", "Doctor", "Professor", "Mister", "Miss", "Lady", "Sir", "Richard", "Dolly", "Angela", "Marie", "Sarah", "Frank", "Harry", "Giga", "Madonna", "Jan-Phillip", "Annegret", "Alonso", "Horst", "Stina", "Uschi", "Manfred", "Albert", "Celine", "Taylor" };

static const char *AuthorLastnames[] = { "Wang", "Walker", "Clark", "Lewis", "Hosen", "Diarra", "Franco", "Moyo", "Watson", "Hughes", "Gomez", "Schneider", "Fischer", "Petrov", "Meyer", "Weber", "Thomas", "Ferrari", "Lauch", "Trump", "Stalin", "Jinping", "Jong Un", "Putin", "Nero", "the Hun", "Pinochet", "Honecker", "Napoleon", "Lenin", "Castro", "Mussolini", "Piranha", "Lewis", "Merkel", "Curie", "Connor", "Cena", "Duck", "Sinatra", "Styles", "Chad", "Krampf-Kabrackenbauer", "Mozart", "Spears", "Dion", "Swift", "Newton", "Einstein" };

static const char *IngridientPrefixes[] = { "Aged", "Amber", "Black", "Blonde", "Bold", "Bourbon", "Brut", "Caramel", "Cherry", "Citrus", "Creamy", "Dark", "Dry", "Elderflower", "Extra", "Fruity", "Golden", "Green", "Honey", "Ice", "Jamaican", "Lemon", "Light", "Lime", "Mango", "Mellow", "Mint", "Moscow", "Navy", "Old", "Orange", "Peach", "Pineapple", "Raspberry", "Red", "Rum", "Smooth", "Spiced", "Strawberry", "Sweet", "Tequila", "Tropical", "Vanilla", "Vermouth", "Vodka", "White", "Whiskey", "Wild", "Winter", "Young", "French", "Beach", "Hot", "Indonesian", "Alaska", "Florida", "Barbados", "Mallorca", "Flavor", "Fidji", "Tiki", "Caribbean", "Mulled" };

static const char *NonBeverageAttributes[] = { "horned", "organic", "pickled", "juicy", "wild", "spiced", "nutty", "spicy", "peeled", "ground", "frozen", "caramelized" };

static const char *NonBeverages[] = { "Apple", "Banana", "Orange", "Grape", "Lemon", "Lime", "Cherry", "Peach", "Pear", "Pineapple", "Plum", "Strawberry", "Blueberry", "Raspberry", "Blackberry", "Watermelon", "Cantaloupe", "Honeydew", "Kiwi", "Mango", "Papaya", "Guava", "Passion fruit", "Dragon fruit", "Lychee", "Rambutan", "Durian", "Jackfruit", "Starfruit", "Coconut", "Starfruit", "Soursop", "Cherimoya", "Sapote", "Longan", "Mangosteen", "Pitaya", "Acerola", "Tamarind", "Breadfruit", "Grapefruit", "Tangerine", "Clementine", "Mandarin", "Pomelo", "Ugli fruit", "Bergamot", "Kumquat", "Yuzu", "Calamondin", "Cranberry", "Gooseberry", "Boysenberry", "Loganberry", "Mulberry", "Elderberry", "Huckleberry", "Salal berry", "Cloudberry", "Sea buckthorn", "Apricot", "Nectarine", "Plumcot", "Aprium", "Peento", "Sloe", "Damson", "Mirabelle", "Greengage", "Salak", "Melon", "Casaba", "Canary", "Crenshaw", "Galia", "Honeydew", "Santa Claus", "Snake", "Sugar Baby", "Tigger", "Akee", "Atemoya", "Custard apple", "Feijoa", "Raisin", "Prune", "Apricot", "Fig", "Date", "Cranberry", "Apple", "Pear", "Mango", "Pineapple", "Carrot", "Potato", "Tomato", "Cucumber", "Lettuce", "Spinach", "Broccoli", "Cauliflower", "Cabbage", "Bell pepper", "Onion", "Garlic", "Zucchini", "Eggplant", "Green bean", "Pea", "Corn", "Radish", "Celery", "Beet", "Kale", "Swiss chard", "Collard greens", "Arugula", "Bok choy", "Endive", "Escarole", "Fennel", "Mustard greens", "Turnip greens", "Sweet potato", "Parsnip", "Turnip", "Rutabaga", "Ginger", "Horseradish", "Jicama", "Daikon", "Burdock", "Celeriac", "Brussels sprouts", "Kale", "Cabbage", "Broccoli rabe", "Kohlrabi", "Radicchio", "Watercress", "Butternut squash", "Acorn squash", "Spaghetti squash", "Pumpkin", "Delicata squash", "Hubbard squash", "Turban squash", "Leek", "Shallot", "Chive", "Green onion", "Ramps", "Elephant garlic", "Tomatillo", "Poblano pepper", "Jalapeño pepper", "Habanero pepper", "Eggplant", "Ground cherry", "Okra", "Artichoke", "Fiddlehead ferns", "Nopales", "Kohlrabi", "Daikon", "Jicama", "Lotus root", "Bitter melon", "Chinese eggplant", "Button mushroom", "Cremini mushroom", "Portobello mushroom", "Shiitake mushroom", "Oyster mushroom", "Enoki mushroom", "Maitake mushroom", "Chanterelle mushroom", "Morel mushroom", "Truffle", "Basil", "Parsley", "Cilantro", "Dill", "Rosemary", "Thyme", "Oregano", "Sage", "Tarragon", "Chives", "Snow pea", "Sugar snap pea", "Fava bean", "Lima bean", "Black-eyed pea", "Chickpea", "Lentil", "Almond", "Cashew", "Hazelnut", "Peanut", "Pecan", "Pistachio", "Walnut", "Chestnut", "Macadamia", "Pine nut", "Apple Pie", "Cupcake", "Caramel", "Honey", "Raisin", "Vanilla", "Pepper", "Salt", "Black Pepper", "Garlic Powder", "Onion Powder", "Paprika", "Cumin", "Coriander", "Turmeric", "Chili Powder", "Oregano", "Basil", "Thyme", "Rosemary", "Sage", "Cinnamon", "Nutmeg", "Cloves", "Allspice", "Ginger", "Mustard Powder", "Cardamom", "Fennel", "Star Anise", "Anise", "Fenugreek", "Mustard", "Cayenne", "Red Pepper Flakes", "White Pepper", "Green Peppercorns", "Dill", "Parsley", "Mint", "Lemon Grass", "Lemon Zest", "Lime Zest" };

static const char *Beverages[] = { "Water", "Coffee", "Tea", "Milk", "Buttermilk", "Juice", "Soda", "Cola", "Ginger Ale", "Water", "Lemonade", "Iced tea", "Hot chocolate", "Smoothie", "Hot cocoa", "Chai", "Horchata", "Kombucha", "Matcha", "Mate", "Beer", "Wine", "Champagne", "Vodka", "Gin", "Rum", "Tequila", "Whiskey", "Bourbon", "Scotch", "Brandy", "Cognac", "Absinthe", "Mezcal", "Sake", "Pisco", "Ouzo", "Raki", "Soju", "Baijiu", "Cachaça", "Poire Williams", "Liqueur", "Armagnac", "Calvados", "Triple Sec", "Rosé", "Soy Sauce", "Terriyaki", "White Wine", "Red Wine", "Prosecco", "Sherry", "Port Wine", "Merlot", "Cider", "Jenever", "Shochu", "Moonshine", "Korn", "Maotai", "Grappa", "Himbeergeist", "Kefir", "Ayran", "Schnapps", "Syrup", "Cointreau", "Limoncello", "Umeshu", "Grand Manier", "Pastis", "Amaretto", "Mist" };

static const char *CocktailAttributesSingle[] = { "on Ice", "on the Rocks" };

static const char *CocktailAttributesPrefix[] = { "á la", "de" };

static const char *Instructions[] = { "Measure", "Peel", "Slice", "Infuse", "Ground", "Blend", "Grate", "Half", "Drink", "Eat", "Mash", "Stir", "Chop", "Shake", "Dice", "Cook", "Reduce", "Boil", "Puree", "Mix", "Toss", "Grill", "Sauté", "Roast", "Steam", "Deglaze", "Smoke", "Toast", "Juice", "Zest", "Whisk", "Open", "Caramelize" };

static const char *SingleInstructions[] = { "Cool to absolute Zero.", "Mix with Ice 3.", "Heat to 90°C.", "Bake for 20 minutes.", "Bring to a boil.", "Reduce to half.", "Deglaze the pan.", "Add a pinch of Salt.", "Shake the mixture.", "Let it sit for 1 hour.", "Cool down to room temperature.", "Refrigerate over night.", "Precool the glass." };

static const char *InstructionAdverbs[] = { "with", "using", "atop", "in front of", "in" };

static const char *CookingUtensils[] = { "Pan", "Oven", "Mixer", "Shaker", "Knive", "Grater", "Stove", "Whisk", "Blender", "Spoon", "Fork", "Glass", "Corkscrew", "Can Openener", "Cutting Board", "Skillet", "Baking Sheet", "Pie Dish" };

static const char *InstructionEndings[] = { "Serve cold.", "Serve with Lime Juice.", "Serve immediately!", "Immediately serve in a Mug.", "Enjoy!", "Pour into a Martini Glass and serve.", "Garnish with an Olive and serve.", "Serve while stirring thoroughly.", "Serve on Ice.", "Serve warm.", "Serve on a warm summer night.", "Enjoy with friends!", "Bon Appetit!", "Decorate with a Cocktail Umbrella and serve." };

void generate_cocktail_name(_Out_ raw_string &name)
{
  constexpr size_t ChanceIngridientAttribute = 40;
  constexpr size_t ChanceCocktailAttribute = 10;

  uint64_t rnd = lsGetRand();

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
  string_append(name, IngridientPrefixes[idxIngridientPrefixA]);
  string_append(name, " ");

  // choose attribute with chance
  if (randChanceIngridientAttribute < ChanceIngridientAttribute)
  {
    string_append(name, NonBeverageAttributes[idxIngridientAttribute]);
    string_append(name, " ");
  }

  // choose random non beverage ingridient
  string_append(name, NonBeverages[idxNonBeverage]);
  string_append(name, " ");

  // choose random prefix
  string_append(name, IngridientPrefixes[idxIngridientPrefixB]);
  string_append(name, " ");

  // choose random beverage
  string_append(name, Beverages[idxBeverage]);

  // choose either single or prefix attribute with chance (if prefix -> choose ingridient)
  if (randChanceCocktailAttribute < ChanceCocktailAttribute)
  {
    rnd >>= 5;
    const bool chanceCocktailAttributeSingle = rnd & 1ULL;

    string_append(name, " ");

    if (chanceCocktailAttributeSingle)
    {
      const size_t idxCocktailAttribute = rnd % LS_ARRAYSIZE(CocktailAttributesSingle);
      string_append(name, CocktailAttributesSingle[idxCocktailAttribute]);
    }
    else
    {
      const size_t idxCocktailAttribute = rnd % LS_ARRAYSIZE(CocktailAttributesPrefix);
      rnd <<= 5;
      const size_t idxNonBeverageB = rnd % LS_ARRAYSIZE(NonBeverages);

      string_append(name, CocktailAttributesPrefix[idxCocktailAttribute]);
      string_append(name, " ");
      string_append(name, NonBeverages[idxNonBeverageB]);
    }
  }
}

void generate_author(_Out_ raw_string &name)
{
  constexpr uint8_t ChanceSecondName = 35;

  uint64_t rnd = lsGetRand();
  const size_t idxFirstName = rnd % LS_ARRAYSIZE(AuthorFirstnames);
  rnd >>= 5;
  const size_t randChanceSecondName = rnd % 100;
  rnd >>= 5;
  const size_t idxSecondName = rnd % LS_ARRAYSIZE(AuthorFirstnames);
  rnd >>= 5;
  const size_t idxLastName = rnd % LS_ARRAYSIZE(AuthorLastnames);

  // choose random first name
  string_append(name, AuthorFirstnames[idxFirstName]);
  string_append(name, " ");

  // choose second name with chance
  if (randChanceSecondName < ChanceSecondName)
  {
    string_append(name, AuthorFirstnames[idxSecondName]);
    string_append(name, " ");
  }

  // choose random lastname
  string_append(name, AuthorLastnames[idxLastName]);
}

void generate_measurement(_Out_ raw_string &text)
{
  constexpr size_t MaxAmount = 200;

  uint64_t rnd = lsGetRand();

  const size_t amount = rnd % MaxAmount;
  rnd >>= 5;

  const size_t idxUnit = rnd % LS_ARRAYSIZE(Units);
  rnd >>= 5;

  // amount
  string_append(text, sformat(amount));
  string_append(text, " ");

  // unit
  string_append(text, Units[idxUnit]);
}

void generate_ingridient(_Out_ raw_string &text)
{
  constexpr size_t BeverageChance = 40;
  constexpr size_t NonBeverageAttributesChance = 10;

  uint64_t rnd = lsGetRand();

  const size_t chanceBeverage = rnd % 100;
  rnd >>= 5;

  // choose ingridient
  if (chanceBeverage < BeverageChance)
  {
    const size_t idxBeverage = rnd % LS_ARRAYSIZE(Beverages);

    string_append(text, Beverages[idxBeverage]);
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
      string_append(text, NonBeverageAttributes[idxNonBeverageAttributes]);
      string_append(text, " ");
    }

    string_append(text, NonBeverages[idxNonBeverage]);
  }
}

void generate_instructions(_Out_ raw_string &instructions)
{
  constexpr size_t MaxInstructions = 16;
  constexpr size_t SingleInstructionChance = 30;
  constexpr size_t MeasurementChance = 25;
  constexpr size_t AdverbChance = 40;

  uint64_t rnd = lsGetRand();

  const size_t instructionCount = (rnd % (MaxInstructions - 1)) + 1;
  rnd >>= 5;

  const size_t idxEndInstruction = rnd % LS_ARRAYSIZE(InstructionEndings);

  for (size_t i = 0; i < instructionCount; i++)
  {
    uint64_t rndInner = lsGetRand();

    const size_t chanceSingleInstuction = rndInner % 100;
    rndInner >>= 5;

    // choose single instruction with chance
    if (chanceSingleInstuction < SingleInstructionChance)
    {
      const size_t idxSingleInstruction = rndInner % LS_ARRAYSIZE(SingleInstructions);

      string_append(instructions, SingleInstructions[idxSingleInstruction]);
    }
    else
    {
      const size_t idxInstruction = rndInner % LS_ARRAYSIZE(Instructions);
      rndInner >>= 5;

      // choose instruction
      string_append(instructions, Instructions[idxInstruction]);
      string_append(instructions, " ");

      // choose ingridient
      const size_t chanceMeasurement = rndInner % 100;
      rndInner >>= 5;

      if (chanceMeasurement > MeasurementChance)
      {
        generate_measurement(instructions);
        string_append(instructions, " ");
      }
      else
      {
        string_append(instructions, "the ");
      }

      generate_ingridient(instructions);

      const size_t chanceAdverb = rndInner % 100;
      rndInner >>= 5;

      if (chanceAdverb < AdverbChance)
      {
        const size_t idxAdverb = rndInner % LS_ARRAYSIZE(InstructionAdverbs);
        rndInner >>= 5;

        const size_t idxUtensil = rndInner % LS_ARRAYSIZE(CookingUtensils);
        rndInner >>= 5;

        // choose adverb with chance
        string_append(instructions, " ");
        string_append(instructions, InstructionAdverbs[idxAdverb]);
        string_append(instructions, " the ");

        // choose utensil
        string_append(instructions, CookingUtensils[idxUtensil]);
      }

      string_append(instructions, ".");
    }

    string_append(instructions, "\n");
  }

  string_append(instructions, InstructionEndings[idxEndInstruction]);
}

cocktail generate_cocktail()
{
  cocktail ret;

  generate_cocktail_name(ret.name);
  generate_author(ret.author_name);
  generate_instructions(ret.instructions);

  return ret;
}

//////////////////////////////////////////////////////////////////////////

lsResult add_cocktail()
{
  lsResult result = lsR_Success;

  const cocktail c = generate_cocktail();

  // Scope Lock.
  {
    std::scoped_lock lock(_ThreadLock);

    size_t _unused;
    LS_ERROR_CHECK(pool_add(&_Cocktails, c, &_unused));
  }

epilogue:
  return result;
}

lsResult update_cocktail(const size_t cocktail_id, const cocktail updated_cocktail)
{
  lsResult result = lsR_Success;
  
  // Scope Lock.
  {
    std::scoped_lock lock(_ThreadLock);

    LS_ERROR_IF(!pool_has(_Cocktails, cocktail_id), lsR_InvalidParameter);
    LS_ERROR_CHECK(pool_insertAt(&_Cocktails, updated_cocktail, cocktail_id, true));
  }

epilogue:
  return result;
}

lsResult remove_cocktail(const size_t cocktail_id)
{
  lsResult result = lsR_Success;

  // Scope Lock.
  {
    std::scoped_lock lock(_ThreadLock);

    LS_ERROR_CHECK(pool_remove_safe(&_Cocktails, cocktail_id));
  }

epilogue:
  return result;
}

lsResult get_cocktails(small_list<std::tuple<size_t, raw_string>> &cocktailInfos)
{
  lsResult result = lsR_Success;

  for ()
  {

  }

epilogue:
  return result;
}
