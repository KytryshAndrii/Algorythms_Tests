#ifndef GRAPH_IO_HPP
#define GRAPH_IO_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include "../VektorImpl/Vektor.hpp"
#include "../Graphs/GraphList.hpp"
#include "../Graphs/GraphMatrix.hpp"
#include "../common/GraphRepresentingShape.hpp"

class GraphIO {
public:
    static IGraph* readGraph(const std::string& filename, GraphRepresentingShape type) {
        int declaredEdges = 0;
        int declaredVertices = 0;

        std::ifstream file(filename);

        if (!isValidGraphFile(file, declaredEdges, declaredVertices)) {
            return nullptr;
        }

        file.clear();
        file.seekg(0);
        std::string line;
        std::getline(file, line);

        IGraph* graph = createGraphInstance(type, declaredVertices, declaredEdges);
        if (!graph) return nullptr;

        for (int i = 0; i < declaredEdges; ++i) {
            int from, to, weight;
            file >> from >> to >> weight;
            graph->addEdge(from, to, weight);
        }

        return graph;
    }

    static void appendGraphHistoryEntry(
        const std::string& filename,
        const std::string& timeStamp,
        const std::string& algorithmName,
        const std::string& graphType,
        int vertexCount,
        int edgeCount,
        int solveTimeMs,
        int repetitionNumber,
        const std::string& resultPath,
        int resultValue
    ) {
        std::ofstream ofs(filename, std::ios::app);
        if (!ofs) {
            std::cerr << "[ERROR] Cannot open history file: " << filename << std::endl;
            return;
        }

        ofs << timeStamp << ';'
            << algorithmName << ';'
            << graphType << ';'
            << vertexCount << ';'
            << edgeCount << ';'
            << solveTimeMs << ';'
            << repetitionNumber << ';'
            << resultPath << ';'
            << resultValue << '\n';

        ofs.close();
    }

    static void appendGraphSummaryEntry(
        const std::string& summaryFile,
        const std::string& timeStamp,
        const std::string& algorithmName,
        const std::string& graphType,
        int vertexCount,
        int edgeCount,
        int repetitions,
        int minTimeMs,
        int maxTimeMs,
        double avgTimeMs,
        double medianTimeMs,
        int minResult,
        int maxResult
    ) {
        std::ofstream ofs(summaryFile, std::ios::app);
        if (!ofs) {
            std::cerr << "[ERROR] Cannot open summary file: " << summaryFile << std::endl;
            return;
        }

        // Format: timestamp;algorithm;graphType;V;E;reps;min;max;avg;med;[minResult,maxResult]
        ofs << timeStamp << ';'
            << algorithmName << ';'
            << graphType << ';'
            << vertexCount << ';'
            << edgeCount << ';'
            << repetitions << ';'
            << minTimeMs << ';'
            << maxTimeMs << ';'
            << avgTimeMs << ';'
            << medianTimeMs << ';'
            << '[' << minResult << ',' << maxResult << ']'
            << '\n';

        ofs.close();
    }



private:
    static bool isValidGraphFile(std::ifstream& file, int& edgeCount, int& vertexCount) {

        if (!file.is_open()) {
            printError("Cannot open file: ");
            return false;
        }

        if (!readHeader(file, edgeCount, vertexCount))
            return false;
        return validateEdgeData(file, edgeCount, vertexCount);
    }

    static bool readHeader(std::ifstream& file, int& edgeCount, int& vertexCount) {
        std::string line;
        if (!std::getline(file, line)) {
            printError("Empty file or invalid format");
            return false;
        }

        std::istringstream header(line);
        if (!(header >> edgeCount >> vertexCount)) {
            printError("Invalid header format (expected: <edges> <vertices>)");
            return false;
        }

        if (edgeCount <= 0 || vertexCount <= 0) {
            printError("Declared edge/vertex count must be positive");
            return false;
        }

        return true;
    }

    static bool validateEdgeData(std::ifstream& file, int declaredEdges, int declaredVertices) {
        Vektor<bool> vertexPresent(declaredVertices);
        for (int i = 0; i < declaredVertices; ++i) vertexPresent[i] = false;

        int actualEdges = 0;
        std::string line;

        while (std::getline(file, line)) {
            int from, to, weight;
            if (!parseEdgeLine(line, from, to, weight))
                return false;

            if (from < 0 || from >= declaredVertices || to < 0 || to >= declaredVertices) {
                printError("Invalid vertex index: " + std::to_string(from) + " or " + std::to_string(to));
                return false;
            }

            vertexPresent[from] = true;
            vertexPresent[to] = true;
            actualEdges++;
        }

        if (actualEdges != declaredEdges) {
            printError("Edge count mismatch. Declared: " + std::to_string(declaredEdges) +
                       ", Found: " + std::to_string(actualEdges));
            return false;
        }

        return true;
    }

    static bool parseEdgeLine(const std::string& line, int& from, int& to, int& weight) {
        std::istringstream edgeStream(line);
        if (!(edgeStream >> from >> to >> weight)) {
            printError("Invalid edge format in line: " + line);
            return false;
        }
        return true;
    }

    static void printError(const std::string& message) {
        std::cerr << "[ERROR] " << message << std::endl;
    }

    // static bool isMissingEdges(const int declaredVertices, Vektor<bool> vertexPresent) {
    //     for (int i = 0; i < declaredVertices - 1; i++) {
    //         if (!vertexPresent[i]) {
    //             printError("Missing vertex with index: " + std::to_string(i));
    //             return false;
    //         }
    //     }
    //     return true;
    // }
    static IGraph* createGraphInstance(GraphRepresentingShape type, int vertices, int edges) {
        if (type == LIST) {
            return new GraphList(vertices);
        }
        if (type == MATRIX) {
            return new GraphMatrix(vertices, edges);
        }
        printError("[ERROR] Unknown graph type");
        return nullptr;
    }

};

#endif
