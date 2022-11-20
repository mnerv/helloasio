/**
 * @file   echoserver.cpp
 * @author mononerv (me@mononerv.dev)
 * @brief  Hello World with asio
 * @date   2022-11-20
 *
 * @copyright Copyright (c) 2022
 */

#include <thread>

#include "fmt/format.h"
#include "asio.hpp"

using tcp_acceptor = asio::use_awaitable_t<>::as_default_on_t<asio::ip::tcp::acceptor>;
using tcp_socket   = asio::use_awaitable_t<>::as_default_on_t<asio::ip::tcp::socket>;

auto echo(tcp_socket socket) -> asio::awaitable<void> {
    try {
        char data[1024];
        for (;;) {
            std::size_t n = co_await socket.async_read_some(asio::buffer(data));
            co_await asio::async_write(socket, asio::buffer(data, n));
        }
    } catch (asio::system_error const& e) {
        if (e.code() != asio::error::misc_errors::eof)
            std::printf("echo exception: %s\n", e.what());
    }
}

auto listener() -> asio::awaitable<void> {
    auto exe = co_await asio::this_coro::executor;
    tcp_acceptor acceptor(exe, {asio::ip::tcp::v4(), 3000});
    fmt::print("server listening at {}:{}\n", acceptor.local_endpoint().address().to_string(), acceptor.local_endpoint().port());
    for (;;) {
        try {
            auto socket = co_await acceptor.async_accept();
            co_spawn(exe, echo(std::move(socket)), asio::detached);
        } catch (std::exception const& e) {
            fmt::print("new connection error: {}\n", e.what());
        }
    }
}

auto main([[maybe_unused]]int argc, [[maybe_unused]]char const* argv[]) -> int {
    try {
        asio::io_context context;
        asio::signal_set signals(context, SIGINT, SIGTERM);
        signals.async_wait([&] (auto, auto) { context.stop(); });
        asio::co_spawn(context, listener, asio::detached);
        context.run();
    } catch (std::exception const& e) {
        fmt::print("exception: {}\n", e.what());
    }
    return 0;
}
