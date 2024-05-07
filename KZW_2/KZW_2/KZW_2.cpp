// KZW_2.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>

struct Task {
    int P;  // czas trwania zadania (duration of the task)
    int W;  // Waga (weight or priority of the task)
    int D;  // termin zakończenia zadania (deadline of the task)
    int Id; // unique identifier for the task
};


void readTasksFromFile(const std::string& filename, std::map<std::string, std::vector<Task>> &dataTasks) {
    std::ifstream file(filename);
    int counter = 10;
    int taskCounter = 0; int id_counter = 1;
    bool writeNextLine = false;
    std::string dict_key;

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;

    // Read the file line by line
    while (getline(file, line)) {
        // Print each line to the console
        if (writeNextLine)
        {
            taskCounter = std::stoi(line);
            writeNextLine = false;
            continue;
        }
        if (line == "data." + std::to_string(counter) + ":")
        {
            dict_key = "data." + std::to_string(counter) + ":";
            counter++;
            writeNextLine = true;
            
        }
        if (taskCounter > 0)
        {
            Task task;
            std::string token;
            size_t pos = 0;
            
			pos = line.find(' ');
            token = line.substr(0, pos);
            task.P = std::stoi(token);
            line.erase(0, pos + 1);
            pos = line.find(' ');
            token = line.substr(0, pos);
            task.W = std::stoi(token);
            line.erase(0, pos + 1);
            task.D = std::stoi(line);
            task.Id = id_counter;
            dataTasks[dict_key].push_back(task);
			taskCounter--;
            id_counter++;
		}
        else
        {
            id_counter = 1;
        }
    }

    file.close();
}

int calculateCostBasedOnDelay(const std::vector<Task>& tasks, int currentEndTime, int taskIndex)
{
    // Calculate delayed time which is non-negative
    int delayedTime = std::max(currentEndTime - tasks[taskIndex].D, 0);
    // Return the cost, which is the delayed time multiplied by the task's weight
    return delayedTime * tasks[taskIndex].W;
}

/*! calculate ending time of permutation */
int calculateEndingTime(const std::vector<Task>& tasks, int subsetMask)
{
    int endTime = 0;
    int taskIndex = 0;
    for (int bit = 1; bit <= subsetMask; bit <<= 1)
    {
        if (subsetMask & bit)
        {
            endTime += tasks[taskIndex].P; // Add the duration of the task included in the subset
        }
        taskIndex++;
    }
    return endTime;
}
int optimizeTaskOrder(const std::vector<Task>& tasks, std::vector<int>& optimalOrder)
{
    size_t numberOfSubsets = static_cast<size_t>(pow(2, tasks.size()));
    std::vector<int> minCostForSubset(numberOfSubsets, std::numeric_limits<int>::max());
    std::vector<std::vector<int>> optimalOrderForSubset(numberOfSubsets);

    minCostForSubset[0] = 0; // Cost of empty set is zero, not needed if we only iterate from 1

    for (int subsetMask = 1; subsetMask < numberOfSubsets; subsetMask++)
    {
        int currentParameterC = calculateEndingTime(tasks, subsetMask);
        int lastAddedTaskIndex = -1; // initialize to an invalid index

        for (int taskBit = 1, taskIndex = 0; taskBit <= subsetMask; taskBit <<= 1, taskIndex++)
        {
            if (taskBit & subsetMask)
            {
                int previousSubsetMask = subsetMask - taskBit;
                int costIncludingTask = minCostForSubset[previousSubsetMask] + calculateCostBasedOnDelay(tasks, currentParameterC, taskIndex);

                if (minCostForSubset[subsetMask] > costIncludingTask)
                {
                    minCostForSubset[subsetMask] = costIncludingTask;
                    optimalOrderForSubset[subsetMask] = optimalOrderForSubset[previousSubsetMask];
                    lastAddedTaskIndex = taskIndex;
                }
                else if (minCostForSubset[subsetMask] == costIncludingTask)
                {
                    if (optimalOrderForSubset[subsetMask].empty() || optimalOrderForSubset[subsetMask] > optimalOrderForSubset[previousSubsetMask])
                    {
                        optimalOrderForSubset[subsetMask] = optimalOrderForSubset[previousSubsetMask];
                        lastAddedTaskIndex = taskIndex;
                    }
                }
            }
        }

        if (lastAddedTaskIndex != -1) // If a task was added to this subset
        {
            optimalOrderForSubset[subsetMask].push_back(tasks[lastAddedTaskIndex].Id);
        }
    }

    optimalOrder = std::move(optimalOrderForSubset.back());
    return minCostForSubset.back();
}
int main()
{
    std::map<std::string, std::vector<Task>> dataTasks;
    readTasksFromFile("witi.data.txt", dataTasks);
    for (const auto& keyValue : dataTasks) 
    {
        std::vector<int> order;
        int result = optimizeTaskOrder(keyValue.second, order);
        for (const auto& elem : order)
            std::cout << elem << " ";
        std::cout << "\n";
    }
}


// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
