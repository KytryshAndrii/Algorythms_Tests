#ifndef GRAPHMATRIX_HPP
#define GRAPHMATRIX_HPP

#include "../VektorImpl/Vektor.hpp"
#include <iostream>

#include "IGraph.hpp"

/**
 * Graph represented using an incidence matrix (good for dense graphs).
 * Each column corresponds to an edge.
 */
class GraphMatrix : public IGraph {
    int vertexCount;
    int edgeLimit;
    int currentEdge;
    Vektor<Vektor<int>> matrix; // Matrix: vertexCount x edgeLimit
    Vektor<int> weights;

public:
    /**
    * Constructs an incidence matrix graph with given number of vertices and max edges.
    * Time complexity: O(V * E)
    */
    GraphMatrix(const int vertices, const int maxEdges)
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
            weights.push_back(0);
        }
    }

    /**
    * Adds an edge from u to v with the given weight.
    * Represented by marking u with 1 and v with -1 in the matrix.
    * Time complexity: O(1)
    */
    void addEdge(const int u, const int v, const int weight) override {
        if (u == v) return;
        if (currentEdge >= edgeLimit) {
            std::cerr << "[GraphMatrix] Edge limit reached!\n";
            return;
        }

        matrix[u][currentEdge] = 1;
        matrix[v][currentEdge] = -1;
        weights[currentEdge] = weight;
        currentEdge++;
    }

    /**
     * Displays the incidence matrix of the graph.
     */
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

    /**
     * Returns the number of vertices in the graph.
     * Time complexity: O(1)
     */
    int getVertexCount() const override {
        return vertexCount;
    }

    /**
     * Returns the current number of added edges.
     * Time complexity: O(1)
     */
    int getEdgeCount() const override {
        return currentEdge;
    }

    /**
     * Returns the source vertex of a given edge index (1 in matrix).
     * Time complexity: O(V)
     */
    int getFrom(const int edgeIndex) const {
        for (int u = 0; u < vertexCount; ++u) {
            if (matrix[u][edgeIndex] == 1) return u;
        }
        return -1;
    }

    /**
     * Returns the destination vertex of a given edge index (-1 in matrix).
     * Time complexity: O(V)
     */
    int getDest(const int edgeIndex) const {
        for (int v = 0; v < vertexCount; ++v) {
            if (matrix[v][edgeIndex] == -1) return v;
        }
        return -1;
    }

    /**
     * Returns the weight of the given edge index.
     * Time complexity: O(1)
     */
    int getWeight(const int edgeIndex) const {
        return weights[edgeIndex];
    }

};

#endif
