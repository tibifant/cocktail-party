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
crow::response handle_delete(const crow::request &req);

//////////////////////////////////////////////////////////////////////////

static std::mutex _ThreadLock;
crow::App<crow::CORSHandler> app;

//////////////////////////////////////////////////////////////////////////

int32_t main(void)
{
  for (size_t i = 0; i < 6; i++)
  {
    size_t _unused;
    if (LS_FAILED(add_cocktail(_unused)))
      print_error_line("Failed to add inital cocktail");
  }

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
  CROW_ROUTE(app, "/remove").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_delete(req); });

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
  ret["author"] = pCocktail->author_name.text;
  ret["instructions"] = pCocktail->instructions.text;

  return ret;
}

crow::response handle_add(const crow::request &req)
{
  (void)req;

  size_t id;

  if (LS_FAILED(add_cocktail(id)))
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

  if (LS_FAILED(update_cocktail(id)))
    return crow::response(crow::status::BAD_REQUEST);

  return crow::response(crow::status::OK);
}

crow::response handle_delete(const crow::request &req)
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
  req.url = "/get_list";

  crow::response res;
  app.handle_full(req, res);
  TESTABLE_ASSERT_FALSE(res.code == crow::status::INTERNAL_SERVER_ERROR);

  {
    auto b = crow::json::load(res.body);

    small_list<cocktail_info> l;
    TESTABLE_ASSERT_SUCCESS(get_cocktails(l));
    TESTABLE_ASSERT_TRUE(l.count > 0);

    size_t i = (size_t)-1;

    for (const cocktail_info &c : l)
    {
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
  req.url = "/get";

  {
    crow::response failRes;
    app.handle_full(req, failRes);
    TESTABLE_ASSERT_TRUE(failRes.code == crow::status::BAD_REQUEST);
  }

  {
    crow::response failRes;
    req.body = "{\"id\":-1}";
    app.handle_full(req, failRes);
    TESTABLE_ASSERT_TRUE(failRes.code == crow::status::BAD_REQUEST);
  }

  {
    req.body = "{\"id\":0}";
    crow::response res;
    app.handle_full(req, res);
    auto b = crow::json::load(res.body);

    cocktail *pCocktail;
    TESTABLE_ASSERT_SUCCESS(get_cocktail(0, &pCocktail));

    TESTABLE_ASSERT_FALSE(res.code == crow::status::BAD_REQUEST);
    TESTABLE_ASSERT_FALSE(res.code == crow::status::INTERNAL_SERVER_ERROR);
    TESTABLE_ASSERT_TRUE(b.has("author"));
    TESTABLE_ASSERT_EQUAL(strcmp(pCocktail->author_name.text, std::string(b["author"]).c_str()), 0);
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
    req.url = "/add";

    crow::response res;
    app.handle_full(req, res);
    auto b = crow::json::load(res.body);

    TESTABLE_ASSERT_FALSE(res.code == crow::status::INTERNAL_SERVER_ERROR);
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

  cocktail *pBefore = nullptr;
  TESTABLE_ASSERT_SUCCESS(get_cocktail(0, &pBefore)); // TODO: copy

  {
    crow::request req;
    req.url = "/update";
    req.body = "{\"id\":0}";

    crow::response res;
    app.handle_full(req, res);
    auto b = crow::json::load(res.body);

    TESTABLE_ASSERT_FALSE(res.code == crow::status::INTERNAL_SERVER_ERROR);
  }

  TESTABLE_ASSERT_NOT_EQUAL();


  goto epilogue;
epilogue:
  return result;
}

DEFINE_TESTABLE(add_get_update_remove_test)
{
  lsResult result = lsR_Success;

  // test 3:
  // add via add_cocktail
  // remove via crow
  // assert its gone via get_cocktail

  small_list<cocktail_info> lBefore;
  small_list<cocktail_info> lAfterAdd;
  crow::response resAdd;

  crow::request reqAdd;
  reqAdd.url = "/add";
  TESTABLE_ASSERT_SUCCESS(get_cocktails(lBefore));
  app.handle_full(reqAdd, resAdd);
  TESTABLE_ASSERT_FALSE(resAdd.code == crow::status::INTERNAL_SERVER_ERROR);
  TESTABLE_ASSERT_SUCCESS(get_cocktails(lAfterAdd));
  TESTABLE_ASSERT_TRUE(lBefore.count < lAfterAdd.count);

  // get with response body from add
  // compare with list entry at same position
  // remove with id from add
  // compare list before and after
  crow::request req;
  req.body = "{\"id\":0}";

  const crow::response r = handle_get(crow::request());
  TESTABLE_ASSERT_FALSE(r.code == crow::status::INTERNAL_SERVER_ERROR);

  goto epilogue;
epilogue:
  return result;
}
