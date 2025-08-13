#include "FUNCTIONS/backup_and_erase.h"
#include "FUNCTIONS/compute_files.h"
#include "FUNCTIONS/copy_paste.h"
#include "FUNCTIONS/get_user_dir.h"
#include "FUNCTIONS/json_ops.h"
#include "FUNCTIONS/list_files.h"
#include "FUNCTIONS/write_read_files.h"


int main() {

    std::vector<fs::path> bases = read_data_and_tmp_dirs();
    const fs::path data_base = bases.at(0);
    const fs::path tmp_base = bases.at(1);

    create_base_and_data_dirs();

    // const fs::path entry_dir_file =          data_base / "source_dirs.txt";
    // const fs::path backup_dir_file =         data_base / "backup_dir.txt";

    const fs::path entry_dir_file =          data_base / "test_source_dirs.txt";
    const fs::path backup_dir_file =         data_base / "test_backup_dir.txt";

    const fs::path all_backed_up_files =     data_base / tmp_base / "backup_files.txt";

    const fs::path to_backup_file =               data_base / tmp_base / "to_backup_file.json";
    const fs::path to_erase_file =                data_base / tmp_base / "to_erase_file.txt";

    // Initialization of GTK
    int argc = 0;
    char **argv = nullptr;
    gtk_init(&argc, &argv);



    // ADDING NEW DIR TO BACKUP LOGIC, MODIFY AFTER TO AVOID BUGS OF CHOICE
    std::unordered_set<fs::path> source_dirs = get_to_backup_folders(entry_dir_file);

    // LOGIC TO GET THE DESTINATION OF THE SAVED FILES (ONLY ONE PATH)
    fs::path backup_dir = get_backup_folder(backup_dir_file);

    create_files(
        backup_dir,
        source_dirs,
        to_erase_file,
        to_backup_file,
        true, // CHECK MODIFICATION DATES
        true // REMOVE DELETED FILES ON PHONE FROM BACKUP
    );

    backup(
        to_backup_file,
        to_erase_file,
        backup_dir
    );
    erase(
        to_erase_file,
        to_backup_file,
        backup_dir
    );
    
    remove_tmp_files(data_base / tmp_base);


    return 0;
}
