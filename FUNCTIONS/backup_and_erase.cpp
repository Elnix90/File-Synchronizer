#include "backup_and_erase.h"


void create_files(
    fs::path backup_dir,
    std::unordered_set<fs::path> source_dirs,
    fs::path to_erase_file,
    fs::path to_backup_file,
    bool check_modification_dates,
    bool remove_deleted_elements
) {

    std::unordered_map<fs::path,fs::path> files_in_backup = get_set_relative_path_from_dir(backup_dir);

    std::unordered_map<fs::path,fs::path> files_in_phone;

    for (const fs::path& dir : source_dirs) {
        std::unordered_map<fs::path,fs::path> dir_map = get_set_relative_path_from_dir(dir,true);
        files_in_phone.merge(dir_map);
    }

    compute_files_to_backup_and_remove(
        files_in_backup,
        files_in_phone,
        backup_dir,
        to_erase_file,
        to_backup_file,
        check_modification_dates,
        remove_deleted_elements
    );

}


int get_usr_agree(std::string question) {
    std::string usr_agreement;
    std::cout << question;
    std::cin >> usr_agreement;
    std::transform(usr_agreement.begin(),usr_agreement.end(),usr_agreement.begin(),::tolower);

    if (usr_agreement == "y") {
        std::cout << "User aggreement received, begining operation..." << std::flush;
        return true;
    } else {
        return false;
    }
        
}


void backup(
    fs::path to_backup_file,
    fs::path to_erase_file,
    fs::path backup_dir
) {
    if (!fs::exists(to_backup_file)) {
    }

    std::unordered_map<fs::path,fs::path> files_to_backup = read_map_from_json(to_backup_file);
    if (!files_to_backup.empty()) {
        if (get_usr_agree("Copy files? (Y/n) : ")) {
            int files_copied = 0;
            int n_files_to_copy = files_to_backup.size();

            for (auto it = files_to_backup.begin(); it != files_to_backup.end(); ) {
                const auto& src_path = it->first;
                const auto& dest_path = it->second;

                if (copy_single_file(src_path, dest_path)) {

                    ++files_copied;
                    std::cout << "\rCopying : " << files_copied << " / " << n_files_to_copy  << "      : " << fs::relative(dest_path,backup_dir) << std::string(120, ' ') << std::flush;
                    it = files_to_backup.erase(it);
                    write_map_to_json(to_backup_file,files_to_backup);

                } else {
                    std::cerr << "\nError copying " << src_path << " to " << dest_path << std::endl;
                    ++it;
                }
            }
            std::cout << std::endl;
        } else {
            std::cerr << "Invalid response received, aborting copy" << std::endl;
        }

    } else {
        std::cout << "No files to copy" << std::endl;
        remove_single_file(to_backup_file);
    }
}



void erase(
    fs::path to_erase_file,
    fs::path to_backup_file,
    fs::path backup_dir
) {
    if (!fs::exists(to_erase_file)) {

    }

    std::unordered_set<fs::path> files_to_erase = read_paths_from_file(to_erase_file);
    if (!files_to_erase.empty()) {

        if (get_usr_agree("Erase files? (Y/n): ")) {

            size_t files_total = files_to_erase.size();
            size_t files_erased = 0;

            for (auto it = files_to_erase.begin(); it != files_to_erase.end(); ) {
                const fs::path& src_path = *it;
                fs::path filepath = backup_dir / src_path;

                if (remove_single_file(filepath)) {
                    ++files_erased;
                    std::cout << "\rErased : " << files_erased << " / " << files_total << " files" << std::flush;

                    it = files_to_erase.erase(it);

                    write_paths_in_file(to_erase_file, files_to_erase);

                } else {
                    std::cerr << "\nError removing " << src_path << std::endl;
                    ++it;
                }
            }
            std::cout << std::endl;

        } else {
            std::cerr << "Invalid response received, aborting erase." << std::endl;
        }
    } else {
        std::cout << "No files to erase" << std::endl;
        remove_single_file(to_erase_file);
    }
}


void remove_tmp_files(fs::path dir) {
        if (get_usr_agree("Erase temporary files? (Y/N) : ")) {
            remove_dir_recursive(dir);
            std::cout << "\rTemporary files erased" << std::string(50,' ') << std::endl;
        }
}
