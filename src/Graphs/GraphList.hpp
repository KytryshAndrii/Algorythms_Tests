
#ifndef GRAPHLIST_HPP
#define GRAPHLIST_HPP
#include "../VektorImpl/Vektor.hpp"
#include <iostream>

struct Edge {
    int dest;
    int weight;
};

class GraphList {
    int vertexCount;
    int edgeCount;
    Vektor<Vektor<Edge>> adjList;

public:
    GraphList(int vertices): vertexCount(vertices), edgeCount(0), adjList(vertices) {}

    void addEdge(int from, int to, int weight) {
        Edge edge = {to, weight};
        adjList[from].push_back(edge);
        edgeCount++;
    }

    const Vektor<Edge>& getNeighbors(int node) const {
        return adjList[node];
    }

    void display() const {
        std::cout << "Adjacency List:\n";
        for (int i = 0; i < vertexCount; ++i) {
            std::cout << i << ": ";
            for (int j = 0; j < adjList[i].size(); ++j) {
                std::cout << "(" << adjList[i][j].dest << ", " << adjList[i][j].weight << ") ";
            }
            std::cout << std::endl;
        }
    }

    int getVertexCount() const {
        return vertexCount;
    }
    int getEdgeCount() const {
        return edgeCount;
    }
};

#endif
