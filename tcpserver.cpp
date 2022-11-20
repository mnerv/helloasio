/**
 * @file server.cpp
 * @author mononerv (me@mononerv.dev)
 * @brief  Server with asio
 * @date   2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <thread>
#include <atomic>
#include <array>
#include <string>
#include <algorithm>

#include "fmt/format.h"
#include "asio.hpp"

namespace nrv {

struct message {};

class connection : std::enable_shared_from_this<connection> {
public:
    connection(asio::ip::tcp::socket socket) : m_socket(std::move(socket)) {
        read_loop();
        write_loop();
    }
    ~connection() { m_socket.close(); }

    auto is_close() const -> bool { return !m_socket.is_open(); }

private:
    auto read_loop() -> void {
        asio::async_read(m_socket, asio::buffer(m_message_in), [this](std::error_code error, std::size_t byte_size) {
            if (!error) {
                fmt::print("byte read: {}\n", byte_size);
                std::string str;
                std::transform(std::begin(m_message_in), std::end(m_message_in), std::back_inserter(str), [](auto const& byte) { return (char)byte; });
                fmt::print("{}\n", str);
                read_loop();
            } else {
                m_socket.close();
            }
        });
    }
    auto write_loop() -> void {
        // asio::async_write(m_socket, asio::buffer(m_str.data(), m_str.size()), [this](std::error_code error, std::size_t byte_size) {
        //     if (!error) {
        //         fmt::print("sent {} bytes\n", byte_size);
        //         m_socket.close();
        //     } else {
        //         m_socket.close();
        //     }
        // });
    }

private:
    asio::ip::tcp::socket m_socket;
    std::array<std::uint8_t, 512> m_message_in{};
};

class serve {
public:
    serve(asio::ip::port_type const& port)
        : m_context(), m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}
    ~serve() { stop(); }

    auto asio_context() -> asio::io_context& { return m_context; }

    auto run() -> void {
        if (m_is_running) return;
        try {
            wait_for_connections();
            m_thread = std::thread([&] { m_context.run(); });
            fmt::print("nrv::serve running at {}:{}\n", m_acceptor.local_endpoint().address().to_string(),
                m_acceptor.local_endpoint().port());
        } catch (std::exception const& e) {
            fmt::print("nrv::serve: {}\n", e.what());
        }
    }
    auto stop() -> void {
        m_context.stop();
        if (m_thread.joinable()) m_thread.join();
    }

protected:
    auto wait_for_connections() -> void {
        m_acceptor.async_accept([this](std::error_code error, asio::ip::tcp::socket socket) {
            if (!error) {
                fmt::print("nrv::serve new connection: {}:{}\n", socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port());
                auto new_conn = std::make_shared<connection>(std::move(socket));
                m_connections.push_back(std::move(new_conn));
            } else {
                fmt::print("nrv:serve new connection error: {}\n", error.message());
            }
            std::erase_if(m_connections, [](auto const& conn) { return conn->is_close(); });
            fmt::print("connection size: {}\n", m_connections.size());
            wait_for_connections();
        });
    }

protected:
    asio::io_context        m_context;
    asio::ip::tcp::acceptor m_acceptor;

    std::vector<std::shared_ptr<connection>> m_connections{};
    std::atomic<bool> m_is_running{false};
    std::thread       m_thread{};
};
}

auto main([[maybe_unused]]int argc, [[maybe_unused]]char const* argv[]) -> int {
    nrv::serve server{3000};
    server.run();
    for (;;);
    return 0;
}

