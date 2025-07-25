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

//////////////////////////////////////////////////////////////////////////

crow::response handle_get_list(const crow::request &req);
crow::response handle_get(const crow::request &req);
crow::response handle_add(const crow::request &req);
crow::response handle_update(const crow::request &req);
crow::response handle_delete(const crow::request &req);

//////////////////////////////////////////////////////////////////////////

static std::mutex _ThreadLock;

//////////////////////////////////////////////////////////////////////////

int32_t main(void)
{
  for (size_t i = 0; i < 6; i++)
  {
    size_t _unused;
    if (LS_FAILED(add_cocktail(_unused)))
      print_error_line("Failed to add inital cocktail");
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

  uint32_t i = (uint32_t)(- 1);
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

  size_t id = body["id"].i();

  cocktail *pCocktail = nullptr; // do I need to free this?

  if (LS_FAILED(get_cocktail(id, &pCocktail)))
    return crow::response(crow::status::INTERNAL_SERVER_ERROR);

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

  size_t id = body["id"].i();

  if (LS_FAILED(update_cocktail(id)))
    return crow::response(crow::status::BAD_REQUEST);

  return crow::response(crow::status::OK);
}

crow::response handle_delete(const crow::request &req)
{
  auto body = crow::json::load(req.body);

  if (!body || !body.has("id"))
    return crow::response(crow::status::BAD_REQUEST);

  size_t id = body["id"].i();
  
  if (LS_FAILED(remove_cocktail(id)))
    return crow::response(crow::status::INTERNAL_SERVER_ERROR);

  return crow::response(crow::status::OK);
}
