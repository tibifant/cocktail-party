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

crow::response handle_get_list(const crow::request &req);
crow::response handle_get(const crow::request &req);
crow::response handle_add(const crow::request &req);
crow::response handle_update(const crow::request &req);
crow::response handle_delete(const crow::request &req);

//////////////////////////////////////////////////////////////////////////

std::atomic<bool> _IsRunning = true;
static std::mutex _ThreadLock;

//////////////////////////////////////////////////////////////////////////

int32_t main(void)
{
  crow::App<crow::CORSHandler> app;

  auto &cors = app.get_middleware<crow::CORSHandler>();
#ifndef COCKTAILPARTY_LOCALHOST
  cors.global().origin(SCHEDD_HOSTNAME);
#else
  cors.global().origin("*");
#endif

  CROW_ROUTE(app, "/get_list").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_get_list(req); });
  CROW_ROUTE(app, "/get").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_get(req); });
  CROW_ROUTE(app, "/add").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_add(req); });
  CROW_ROUTE(app, "/update").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_update(req); });
  CROW_ROUTE(app, "/remove").methods(crow::HTTPMethod::POST)([](const crow::request &req) { return handle_delete(req); });

  app.port(61919).multithreaded().run();

  _IsRunning = false;
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

  cocktail c;

  if (LS_FAILED(get_cocktail(id, &c)))
    return crow::response(crow::status::INTERNAL_SERVER_ERROR);

  crow::json::wvalue ret;
  ret["title"] = c.title.text;
  ret["author"] = c.author_name.text;
  ret["instructions"] = c.instructions.text;

  return ret;
}

crow::response handle_add(const crow::request &req)
{
  (void)req;

  size_t id;
  raw_string title; // TODO: 

  if (LS_FAILED(add_cocktail(id, title)))
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
