
#include "C_max_calculate.h"


std::vector<int> calculate_C_max_all(std::unordered_map<std::string, std::vector<std::array<int, 4>>>& map)
{
	int C_max;
	std::vector<int> C_max_vector;


	for (const auto& data : map)
	{
		int current_time = 0;
		int lowest_C = 0;
		int highest_C = 0;
		for (int i = 0; i < data.second.size(); i++)
		{
			if (current_time - data.second[i][0] < lowest_C)
				lowest_C = current_time - data.second[i][0];
			current_time += data.second[i][1];
			if (current_time + data.second[i][2] > highest_C)
				highest_C = current_time + data.second[i][2];
		}
		C_max = highest_C - lowest_C;
		C_max_vector.push_back(C_max);
	}

	C_max = 0;

	for (int i = 0; i < C_max_vector.size(); i++)
	{
		C_max += C_max_vector[i];
	}
	C_max_vector.push_back(C_max);


	return C_max_vector;
}

int calculate_C_max(std::vector<std::array<int,4>>& data)
{
	int current_time = 0;
	int lowest_C = 0;
	int highest_C = 0;
	for (int i = 0; i < data.size(); i++)
	{
		if (current_time - data[i][0] < lowest_C)
			lowest_C = current_time - data[i][0];
		current_time += data[i][1];
		if (current_time + data[i][2] > highest_C)
			highest_C = current_time + data[i][2];
	}
	return (highest_C - lowest_C);

}

void C_max_display(std::vector<int>& C_max_vector)
{
	size_t C_max_size = C_max_vector.size();
	for (int i = 0; i < C_max_size - 1; i++)
	{
		std::cout << "C_max data." << i + 1 << ": " << C_max_vector[i] << std::endl;
	}
	std::cout << "C_max total: " << C_max_vector[C_max_size - 1] << std::endl;
}