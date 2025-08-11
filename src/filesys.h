#pragma once

#include <cstdlib>
#include <filesystem>

namespace filesys {
namespace fs = std::filesystem;

/**
 * @brief returns the save path that stores timeline saves
 */
fs::path get_save_path();

} // namespace filesys
