#include <span>
#include "fmt/format.h"

static auto entry([[maybe_unused]]std::span<char const*> const& args) -> void {
    fmt::print("Hello, World!\n");
}

auto main(int argc, char const* argv[]) -> int {
    try {
        entry({argv, std::next(argv, argc)});
    } catch (std::exception const& e) {
        fmt::print(stderr, "Error at entry: {}\n", e.what());
        return 1;
    }
    return 0;
}
