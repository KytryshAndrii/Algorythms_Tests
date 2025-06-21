
#ifndef GRAPHLIST_HPP
#define GRAPHLIST_HPP
#include "../VektorImpl/Vektor.hpp"
#include <iostream>

#include "IGraph.hpp"

/**
 * Struct representing a single edge in adjacency list.
 */
struct Edge {
    int dest;
    int weight;
};

/**
 * Graph represented using an adjacency list (sparse graph friendly).
 */
class GraphList: public IGraph{
    int vertexCount;
    int edgeCount;
    Vektor<Vektor<Edge>> adjList;

public:
    /**
     * Constructs a graph with the given number of vertices.
     * Time complexity: O(V), where V is the number of vertices.
     */
    GraphList(int vertices): vertexCount(vertices), edgeCount(0), adjList(vertices) {}


    /**
     * Adds a directed edge from 'from' to 'to' with specified weight.
     * Time complexity: O(1)
     */
    void addEdge(int from, int to, int weight) override {
        Edge edge = {to, weight};
        if (from == to)
            return;
        adjList[from].push_back(edge);
        edgeCount++;
    }

    /**
    * Returns all outgoing edges from a given node.
    * Time complexity: O(1)
    */
    const Vektor<Edge>& getNeighbors(int node) const {
        return adjList[node];
    }

    /**
    * Displays the adjacency list of the graph.
    * Time complexity: O(V + E)
    */
    void display() const override {
        std::cout << "Adjacency List:\n";
        for (int i = 0; i < vertexCount; ++i) {
            std::cout << "e" << i << ": ";
            for (int j = 0; j < adjList[i].size(); ++j) {
                std::cout << "(" << adjList[i][j].dest << ", " << adjList[i][j].weight << ") ";
            }
            std::cout << std::endl;
        }
    }

    /**
    * Returns the number of vertices.
    * Time complexity: O(1)
    */
    int getVertexCount() const override {
        return vertexCount;
    }

    /**
     * Returns the total number of added edges.
     * Time complexity: O(1)
     */
    int getEdgeCount() const override {
        return edgeCount;
    }
};

#endif
