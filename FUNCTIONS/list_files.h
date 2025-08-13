#ifndef LIST_FILES
#define LIST_FILES


#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <fstream>

namespace fs = std::filesystem;

std::unordered_map<fs::path,fs::path> get_set_relative_path_from_dir(const fs::path& path,const bool& include_child_folder = false);

#endif