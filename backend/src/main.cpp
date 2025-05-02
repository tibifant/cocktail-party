#include <stdio.h>
#include <exception>

#define ASIO_STANDALONE 1
#define ASIO_NO_EXCEPTIONS 1

#define COCKTAILPARTY_LOCALHOST
#define COCKTAILPARTY_HOSTNAME "https://hostname_not_configured"

namespace asio
{
  namespace detail
  {
    template <typename Exception>
    void throw_exception(const Exception &e)
    {
      printf("Exception thrown: %s.\n", e.what());
    }
  }
}

//////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4702) // unreachable (somewhere in json.h)
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning (push, 0)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif
#include "crow.h"
#include "crow/middlewares/cors.h"
#ifdef _MSC_VER
#pragma warning (pop)
#else
#pragma GCC diagnostic pop
#endif

//////////////////////////////////////////////////////////////////////////

#include "cocktail_party.h"
#include "sformat.h"
#include "raw_string.h"

//////////////////////////////////////////////////////////////////////////

std::atomic<bool> _IsRunning = true;

//////////////////////////////////////////////////////////////////////////

int32_t main(void)
{
  //for (size_t i = 0; i < 32; i++)
  //{
  //  raw_string name;
  //  generate_cocktail_name(name);
  //  raw_string author;
  //  generate_author(author);
  //  print(name.text, " by ", author.text, '\n');
  //}

  raw_string test;
  generate_ingridients(test);
  print(test.text);

  crow::App<crow::CORSHandler> app;

  auto &cors = app.get_middleware<crow::CORSHandler>();
#ifndef COCKTAILPARTY_LOCALHOST
  cors.global().origin(SCHEDD_HOSTNAME);
#else
  cors.global().origin("*");
#endif

  app.port(61919).multithreaded().run();

  _IsRunning = false;
}

//////////////////////////////////////////////////////////////////////////

