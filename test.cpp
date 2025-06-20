#include "src/FileIO/GraphFileIO.hpp"
#include "src/GraphsAlgorithms/MSTProblem/KruskalList.hpp"
#include "src/GraphsAlgorithms/MSTProblem/KruskalMatrix.hpp"
#include "src/GraphsAlgorithms/MSTProblem/PrimList.hpp"
#include "src/GraphsAlgorithms/MSTProblem/PrimMatrix.hpp"
#include "src/GraphsImpl/GraphMatrix.hpp"
#include "src/GraphsImpl/GraphList.hpp"

int main() {
    const std::string filePath = "./graphBenchmarkData/graph_v50_d25_id0.txt";
    std::ifstream file(filePath);

    if (!file) {
        std::cerr << "[ERROR] Cannot open file: " << filePath << "\n";
        return 1;
    }

    int edgeCount, vertexCount;
    file >> edgeCount >> vertexCount;

    GraphMatrix graphMatrix(vertexCount, edgeCount);
    GraphList graphList(vertexCount);

    for (int i = 0; i < edgeCount; ++i) {
        int u, v, w;
        file >> u >> v >> w;
        graphMatrix.addEdge(u, v, w);
        graphList.addEdge(u, v, w);
        graphList.addEdge(v, u, w); // symetrycznie dla nieskierowanego
    }

    std::cout << "\n[INFO] MST from GraphMatrix:\n";
    Vektor<EdgeTriple> mstMatrix = PrimMatrix::findMST(&graphMatrix);
    KruskalMatrix::displayMST(mstMatrix);

    std::cout << "\n[INFO] MST from GraphList:\n";
    Vektor<EdgeTriple> mstList = PrimList::findMST(&graphList);
    KruskalList::displayMST(mstList);

    return 0;
}
