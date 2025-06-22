#ifndef DIJKSTRA_LIST_HPP
#define DIJKSTRA_LIST_HPP

#include "../../GraphsImpl/GraphList.hpp"
#include "../../common/EdgeTriple.hpp"
#include "../../VektorImpl/Vektor.hpp"
#include <limits>
#include <queue>
#include <iostream>

#include "../../HeapImpl/MinHeap.hpp"

class DijkstraList {
public:
    /**
     * Computes shortest paths from the source using Dijkstra's algorithm
     * for a directed graph represented as an adjacency list.
     * Time complexity: O((V + E) * log V) using a priority queue.
     */
    static Vektor<EdgeTriple> findShortestPaths(const GraphList* graph, int source = 0, int destination = -1) {
        const int vertexQuantity = graph->getVertexCount();
        Vektor<int> distances = initializeTSPDistances(vertexQuantity);
        Vektor<int> previous = initializeTSPPreviousParents(vertexQuantity);
        MinHeap<std::pair<int, int>> minHeap; // (distance, vertex)
        distances[source] = 0;
        minHeap.push({0, source});

        while (!minHeap.empty()) {
            std::pair<int, int> top = minHeap.top();
            minHeap.pop();

            int currentVertex = top.second;
            int currentDistance = top.first;

            if (currentDistance > distances[currentVertex]) continue;

            const Vektor<Edge>& neighbors = graph->getNeighbors(currentVertex);
            for (int i = 0; i < neighbors.size(); ++i) {
                int neighbor = neighbors[i].dest;
                int weight = neighbors[i].weight;

                if (distances[currentVertex] + weight < distances[neighbor]) {
                    distances[neighbor] = distances[currentVertex] + weight;
                    previous[neighbor] = currentVertex;
                    minHeap.push({distances[neighbor], neighbor});
                }
            }
        }
        if (destination != -1)
            return reconstructSinglePath(previous, distances, source, destination);

        return reconstructPaths(previous, distances, source);
    }

    static std::string calculatePathWeightAsString(const Vektor<EdgeTriple>& paths, bool destination=false) {
        if (paths.size() == 0) return "0";

        int min = std::numeric_limits<int>::max();
        int max = std::numeric_limits<int>::min();

        for (int i = 0; i < paths.size(); ++i) {
            if (paths[i].weight < min) min = paths[i].weight;
            if (paths[i].weight > max) max = paths[i].weight;
        }

        if (min == max || destination) return std::to_string(max);
        return "[" + std::to_string(min) + " - " + std::to_string(max) + "]";
    }

    /**
     * Displays all shortest paths from the given source vertex.
     */
    static void displayShortestPaths(const Vektor<EdgeTriple>& paths, int source) {
        std::cout << "Dijkstra (List) shortest paths from source " << source << ":\n";
        for (int i = 0; i < paths.size(); ++i) {
            std::cout << paths[i].from << " -> " << paths[i].to << " [w=" << paths[i].weight << "]\n";
        }
    }

private:

    static Vektor<int> initializeTSPDistances(const int vertexQuantity) {
        Vektor<int> distances;
        for (int i = 0; i < vertexQuantity; ++i)
            distances.push_back(std::numeric_limits<int>::max());
        return distances;
    }

    static Vektor<int> initializeTSPPreviousParents(const int vertexQuantity) {
        Vektor<int> previous;
        for (int i = 0; i < vertexQuantity; ++i)
            previous.push_back(-1);
        return previous;
    }

    /**
     * Reconstructs a vector of EdgeTriple from parent and distance arrays.
     */
    static Vektor<EdgeTriple> reconstructPaths(const Vektor<int>& parents, const Vektor<int>& distances, int source) {
        Vektor<EdgeTriple> result;
        for (int v = 0; v < parents.size(); ++v) {
            if (v != source && parents[v] != -1) {
                result.push_back({parents[v], v, distances[v]});
            }
        }
        return result;
    }

    static Vektor<EdgeTriple> reconstructSinglePath(const Vektor<int>& parents, const Vektor<int>& distances, int source, int dest) {
        Vektor<EdgeTriple> result;
        if (distances[dest] == std::numeric_limits<int>::max()) return result;
        int current = dest;
        while (current != source && parents[current] != -1) {
            result.push_back({parents[current], current, distances[current]});
            current = parents[current];
        }
        return result;
    }
};

#endif
