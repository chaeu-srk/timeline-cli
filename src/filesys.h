#pragma once

#include <cstdlib>
#include <filesystem>

namespace filesys {

inline constexpr std::string CACHE_FILE_NAME{ "cache.txt" };

/**
 * @brief gets the save path used by the program
 */
std::filesystem::path get_save_path();

/**
 * All it does is check if the project already exists,
 * File creation can just be delagated to ofstream
 */
std::filesystem::path create_new_project_save(const std::string& project_name);

/**
 * @brief gets the path of last opened project
 *
 * @throws invalid argument if cache file doesnt exist
 */
std::filesystem::path get_cached_project_path();

/**
 * @brief saves the project passed into the cache file
 */
void cache_project_path(const std::string& project_name);

} // namespace filesys
