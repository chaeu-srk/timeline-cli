#pragma once
#include "filesys.h"
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace timeline {

using time_point = std::chrono::time_point<std::chrono::system_clock>;
inline constexpr std::string SEPERATOR_STRING{ "-----" };
inline constexpr std::string SAVE_FILE_NAME{ "save.txt" };
inline constexpr int date_string_size{ 6 };

struct Item {
    std::string name{};
    time_point time{};
};

/// Provides overloaded behaviour for putting Item into output streams
std::ostream& operator<<(std::ostream& stream, const Item& item);

/// Provides overloaded behaviour for extracting Item from input streams
std::istream& operator>>(std::istream& stream, Item& item);

/// Provides configureable properties of the Timeline class
struct Config {
    bool debug = false;
    bool dont_collapse_timestamps = true;
};

class Timeline {
public:
    Timeline(Config& config) : config{ std::move(config) } { init(); }

    void append_to_timeline(std::string name);

    /**
     * @brief Sorts the timeline by asceding date
     *
     * @requires items != null
     */
    void sort_timeline();

    /**
     * @brief Displayable string of the timeline
     *
     * Formats the timeline items into a single string that can be displayed to stdout
     */
    std::string to_string();

    /**
     * @brief Converts an item to a string (used by Timeline.to_string())
     */
    std::string display_item(const Item& item, bool collapse_timestamps);

    void write_to_save();

    void read_from_save();

    /**
     * @brief Clears the items vector then writes to save;
     */
    void reset();

private:
    Config config;

    std::vector<Item> items;
    std::filesystem::path save_file_path;

    /**
     * @brief Sets the save_file_path
     * Gaurenteed to run on object creation
     */
    void init();
};

} // namespace timeline
