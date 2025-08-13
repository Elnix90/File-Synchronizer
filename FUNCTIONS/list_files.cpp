#include "list_files.h"

std::unordered_map<fs::path,fs::path> get_set_relative_path_from_dir(const fs::path& path,const bool& include_child_folder) {
    std::unordered_map<fs::path,fs::path> paths;

    std::ifstream file(path);
    int filecount = 0;
    std::cout << std::endl;

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            fs::path parent_path = path;
            if (include_child_folder) {
                parent_path = path.parent_path();
            }
            fs::path rel_path = fs::relative(entry,parent_path);
            paths.insert({entry,rel_path});
            filecount++;
            std::cout << "\r" << filecount << " files found in" << path << std::flush;
        }
    }
    return paths;
}