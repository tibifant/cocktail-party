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
#ifdef _MSC_VER
      __debugbreak(); // windows only, sorry!
#endif
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
#pragma GCC diagnostic ignored "-Wnan-infinity-disabled"
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
#include "testable.h"

//////////////////////////////////////////////////////////////////////////

crow::response handle_get_list(const crow::request &req);
crow::response handle_get(const crow::request &req);
crow::response handle_add(const crow::request &req);
crow::response handle_update(const crow::request &req);
crow::response handle_remove(const crow::request &req);

//////////////////////////////////////////////////////////////////////////

std::optional<rand_seed> _CocktailSeed;

//////////////////////////////////////////////////////////////////////////

int32_t main(const int32_t argc, char **pArgv)
{
  bool runTests = false;
  bool runFrontendTests = false;

  for (size_t i = 1; i < (size_t)argc; i++)
  {
    if (lsStringEquals("--run-tests", pArgv[i]))
      runTests = true;
    if (lsStringEquals("--run-frontend-tests", pArgv[i]))
      runFrontendTests = true;
  }

  if (runTests || runFrontendTests)
  {
    _CocktailSeed = rand_seed(0xDEADF00DBABEFULL, 67890);

    for (size_t i = 0; i < 1024; i++)
    {
      size_t _unused;
      if (LS_FAILED(add_cocktail(_unused, _CocktailSeed.value())))
        print_error_line("Failed to add inital cocktail");
    }
  }

  crow::App<crow::CORSHandler> app;
  auto &cors = app.get_middleware<crow::CORSHandler>();
#ifndef COCKTAILPARTY_LOCALHOST
  cors.global().origin(COCKTAILPARTY_HOSTNAME);
#else
  cors.global().origin("*");
#endif

  CROW_ROUTE(app, "/get_list").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_get_list(req); });
  CROW_ROUTE(app, "/get").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_get(req); });
  CROW_ROUTE(app, "/add").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_add(req); });
  CROW_ROUTE(app, "/update").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_update(req); });
  CROW_ROUTE(app, "/remove").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_remove(req); });

  if (runTests)
    run_testables();
  else
    app.port(61919).multithreaded().run();
}

//////////////////////////////////////////////////////////////////////////

crow::response handle_get_list(const crow::request &req)
{
  (void)req;

  small_list<cocktail_info> list;

  if (LS_FAILED(get_cocktails(list)))
    return crow::response(crow::status::INTERNAL_SERVER_ERROR);

  crow::json::wvalue ret;

  uint32_t i = (uint32_t)(-1);
  for (const auto &item : list)
  {
    i++;
    ret[i]["id"] = item.id;
    ret[i]["title"] = item.title.text;
  }

  return ret;
}

crow::response handle_get(const crow::request &req)
{
  auto body = crow::json::load(req.body);

  if (!body || !body.has("id"))
    return crow::response(crow::status::BAD_REQUEST);

  size_t id = body["id"].u();

  cocktail *pCocktail = nullptr;

  if (LS_FAILED(get_cocktail(id, &pCocktail)))
    return crow::response(crow::status::BAD_REQUEST);

  crow::json::wvalue ret;
  ret["title"] = pCocktail->title.text;
  ret["author"] = pCocktail->author.text;
  ret["instructions"] = pCocktail->instructions.text;

  return ret;
}

crow::response handle_add(const crow::request &req)
{
  (void)req;

  size_t id;

  rand_seed s = rand_seed();
  rand_seed *pS = &s;

  if (_CocktailSeed.has_value())
    pS = &_CocktailSeed.value();

  if (LS_FAILED(add_cocktail(id, *pS)))
    return crow::response(crow::status::INTERNAL_SERVER_ERROR);

  crow::json::wvalue ret;
  ret["id"] = id;

  return ret;
}

crow::response handle_update(const crow::request &req)
{
  auto body = crow::json::load(req.body);

  if (!body || !body.has("id"))
    return crow::response(crow::status::BAD_REQUEST);

  size_t id = body["id"].u();

  rand_seed s = rand_seed();
  rand_seed *pS = &s;

  if (_CocktailSeed.has_value())
    pS = &_CocktailSeed.value();

  if (LS_FAILED(update_cocktail(id, *pS)))
    return crow::response(crow::status::BAD_REQUEST);

  return crow::response(crow::status::OK);
}

crow::response handle_remove(const crow::request &req)
{
  auto body = crow::json::load(req.body);

  if (!body || !body.has("id"))
    return crow::response(crow::status::BAD_REQUEST);

  size_t id = body["id"].u();

  if (LS_FAILED(remove_cocktail(id)))
    return crow::response(crow::status::INTERNAL_SERVER_ERROR);

  return crow::response(crow::status::OK);
}

//////////////////////////////////////////////////////////////////////////

REGISTER_TESTABLE_FILE(0)

DEFINE_TESTABLE(get_list_test)
{
  lsResult result = lsR_Success;

  crow::request req;
  crow::response res = handle_get_list(req);
  TESTABLE_ASSERT_NOT_EQUAL(res.code, crow::status::INTERNAL_SERVER_ERROR);

  {
    auto b = crow::json::load(res.body);

    small_list<cocktail_info> l;
    TESTABLE_ASSERT_SUCCESS(get_cocktails(l));
    TESTABLE_ASSERT_TRUE(l.count > 0);

    size_t i = (size_t)-1;

    for (const cocktail_info &c : l)
    {
      i++;
      TESTABLE_ASSERT_TRUE(b[i].has("id"));
      TESTABLE_ASSERT_EQUAL(c.id, b[i]["id"].u());
      TESTABLE_ASSERT_TRUE(b[i].has("title"));
      TESTABLE_ASSERT_EQUAL(strcmp(c.title.text, std::string(b[i]["title"]).c_str()), 0);
    }
  }

  goto epilogue;
epilogue:
  return result;
}

DEFINE_TESTABLE(get_test)
{
  lsResult result = lsR_Success;

  crow::request req;

  {
    crow::response res = handle_get(req);
    TESTABLE_ASSERT_EQUAL(res.code, crow::status::BAD_REQUEST);
  }

  {
    req.body = "{\"id\":-1}";
    crow::response res = handle_get(req);
    TESTABLE_ASSERT_EQUAL(res.code, crow::status::BAD_REQUEST);
  }

  {
    req.body = "{\"id\":0}";
    crow::response res = handle_get(req);
    auto b = crow::json::load(res.body);

    cocktail *pCocktail = nullptr;
    TESTABLE_ASSERT_SUCCESS(get_cocktail(0, &pCocktail));

    TESTABLE_ASSERT_NOT_EQUAL(res.code, crow::status::BAD_REQUEST);
    TESTABLE_ASSERT_NOT_EQUAL(res.code, crow::status::INTERNAL_SERVER_ERROR);
    TESTABLE_ASSERT_TRUE(b.has("author"));
    TESTABLE_ASSERT_EQUAL(strcmp(pCocktail->author.text, std::string(b["author"]).c_str()), 0);
    TESTABLE_ASSERT_TRUE(b.has("title"));
    TESTABLE_ASSERT_EQUAL(strcmp(pCocktail->title.text, std::string(b["title"]).c_str()), 0);
    TESTABLE_ASSERT_TRUE(b.has("instructions"));
    TESTABLE_ASSERT_EQUAL(strcmp(pCocktail->instructions.text, std::string(b["instructions"]).c_str()), 0);
  }

  goto epilogue;
epilogue:
  return result;
}

DEFINE_TESTABLE(add_test)
{
  lsResult result = lsR_Success;

  small_list<cocktail_info> lBefore;
  TESTABLE_ASSERT_SUCCESS(get_cocktails(lBefore));

  {
    crow::request req;
    crow::response res = handle_add(req);
    auto b = crow::json::load(res.body);

    TESTABLE_ASSERT_NOT_EQUAL(res.code, crow::status::INTERNAL_SERVER_ERROR);
    TESTABLE_ASSERT_TRUE(b.has("id"));

    small_list<cocktail_info> lAfter;
    TESTABLE_ASSERT_SUCCESS(get_cocktails(lAfter));

    TESTABLE_ASSERT_TRUE(lBefore.count < lAfter.count);
  }

  goto epilogue;
epilogue:
  return result;
}

DEFINE_TESTABLE(update_test)
{
  lsResult result = lsR_Success;

  crow::request req;

  cocktail *pCocktail = nullptr;
  cocktail before;
  TESTABLE_ASSERT_SUCCESS(get_cocktail(0, &pCocktail));
  TESTABLE_ASSERT_SUCCESS(copy(*pCocktail, before));

  {
    crow::response res = handle_update(req);
    TESTABLE_ASSERT_EQUAL(res.code, crow::status::BAD_REQUEST);
  }

  {
    req.body = "{\"id\":-1}";
    crow::response res = handle_update(req);
    TESTABLE_ASSERT_EQUAL(res.code, crow::status::BAD_REQUEST);
  }

  {
    req.body = "{\"id\":0}";
    crow::response res = handle_update(req);
    auto b = crow::json::load(res.body);

    TESTABLE_ASSERT_NOT_EQUAL(res.code, crow::status::INTERNAL_SERVER_ERROR);
  }

  TESTABLE_ASSERT_NOT_EQUAL(strcmp(pCocktail->instructions.text, before.instructions.text), 0);

  goto epilogue;
epilogue:
  return result;
}

DEFINE_TESTABLE(remove_test)
{
  lsResult result = lsR_Success;

  crow::request req;

  size_t id;
  TESTABLE_ASSERT_SUCCESS(add_cocktail(id, _CocktailSeed.value()));

  {
    crow::response res = handle_remove(req);
    TESTABLE_ASSERT_EQUAL(res.code, crow::status::BAD_REQUEST);
  }

  {
    req.body = "{\"id\":-1}";
    crow::response res = handle_remove(req);
    TESTABLE_ASSERT_EQUAL(res.code, crow::status::INTERNAL_SERVER_ERROR);
  }

  {
    req.body = std::string(sformat("{\"id\":", id, "}"));
    crow::response res = handle_remove(req);
    cocktail *pUnused = nullptr;

    TESTABLE_ASSERT_NOT_EQUAL(res.code, crow::status::INTERNAL_SERVER_ERROR);
    TESTABLE_ASSERT_FAILURE(get_cocktail(id, &pUnused));
  }

  goto epilogue;
epilogue:
  return result;
}
