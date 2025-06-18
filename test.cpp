#include <filesystem>

#include "src/FileIO/GraphFileIO.hpp"
#include "src/GraphsImpl/GraphMatrix.hpp"
#include "src/GraphsImpl/GraphList.hpp"
#include "src/RandomGenerator/RandomGraphGenerator.hpp"

int main() {
    // Test listy sąsiedztwa
    // GraphList gl(4);
    // gl.addEdge(0, 1, 10);
    // gl.addEdge(1, 2, 5);
    // gl.addEdge(2, 3, 2);
    // gl.display();
    //
    // // Test macierzy incydencji
    // GraphMatrix gm(4, 3);
    // gm.addEdge(0, 1, 10);
    // gm.addEdge(1, 2, 5);
    // gm.addEdge(2, 3, 2);
    // gm.display();

    // IGraph* listGraf = GraphIO::readGraph("./data/input_graph.txt", LIST);
    // if (listGraf == nullptr) {
    //     std::cerr << "[MAIN] Failed to load graph as LIST.\n";
    // }else {
    //     std::cout << "=== Adjacency List Representation ===\n";
    //     listGraf->display();
    //     delete listGraf;
    // }
    //
    // std::cout << "\n--------------------------------------\n\n";
    //
    // // Read and map to MATRIX
    // IGraph* matrixGraph = GraphIO::readGraph("./data/input_graph.txt", MATRIX);
    // if (matrixGraph == nullptr) {
    //     std::cerr << "[MAIN] Failed to load graph as MATRIX.\n";
    // }else {
    //     std::cout << "=== Incidence Matrix Representation ===\n";
    //     matrixGraph->display();
    //     GraphIO::appendGraphHistoryEntry(
    //         "./data/output_graph_history.txt",
    //         "2025-06-16 17:34:00",
    //         "Dijkstra",
    //         "List",
    //         matrixGraph->getVertexCount(),
    //         matrixGraph->getEdgeCount(),
    //         23,            // ms
    //         1,             // repetition
    //         "0->1->4->5",  // path
    //         15             // total weight
    //     );
    //     delete matrixGraph;
    // }
    // RandomGraphGenerator generator;
    //
    // int vertexCount = 10;
    // double density = 99.0; // or 50.0 or 99.0
    //
    // // Generate Adjacency List representation
    // std::cout << "=== Adjacency List Representation ===" << std::endl;
    // IGraph* listGraph = generator.generateGraphInStructure(vertexCount, density, LIST);
    // if (listGraph) {
    //     listGraph->display();
    //     delete listGraph;
    // } else {
    //     std::cerr << "[MAIN] Failed to generate list graph.\n";
    // }
    //
    // std::cout << "\n--------------------------------------\n" << std::endl;
    //
    // // Generate Incidence Matrix representation
    // std::cout << "=== Incidence Matrix Representation ===" << std::endl;
    // IGraph* matrixGraph = generator.generateGraphInStructure(vertexCount, density, MATRIX);
    // if (matrixGraph) {
    //     matrixGraph->display();
    //     delete matrixGraph;
    // } else {
    //     std::cerr << "[MAIN] Failed to generate matrix graph.\n";
    // }
    RandomGraphGenerator generator;
    std::string basePath = "./graphBenchmarkData/";
    std::filesystem::create_directories(basePath);  // ensure output folder exists

    int sizes[] = {100, 250, 500, 750, 1000, 3000, 5000};
    double densities[] = {25.0, 50.0, 99.0};

    for (int s = 0; s < 7; ++s) {
        int vertexCount = sizes[s];

        for (int d = 0; d < 3; ++d) {
            double density = densities[d];

            for (int i = 0; i < 50; ++i) {
                Vektor<EdgeTriple> edges;

                if ((int)density == 25)
                    edges = generator.generateGraph25(vertexCount);
                else if ((int)density == 50)
                    edges = generator.generateGraph50(vertexCount);
                else
                    edges = generator.generateGraph99(vertexCount);

                std::string filename = basePath + "graph_v" + std::to_string(vertexCount) +
                                       "_d" + std::to_string((int)density) +
                                       "_id" + std::to_string(i) + ".txt";

                GraphIO::writeGraphToFile(filename, edges, vertexCount);
            }
        }
    }



    return 0;
}
