#ifndef COMPUTE_FILES
#define COMPUTE_FILES

#include "json_ops.h"
#include "write_read_files.h"

#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

namespace fs = std::filesystem;

void compute_files_to_backup_and_remove(
    std::unordered_map<fs::path,fs::path> files_in_backup,
    std::unordered_map<fs::path,fs::path> files_in_phone,
    fs::path backup_dir,
    fs::path to_erase_file,
    fs::path to_backup_file,
    bool check_modification_dates = true,
    bool remove_deleted_elements = true
);

#endif