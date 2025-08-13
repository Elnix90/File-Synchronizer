#ifndef WRITE_READ_FILES
#define WRITE_READ_FILES


#include <iostream>
#include <fstream>
#include <unordered_set>
#include <filesystem>
#include <vector>


namespace fs = std::filesystem;

void add_directory_to_file(const std::string& directory, const fs::path& filename);
std::unordered_set<fs::path> read_source_paths(const fs::path& file_to_read);
fs::path read_backup_path(const fs::path& file_to_read);
int list_files_in_path(const fs::path& path, const fs::path& file_to_write);
std::unordered_set<fs::path> read_paths_from_file(const fs::path& filename);
int write_paths_in_file(const fs::path& filename,const std::unordered_set<fs::path>& paths);
std::vector<fs::path>  read_data_and_tmp_dirs(const fs::path filepath = "datadirs.txt",const std::vector<fs::path> default_paths = {"DATA","TMP"});
void create_base_and_data_dirs(const fs::path data_base = "DATA",const fs::path tmp_base = "TMP");

#endif