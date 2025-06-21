#ifndef GRAPHSMANAGER_HPP
#define GRAPHSMANAGER_HPP

#include "../GraphsImpl/GraphMatrix.hpp"
#include "../GraphsImpl/GraphList.hpp"
#include "../FileIO/GraphIO.hpp"
#include "../Manager/TimerManager.hpp"
#include "../GraphsAlgorithms/MSTProblem/PrimList.hpp"
#include "../GraphsAlgorithms/MSTProblem/PrimMatrix.hpp"
#include "../GraphsAlgorithms/MSTProblem/KruskalList.hpp"
#include "../GraphsAlgorithms/MSTProblem/KruskalMatrix.hpp"
#include "../GraphsAlgorithms/TSPProblem/DijkstraList.hpp"
#include "../GraphsAlgorithms/TSPProblem/DijkstraMatrix.hpp"
#include "../GraphsAlgorithms/TSPProblem/FordBellmanList.hpp"
#include "../GraphsAlgorithms/TSPProblem/FordBellmanMatrix.hpp"

class GraphsManager {
public:
    static void fileInputMode(int problem, int algorithm, GraphRepresentingShape representation,
                              const std::string& inputFile, const std::string& outputFile = "") {
        const std::string timestamp = getCurrentTimestamp();

        IGraph* baseGraph = GraphIO::readGraph(inputFile, representation);
        if (!baseGraph) return;

        if (problem == 0) { // MST
            handleMST(algorithm, representation, baseGraph, timestamp, inputFile, outputFile);
        } else if (problem == 1) { // TSP (shortest path)
            handleTSP(algorithm, representation, baseGraph, timestamp, inputFile, outputFile);
        }
        delete baseGraph;
    }

private:
    static void handleMST(int algorithm, GraphRepresentingShape shape, IGraph* g, const std::string& ts,
                          const std::string& inputFile, const std::string& outputFile) {
        if (shape == LIST) {
            GraphList* graph = dynamic_cast<GraphList*>(g);
            if (!graph) return;
            if (algorithm == 0 || algorithm == 1) runAndLogMST(graph, PrimList::findMST, PrimList::calculateMSTWeightAsString, "Prim", "data/historyPrimList.txt", ts);
            if (algorithm == 0 || algorithm == 2) runAndLogMST(graph, KruskalList::findMST, KruskalList::calculateMSTWeightAsString, "Kruskal", "data/historyKruskalList.txt", ts);
        } else {
            GraphMatrix* graph = dynamic_cast<GraphMatrix*>(g);
            if (!graph) return;
            if (algorithm == 0 || algorithm == 1) runAndLogMST(graph, PrimMatrix::findMST, PrimMatrix::calculateMSTWeightAsString, "Prim", "data/historyPrimMatrix.txt", ts);
            if (algorithm == 0 || algorithm == 2) runAndLogMST(graph, KruskalMatrix::findMST, KruskalList::calculateMSTWeightAsString, "Kruskal", "data/historyKruskalMatrix.txt", ts);
        }
    }

    static void handleTSP(int algorithm, GraphRepresentingShape shape, IGraph* g, const std::string& ts,
                          const std::string& inputFile, const std::string& outputFile) {
        int source = 0, destination = 1;
        if (shape == LIST) {
            GraphList* graph = dynamic_cast<GraphList*>(g);
            if (!graph) return;
            if (algorithm == 0 || algorithm == 1) runAndLogTSP(graph, DijkstraList::findShortestPaths, DijkstraList::calculatePathWeightAsString, "Dijkstra", "data/historyDijkstraList.txt", ts, source, destination);
            if (algorithm == 0 || algorithm == 2) runAndLogTSP(graph, FordBellmanList::findShortestPaths, FordBellmanList::calculatePathWeightAsString, "FordBellman", "data/historyFordList.txt", ts, source, destination);
        } else {
            GraphMatrix* graph = dynamic_cast<GraphMatrix*>(g);
            if (!graph) return;
            if (algorithm == 0 || algorithm == 1) runAndLogTSP(graph, DijkstraMatrix::findShortestPaths, DijkstraMatrix::calculatePathWeightAsString, "Dijkstra", "data/historyDijkstraMatrix.txt", ts, source, destination);
            if (algorithm == 0 || algorithm == 2) runAndLogTSP(graph, FordBellmanMatrix::findShortestPaths, FordBellmanMatrix::calculatePathWeightAsString, "FordBellman", "data/historyFordMatrix.txt", ts, source, destination);
        }
    }

    template <typename G, typename AlgoFunc, typename WeightFunc>
    static void runAndLogMST(G* g, AlgoFunc algo, WeightFunc weightFunc, const std::string& name,
                             const std::string& historyFile, const std::string& ts) {
        auto result = algo(g);
        int execTime = TimerManager::measureTimeMs([&]() { algo(g); });
        GraphIO::appendGraphHistoryEntry(historyFile, ts, name, getShape<G>(), g->getVertexCount(), g->getEdgeCount(), execTime, 0, formatResultPath(result), weightFunc(result));
    }

    template <typename G, typename AlgoFunc, typename WeightFunc>
    static void runAndLogTSP(G* g, AlgoFunc algo, WeightFunc weightFunc, const std::string& name,
                             const std::string& historyFile, const std::string& ts, int src, int dest) {
        auto result = algo(g, src, dest);
        int execTime = TimerManager::measureTimeMs([&]() { algo(g, src, dest); });
        GraphIO::appendGraphHistoryEntry(historyFile, ts, name, getShape<G>(), g->getVertexCount(), g->getEdgeCount(), execTime, 0, formatResultPath(result), weightFunc(result, true));
    }

    template <typename G>
    static GraphRepresentingShape getShape() {
        if constexpr (std::is_same_v<G, GraphList*> || std::is_same_v<G, GraphList>) return LIST;
        else return MATRIX;
    }

    static std::string getCurrentTimestamp() {
        auto now = std::time(nullptr);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&now), "%Y-%m-%d-%H-%M-%S");
        return oss.str();
    }

    static std::string formatResultPath(const Vektor<EdgeTriple>& result) {
        std::ostringstream oss;
        for (int i = result.size() - 1; i >= 0; --i) {
            oss << result[i].from << "->";
        }
        if (result.size() > 0) oss << result[0].to;
        return oss.str();
    }
};

#endif
