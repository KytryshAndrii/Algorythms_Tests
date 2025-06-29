#ifndef GRAPH_IO_HPP
#define GRAPH_IO_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../common/GraphFactory.hpp"
#include "../VektorImpl/Vektor.hpp"
#include "../GraphsImpl/GraphList.hpp"
#include "../common/GraphRepresentingShape.hpp"
#include "../RandomGenerator/RandomGraphGenerator.hpp"

class GraphIO {
public:

    /**
   * Reads a graph from a file and creates an instance of the selected graph type.
   * Performs basic format and value validation.
   * Time complexity: O(E) for edge reading, O(V + E) for validation.
   */
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
            if (from != to || to != from) {
                graph->addEdge(from, to, weight);
            }
        }

        return graph;
    }

    /**
    * Writes a vector of edges and vertex count into a text file.
    * Format: first line "E V", followed by "from to weight" per edge.
    * Time complexity: O(E)
    */
    static void writeGraphToFile(const std::string& filename, const Vektor<EdgeTriple>& edges, int vertexCount) {
        std::ofstream ofs(filename);
        if (!ofs) {
            std::cerr << "[ERROR] Cannot open file to write graph: " << filename << std::endl;
            return;
        }

        ofs << edges.size() << " " << vertexCount << "\n";
        for (int i = 0; i < edges.size(); ++i) {
            ofs << edges[i].from << " " << edges[i].to << " " << edges[i].weight << "\n";
        }
        ofs.close();
    }

    /**
    * Appends a single test run entry to the graph history CSV-style log.
    */
    static void appendGraphHistoryEntry(
        const std::string& filename,
        const std::string& timeStamp,
        const std::string& algorithmName,
        const GraphRepresentingShape& graphType,
        int vertexCount,
        int edgeCount,
        int solveTimeMs,
        int repetitionNumber,
        const std::string& resultPath,
        const std::string& resultValue
    ) {
        std::ofstream ofs(filename, std::ios::app);
        if (!ofs) {
            std::cerr << "[ERROR] Cannot open history file: " << filename << std::endl;
            return;
        }
        std::string graphTypeStr = (graphType == LIST) ? "LIST" : "MATRIX";

        ofs << timeStamp << ';'
            << algorithmName << ';'
            << graphTypeStr << ';'
            << vertexCount << ';'
            << edgeCount << ';'
            << repetitionNumber << ';'
            << resultPath << ';'
            << resultValue << ';'
            << solveTimeMs << '\n';

        ofs.close();
    }

    /**
     * Appends a summary row to the summary results file.
     * Includes statistics like avg, median, min/max for timing and result values.
     */
    static void appendGraphSummaryEntry(
       const std::string& summaryFile,
       const std::string& timeStamp,
       const std::string& algorithmName,
       const GraphRepresentingShape& graphType,
       int vertexCount,
       int edgeCount,
       double density,
       int repetitions,
       int minTimeMs,
       int maxTimeMs,
       double avgTimeMs,
       double medianTimeMs
   ) {
        // Spróbuj otworzyć plik
        std::ofstream ofs(summaryFile, std::ios::app);
        if (!ofs.is_open()) {
            std::cerr << "[ERROR] Cannot open summary file: " << summaryFile << std::endl;
            return;
        }

        std::string graphTypeStr = (graphType == LIST) ? "LIST" : "MATRIX";

        ofs << timeStamp << ';'
            << algorithmName << ';'
            << graphTypeStr << ';'
            << vertexCount << ';'
            << edgeCount << ';'
            << density << ';'
            << repetitions << ';'
            << minTimeMs << ';'
            << maxTimeMs << ';'
            << avgTimeMs << ';'
            << medianTimeMs << ';' << '\n';

        ofs.close();
    }

private:

    /**
    * Performs full validation: header format, edge data, index bounds.
    * Time complexity: O(E + V)
    */
    static bool isValidGraphFile(std::ifstream& file, int& edgeCount, int& vertexCount) {

        if (!file.is_open()) {
            printError("Cannot open file: ");
            return false;
        }

        if (!readHeader(file, edgeCount, vertexCount))
            return false;
        return validateEdgeData(file, edgeCount, vertexCount);
    }

    /**
     * Reads and validates the header line "<edgeCount> <vertexCount>".
     * Returns true only if both values are positive integers.
     */
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

    /**
    * Validates edge lines: correct number, valid vertex indices, and proper formatting.
    * Also ensures all vertices are referenced at least once.
    */
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

    /**
    * Parses a line with format "from to weight" into integers.
    * Returns false if parsing fails.
    */
    static bool parseEdgeLine(const std::string& line, int& from, int& to, int& weight) {
        std::istringstream edgeStream(line);
        if (!(edgeStream >> from >> to >> weight)) {
            printError("Invalid edge format in line: " + line);
            return false;
        }
        return true;
    }

    /**
    * Prints a standard formatted error message.
    */
    static void printError(const std::string& message) {
        std::cerr << "[ERROR] " << message << std::endl;
    }
};

#endif
