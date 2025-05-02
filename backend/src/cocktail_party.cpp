#include "cocktail_party.h"
#include "pool.h"

pool<cocktail> Cocktails;

static const char *Units[] =
{ "g", "mg", "kg", "cg", "dg", "hg", "dag", "t", "oz", "lb", "st", "long ton", "short ton", "L", "mL", "m³", "cm³", "dm³", "hL", "dL", "cL", "gal", "qt", "pt", "gi", "min", "bbl", "tsp", "tbsp", "c", "pt", "qt", "gal", "fl oz", "in³", "ft³", "yd³", "whole", "bowl(s)" };

static const char *AuthorFirstnames[] = { "Mamadou", "Judith", "Kenneth", "Mostafa", "Chris", "Helen", "Nikolay", "Rina", "Zhiqiang", "Marcos", "Mária", "Norma", "Anton", "Raul", "Cristina", "Xiaohong", "Henry", "Wai", "Antonia", "Betty", "Alejandro", "Nelson", "Igor", "Evgeniy", "Adriana", "Amir", "Pablo", "Raj", "Regina", "Rajendra", "Brenda", "Linh", "Sani", "Hussein", "Gul", "Mikhail", "Jaime", "Nicole", "Sima", "Giuseppe", "Dinesh", "Tatiana", "Bernard", "Gary", "Lijun", "Sita", "Javier", "Shan", "Hasan", "Yuliya", "Ni", "Moses", "Agnes", "Cesar", "Xiaoli", "Usha", "Alfredo", "Meng", "Jianguo", "Kiran", "Dennis", "Khaled", "Carol", "Rani", "Yusuf", "Xiaoping", "Ha", "Rakesh", "Isaac", "Luiz", "Josephine", "Krishna", "Michael", "Juan", "Anna", "Mary", "Jean", "Robert", "Daniel", "Luis", "Carlos", "James", "Antonio", "Joseph", "Hui", "Elena", "Francisco", "Hong", "Marie", "Andrea", "Christine", "Irina", "Laura", "Linda", "Marina", "Carmen", "Ghulam", "Vladimir", "Barbara", "Angel", "Günther", "Kai", "Rainer" };

static const char *AuthorLastnames[] = { "Wang", "Li", "Zhang", "Chen", "Liu", "Devi", "Yang", "Mohamed", "Islam", "Shi", "Song", "Xie", "Sharif", "Juarez", "Patal", "Kamal", "Jain", "Phiri", "Salah", "Walker", "Akbar", "Clark", "Lewis", "Hosen", "Diarra", "Avila", "Chaudhary", "Chaudhari", "Franco", "Moyo", "Watson", "Hughes", "Ochoa", "Paredes", "Mahmood", "Lozano", "Garcia",
"Martin", "Müller", "Rodriguez", "Fernandez", "Sanchez", "Perez	", "Ivanov", "Ivanova	", "Schmidt	", "Smith	", "Jones", "Gomez", "Schneider", "Fischer", "Petrov", "Meyer", "Weber", "Thomas", "Ruiz", "Alonso", "Novikova", "Ramos", "Ferrari", "Schmid", "Kravchenko", "Torres", "Sokolova", "Meier", "Navarro", "Roberts", "Gutierrez", "Zimmermann", "Lauch", "Trump", "Stalin", "Jinping", "Nero", "the Hun", "Pinochet", "Honecker", "Napoleon", "Lenin", "Castro", "Mussolini", "Piranha", "Zufall" };

static const char *IngridientPrefixes[] = { "Aged", "Amber", "Black", "Blonde", "Bold", "Bourbon", "Brut", "Caramel", "Cherry", "Citrus", "Creamy", "Dark", "Dry", "Elderflower", "Extra", "Fruity", "Golden", "Green", "Honey", "Ice", "Jamaican", "Lemon", "Light", "Lime", "Mango", "Mellow", "Mint", "Moscow", "Navy", "Old", "Orange", "Peach", "Pineapple", "Raspberry", "Red", "Rum", "Smooth", "Spiced", "Strawberry", "Sweet", "Tequila", "Tropical", "Vanilla", "Vermouth", "Vodka", "White", "Whiskey", "Wild", "Winter", "Young", "French", "Beach", "Hot", "Indonesian", "Alaska", "Florida", "Barbados", "Mallorca", "Flavor", "Fidji", "Tiki", "Caribbean", "Mulled" };

static const char *NonBeverageAttributes[] = { "horned", "organic", "pickled", "juicy", "wild", "spiced", "nutty", "spicy", "peeled", "ground", "frozen", "caramelized" };

static const char *NonBeverages[] = { "Apple", "Banana", "Orange", "Grape", "Lemon", "Lime", "Cherry", "Peach", "Pear", "Pineapple", "Plum", "Strawberry", "Blueberry", "Raspberry", "Blackberry", "Watermelon", "Cantaloupe", "Honeydew", "Kiwi", "Mango", "Papaya", "Guava", "Passion fruit", "Dragon fruit", "Lychee", "Rambutan", "Durian", "Jackfruit", "Starfruit", "Coconut", "Starfruit", "Soursop", "Cherimoya", "Sapote", "Longan", "Mangosteen", "Pitaya", "Acerola", "Tamarind", "Breadfruit", "Grapefruit", "Tangerine", "Clementine", "Mandarin", "Pomelo", "Ugli fruit", "Bergamot", "Kumquat", "Yuzu", "Calamondin", "Cranberry", "Gooseberry", "Boysenberry", "Loganberry", "Mulberry", "Elderberry", "Huckleberry", "Salal berry", "Cloudberry", "Sea buckthorn", "Apricot", "Nectarine", "Plumcot", "Aprium", "Peento", "Sloe", "Damson", "Mirabelle", "Greengage", "Salak", "Melon", "Casaba", "Canary", "Crenshaw", "Galia", "Honeydew", "Santa Claus", "Snake", "Sugar Baby", "Tigger", "Akee", "Atemoya", "Custard apple", "Feijoa", "Raisin", "Prune", "Apricot", "Fig", "Date", "Cranberry", "Apple", "Pear", "Mango", "Pineapple", "Carrot", "Potato", "Tomato", "Cucumber", "Lettuce", "Spinach", "Broccoli", "Cauliflower", "Cabbage", "Bell pepper", "Onion", "Garlic", "Zucchini", "Eggplant", "Green bean", "Pea", "Corn", "Radish", "Celery", "Beet", "Kale", "Swiss chard", "Collard greens", "Arugula", "Bok choy", "Endive", "Escarole", "Fennel", "Mustard greens", "Turnip greens", "Sweet potato", "Parsnip", "Turnip", "Rutabaga", "Ginger", "Horseradish", "Jicama", "Daikon", "Burdock", "Celeriac", "Brussels sprouts", "Kale", "Cabbage", "Broccoli rabe", "Kohlrabi", "Radicchio", "Watercress", "Butternut squash", "Acorn squash", "Spaghetti squash", "Pumpkin", "Delicata squash", "Hubbard squash", "Turban squash", "Leek", "Shallot", "Chive", "Green onion", "Ramps", "Elephant garlic", "Tomatillo", "Poblano pepper", "Jalapeño pepper", "Habanero pepper", "Eggplant", "Ground cherry", "Okra", "Artichoke", "Fiddlehead ferns", "Nopales", "Kohlrabi", "Daikon", "Jicama", "Lotus root", "Bitter melon", "Chinese eggplant", "Button mushroom", "Cremini mushroom", "Portobello mushroom", "Shiitake mushroom", "Oyster mushroom", "Enoki mushroom", "Maitake mushroom", "Chanterelle mushroom", "Morel mushroom", "Truffle", "Basil", "Parsley", "Cilantro", "Dill", "Rosemary", "Thyme", "Oregano", "Sage", "Tarragon", "Chives", "Snow pea", "Sugar snap pea", "Fava bean", "Lima bean", "Black-eyed pea", "Chickpea", "Lentil", "Almond", "Cashew", "Hazelnut", "Peanut", "Pecan", "Pistachio", "Walnut", "Chestnut", "Macadamia", "Pine nut", "Apple Pie", "Cupcake", "Caramel", "Honey", "Raisin", "Vanilla", "Pepper", "Salt", "Black Pepper", "Garlic Powder", "Onion Powder", "Paprika", "Cumin", "Coriander", "Turmeric", "Chili Powder", "Oregano", "Basil", "Thyme", "Rosemary", "Sage", "Cinnamon", "Nutmeg", "Cloves", "Allspice", "Ginger", "Mustard Powder", "Cardamom", "Fennel", "Star Anise", "Anise", "Fenugreek", "Mustard", "Cayenne", "Red Pepper Flakes", "White Pepper", "Green Peppercorns", "Dill", "Parsley", "Mint", "Lemon Grass", "Lemon Zest", "Lime Zest" };

static const char *Beverages[] = { "Water", "Coffee", "Tea", "Milk", "Buttermilk", "Juice", "Soda", "Cola", "Ginger Ale", "Water", "Lemonade", "Iced tea", "Hot chocolate", "Smoothie", "Hot cocoa", "Chai", "Horchata", "Kombucha", "Matcha", "Mate", "Beer", "Wine", "Champagne", "Vodka", "Gin", "Rum", "Tequila", "Whiskey", "Bourbon", "Scotch", "Brandy", "Cognac", "Absinthe", "Mezcal", "Sake", "Pisco", "Ouzo", "Raki", "Soju", "Baijiu", "Cachaça", "Poire Williams", "Liqueur", "Armagnac", "Calvados", "Triple Sec", "Rosé", "Soy Sauce", "Terriyaki", "White Wine", "Red Wine", "Prosecco", "Sherry", "Port Wine", "Merlot", "Cider", "Jenever", "Shochu", "Moonshine", "Korn", "Maotai", "Grappa", "Himbeergeist", "Kefir", "Ayran", "Schnapps", "Syrup", "Cointreau", "Limoncello", "Umeshu", "Grand Manier", "Pastis", "Amaretto", "Mist" };

static const char *CocktailAttributesSingle[] = { "on Ice", "on the Rocks" };

static const char *CocktailAttributesPrefix[] = { "á la", "de" };

static const char *Instructions[] = { "Measure", "Peel", "Slice", "Infuse", "Ground", "Blend", "Grate", "Half", "Drink", "Eat", "Mash", "Stir", "Chop", "Shake", "Dice", "Cook", "Reduce", "Boil", "Puree", "Mix", "Toss", "Grill", "Sauté", "Roast", "Steam", "Deglaze", "Smoke", "Toast", "Juice", "Zest", "Whisk", "Open", "Caramelize" };

static const char *SingleInstructions[] = { "Cool to absolute Zero", "Mix with Ice 3", "Heat to 90°", "Bake for 20 minutes", "Bring to a boil", "Reduce to half", "Deglaze the pan", "Add a pinch of Salt", "Shake the mixture", "Let it sit for 1 hour", "Cool down to room temperature", "Refrigerate over night", "Precool the glass" };

static const char *InstructionAdverbs[] = { "with", "using", "atop", "in front of", "in" };

static const char *CookingUtensils[] = { "Pan", "Oven", "Mixer", "Shaker", "Knive", "Grater", "Stove", "Whisk", "Blender", "Spoon", "Fork", "Glass", "Corkscrew", "Can openener", "Cutting Board", "Skillet", "Baking Sheet", "Pie Dish" };

static const char *InstructionEndings[] = { "Serve cold.", "Serve with Lime Juice.", "Serve immediately!", "Immediately serve in a Mug.", "Enjoy!", "Pour into a Martini Glass and serve.", "Garnish with an Olive and serve.", "Serve while stirring thoroughly.", "Serve on Ice.", "Serve warm.", "Serve on a warm summer night.", "Enjoy with friends!", "Bon Appetit!", "Decorate with an Cocktail Umbrella and serve." };

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
  constexpr uint8_t ChanceSecondName = 20;

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

void generate_ingridients(_Out_ raw_string &ingridients)
{
  constexpr size_t MaxIngridients = 16;
  constexpr size_t MaxAmount = 200;
  constexpr size_t BeverageChance = 40;
  constexpr size_t NonBeverageAttributesChance = 10;

  const size_t ingridientCount = (lsGetRand() % (MaxIngridients - 1)) + 1;

  for (size_t i = 0; i < ingridientCount; i++)
  {
    uint64_t rnd = lsGetRand();

    const size_t amount = rnd % MaxAmount;
    rnd >>= 5;

    const size_t idxUnit = rnd % LS_ARRAYSIZE(Units);
    rnd >>= 5;

    const size_t chanceBeverage = rnd % 100;
    rnd >>= 5;

    // choose amount
    string_append(ingridients, sformat(amount));
    string_append(ingridients, " ");

    // choose unit
    string_append(ingridients, Units[idxUnit]);
    string_append(ingridients, " ");


    // choose ingridient
    if (chanceBeverage < BeverageChance)
    {
      const size_t idxBeverage = rnd % LS_ARRAYSIZE(Beverages);

      string_append(ingridients, Beverages[idxBeverage]);
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
        string_append(ingridients, NonBeverageAttributes[idxNonBeverageAttributes]);
        string_append(ingridients, " ");
      }
      
      string_append(ingridients, NonBeverages[idxNonBeverage]);
    }

    if (i < ingridientCount - 1)
      string_append(ingridients, "\n");
  }
}
