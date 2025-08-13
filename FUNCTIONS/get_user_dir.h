#ifndef GET_USER_DIR_H
#define GET_USER_DIR_H

#include <string>
#include <unordered_set>
#include <filesystem>
#include <vector>
#include <gtk/gtk.h>

namespace fs = std::filesystem;


fs::path get_user_directory();
bool ask_select_more();
std::unordered_set<fs::path> get_multiple_user_directories();

std::unordered_set<fs::path> get_to_backup_folders(const fs::path& entry_dir_file);
fs::path get_backup_folder(const fs::path& backup_dir_file);

#endif