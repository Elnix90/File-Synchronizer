#ifndef BACKUP_AND_ERASE
#define BACKUP_AND_ERASE

#include "list_files.h"
#include "compute_files.h"
#include "write_read_files.h"
#include "copy_paste.h"

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <filesystem>

namespace fs = std::filesystem;

void create_files(
    fs::path backup_dir,
    std::unordered_set<fs::path> source_dirs,
    fs::path to_erase_file,
    fs::path to_backup_file,
    bool check_modification_dates,
    bool remove_deleted_elements
);

int get_urs_agree(std::string question);



void backup(
    fs::path to_backup_file,
    fs::path to_erase_file,
    fs::path backup_dir
);


void erase(
    fs::path to_erase_file,
    fs::path to_backup_file,
    fs::path backup_dir
);

void remove_tmp_files(fs::path dir);

#endif