#pragma once
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
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

    friend std::ostream& operator<<(std::ostream& stream, const Item& item) {
        stream << item.name << '\n'
               << std::chrono::system_clock::to_time_t(item.time) << '\n'
               << SEPERATOR_STRING << '\n';
        return stream;
    }

    friend std::istream& operator>>(std::istream& stream, Item& item) {
        std::string name;
        time_t time;
        std::string seperator;

        stream >> std::ws;
        std::getline(stream, name);

        if (stream >> time >> seperator) {
            item = Item{ name, std::chrono::system_clock::from_time_t(time) };
        }
        return stream;
    }
};

struct Config {
    bool debug = false;
    bool dont_collapse_timestamps = true;
};

class Timeline {
public:
    Timeline() = default;
    Timeline(Config& config) : config{ std::move(config) } { }

    void append_to_timeline(std::string name) {
        items.emplace_back(name, std::chrono::system_clock::now());
    }

    // sorts timeline by ascending
    void sort_timeline() {
        std::sort(items.begin(), items.end(),
                  [](const Item& a, const Item& b) { return a.time < b.time; });
    }

    std::string to_string() {
        size_t i{ 0 };
        std::stringstream res;
        if (items.empty()) {
            return "Empty timeline! Append something to the timeline.\n";
        }

        std::chrono::year_month_day previous_time_p;
        for (auto& item : items) {
            bool collapse = (previous_time_p == floor<std::chrono::days>(item.time)) &&
                            config.dont_collapse_timestamps;
            res << display_item(item, collapse) << '\n';

            if (i < items.size() - 1) {
                res << res.fill(' ') << std::setw(date_string_size) << '|' << '\n';
            }
            ++i;
            previous_time_p = floor<std::chrono::days>(item.time);
        }

        return res.str();
    }

    std::string display_item(const Item& item, bool collapse_timestamps) {
        std::string res;
        if (collapse_timestamps) {
            res = std::string(date_string_size, ' ');
            res += "* " + item.name;
        } else {
            res = std::format("{0:%d}/{0:%m} * {1:}", item.time, item.name);
        }
        return res;
    }

    void write_to_save() {
        std::ofstream outfile{ SAVE_FILE_NAME };
        for (auto& item : items) {
            outfile << item;
        }
    }

    void read_from_save() {
        std::ifstream infile{ SAVE_FILE_NAME };
        Item item_buf;

        while (infile >> item_buf) {
            items.push_back(item_buf);
        }

        if (config.debug) {
            if (infile.eof()) {
                std::cout << "Reached end of file.\n";
            } else if (infile.fail()) {
                std::cout << "Data format error.\n";
            } else if (infile.bad()) {
                std::cout << "Fatal I/O error.\n";
            }
        }
    }

private:
    Config config;

    std::vector<Item> items;
};

} // namespace timeline
