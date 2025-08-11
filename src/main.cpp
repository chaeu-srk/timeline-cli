#include "timeline.h"
#include <CLI11.hpp>
#include <string>

int main(int argc, char* argv[]) {
    CLI::App app{};
    timeline::Config config;

    auto append = app.add_subcommand("append", "append to the timeline");

    app.add_flag("-d,--debug", config.debug, "add debug printing");
    app.add_flag("--no-collapse-dates", config.dont_collapse_timestamps, "Don't collapse dates");
    auto reset = app.add_flag("--hard-reset", "reset saved timeline");

    std::string append_buf;
    append->add_option("-n,--name", append_buf, "name to append");

    CLI11_PARSE(app, argc, argv);

    timeline::Timeline tl{ config };
    tl.read_from_save();

    if (*append) {
        if (append_buf.empty()) {
            std::cout << "Append new item: \n";
            std::getline(std::cin, append_buf);
            // TODO: handle errors
        }

        tl.append_to_timeline(append_buf);
        std::cout << tl.to_string();
        tl.write_to_save();

    } else if (reset->count()) {
        tl.reset();

    } else {
        std::cout << tl.to_string();
    }
    return 0;
}
