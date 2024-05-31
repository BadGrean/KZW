#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <chrono>

// Define the struct to hold job data
struct Job {
    int id;
    std::vector<int> data;
};

// Define the type for the dataset map
using DatasetMap = std::map<std::string, std::pair<int, std::vector<Job>>>;

DatasetMap readDataFromFile(const std::string& filePath) {
    // Open the input file
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return {};
    }

    // Define a map to hold the datasets
    DatasetMap datasets;
    std::string line;

    // Read the file line by line
    while (std::getline(inputFile, line)) {
        // Check if the line starts with "data."
        if (line.find("data.") == 0) {
            // Extract the dataset key
            std::string datasetKey = line.substr(0, 9);

            // Extract the number of lines and machines
            int numLines, numMachines;
            inputFile >> numLines >> numMachines;
            inputFile.ignore();  // Ignore the newline character after the numbers

            // Prepare a vector to hold jobs
            std::vector<Job> jobs;
            for (int i = 0; i < numLines; ++i) {
                std::getline(inputFile, line);
                std::istringstream iss(line);
                Job job;
                job.id = i + 1;  // Assign job id starting from 1
                int value;
                while (iss >> value) {
                    job.data.push_back(value);
                }
                jobs.push_back(job);
            }

            // Store the data in the map
            datasets[datasetKey] = std::make_pair(numMachines, jobs);
        }
    }

    // Close the input file
    inputFile.close();

    return datasets;
}

int calculate_C_max(const std::vector<std::vector<int>>& graph) {
    int C_max = graph[graph.size() - 1][graph[0].size() - 1];
    return C_max;
}

std::vector<std::vector<int>> create_graph(const std::vector<Job>& Jobs, const std::vector<int>& order) {
    std::vector<std::vector<int>> graph;

    // Initialize the first column of the graph
    std::vector<int> first_column;
    for (size_t j = 0; j < Jobs[order[0] - 1].data.size(); j++) {
        if (j == 0) {
            first_column.push_back(Jobs[order[0] - 1].data[0]);
        }
        else {
            first_column.push_back(first_column[j - 1] + Jobs[order[0] - 1].data[j]);
        }
    }
    graph.push_back(first_column);

    // Calculate the rest of the graph
    for (size_t i = 1; i < order.size(); i++) {
        std::vector<int> column;
        for (size_t j = 0; j < Jobs[order[i] - 1].data.size(); j++) {
            if (j == 0) {
                column.push_back(Jobs[order[i] - 1].data[0] + graph[i - 1][0]);
            }
            else {
                int bigger_value = std::max(column[j - 1], graph[i - 1][j]);
                column.push_back(Jobs[order[i] - 1].data[j] + bigger_value);
            }
        }
        graph.push_back(column);
    }

    return graph;
}

std::vector<std::vector<int>> create_reverse_graph(const std::vector<Job>& Jobs, const std::vector<int>& order) {
    size_t numJobs = order.size();
    size_t numMachines = Jobs[order[0] - 1].data.size();
    std::vector<std::vector<int>> graph(numJobs, std::vector<int>(numMachines));

    // Initialize the last column of the graph
    for (size_t j = numMachines; j > 0; j--) {
        if (j == numMachines) {
            graph[numJobs - 1][j - 1] = Jobs[order[numJobs - 1] - 1].data[j - 1];
        }
        else {
            graph[numJobs - 1][j - 1] = graph[numJobs - 1][j] + Jobs[order[numJobs - 1] - 1].data[j - 1];
        }
    }

    // Calculate the rest of the graph in reverse order
    for (size_t i = numJobs; i > 1; i--) {
        for (size_t j = numMachines; j > 0; j--) {
            if (j == numMachines) {
                graph[i - 2][j - 1] = graph[i - 1][j - 1] + Jobs[order[i - 2] - 1].data[j - 1];
            }
            else {
                int bigger_value = std::max(graph[i - 2][j], graph[i - 1][j - 1]);
                graph[i - 2][j - 1] = Jobs[order[i - 2] - 1].data[j - 1] + bigger_value;
            }
        }
    }

    return graph;
}

int index_of_highest_value(const std::vector<Job>& left_jobs) {
    int highest_value = 0;
    int index = 0;

    for (size_t i = 0; i < left_jobs.size(); i++) {
        int temp = 0;
        for (size_t j = 0; j < left_jobs[i].data.size(); j++) {
            temp += left_jobs[i].data[j];
        }
        if (temp > highest_value) {
            highest_value = temp;
            index = i;
        }
    }
    return index;
}

std::string format_duration(long long milliseconds) {
    long long seconds = milliseconds / 1000;
    milliseconds %= 1000;
    std::ostringstream oss;
    oss << seconds << "." << std::setfill('0') << std::setw(3) << milliseconds;
    return oss.str();
}

int main() {
    // Call the function with the file path
    std::string filePath = "data.txt";
    DatasetMap datasets = readDataFromFile(filePath);
    std::vector<std::vector<int>> orders;

    auto total_start = std::chrono::steady_clock::now();

    for (const auto& dataset : datasets) {
        auto start = std::chrono::steady_clock::now();


        std::vector<int> order;
        std::vector<Job> left_jobs = dataset.second.second;
        std::vector<int> best_order;
        while (!left_jobs.empty()) {
            int chosen_index = index_of_highest_value(left_jobs);
            if (order.empty()) {
                order.push_back(left_jobs[chosen_index].id);
                left_jobs.erase(left_jobs.begin() + chosen_index);
                continue;
            }
            int best_c_max = INT_MAX;
            int best_index = 0;
            std::vector<std::vector<int>> normal_graph = create_graph(dataset.second.second, order);
            std::vector<std::vector<int>> reverse_graph = create_reverse_graph(dataset.second.second, order);

            for (size_t i = 0; i < order.size() + 1; i++) {
                std::vector<int> temp_order = order;
                std::vector<int> temp_vector;
                int temp_c_max = INT_MAX;

                if (i == 0)
                {
                    temp_vector = reverse_graph[0];
                    for (int j = 0; j < temp_vector.size(); j++)
                    {
                        temp_vector[j] += left_jobs[chosen_index].data[j];
                    }
                    temp_c_max = *std::max_element(temp_vector.begin(), temp_vector.end());
                }
                else if (i == order.size())
                {
                    temp_vector = normal_graph[i - 1];
                    for (int j = 0; j < temp_vector.size(); j++)
                    {
                        if (j == 0)
                        {
                            temp_vector[j] += left_jobs[chosen_index].data[j];
                        }
                        else
                        {
                            temp_vector[j] = std::max(left_jobs[chosen_index].data[j] + temp_vector[j - 1], left_jobs[chosen_index].data[j] + temp_vector[j]);
                        }
                    }
                    temp_c_max = *std::max_element(temp_vector.begin(), temp_vector.end());

                }
                else
                {
                    temp_vector = normal_graph[i - 1];
                    for (int j = 0; j < temp_vector.size(); j++)
                    {
                        if (j == 0)
                        {
                            temp_vector[j] += left_jobs[chosen_index].data[j];
                        }
                        else
                        {
                            temp_vector[j] = std::max(left_jobs[chosen_index].data[j] + temp_vector[j - 1], left_jobs[chosen_index].data[j] + temp_vector[j]);
                        }
                    }
                    for (int j = 0; j < temp_vector.size(); j++)
                    {

                        temp_vector[j] += reverse_graph[i][j];
                    }
                    temp_c_max = *std::max_element(temp_vector.begin(), temp_vector.end());
                }

                if (temp_c_max < best_c_max) 
                {
                    best_c_max = temp_c_max;
                    best_index = i;
                }
            }
            order.insert(order.begin() + best_index, left_jobs[chosen_index].id);
            left_jobs.erase(left_jobs.begin() + chosen_index);
        }
        orders.push_back(order);

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Execution time for " << dataset.first << ": " << format_duration(duration) << " seconds" << std::endl;
    }

    auto total_end = std::chrono::steady_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count();
    std::cout << "Total execution time: " << format_duration(total_duration) << " seconds" << std::endl;

    std::ofstream outputFile("results.txt");
    if (outputFile.is_open()) {
        for (size_t i = 0; i < orders.size(); ++i) {
            const auto& dataset = std::next(datasets.begin(), i);
            const auto& order = orders[i];

            outputFile << dataset->first << std::endl;
            for (size_t j = 0; j < order.size(); j++) {
                outputFile << order[j] << " ";
            }
            outputFile << std::endl;
        }
        outputFile.close();
    }
    else {
        std::cerr << "Error opening output file." << std::endl;
    }

    return 0;
}