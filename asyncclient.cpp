/**
 * @file   asyncclient.cpp
 * @author mononerv (me@mononerv.dev)
 * @brief  Hello World with asio
 * @date   2022-11-20
 *
 * @copyright Copyright (c) 2022
 */

#include <thread>
#include <string>

#include "fmt/format.h"
#include "asio.hpp"

auto send_receive(asio::io_context& context) -> asio::awaitable<void> {
    asio::ip::tcp::socket socket(context);
    asio::ip::tcp::resolver resolver(context);
    co_await asio::async_connect(socket, resolver.resolve("localhost", "3000"), asio::use_awaitable);
    std::string str = "Hello, World!\n";
    co_await asio::async_write(socket, asio::buffer(str), asio::use_awaitable);
    char data[1024]{0};
    auto n = co_await socket.async_read_some(asio::buffer(data), asio::use_awaitable);
    fmt::print("{}", data);

    context.stop();
}

auto main([[maybe_unused]]int argc, [[maybe_unused]]char const* argv[]) -> int {
    try {
        asio::io_context context;
        asio::signal_set signals(context, SIGINT, SIGTERM);
        signals.async_wait([&] (auto, auto) { context.stop(); });
        asio::co_spawn(context, send_receive(context), asio::detached);
        context.run();
    } catch (std::exception const& e) {
        fmt::print("exception: {}\n", e.what());
    }
    return 0;
}
