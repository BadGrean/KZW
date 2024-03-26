#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <array>
#include <vector>


std::unordered_map<std::string, std::vector<std::array<int, 4>>> read_file_to_map(std::string file_name);



