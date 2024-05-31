#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <chrono>

struct Job {
    int id;
    std::vector<int> data;
};

using DatasetMap = std::map<std::string, std::pair<int, std::vector<Job>>>;

DatasetMap readDataFromFile(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return {};
    }

    DatasetMap datasets;
    std::string line;

    while (std::getline(inputFile, line)) {
        if (line.find("data.") == 0) {
            std::string datasetKey = line.substr(0, 9);
            int numLines, numMachines;
            inputFile >> numLines >> numMachines;
            inputFile.ignore();

            std::vector<Job> jobs;
            jobs.reserve(numLines);
            for (int i = 0; i < numLines; ++i) {
                std::getline(inputFile, line);
                std::istringstream iss(line);
                Job job;
                job.id = i + 1;
                int value;
                while (iss >> value) {
                    job.data.push_back(value);
                }
                jobs.push_back(job);
            }
            datasets[datasetKey] = std::make_pair(numMachines, jobs);
        }
    }
    inputFile.close();
    return datasets;
}

int calculate_C_max(const std::vector<std::vector<int>>& graph) {
    return graph.back().back();
}

std::vector<std::vector<int>> create_graph(const std::vector<Job>& jobs, const std::vector<int>& order) {
    std::vector<std::vector<int>> graph(order.size(), std::vector<int>(jobs[0].data.size()));

    for (size_t j = 0; j < jobs[order[0] - 1].data.size(); ++j) {
        graph[0][j] = (j == 0) ? jobs[order[0] - 1].data[0] : graph[0][j - 1] + jobs[order[0] - 1].data[j];
    }

    for (size_t i = 1; i < order.size(); ++i) {
        for (size_t j = 0; j < jobs[order[i] - 1].data.size(); ++j) {
            graph[i][j] = (j == 0) ? jobs[order[i] - 1].data[0] + graph[i - 1][0]
                : jobs[order[i] - 1].data[j] + std::max(graph[i - 1][j], graph[i][j - 1]);
        }
    }
    return graph;
}

int index_of_highest_value(const std::vector<Job>& left_jobs) {
    int highest_value = 0;
    int index = 0;

    for (size_t i = 0; i < left_jobs.size(); ++i) {
        int temp = 0;
        for (const auto& value : left_jobs[i].data) {
            temp += value;
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
    std::string filePath = "data.txt";
    auto datasets = readDataFromFile(filePath);
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
            for (size_t i = 0; i <= order.size(); ++i) {
                std::vector<int> temp_order = order;
                temp_order.insert(temp_order.begin() + i, left_jobs[chosen_index].id);
                auto temp_graph = create_graph(dataset.second.second, temp_order);
                int temp_c_max = calculate_C_max(temp_graph);

                if (temp_c_max < best_c_max) {
                    best_c_max = temp_c_max;
                    best_order = temp_order;
                }
            }
            order = best_order;
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
            for (const auto& job_id : order) {
                outputFile << job_id << " ";
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
