#include "filesys.h"

namespace filesys {

fs::path get_save_path() {
    const char* xdg_data_home = std::getenv("XDG_DATA_HOME");

    fs::path base_path;
    // Checking if null ptr and if xdg_data_home exists
    if (xdg_data_home && *xdg_data_home) {
        base_path = xdg_data_home;
    } else {
        const char* home = std::getenv("HOME");
        if (!home) {
            throw std::runtime_error("HOME environment var not set");
        }
        base_path = fs::path(home) / ".local" / "share";
    }

    fs::path app_path = base_path / "timeline";

    fs::create_directory(app_path);
    return app_path;
}

} // namespace filesys
