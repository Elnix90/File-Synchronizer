#ifndef COPY_PASTE
#define COPY_PASTE


#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

bool copy_single_file(const fs::path& source, const fs::path& destination);
bool remove_single_file(const fs::path& file_to_remove);
bool remove_dir_recursive(const fs::path& dir);

#endif