/**
 * @file   helloworld.cpp
 * @author mononerv (me@mononerv.dev)
 * @brief  Hello World with asio
 * @date   2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <thread>

#include "fmt/format.h"
#include "asio.hpp"

auto echo(asio::ip::tcp::socket socket) -> asio::awaitable<void> {
  try {
    char data[1024];
    for (;;) {
      std::size_t n = co_await socket.async_read_some(asio::buffer(data), asio::use_awaitable);
      co_await asio::async_write(socket, asio::buffer(data, n), asio::use_awaitable);
    }
  } catch (std::exception& e)
  {
    std::printf("echo Exception: %s\n", e.what());
  }
}

auto listener() -> asio::awaitable<void> {
    auto exe = co_await asio::this_coro::executor;
    asio::ip::tcp::acceptor acceptor(exe, {asio::ip::tcp::v4(), 3000});
    for (;;) {
        auto socket = co_await acceptor.async_accept(asio::use_awaitable);
        co_spawn(exe, echo(std::move(socket)), asio::detached);
    }
}

auto main([[maybe_unused]]int argc, [[maybe_unused]]char const* argv[]) -> int {
    try {
        asio::io_context context;
        asio::signal_set signals(context, SIGINT, SIGTERM);
        signals.async_wait([&] (auto, auto) { context.stop(); });
        co_spawn(context, listener, asio::detached);
        context.run();
    } catch (std::exception const& e) {
        fmt::print("exception: {}\n", e.what());
    }
    return 0;
}
