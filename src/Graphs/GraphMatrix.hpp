#ifndef GRAPHMATRIX_HPP
#define GRAPHMATRIX_HPP

#include "../VektorImpl/Vektor.hpp"
#include <iostream>

class GraphMatrix : public IGraph {
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

    void addEdge(int u, int v, int weight) override {
        if (currentEdge >= edgeLimit) {
            std::cerr << "[GraphMatrix] Edge limit reached!\n";
            return;
        }

        matrix[u][currentEdge] = 1;
        matrix[v][currentEdge] = -1;
        from[currentEdge] = u;
        dest[currentEdge] = v;
        weights[currentEdge] = weight;
        currentEdge++;
    }

    void display() const override {
        std::cout << "Incidence Matrix (" << vertexCount << " x " << currentEdge << "):\n";

        std::cout << "\t";
        for (int j = 0; j < currentEdge; ++j) {
            std::cout << "v" << j << "\t";
        }
        std::cout << "\n";

        for (int i = 0; i < vertexCount; ++i) {
            std::cout << "e" << i << "\t";
            for (int j = 0; j < currentEdge; ++j) {
                std::cout << matrix[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }

    int getVertexCount() const override {
        return vertexCount;
    }
    int getEdgeCount() const override {
        return currentEdge;
    }
};

#endif
