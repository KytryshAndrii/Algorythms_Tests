#ifndef GRAPHMATRIX_HPP
#define GRAPHMATRIX_HPP

#include "../VektorImpl/Vektor.hpp"
#include <iostream>

class GraphMatrix {
    int vertexCount;
    int edgeLimit;
    int currentEdge;
    Vektor<Vektor<int>> matrix;     // V x E
    Vektor<int> from;
    Vektor<int> dest;
    Vektor<int> weights;

public:
    GraphMatrix(int vertices, int maxEdges)
        : vertexCount(vertices), edgeLimit(maxEdges), currentEdge(0)
    {
        // Initialize incidence matrix: V rows, each with E zeros
        for (int i = 0; i < vertexCount; ++i) {
            Vektor<int> row;
            for (int j = 0; j < edgeLimit; ++j) {
                row.push_back(0);
            }
            matrix.push_back(row);
        }

        // Prepare edge vectors
        for (int i = 0; i < edgeLimit; ++i) {
            from.push_back(-1);
            dest.push_back(-1);
            weights.push_back(0);
        }
    }

    void addEdge(int u, int v, int weight) {
        if (currentEdge >= edgeLimit) {
            std::cerr << "Edge limit reached!\n";
            return;
        }

        matrix[u][currentEdge] = -1;
        matrix[v][currentEdge] = 1;
        from[currentEdge] = u;
        dest[currentEdge] = v;
        weights[currentEdge] = weight;
        currentEdge++;
    }

    void display() const {
        std::cout << "Incidence Matrix (" << vertexCount << " x " << currentEdge << "):\n";
        for (int i = 0; i < vertexCount; ++i) {
            for (int j = 0; j < currentEdge; ++j) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    int getVertexCount() const {
        return vertexCount;
    }
    int getEdgeCount() const {
        return currentEdge;
    }
};

#endif
