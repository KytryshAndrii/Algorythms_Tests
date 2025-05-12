#include "src/Graphs/GraphMatrix.hpp"
#include "src/Graphs/GraphList.hpp"

int main() {
    // Test listy sąsiedztwa
    GraphList gl(4);
    gl.addEdge(0, 1, 10);
    gl.addEdge(1, 2, 5);
    gl.addEdge(2, 3, 2);
    gl.display();

    // Test macierzy incydencji
    GraphMatrix gm(4, 3);
    gm.addEdge(0, 1, 10);
    gm.addEdge(1, 2, 5);
    gm.addEdge(2, 3, 2);
    gm.display();

    return 0;
}
