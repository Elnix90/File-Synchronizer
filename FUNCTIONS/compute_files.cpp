#include "compute_files.h"

void compute_files_to_backup_and_remove(
    std::unordered_map<fs::path,fs::path> files_in_backup,
    std::unordered_map<fs::path,fs::path> files_in_phone,
    fs::path backup_dir,
    fs::path to_erase_file,
    fs::path to_backup_file,
    bool check_modification_dates,
    bool remove_deleted_elements
) {

    std::unordered_map<fs::path,fs::path> files_to_backup;

    int updated_files = 0;
    int new_tracked_files = 0;

    std::unordered_set<fs::path> rel_paths_in_backup;
    for (const auto& [key,value] : files_in_backup) {
        rel_paths_in_backup.insert(value);
    }

    int filecount = 0;
    int tot_filecount = files_in_phone.size();
    std::cout << std::endl;

    for (const auto& [full_path, rel_path] : files_in_phone) {
        fs::path backup_file_path = backup_dir / rel_path;
        if (rel_paths_in_backup.find(rel_path) != rel_paths_in_backup.end()) { // Path present in backup
            
            
            if (check_modification_dates) {
                try {
                    auto source_time = fs::last_write_time(full_path);
                    auto backup_time = fs::last_write_time(backup_file_path);

                    // If source is more recent, add to backup
                    if (source_time > backup_time) {
                        files_to_backup.insert({full_path, backup_file_path});
                        updated_files++;
                    }
                } catch (const fs::filesystem_error& e) {
                    std::cerr << "Error checking modification times for " 
                            << full_path << " and " << backup_file_path 
                            << ": " << e.what() << "\n" << std::endl;
                }

            } else {
                files_to_backup.insert({full_path, backup_file_path});
                new_tracked_files++;
            }

        } else {
            files_to_backup.insert({full_path, backup_file_path});
            new_tracked_files++;

        }

        rel_paths_in_backup.erase(rel_path);

        filecount++;
        std::cout << "\r" << filecount << " / " << tot_filecount << " files computed" << std::flush;
    }
    std::cout << "\n" << std::endl;

    write_paths_in_file(to_erase_file,rel_paths_in_backup);
    write_map_to_json(to_backup_file,files_to_backup);

    std::cout << "TOTAL:" << std::endl;
    std::cout << rel_paths_in_backup.size() << " files to remove" << std::endl;
    std::cout << new_tracked_files << " new files to backup" << std::endl;
    std::cout << updated_files << " files to update\n\n" << std::endl;

}