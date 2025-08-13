#include "write_read_files.h"

void add_directory_to_file(const std::string& directory, const fs::path& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << directory << std::endl;
        file.close();
        std::cout << directory << " added to " << filename << std::endl;
    } else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}



std::unordered_set<fs::path> read_source_paths(const fs::path& file_to_read) {

    std::unordered_set<fs::path> paths;

    std::ifstream file(file_to_read);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return paths; // Returns empty list if error of reading
    } else {
    // std::cout << "DEBUG: file '" << file_to_read << "' open" << std::endl;
    std::string path;
    while (std::getline(file, path)) {
        paths.insert(path);
        std::cout << "Source path found in ( "<< file_to_read << " ) : " << path << std::endl;
    }
    
    file.close();
    return paths;

    }
}


fs::path read_backup_path(const fs::path& file_to_read) {
    std::ifstream file(file_to_read);
    if (!file) {
        std::cerr << "Error opening file for reading: " << file_to_read << std::endl;
        return {}; // return empty path
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "File is empty or could not read a line: " << file_to_read << std::endl;
        return {};
    }

    fs::path result = line; // implicitly converts from std::string
    std::cout << "Backup path found in (" << file_to_read << "): " << result << std::endl;
    return result;
}



int list_files_in_path(const fs::path& path, const fs::path& file_to_write) {
    std::ofstream file(file_to_write);
    if (!file.is_open()) {
        std::cerr << "Error opening file to write path" << std::endl;
        return -1;
    }

    int n = 0;
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            file << entry.path().string() << std::endl;
            n += 1;
        }
    }

    std::cout << n << " files found in " << path << std::endl; 
    // std::cout << "Wrote in " << file_to_write << std::endl;

    file.close();
    return n;
}


std::unordered_set<fs::path> read_paths_from_file(const fs::path& filename) {
    
    std::unordered_set<fs::path> paths;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return paths;
    } else {

    std::string path;
    int n = 0;
    while (std::getline(file, path)) {
        paths.insert(path);
        n++;
    }
    // std::cout << n << " paths found" << std::endl;
    
    file.close();
    return paths;

    }
}

int write_paths_in_file(const fs::path& filename,const std::unordered_set<fs::path>& paths) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file to write path" << std::endl;
        return 1;
    }

    for (const std::string& entry : paths) {
        file << entry << std::endl;
    }
    file.close();
    return 0;

}

void create_base_and_data_dirs(
    const fs::path data_base,
    const fs::path tmp_base) {
    
    fs::create_directories(data_base);
    fs::create_directories(data_base / tmp_base);

}

std::vector<fs::path> read_data_and_tmp_dirs(const fs::path filepath,const std::vector<fs::path> default_paths) {

    std::vector<fs::path> paths;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return default_paths;
    } else {
        std::string path;
        while (std::getline(file,path)) {
            fs::path Path(path);
            paths.push_back(Path);
        }
        return paths;
    }
}

