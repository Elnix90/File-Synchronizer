#include "copy_paste.h"

bool copy_single_file(const fs::path& source, const fs::path& destination) {
    try {
        // Ensure the parent directory of destination exists
        fs::create_directories(destination.parent_path());

        // Copy file with overwrite if file exists
        fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Copy failed: " << e.what() << std::endl;
        return false;
    }
}

bool remove_single_file(const fs::path& file_to_remove) {
    try {
        return fs::remove(file_to_remove);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Remove failed: " << e.what() << std::endl;
        return false;
    }
}

bool remove_dir_recursive(const fs::path& dir) {
    try {
        fs::remove_all(dir);
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing directory " << dir << " : " << e.what() << std::endl;
        return false;
    }
}
