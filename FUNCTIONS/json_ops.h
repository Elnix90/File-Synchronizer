#ifndef JSON_OPS
#define JSON_OPS

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

std::unordered_map<fs::path, fs::path> read_map_from_json(const fs::path& filename);
bool write_map_to_json(const fs::path& filename,const std::unordered_map<fs::path,fs::path>& map);

#endif