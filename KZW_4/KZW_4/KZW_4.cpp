#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

// Function prototypes
void WriteInitialData();
void WriteResultData();
void WriteBestDistance(double bestDist);
void WriteIterationData();
void Draw();
void Run();
void Loop();
void SaStep(vector<int>& path, double temp);
pair<int, int> RandAB();
void PathMove(vector<int>& path, pair<int, int> move);
double PathDist(const vector<int>& path);
vector<int> RandPath();
void NodeRand();

// Global variables
const int nodeN = 40;
vector<pair<int, int>> node(nodeN);  // dane instancji (koordynaty miast)
vector<int> saPath;                 // aktualne rozwiązanie
vector<double> saDataDist;          // dane dla wykresu odległości
vector<pair<double, bool>> saDataPraw; // dane dla wykresu prawdopodobieństwa
vector<double> saDataTemp;          // dane dla wykresu temperatury
double saBestDist;                  // najlepsza znaleziona odległość
vector<int> saBestPath;             // najlepsza znaleziona ścieżka
const double saTempStart = 10000;   // początkowa temperatura
const double saTempAlpha = 0.976;   // współczynnik zmniejszania temperatury
double saTemp;                      // aktualna temperatura

int loopN = 0;                      // liczba pozostałych kroków do wykonania
const int loopDelay = 10;           // opóźnienie pętli (nieużywane w wersji C++)

void WriteInitialData() {
    ofstream outFile("data.csv");
    if (outFile.is_open()) {
        for (const auto& point : node) {
            outFile << point.first << "," << point.second << endl;
        }
        outFile.close();
    }
    else {
        cout << "Unable to open file" << endl;
    }
}

void WriteResultData() {
    ofstream outFile("result.csv");
    if (outFile.is_open()) {
        for (int i : saBestPath) {
            outFile << node[i].first << "," << node[i].second << endl;
        }
        // To close the path (return to the starting point)
        outFile << node[saBestPath[0]].first << "," << node[saBestPath[0]].second << endl;
        outFile.close();
    }
    else {
        cout << "Unable to open file" << endl;
    }
}

void WriteBestDistance(double bestDist) {
    ofstream outFile("best_distances.csv", ios_base::app); // Append mode
    if (outFile.is_open()) {
        outFile << bestDist << endl;
        outFile.close();
    }
    else {
        cout << "Unable to open file" << endl;
    }
}

void WriteIterationData() {
    ofstream outFile("result_path.csv", ios_base::app); // Append mode
    if (outFile.is_open()) {
        outFile << "Iteration Best Distance: " << saBestDist << endl;
        for (int i : saBestPath) {
            outFile << node[i].first << "," << node[i].second << endl;
        }
        // To close the path (return to the starting point)
        outFile << node[saBestPath[0]].first << "," << node[saBestPath[0]].second << endl;
        outFile.close();
    }
    else {
        cout << "Unable to open file" << endl;
    }
}

void Draw() {
    // Write to CSV file
    WriteResultData();
    WriteIterationData();

    // Print results to console
    cout << "Best Distance: " << saBestDist << endl;
    for (int i : saBestPath) {
        cout << node[i].first << "," << node[i].second << endl;
    }
    // To close the path (return to the starting point)
    cout << node[saBestPath[0]].first << "," << node[saBestPath[0]].second << endl;
}

void Run() {
    vector<int> startPath = RandPath();
    double startDist = PathDist(startPath);
    saPath = startPath;
    saDataDist.clear();
    saDataDist.push_back(startDist);
    saBestDist = startDist;
    saBestPath = startPath;

    loopN = 400;
    saTemp = saTempStart;
    saDataTemp.clear();
    saDataPraw.clear();
    Loop();
}

void Loop() {
    while (loopN > 0) {
        loopN--;
        SaStep(saPath, saTemp);
        saTemp = saTemp * saTempAlpha;
        if (loopN == 0) {
            saDataDist.push_back(saBestDist);
            saPath = saBestPath;
        }
        Draw();
    }
}

void SaStep(vector<int>& path, double temp) {
    double c0 = PathDist(path);
    double praw = 0, akce = 0;
    for (int i = 0; i < 100; i++) {
        pair<int, int> move = RandAB();
        PathMove(path, move);
        double c1 = PathDist(path);
        if (c1 > c0) {
            praw = exp((c0 - c1) / temp);
            akce = ((double)rand() / RAND_MAX) < praw;
        }
        if (c1 <= c0 || akce) {
            c0 = c1;  // akceptuje ruch
        }
        else {
            PathMove(path, move);  // odrzucenie ruchu
        }
        if (saBestDist > c0) {
            saBestDist = c0;
            saBestPath = path;
            WriteBestDistance(saBestDist); // Log best distance immediately when it changes
        }
    }
    saDataDist.push_back(c0);
    saDataTemp.push_back(saTemp);
    saDataPraw.push_back(make_pair(praw, akce));
}

pair<int, int> RandAB() {
    int a = rand() % node.size();
    int b = rand() % (node.size() - 1);
    if (b >= a) b++;
    if (a > b) swap(a, b);
    return make_pair(a, b);
}

void PathMove(vector<int>& path, pair<int, int> move) {
    int a = move.first, b = move.second;
    while (a < b) {
        swap(path[a], path[b]);
        a++;
        b--;
    }
}

double PathDist(const vector<int>& path) {
    double dist = 0;
    for (size_t i = 0; i < path.size(); i++) {
        int n0 = path[i];
        int n1 = path[(i + 1) % path.size()];
        int x0 = node[n0].first;
        int y0 = node[n0].second;
        int x1 = node[n1].first;
        int y1 = node[n1].second;
        dist += round(sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1)));
    }
    return dist;
}

vector<int> RandPath() {
    vector<int> path(node.size());
    for (size_t i = 0; i < node.size(); i++) path[i] = static_cast<int>(i); // static_cast for size_t to int
    random_shuffle(path.begin(), path.end());
    return path;
}

void NodeRand() {
    loopN = 0;
    node.clear();
    saPath.clear();
    saBestPath.clear();
    saDataDist.clear();
    saDataPraw.clear();
    saDataTemp.clear();
    for (int i = 0; i < nodeN; i++) {
        node.push_back(make_pair(rand() % 400, rand() % 200));
    }
    WriteInitialData();
    //Draw();
}

int main() {
    srand(static_cast<unsigned int>(time(NULL))); // cast to unsigned int to avoid warning
    NodeRand();
    Run();
    return 0;
}