#include "filesys.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace filesys {

std::filesystem::path get_save_path() {
    const char* xdg_data_home = std::getenv("XDG_DATA_HOME");

    std::filesystem::path base_path;
    // Checking if null ptr and if xdg_data_home exists
    if (xdg_data_home && *xdg_data_home) {
        base_path = xdg_data_home;
    } else {
        const char* home = std::getenv("HOME");
        if (!home) {
            throw std::runtime_error("HOME environment var not set");
        }
        base_path = std::filesystem::path(home) / ".local" / "share";
    }

    std::filesystem::path app_path = base_path / "timeline";

    std::filesystem::create_directory(app_path);
    return app_path;
}

std::filesystem::path create_new_project_save(const std::string& project_name) {
    std::filesystem::path save_path = get_save_path() / project_name;

    // check if it exists
    if (std::filesystem::exists(save_path)) {
        throw std::invalid_argument("project already exists");
    }

    return save_path;
}

std::filesystem::path get_cached_project_path() {
    const std::filesystem::path save_path = get_save_path();
    std::ifstream cache_file{ save_path / CACHE_FILE_NAME };

    if (!cache_file) {
        throw std::invalid_argument("Cache file error: " + save_path.string() + '\n');
    }

    std::string proj_name;
    cache_file >> proj_name;

    return save_path / proj_name;
}

void cache_project_path(const std::string& project_name) {
    const std::filesystem::path save_path = get_save_path();
    std::ofstream cache_file{ save_path / CACHE_FILE_NAME };
    cache_file << project_name;
}

} // namespace filesys
