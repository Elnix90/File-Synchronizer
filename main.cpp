#include "get_user_dir.h"
#include "write_read_files.h"
#include "copy_paste.h"
#include "json_ops.h"


int main() {

    std::vector<fs::path> bases = read_data_and_tmp_dirs();
    const fs::path data_base = bases.at(0);
    const fs::path tmp_base = bases.at(1);

    create_base_and_data_dirs();

    const fs::path entry_dir_file =          data_base / "source_dirs.txt";
    const fs::path backup_dir_file =         data_base / "backup_dir.txt";

    // const fs::path entry_dir_file =          data_base / "test_source_dirs.txt";
    // const fs::path backup_dir_file =         data_base / "test_backup_dir.txt";

    const fs::path all_backed_up_files =     data_base / tmp_base / "backup_files.txt";

    const fs::path to_backup =               data_base / tmp_base / "to_backup_file.json";
    const fs::path to_erase =                data_base / tmp_base / "to_erase_file.txt";

    // Initialization of GTK
    int argc = 0;
    char **argv = nullptr;
    gtk_init(&argc, &argv);



    // ADDING NEW DIR TO BACKUP LOGIC, MODIFY AFTER TO AVOID BUGS OF CHOICE
    std::unordered_set<fs::path> source_dirs = get_to_backup_folders(entry_dir_file);

    // LOGIC TO GET THE DESTINATION OF THE SAVED FILES (ONLY ONE PATH)
    fs::path backup_dir = get_backup_folder(backup_dir_file);


    std::cout << "\n" << std::endl;


    // Here I have the list of the paths to backup and the destination backup folder

    std::unordered_map<fs::path,fs::path> files_to_backup;
    std::unordered_set<fs::path> files_to_erase;
    std::unordered_set<fs::path> backup_paths;

    // List files in backup folder
    if (!fs::exists(to_backup) && !fs::exists(to_erase)) { //Do not compute if the files have alreay been computed
    // if (true) { // Only for tests, computes every time

        list_files_in_path(backup_dir,all_backed_up_files);
        std::cout << std::endl;

        std::ifstream file(all_backed_up_files);
        std::string line;

        while (std::getline(file, line)) {
            if (!line.empty()) {
                fs::path full_path(line);
                // std::cout << "Full path   : " << full_path << std::endl;
                // std::cout << "Base path   : " << base_path << std::endl;
                // std::cout << "Backup dir  : " << backup_dir << std::endl;

                fs::path rel_path;
                try {
                    rel_path = fs::relative(full_path, backup_dir);
                } catch (const std::exception& e) {
                    std::cerr << "Error converting path: " << e.what() << "\n";
                    continue;
                }
                // std::cout << "rel path : " << rel_path << std::endl;
                backup_paths.insert(rel_path);
            }
        }

        file.close();



        // List every file in every source folders
        int OLDFILES = 0;
        int UPDATEDFILES = 0;
        

        for (const fs::path& dir : source_dirs) {


            fs::path filepath = data_base / tmp_base / dir.filename();
            filepath += ".txt";

            // Write the list of files from this source directory
            int filenumber = list_files_in_path(dir, filepath);
            if (filenumber != -1) {

                // Open the file with files from this source directory
                std::ifstream file(filepath);
                if (!file.is_open()) {
                    std::cerr << "Failed to open " << filepath << std::endl;
                    continue;
                }
                

                int computed_files = 0;
                int new_files = 0;
                int updated_files = 0;
                std::string line;
                while (std::getline(file, line)) {
                    if (line.empty())
                        continue;

                    fs::path full_path(line);
                    fs::path relative_path;

                    // std::cout << "Full path : " << full_path << std::endl;

                    
                    try {
                        // Compute the relative path of this file from the current source directory
                        relative_path = fs::relative(full_path, dir.parent_path());
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error computing relative path for " << line << ": " << e.what() << "\n" << std::endl;
                        continue;
                    }


                    // std::cout << "Rel path  : " << relative_path << std::endl;


                    // Check if this relative path exists in backup_paths
                    if (backup_paths.find(relative_path) == backup_paths.end()) {


                        fs::path copy_path = backup_dir / relative_path;

                        files_to_backup.insert({full_path,copy_path});
                        // std::cout << "Path : " << relative_path << " not found in backup folder, adding it to files_to_backup" << std::endl;
                        new_files++;

                    } else {
                        // The file exists in backup, compare modification times.
                        fs::path backup_file_path = backup_dir / relative_path;

                        try {
                            auto source_time = fs::last_write_time(full_path);
                            auto backup_time = fs::last_write_time(backup_file_path);

                            // If source is more recent, add to backup
                            if (source_time > backup_time) {
                                files_to_backup.insert({full_path, backup_file_path});
                                updated_files++;
                                UPDATEDFILES++;
                            }
                        } catch (const fs::filesystem_error& e) {
                            std::cerr << "Error checking modification times for " 
                                    << full_path << " and " << backup_file_path 
                                    << ": " << e.what() << "\n" << std::endl;
                        }

                        // Remove from backup_paths (these files still exist in source)
                        backup_paths.erase(relative_path);
                        OLDFILES++;
                    }
                    computed_files ++;
                    std::cout << "\r" << computed_files << " / " << filenumber << " files computed :   " << new_files << " files to backup | " << updated_files << " files to update" << std::flush;
                }
                file.close();
                std::cout << "\n" << std::endl;
                // std::cout << new_files << " new files to backup" << std::endl;
                // std::cout << updated_files << " files upadated to overwrite" << std::endl;
            } else {
                continue;
            }
        }

        // Add full path to files to erase
        fs::path full_path;
        std::unordered_set<fs::path> files_to_erase;
        for (const fs::path& dir : backup_paths) {
            full_path = backup_dir / dir;
            files_to_erase.insert(full_path);
        }

        
        write_paths_in_file(to_erase,files_to_erase);
        write_map_to_json(to_backup,files_to_backup);

        std::cout << "TOTAL:" << std::endl;
        std::cout << UPDATEDFILES << " updated files to overwrite" << std::endl;
        std::cout << OLDFILES << " files already saved" << std::endl;

    }

    files_to_backup = read_map_from_json(to_backup);
    files_to_erase = read_paths_from_file(to_erase);

    std::cout << files_to_erase.size() << " files to delete" << std::endl;
    std::cout << files_to_backup.size() << " new files to backup" << std::endl;

    // Copy logic (waits user confirmation)
    if (!files_to_backup.empty()) {
        std::string usr_aggreement_copy;
        std::cout << "Copy files? (Y/n) : ";
        std::cin >> usr_aggreement_copy;
        std::cout << std::endl;
        std::transform(usr_aggreement_copy.begin(),usr_aggreement_copy.end(),usr_aggreement_copy.begin(),::tolower);

        if (usr_aggreement_copy == "y") {
            std::cout << "User aggreement received, begining copy" << std::endl;
            
            int files_copied = 0;
            int files_to_copy = files_to_backup.size();
            for (auto it = files_to_backup.begin(); it != files_to_backup.end(); ) {
                const auto& src_path = it->first;
                const auto& dest_path = it->second;

                if (copy_single_file(src_path, dest_path)) {

                    ++files_copied;
                    std::cout << "\rCopying : " << files_copied << " / " << files_to_copy  << "      : " << fs::relative(dest_path,backup_dir) << std::flush;
                    it = files_to_backup.erase(it);
                    write_map_to_json(to_backup,files_to_backup);

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
        if (fs::exists(to_backup)) {
            remove_single_file(to_backup);
        }
    }
    // remove_single_file(to_backup);


    // Erase logic

    if (!files_to_erase.empty()) {
        std::string usr_agreement_erase;
        std::cout << "Erase files? (Y/n): ";
        std::cin >> usr_agreement_erase;
        std::cout << std::endl;
        std::transform(usr_agreement_erase.begin(), usr_agreement_erase.end(),
                    usr_agreement_erase.begin(), ::tolower);

        if (usr_agreement_erase == "y") {
            std::cout << "User agreement received, beginning erasing..." << std::endl;

            size_t files_total = files_to_erase.size();
            size_t files_erased = 0;

            for (auto it = files_to_erase.begin(); it != files_to_erase.end(); ) {
                const fs::path& src_path = *it;
                fs::path filepath = backup_dir / src_path;

                if (remove_single_file(filepath)) {
                    ++files_erased;
                    std::cout << "\rErased : " << files_erased << " / " << files_total << " files" << std::flush;

                    it = files_to_erase.erase(it);

                    // Optionally, erase from backup_paths if needed:
                    // backup_paths.erase(src_path);

                    // Write updated backup_paths to file if that is intended
                    write_paths_in_file(to_erase, files_to_erase);

                } else {
                    std::cerr << "\nError removing " << src_path << std::endl;
                    ++it;
                }
            }


            std::cout << std::endl;

            // Write any updated data if needed after removals
            write_map_to_json(to_backup, files_to_backup);

        } else {
            std::cerr << "Invalid response received, aborting erase." << std::endl;
        }
    } else {
        std::cout << "No files to erase" << std::endl;
        if (fs::exists(to_erase)) {
            remove_single_file(to_erase);
        }
    }

    std::string usr_agreement_erase_tmp_files;
        std::cout << "Erase temporary files in " << data_base / tmp_base << " ? (Y/n): ";
        std::cin >> usr_agreement_erase_tmp_files;
        std::cout << std::endl;
        std::transform(usr_agreement_erase_tmp_files.begin(), usr_agreement_erase_tmp_files.end(),
        usr_agreement_erase_tmp_files.begin(), ::tolower);

        if (usr_agreement_erase_tmp_files == "y") {
            remove_dir_recursive(data_base / tmp_base);
        }


    return 0;
}
