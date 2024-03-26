#include <iostream>
#include <string>
#include<chrono>
#include <algorithm>

#include "File_handler.h"
#include "C_max_calculate.h"
#include "C_max_minimalization.h"

int main()
{
	std::chrono::high_resolution_clock::time_point start, end;
	start = std::chrono::high_resolution_clock::now();

	std::unordered_map<std::string, std::vector<std::array<int, 4>>> data_map = read_file_to_map("data/data.txt");

	for (auto& data : data_map)
	{
		data.second = calculate_best_C_max(data.second);
		for (auto& data2 : data.second)
		{
			std::cout << data2[3] << " ";
		}
		std :: cout << std::endl;
	}

	
	std::vector<int> C_max_vector = calculate_C_max_all(data_map);
	C_max_display(C_max_vector);




	end = std::chrono::high_resolution_clock::now();
	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
	return 0;
}