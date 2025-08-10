# include "get_user_dir.h"
#include "write_read_files.h"


fs::path get_user_directory() {
    GtkWidget *dialog;
    fs::path selected_dir;

    

    std::string dialog_title = "Select backup location";

    // Create a GTK folder picker dialog
    dialog = gtk_file_chooser_dialog_new(
        dialog_title.c_str(),        // Dialog title
        nullptr,                    // No parent window
        GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Open", GTK_RESPONSE_ACCEPT,
        nullptr
    );

    // Run the dialog and check user response
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *folder_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (folder_path != nullptr) {
            selected_dir = std::string(folder_path);  // Save the selected path
            g_free(folder_path);                       // Free GTK string
        }
    }

    gtk_widget_destroy(dialog); // Clean up dialog

    return selected_dir;        // Empty string if canceled
}



// Helper: Dialog to ask "Do you want to select more directories?"
bool ask_select_more() {
    GtkWidget *dialog = gtk_message_dialog_new(
        nullptr,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "Do you want to select another directory?"
    );

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    return (response == GTK_RESPONSE_YES);
}

std::unordered_set<fs::path> get_multiple_user_directories() {
    
    
    std::unordered_set<fs::path> selected_dirs;


    bool continue_selecting = true;

    while (continue_selecting) {
        
        GtkWidget *dialog = gtk_file_chooser_dialog_new(
            ("Select a folder to backup"),
            nullptr,
            GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
            "_Cancel", GTK_RESPONSE_CANCEL,
            "_Open", GTK_RESPONSE_ACCEPT,
            nullptr
        );

        // Run dialog and get response
        gint res = gtk_dialog_run(GTK_DIALOG(dialog));
        if (res == GTK_RESPONSE_ACCEPT) {
            char *folder_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            if (folder_path) {
                selected_dirs.insert(folder_path);
                g_free(folder_path);
            }
            gtk_widget_destroy(dialog);

            // Ask if user wants to select more
            continue_selecting = ask_select_more();
        } else {
            // User cancelled - break loop
            gtk_widget_destroy(dialog);
            break;
        }
    }

    return selected_dirs;
}

std::unordered_set<fs::path> get_to_backup_folders(const fs::path& entry_dir_file) {
    std::unordered_set<fs::path> source_dirs;

    if (fs::exists(entry_dir_file)) { //Trying to get the path from file if already exixts

            source_dirs = read_source_paths(entry_dir_file);
    }
    
    if (source_dirs.empty()){ // If source_dirs is still empty (if it is created or an attempt of reading failed)
        
        
        std::unordered_set<fs::path> source_dirs = get_multiple_user_directories();


        // std::cout << "DEBUG: user prompt source folder, returned : " << result_dir << std::endl;
        
        // Write in text files the paths of the directories if they aren't empty
        if (!source_dirs.empty()) {

            write_paths_in_file(entry_dir_file,source_dirs);

        } else {
            std::cout << "No directories selected." << std::endl;
        }
    }
    return source_dirs;
}

fs::path get_backup_folder(const fs::path& backup_dir_file) {
        fs::path backup_dir;

    if (fs::exists(backup_dir_file)) {
        backup_dir = read_backup_path(backup_dir_file);
    }

    if (backup_dir.empty()) {

        backup_dir = get_user_directory();

        // Write in text files the paths of the directories if they aren't empty
        if (!backup_dir.empty()) {
            
            add_directory_to_file(backup_dir, backup_dir_file);
            std::cout << "backup_dir saved successfully : " << backup_dir << std::endl;
        } else {
            std::cout << "No directories selected." << std::endl;
        }
    }
    return backup_dir;

}