#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <mutex>

#include "src/Manager/TimerManager.hpp"
#include "src/FileIO/GraphFileIO.hpp"
#include "src/GraphsAlgorithms/TSPProblem/DijkstraList.hpp"
#include "src/GraphsAlgorithms/TSPProblem/DijkstraMatrix.hpp"
#include "src/GraphsAlgorithms/TSPProblem/FordBellmanList.hpp"
#include "src/GraphsAlgorithms/TSPProblem/FordBellmanMatrix.hpp"
#include "src/GraphsImpl/GraphList.hpp"
#include "src/GraphsImpl/GraphMatrix.hpp"

std::mutex fileWriteMutex;

void runBenchmarkOnDirectory(const std::string& dirPath, const std::string& algorithmName, bool isDijkstra, bool useMatrix) {
    using namespace std::filesystem;
    std::vector<int> times;
    std::string timestamp;

    auto now = std::time(nullptr);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now), "%Y-%m-%d-%H-%M-%S");
    timestamp = oss.str();

    std::string summaryFile = "../data/tsp_summary.txt";
    std::cout << "[THREAD START] " << algorithmName << (useMatrix ? " [MATRIX] " : " [LIST] ") << " on directory: " << dirPath << std::endl;

    for (const auto& entry : directory_iterator(dirPath)) {
        if (!entry.is_regular_file()) continue;
        std::string filePath = entry.path().string();
        std::cout << "[RUNNING] " << algorithmName << (useMatrix ? " [MATRIX] " : " [LIST] ") << " on file: " << filePath << std::endl;

        int execTime = 0;

        if (useMatrix) {
            if (auto* g = dynamic_cast<GraphMatrix*>(GraphIO::readGraph(filePath, GraphRepresentingShape::MATRIX))) {
                execTime = TimerManager::measureTimeMs([&]() {
                    if (isDijkstra) DijkstraMatrix::findShortestPaths(g);
                    else FordBellmanMatrix::findShortestPaths(g);
                });
                delete g;
            }
        } else {
            if (auto* g = dynamic_cast<GraphList*>(GraphIO::readGraph(filePath, GraphRepresentingShape::LIST))) {
                execTime = TimerManager::measureTimeMs([&]() {
                    if (isDijkstra) DijkstraList::findShortestPaths(g);
                    else FordBellmanList::findShortestPaths(g);
                });
                delete g;
            }
        }
        if (execTime > 0) times.push_back(execTime);
    }

    if (!times.empty()) {
        int minTime = *std::min_element(times.begin(), times.end());
        int maxTime = *std::max_element(times.begin(), times.end());
        double avgTime = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        std::sort(times.begin(), times.end());
        double median = times.size() % 2 == 0
                      ? (times[times.size() / 2 - 1] + times[times.size() / 2]) / 2.0
                      : times[times.size() / 2];

        int vertexCount = 0, edgeCount = 0;
        for (const auto& entry : directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                if (auto* g = GraphIO::readGraph(entry.path().string(), useMatrix ? MATRIX : LIST)) {
                    vertexCount = g->getVertexCount();
                    edgeCount = g->getEdgeCount();
                    delete g;
                }
                break;
            }
        }

        std::lock_guard<std::mutex> lock(fileWriteMutex);
        GraphIO::appendGraphSummaryEntry(
            summaryFile,
            timestamp,
            algorithmName,
            useMatrix ? MATRIX : LIST,
            vertexCount,
            edgeCount,
            times.size(),
            minTime,
            maxTime,
            avgTime,
            median
        );
    }
    std::cout << "[THREAD END] " << algorithmName << (useMatrix ? " [MATRIX] " : " [LIST] ") << " finished on: " << dirPath << std::endl;
}

int main() {
    std::string baseDir = "../graphBenchmarkData";
    std::vector<std::thread> threads;

    for (const auto& folder : std::filesystem::directory_iterator(baseDir)) {
        if (!folder.is_directory()) continue;
        std::string dirPath = folder.path().string();

        threads.emplace_back(runBenchmarkOnDirectory, dirPath, "Dijkstra", true, true);
        threads.emplace_back(runBenchmarkOnDirectory, dirPath, "Dijkstra", true, false);
        threads.emplace_back(runBenchmarkOnDirectory, dirPath, "FordBellman", false, true);
        threads.emplace_back(runBenchmarkOnDirectory, dirPath, "FordBellman", false, false);
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    std::cout << "[ALL DONE] Benchmark finished for all folders." << std::endl;
    return 0;
}
