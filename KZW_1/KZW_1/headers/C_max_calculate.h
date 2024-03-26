#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>



std::vector<int> calculate_C_max_all(std::unordered_map<std::string, std::vector<std::array<int, 4>>>& map);
int calculate_C_max(std::vector<std::array<int, 4>>& data);
void C_max_display(std::vector<int>& C_max_vector);