#ifndef GRAPHSMANAGER_HPP
#define GRAPHSMANAGER_HPP

#include "../common/GraphRepresentingShape.hpp"
#include "../FileIO/GraphFileIO.hpp"
#include "../GraphsImpl/GraphMatrix.hpp"
#include "../GraphsImpl/GraphList.hpp"
#include "../Manager/TimerManager.hpp"
#include "../GraphsAlgorithms/MSTProblem/PrimList.hpp"
#include "../GraphsAlgorithms/MSTProblem/PrimMatrix.hpp"
#include "../GraphsAlgorithms/MSTProblem/KruskalList.hpp"
#include "../GraphsAlgorithms/MSTProblem/KruskalMatrix.hpp"
#include "../GraphsAlgorithms/TSPProblem/DijkstraList.hpp"
#include "../GraphsAlgorithms/TSPProblem/DijkstraMatrix.hpp"
#include "../GraphsAlgorithms/TSPProblem/FordBellmanList.hpp"
#include "../GraphsAlgorithms/TSPProblem/FordBellmanMatrix.hpp"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <filesystem>

class GraphsManager {
public:

    /// Reads a graph from a file, runs the chosen problem/algorithm on it, logs results to history.
    static void fileInputMode(const char* problemFlag, const char* algorithmFlag, const char* shapeFlag,
                           const std::string& inputFile, const char* outputFile = "", const int src = 0,
                           const int dst = -1) {
        const GraphRepresentingShape shape = getShapeFlag(shapeFlag);

        std::cout << "[INFO] Chosen problem: " << getProblemDisplayName(problemFlag) << std::endl;
        std::cout << "[INFO] Chosen algorithm: " << getAlgorithmDisplayName(algorithmFlag) << std::endl;
        std::cout << "[INFO] Graph representation: " << shapeFlag << std::endl;

        const std::string timestamp = getCurrentTimestamp();
        dispatchProblemByShapeFile(problemFlag, algorithmFlag, src, dst, shape, inputFile, outputFile, timestamp);
    }

    /// Runs a series of random‐graph benchmarks, writes per‐run history and one summary entry.
    static void benchmarkMode(const char* problemFlag, const char* algorithmFlag, const char* shapeFlag,
                          int size, double density, int repeatCount, const char* outputFile) {
        const GraphRepresentingShape shape = getShapeFlag(shapeFlag);

        std::cout << "[INFO] BENCHMARK MODE STARTED" << std::endl;
        std::cout << "[INFO] Chosen problem: " << getProblemDisplayName(problemFlag) << std::endl;
        std::cout << "[INFO] Chosen algorithm: " << getAlgorithmDisplayName(algorithmFlag) << std::endl;
        std::cout << "[INFO] Graph representation: " << shapeFlag << std::endl;
        std::cout << "[INFO] Graph size: " << size << ", density: " << density << ", repeat count: " << repeatCount << std::endl;

        const std::string timestamp = getCurrentTimestamp();
        dispatchProblemByShapeBenchmark(problemFlag, algorithmFlag, shape, size, density, repeatCount, outputFile, timestamp);
    }


private:

    /// Chooses LIST/MATRIX/BOTH and forwards to file or benchmark dispatch.
    static void dispatchProblemByShapeFile(const char* problemFlag, const char* algorithmFlag, int src, int dst,
                                    const GraphRepresentingShape shape, const std::string& inputFile,
                                    const char* outputFile, const std::string& timestamp) {
        if (shape == BOTH) {
            dispatchProblemByAlgorithmFile(problemFlag, algorithmFlag, src, dst, MATRIX, inputFile, outputFile, timestamp);
            dispatchProblemByAlgorithmFile(problemFlag, algorithmFlag, src, dst, LIST, inputFile, outputFile, timestamp);
        } else {
            dispatchProblemByAlgorithmFile(problemFlag, algorithmFlag, src, dst, shape, inputFile, outputFile, timestamp);
        }
    }

    /// Reads the graph, displays it, then invokes MST or TSP dispatch.
    static void dispatchProblemByAlgorithmFile(const char* problem, const char* algorithm, int src, int dst,
                                        const GraphRepresentingShape shape, const std::string& inputFile,
                                        const char* outputFile, const std::string& timestamp) {
        IGraph* baseGraph = GraphIO::readGraph(inputFile, shape);
        if (!baseGraph) {
            std::cerr << "[ERROR] Graph could not be read." << std::endl;
            return;
        }
        baseGraph->display();

        if (std::strcmp(problem, "--mst") == 0)
            dispatchMST(algorithm, shape, baseGraph, outputFile, timestamp);
        else if (std::strcmp(problem, "--tsp") == 0)
            dispatchTSP(src, dst, algorithm, shape, baseGraph, outputFile, timestamp);
        else
            std::cerr << "[ERROR] Unknown problem type: " << problem << std::endl;

        delete baseGraph;
    }

    /// Chooses LIST/MATRIX/BOTH and forwards to benchmark dispatch.
    static void dispatchProblemByShapeBenchmark(const char* problemFlag, const char* algorithmFlag,
                                    const GraphRepresentingShape shape, int size, double density, int repeatCount,
                                    const char* outputFile, const std::string& timestamp) {
        if (shape == BOTH) {
            dispatchProblemByAlgorithmBenchmark(problemFlag, algorithmFlag, MATRIX, size, density, repeatCount,
                outputFile, timestamp);
            dispatchProblemByAlgorithmBenchmark(problemFlag, algorithmFlag, LIST, size, density, repeatCount,
                outputFile, timestamp);
        } else {
            dispatchProblemByAlgorithmBenchmark(problemFlag, algorithmFlag, shape, size, density, repeatCount,
                outputFile, timestamp);
        }
    }

    /// Generates random graphs, runs MST/TSP per repeat, collects all times, writes summary.
    static void dispatchProblemByAlgorithmBenchmark(const char* problem, const char* algorithm,
                                          const GraphRepresentingShape shape, int size, double density, int repeatCount,
                                          const char* outputFile, const std::string& timestamp) {
        Vektor<int> allTimes;
        RandomGraphGenerator randomGraphGenerator;

        for (int i = 0; i < repeatCount; ++i) {
            IGraph* baseGraph = randomGraphGenerator.generateGraphInStructure(size, density, shape);
            if (!baseGraph) {
                std::cerr << "[ERROR] Error generating random graph." << std::endl;
                return;
            }

            std::cout << "[RUN] " << i + 1 << "/" << repeatCount << " - ";
            baseGraph->display();

            Vektor<int> execTimes;
            if (std::strcmp(problem, "--mst") == 0)
                execTimes = dispatchMST(algorithm, shape, baseGraph, outputFile, timestamp);
            else if (std::strcmp(problem, "--tsp") == 0)
                execTimes = dispatchTSP(0, 1, algorithm, shape, baseGraph, outputFile, timestamp);
            else
                std::cerr << "[ERROR] Unknown problem type: " << problem << std::endl;

            allTimes.insertEnd(execTimes.begin(), execTimes.end());
            delete baseGraph;
        }

        if (!allTimes.empty()) {
            int minTime = *std::min_element(allTimes.begin(), allTimes.end());
            int maxTime = *std::max_element(allTimes.begin(), allTimes.end());
            double avgTime = std::accumulate(allTimes.begin(), allTimes.end(), 0.0) / allTimes.size();
            SortAlgorithms<int>::quickSort(allTimes);
            double median = allTimes.size() % 2 == 0
                            ? (allTimes[allTimes.size() / 2 - 1] + allTimes[allTimes.size() / 2]) / 2.0
                            : allTimes[allTimes.size() / 2];
            const double maxPossibleEdges =  static_cast<int>(size * (size - 1) * density / 200.0);

            GraphIO::appendGraphSummaryEntry(
                "./benchmark_summary.txt",
                timestamp,
                getAlgorithmDisplayName(algorithm),
                shape,
                size,
                maxPossibleEdges,
                density,
                allTimes.size(),
                minTime,
                maxTime,
                avgTime,
                median
            );
        }
    }

    /// Runs the requested MST variants on one graph, logs each, returns vector of execution times.
    static Vektor<int> dispatchMST(const char* algorithm, GraphRepresentingShape shape, IGraph* g,
                                    const char* outputFile, const std::string& timestamp) {
        Vektor<int> times;
        if (std::strcmp(algorithm, "--all") == 0) {
            if (shape == LIST) {
                times.push_back(runAndLogMST<GraphList,  PrimList>(dynamic_cast<GraphList*>(g), "Prim",
                    defaultOut(outputFile, "./historyPrim.txt"), timestamp));
                times.push_back(runAndLogMST<GraphList,  KruskalList>(dynamic_cast<GraphList*>(g), "Kruskal",
                    defaultOut(outputFile, "./historyKruskal.txt"), timestamp));
            } else {
                times.push_back(runAndLogMST<GraphMatrix, PrimMatrix>(dynamic_cast<GraphMatrix*>(g), "Prim",
                   defaultOut(outputFile, "./historyPrim.txt"), timestamp));
                times.push_back(runAndLogMST<GraphMatrix,  KruskalMatrix>(dynamic_cast<GraphMatrix*>(g), "Kruskal",
                    defaultOut(outputFile, "./historyKruskal.txt"), timestamp));
            }
        }
        if (std::strcmp(algorithm, "--prim") == 0) {
            if (shape == LIST)
                times.push_back(runAndLogMST<GraphList,  PrimList>(dynamic_cast<GraphList*>(g), "Prim",
                   defaultOut(outputFile, "./historyPrim.txt"), timestamp));
            else
                times.push_back(runAndLogMST<GraphMatrix,  PrimMatrix>(dynamic_cast<GraphMatrix*>(g), "Prim",
                   defaultOut(outputFile, "./historyPrim.txt"), timestamp));
        }
        if (std::strcmp(algorithm, "--krus") == 0) {
            if (shape == LIST)
                times.push_back(runAndLogMST<GraphList,  KruskalList>(dynamic_cast<GraphList*>(g), "Kruskal",
                   defaultOut(outputFile, "./historyKruskal.txt"), timestamp));
            else
                times.push_back(runAndLogMST<GraphMatrix,  KruskalMatrix>(dynamic_cast<GraphMatrix*>(g), "Kruskal",
                    defaultOut(outputFile, "./historyKruskal.txt"), timestamp));
        }

        return times;
    }

    /// Executes one MST algorithm, logs to history, returns the execution time in ms.
    static Vektor<int> dispatchTSP(int src, int dest, const char* algorithm, GraphRepresentingShape shape, IGraph* g,
                                                            const char* outputFile, const std::string& timestamp) {
        Vektor<int> times;
        if (std::strcmp(algorithm, "--all") == 0) {
            if (shape == LIST) {
                 times.push_back(runAndLogTSP<GraphList,   DijkstraList>(dynamic_cast<GraphList*>(g),
                     src, dest, "Dijkstra",
                     defaultOut(outputFile, "./historyDijkstra.txt"), timestamp));
                 times.push_back(runAndLogTSP<GraphList,   FordBellmanList>(dynamic_cast<GraphList*>(g),
                     src, dest, "FordBellman",
                     defaultOut(outputFile, "./historyFordBellman.txt"), timestamp));
            }
            else {
                times.push_back(runAndLogTSP<GraphMatrix,   DijkstraMatrix>(dynamic_cast<GraphMatrix*>(g),
                    src, dest, "Dijkstra",
                    defaultOut(outputFile, "./historyDijkstra.txt"), timestamp));
                times.push_back(runAndLogTSP<GraphMatrix,   FordBellmanMatrix>(dynamic_cast<GraphMatrix*>(g),
                    src, dest, "FordBellman",
                    defaultOut(outputFile, "./historyFordBellman.txt"), timestamp));
            }
        }

        if (std::strcmp(algorithm, "--dij") == 0) {
            if (shape == LIST)
                times.push_back(runAndLogTSP<GraphList,   DijkstraList>(dynamic_cast<GraphList*>(g),
                    src, dest, "Dijkstra",
                    defaultOut(outputFile, "./historyDijkstra.txt"), timestamp));
            else
                times.push_back(runAndLogTSP<GraphMatrix,   DijkstraMatrix>(dynamic_cast<GraphMatrix*>(g),
                    src, dest, "Dijkstra",
                    defaultOut(outputFile, "./historyDijkstra.txt"), timestamp));
        }else if (std::strcmp(algorithm, "--ford") == 0 ) {
            if (shape == LIST)
                times.push_back(runAndLogTSP<GraphList,   FordBellmanList>(dynamic_cast<GraphList*>(g),
                   src, dest, "FordBellman",
                   defaultOut(outputFile, "./historyFordBellman.txt"), timestamp));
            else
                times.push_back(runAndLogTSP<GraphMatrix,   FordBellmanMatrix>(dynamic_cast<GraphMatrix*>(g),
                   src, dest, "FordBellman",
                   defaultOut(outputFile, "./historyFordBellman.txt"), timestamp));
        }
        return times;
    }

    /// Executes one TSP algorithm (src→dest), logs to history, returns the execution time in ms.
    template<typename G, typename Algo>
    static int runAndLogMST(G* g, const std::string& algoName, const std::string& historyFile, const std::string& ts)
    {
        auto result = Algo::findMST(g);
        int execTime = TimerManager::measureTimeMs([&]() { Algo::findMST(g); });
        Algo::displayMST(result);
        GraphIO::appendGraphHistoryEntry(
          historyFile,
          ts,
          algoName,
          getShape<G>(),
          g->getVertexCount(),
          g->getEdgeCount(),
          execTime,
          0,
          formatResultPath(result),
          Algo::calculateMSTWeightAsString(result)
        );
        return execTime;
    }

    /// Executes one TSP algorithm (src→dest), logs to history, returns the execution time in ms.
    template<typename G, typename Algo>
    static int runAndLogTSP(G* g, int src, int dst, const std::string& algoName, const std::string& historyFile,
     const std::string& ts)
    {
        auto result = Algo::findShortestPaths(g, src, dst);
        int execTime = TimerManager::measureTimeMs([&]() { Algo::findShortestPaths(g, src, dst); });
        Algo::displayShortestPaths(result, src);
        GraphIO::appendGraphHistoryEntry(
          historyFile,
          ts,
          algoName,
          getShape<G>(),
          g->getVertexCount(),
          g->getEdgeCount(),
          execTime,
          0,
          formatResultPath(result),
          Algo::calculatePathWeightAsString(result, true)
        );
        return execTime;
    }

    /// Generic type→LIST/MATRIX resolver used by runAndLog….
    template <typename G>
    static GraphRepresentingShape getShape() {
        if constexpr (std::is_same_v<G, GraphList*> || std::is_same_v<G, GraphList>) return LIST;
        else return MATRIX;
    }

    /// Helper: prints timestamp “YYYY-MM-DD-HH-MM-SS”.
    static std::string getCurrentTimestamp() {
        auto now = std::time(nullptr);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&now), "%Y-%m-%d-%H-%M-%S");
        return oss.str();
    }

    /// Helper: build a “v1->v2->…->vk” string from an EdgeTriple path.
    static std::string formatResultPath(const Vektor<EdgeTriple>& result) {
        std::ostringstream oss;
        for (int i = result.size() - 1; i >= 0; --i) {
            oss << result[i].from << "->";
        }
        if (result.size() > 0) oss << result[0].to;
        return oss.str();
    }

    /// Helper: fallback for optional output file argument.
    static const char* defaultOut(const char* provided, const char* fallback) {
        return (std::strcmp(provided, "") == 0) ? fallback : provided;
    }

    /// Helper: returns LIST or MATRIX based on shape flag string.
    static GraphRepresentingShape getShapeFlag(const char* shapeStr) {
        if (std::strcmp(shapeStr, "--list") == 0)
            return LIST;
        if (std::strcmp(shapeStr,"--matrix") == 0)
            return MATRIX;
        if (std::strcmp(shapeStr, "--all") == 0)
            return BOTH;

        std::cerr << "[ERROR] Unknown shape flag: " << shapeStr << std::endl;
        std::exit(EXIT_FAILURE);
    }

    /// Helper: returns “Prim”/“Kruskal”/“Dijkstra”/“Ford-Bellman”/etc. for display.
    static const char* getAlgorithmDisplayName(const char* algFlag) {
        if (std::strcmp(algFlag, "--prim") == 0)
            return "Prim";
        if (std::strcmp(algFlag, "--krus") == 0)
            return "Kruskal";
        if (std::strcmp(algFlag, "--dij") == 0)
            return "Dijkstra";
        if (std::strcmp(algFlag, "--ford") == 0)
            return "Ford-Bellman";
        if (std::strcmp(algFlag, "--all") == 0)
            return "All Algorithms For Current Problem";
        return "Unknown Algorithm";
    }

    /// Helper: returns “Minimum Spanning Tree” or “Shortest Path (TSP)”.
    static const char* getProblemDisplayName(const char* problemFlag) {
        if (std::strcmp(problemFlag, "--mst") == 0)
            return "Minimum Spanning Tree";
        if (std::strcmp(problemFlag, "--tsp") == 0)
            return "Shortest Path (TSP)";
        return "Unknown Problem";
    }
};

#endif
