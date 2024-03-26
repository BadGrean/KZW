#include "File_handler.h"


void make_array(std::string line, std::array<int, 4>& data_array)
{
	std::string temp;
	int counter = 0;
	size_t first_space = line.find(' ');
	size_t last_space = line.find(' ', first_space + 1);
	data_array[0] = std::stoi(line.substr(0, first_space));
	data_array[1] = std::stoi(line.substr(first_space + 1, last_space));
	data_array[2] = std::stoi(line.substr(last_space + 1));
}


std::unordered_map<std::string, std::vector<std::array<int, 4>>> read_file_to_map(std::string file_name)
{
	std::ifstream data_file(file_name);
	std::string line;
	std::unordered_map<std::string, std::vector<std::array<int, 4>>> data_map;
	std::array<int, 4> data_array;
	int i = 1, j = 1;
	bool write_next_line = false;
	int counter = 0;

	if (!data_file)
	{
		std::cerr << "Error: Could not open file" << std::endl;
		exit(1);
	}

	while (std::getline(data_file, line))
	{
		if (write_next_line)
		{
			if (counter == 0)
			{
				counter = std::stoi(line);
			}
			else
			{
				counter -= 1;
				make_array(line, data_array);
				data_array[3] = j;
				j++;
				data_map["data." + std::to_string(i)].push_back(data_array);
				
			}
			if (counter == 0)
			{
				write_next_line = false;
				i++;
				j = 1;
			}
		}

		if (line == ("data." + std::to_string(i)))
		{
			data_map[line];
			write_next_line = true;

		}
	}
	return data_map;
}

