#include "C_max_minimalization.h"
#include "C_max_calculate.h"


void check_to_add_empty(std::vector<std::array<int, 4>>& data)
{
	for (size_t it = 0; it <= data.size(); ++it) {
		std::vector<std::array<int, 4>> temp_result = data;
		temp_result.insert(temp_result.begin() + it, { 0, 1, 0, 0});
		int temp = calculate_C_max(temp_result);
		if (calculate_C_max(temp_result) < calculate_C_max(data)) 
		{
			data = temp_result;
		}
	}
}

std::vector<std::array<int, 4>> check_further(std::vector<std::array<int, 4>> data, std::vector<std::pair<std::array<int, 4>, int>> Array_of_tasks_sorted_by_C_max_of_task, int i_start, int depth)
{
	if (depth == 0 || i_start < 0)
		return data;

	int min_C_max = INT32_MAX;
	int index_to_insert = 0;
	for (int j = 0; j < data.size(); j++)
	{
		std::vector<std::array<int, 4>> temp_result = data;
		std::vector<std::array<int, 4>> temp_result_offset = data;
		temp_result.insert(temp_result.begin() + j, Array_of_tasks_sorted_by_C_max_of_task[i_start].first);
		temp_result_offset.insert(temp_result_offset.begin() + j + 1, Array_of_tasks_sorted_by_C_max_of_task[i_start].first);
		int c_max_temp = calculate_C_max(temp_result);
		int c_max_temp_offset = calculate_C_max(temp_result_offset);

		if (c_max_temp < min_C_max || c_max_temp_offset < min_C_max)
		{
			if (c_max_temp == c_max_temp_offset)
			{
				std::vector<std::array<int, 4>> potential_temp_result = check_further(temp_result, Array_of_tasks_sorted_by_C_max_of_task, i_start - 1, depth - 1);
				std::vector<std::array<int, 4>> potential_temp_result_offset = check_further(temp_result_offset, Array_of_tasks_sorted_by_C_max_of_task, i_start - 1, depth - 1);
				int potential_c_max_temp = calculate_C_max(potential_temp_result);
				int potential_c_max_temp_offset = calculate_C_max(potential_temp_result_offset);
				if (calculate_C_max(potential_temp_result) > calculate_C_max(potential_temp_result_offset))
				{
					min_C_max = potential_c_max_temp_offset;
					index_to_insert = j + 1;
				}
				else
				{
					min_C_max = potential_c_max_temp;
					index_to_insert = j;
				}

			}
			if (c_max_temp < c_max_temp_offset)
			{
				min_C_max = c_max_temp;
				index_to_insert = j;
			}
			else
			{
				min_C_max = c_max_temp_offset;
				index_to_insert = j + 1;
			}
		}

	}
	data.insert(data.begin() + index_to_insert, Array_of_tasks_sorted_by_C_max_of_task[i_start].first);
		
	return data;

}



std::vector<std::array<int, 4>> calculate_greedy_C_max(std::vector<std::array<int, 4>>& data)
{
	std::vector<std::pair<std::array<int, 4>, int>> Array_of_tasks_sorted_by_C_max_of_task;

	for (int i = 0; i < data.size(); i++)
	{
		Array_of_tasks_sorted_by_C_max_of_task.push_back(std::make_pair(data[i], data[i][0] + data[i][1] + data[i][2]));
	}

	std::sort(Array_of_tasks_sorted_by_C_max_of_task.begin(), Array_of_tasks_sorted_by_C_max_of_task.end(), [](std::pair<std::array<int, 4>, int>& a, std::pair<std::array<int, 4>, int>& b) {return a.second < b.second;});


	std::vector<std::array<int, 4>> result;

	for (int i = Array_of_tasks_sorted_by_C_max_of_task.size() - 1; i > -1; i--)
	{
		if (i == Array_of_tasks_sorted_by_C_max_of_task.size() - 1)
		{
			result.push_back(Array_of_tasks_sorted_by_C_max_of_task[i].first);
		}
		if (i != Array_of_tasks_sorted_by_C_max_of_task.size() - 1)
			result = check_further(result, Array_of_tasks_sorted_by_C_max_of_task, i, 1);
	}
	
	check_to_add_empty(result);

	return result;

}


std::vector<std::array<int, 4>> calculate_greedy_C_max_no_q(std::vector<std::array<int, 4>>& data)
{
	std::vector<std::pair<std::array<int, 4>, int>> Array_of_tasks_sorted_by_C_max_of_task;

	for (int i = 0; i < data.size(); i++)
	{
		Array_of_tasks_sorted_by_C_max_of_task.push_back(std::make_pair(data[i], data[i][0] + data[i][1] ));
	}

	std::sort(Array_of_tasks_sorted_by_C_max_of_task.begin(), Array_of_tasks_sorted_by_C_max_of_task.end(), [](std::pair<std::array<int, 4>, int>& a, std::pair<std::array<int, 4>, int>& b) {return a.second < b.second; });


	std::vector<std::array<int, 4>> result;

	for (int i = Array_of_tasks_sorted_by_C_max_of_task.size() - 1; i > -1; i--)
	{
		if (i == Array_of_tasks_sorted_by_C_max_of_task.size() - 1)
		{
			result.push_back(Array_of_tasks_sorted_by_C_max_of_task[i].first);
		}
		if (i != Array_of_tasks_sorted_by_C_max_of_task.size() - 1)
			result = check_further(result, Array_of_tasks_sorted_by_C_max_of_task, i, 1);
	}

	check_to_add_empty(result);

	return result;

}

std::vector<std::array<int, 4>> calculate_greedy_C_max_no_pq(std::vector<std::array<int, 4>>& data)
{
	std::vector<std::pair<std::array<int, 4>, int>> Array_of_tasks_sorted_by_C_max_of_task;

	for (int i = 0; i < data.size(); i++)
	{
		Array_of_tasks_sorted_by_C_max_of_task.push_back(std::make_pair(data[i], data[i][0]));
	}

	std::sort(Array_of_tasks_sorted_by_C_max_of_task.begin(), Array_of_tasks_sorted_by_C_max_of_task.end(), [](std::pair<std::array<int, 4>, int>& a, std::pair<std::array<int, 4>, int>& b) {return a.second < b.second; });


	std::vector<std::array<int, 4>> result;

	for (int i = Array_of_tasks_sorted_by_C_max_of_task.size() - 1; i > -1; i--)
	{
		if (i == Array_of_tasks_sorted_by_C_max_of_task.size() - 1)
		{
			result.push_back(Array_of_tasks_sorted_by_C_max_of_task[i].first);
		}
		if (i != Array_of_tasks_sorted_by_C_max_of_task.size() - 1)
			result = check_further(result, Array_of_tasks_sorted_by_C_max_of_task, i, 1);
	}

	check_to_add_empty(result);

	return result;

}

std::vector<std::array<int, 4>> calculate_target(std::vector<std::array<int, 4>>& data)
{
	std::vector<std::pair<std::array<int, 4>, int>> Array_of_tasks_sorted_by_C_max_of_task;

	for (int i = 0; i < data.size(); i++)
	{
		Array_of_tasks_sorted_by_C_max_of_task.push_back(std::make_pair(data[i], data[i][0] + data[i][2]));
	}

	std::sort(Array_of_tasks_sorted_by_C_max_of_task.begin(), Array_of_tasks_sorted_by_C_max_of_task.end(), [](std::pair<std::array<int, 4>, int>& a, std::pair<std::array<int, 4>, int>& b) {return a.second < b.second; });
	std::sort(Array_of_tasks_sorted_by_C_max_of_task.begin(), Array_of_tasks_sorted_by_C_max_of_task.end() - 1, [](std::pair<std::array<int, 4>, int>& a, std::pair<std::array<int, 4>, int>& b) {return a.first[1] < b.first[1]; });
	int target_r = Array_of_tasks_sorted_by_C_max_of_task.back().first[0];
	int target_q = Array_of_tasks_sorted_by_C_max_of_task.back().first[2];

	std::vector<std::array<int, 4>> result;
	result.push_back(Array_of_tasks_sorted_by_C_max_of_task.back().first);

	for (int i = 0; i < Array_of_tasks_sorted_by_C_max_of_task.size() - 1; i++)
	{
		if (target_r < target_q)
		{
			target_q -= Array_of_tasks_sorted_by_C_max_of_task[i].first[1];
			result.push_back(Array_of_tasks_sorted_by_C_max_of_task[i].first);
		}
		else
		{
			target_r -= Array_of_tasks_sorted_by_C_max_of_task[i].first[1];
			result.insert(result.begin(), Array_of_tasks_sorted_by_C_max_of_task[i].first);
		}
	}
	return result;

}

void brute_force(std::vector<std::array<int, 4>>& data, int strength)
{
	for (int k = 0; k < strength; k++)
	{
		for (int i = 0; i < data.size(); i++)
		{
			for (int j = 0; j < data.size(); j++)
			{
				if (i != j)
				{
					int old = calculate_C_max(data);
					std::swap(data[i], data[j]);
					if ( old < calculate_C_max(data))
					{
						std::swap(data[i], data[j]);
					}
				}
			}
		}
	}
}





std::vector<std::array<int, 4>> calculate_best_C_max(std::vector<std::array<int, 4>>& data)
{
	std::vector<std::array<int, 4>> greedy = calculate_greedy_C_max(data);
	std::vector<std::array<int, 4>> greedy_no_q = calculate_greedy_C_max_no_q(data);
	std::vector<std::array<int, 4>> greedy_no_pq = calculate_greedy_C_max_no_pq(data);
	std::vector<std::array<int, 4>> calculate_target_array = calculate_target(data);
	int bruteforce_strength = 1;

	brute_force(greedy, bruteforce_strength);
	brute_force(greedy_no_q, bruteforce_strength);
	brute_force(greedy_no_pq, bruteforce_strength);
	brute_force(calculate_target_array, bruteforce_strength);

	std::vector<std::pair<std::vector<std::array<int, 4>>*, int>> results;

	results.push_back({ &greedy, calculate_C_max(greedy) });
	results.push_back({ &greedy_no_q, calculate_C_max(greedy_no_q) });
	results.push_back({ &greedy_no_pq, calculate_C_max(greedy_no_pq) });
	results.push_back({ &calculate_target_array, calculate_C_max(calculate_target_array) });

	std::sort(results.begin(), results.end(), [](std::pair<std::vector<std::array<int, 4>>*, int>& a, std::pair<std::vector<std::array<int, 4>>*, int>& b) {return a.second < b.second; });


	return *results[0].first;
		

	//use other methods and chose best calculated c_max
}