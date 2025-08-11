#include "timeline.h"
#include <algorithm>
#include <fstream>
#include <iomanip>

namespace timeline {

std::istream& operator>>(std::istream& stream, Item& item) {
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

std::ostream& operator<<(std::ostream& stream, const Item& item) {
    stream << item.name << '\n'
           << std::chrono::system_clock::to_time_t(item.time) << '\n'
           << SEPERATOR_STRING << '\n';
    return stream;
}

// ------------------------------------------------------------------
// TimeLine implementations

void Timeline::append_to_timeline(std::string name) {
    items.emplace_back(name, std::chrono::system_clock::now());
}

void Timeline::sort_timeline() {
    std::sort(items.begin(), items.end(),
              [](const Item& a, const Item& b) { return a.time < b.time; });
}

std::string Timeline::to_string() {
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

        // Only creates seperator if not last item
        if (i < items.size() - 1) {
            res << res.fill(' ') << std::setw(date_string_size) << '|' << '\n';
        }

        ++i;
        previous_time_p = floor<std::chrono::days>(item.time);
    }

    return res.str();
}

std::string Timeline::display_item(const Item& item, bool collapse_timestamps) {
    std::string res;
    if (collapse_timestamps) {
        res = std::string(date_string_size, ' ');
        res += "* " + item.name;
    } else {
        res = std::format("{0:%d}/{0:%m} * {1:}", item.time, item.name);
    }
    return res;
}

void Timeline::write_to_save() {
    std::ofstream outfile{ SAVE_FILE_NAME };
    for (auto& item : items) {
        outfile << item;
    }
}

void Timeline::read_from_save() {
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

void Timeline::reset() {
    items.clear();
    write_to_save();
}

} // namespace timeline
