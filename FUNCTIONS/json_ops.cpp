#include "json_ops.h"

bool write_map_to_json(const fs::path& filename,const std::unordered_map<fs::path,fs::path>& map){

    json j = map;

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return 1;
    }
    file << j.dump(4);
    file.close();
    return 0;
    
}


std::unordered_map<fs::path, fs::path> read_map_from_json(const fs::path& filename) {
    std::unordered_map<fs::path, fs::path> result;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << " for reading." << std::endl;
        return result;
    }

    json j;
    try {
        file >> j;
    } catch (const json::parse_error& e) {
        std::cerr << "Failed to parse JSON from " << filename << ": " << e.what() << std::endl;
        return result;
    }

    try {
        auto tmp_map = j.get<std::unordered_map<std::string, std::string>>();
        for (const auto& [key_str, value_str] : tmp_map) {
            result[fs::path(key_str)] = fs::path(value_str);
        }
    } catch (const json::type_error& e) {
        std::cerr << "JSON format error in " << filename << ": " << e.what() << std::endl;
        return {}; // empty map on error
    }

    return result;
}

