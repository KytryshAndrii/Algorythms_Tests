#include "src/FileIO/GraphFileIO.hpp"
#include "src/Graphs/GraphMatrix.hpp"
#include "src/Graphs/GraphList.hpp"

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

    IGraph* listGraf = GraphIO::readGraph("./data/input_graph.txt", LIST);
    if (listGraf == nullptr) {
        std::cerr << "[MAIN] Failed to load graph as LIST.\n";
    }else {
        std::cout << "=== Adjacency List Representation ===\n";
        listGraf->display();
        delete listGraf;
    }

    std::cout << "\n--------------------------------------\n\n";

    // Read and map to MATRIX
    IGraph* matrixGraph = GraphIO::readGraph("./data/input_graph.txt", MATRIX);
    if (matrixGraph == nullptr) {
        std::cerr << "[MAIN] Failed to load graph as MATRIX.\n";
    }else {
        std::cout << "=== Incidence Matrix Representation ===\n";
        matrixGraph->display();
        GraphIO::appendGraphHistoryEntry(
            "./data/output_graph_history.txt",
            "2025-06-16 17:34:00",
            "Dijkstra",
            "List",
            matrixGraph->getVertexCount(),
            matrixGraph->getEdgeCount(),
            23,            // ms
            1,             // repetition
            "0->1->4->5",  // path
            15             // total weight
        );
        delete matrixGraph;
    }

    return 0;
}
