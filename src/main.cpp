#include "timeline.h"
#include <CLI11.hpp>

int main(int argc, char* argv[]) {
    CLI::App app{};

    auto append = app.add_subcommand("append", "append to the timeline");
    bool debug_flag;
    app.add_flag("-d,--debug", debug_flag, "add debug printing");

    std::string append_buf;
    append->add_option("-n,--name", append_buf, "name to append");

    timeline::Config config;
    config.debug = debug_flag;

    CLI11_PARSE(app, argc, argv);
    timeline::Timeline tl{ config };
    tl.read_from_save();

    if (*append) {
        if (append_buf.empty()) {
            std::cout << "Append new item: \n";
            std::cin >> append_buf;
            // TODO: handle errors
        }

        tl.append_to_timeline(append_buf);
        std::cout << tl.to_string();

    } else {
        std::cout << tl.to_string();
    }
    return 0;
}
