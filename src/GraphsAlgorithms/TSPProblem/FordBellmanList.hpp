#ifndef FORD_BELLMAN_LIST_HPP
#define FORD_BELLMAN_LIST_HPP

#include "../../GraphsImpl/GraphList.hpp"
#include "../../common/EdgeTriple.hpp"
#include "../../VektorImpl/Vektor.hpp"
#include <limits>
#include <iostream>

class FordBellmanList {
public:
    /**
     * Computes shortest paths from the source using Ford-Bellman algorithm
     * for a directed graph represented as an adjacency list.
     * Time complexity: O(V * E)
     */
    static Vektor<EdgeTriple> findShortestPaths(const GraphList* graph, int source = 0, int destination = -1) {
        const int vertexQuantity = graph->getVertexCount();
        Vektor<int> distances = initializeTSPDistances(vertexQuantity);
        Vektor<int> previous = initializeTSPPreviousParents(vertexQuantity);
        distances[source] = 0;

        for (int i = 1; i < vertexQuantity; ++i) {
            for (int u = 0; u < vertexQuantity; ++u) {
                const Vektor<Edge>& neighbors = graph->getNeighbors(u);
                for (int j = 0; j < neighbors.size(); ++j) {
                    int v = neighbors[j].dest;
                    int weight = neighbors[j].weight;
                    if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                        distances[v] = distances[u] + weight;
                        previous[v] = u;
                    }
                }
            }
        }

        // Check for negative weight cycles
        for (int u = 0; u < vertexQuantity; ++u) {
            const Vektor<Edge>& neighbors = graph->getNeighbors(u);
            for (int j = 0; j < neighbors.size(); ++j) {
                int v = neighbors[j].dest;
                int weight = neighbors[j].weight;
                if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                    std::cerr << "[ERROR] Graph contains a negative weight cycle\n";
                    return {};
                }
            }
        }

        if(destination != -1)
            return reconstructSinglePath(previous, distances, source, destination);
        return reconstructAllPaths(previous, distances, source);
    }

    static std::string calculatePathWeightAsString(const Vektor<EdgeTriple>& paths, bool destination=false) {
        if (paths.size() == 0) return "0";
        if (paths.size() == 1) return std::to_string(paths[0].weight);

        int minW = paths[0].weight;
        int maxW = paths[0].weight;
        for (int i = 1; i < paths.size(); ++i) {
            if (paths[i].weight < minW) minW = paths[i].weight;
            if (paths[i].weight > maxW) maxW = paths[i].weight;
        }

        if (minW == maxW || destination) return std::to_string(maxW);
        return "[" + std::to_string(minW) + " - " + std::to_string(maxW) + "]";
    }

    static void displayShortestPaths(const Vektor<EdgeTriple>& paths, int source) {
        std::cout << "Ford-Bellman (List) shortest paths from source " << source << ":\n";
        for (int i = 0; i < paths.size(); ++i) {
            std::cout << paths[i].from << " -> " << paths[i].to << " [weight=" << paths[i].weight << "]\n";
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


    static Vektor<EdgeTriple> reconstructAllPaths(const Vektor<int>& previous, const Vektor<int>& distances, int source) {
        Vektor<EdgeTriple> result;
        for (int i = 0; i < previous.size(); ++i) {
            if (i != source && previous[i] != -1 && distances[i] != std::numeric_limits<int>::max()) {
                result.push_back({previous[i], i, distances[i]});
            }
        }
        return result;
    }

    static Vektor<EdgeTriple> reconstructSinglePath(const Vektor<int>& previous, const Vektor<int>& distances, int source, int dest) {
        Vektor<EdgeTriple> result;
        if (distances[dest] == std::numeric_limits<int>::max()) return result;
        int current = dest;
        while (current != source && previous[current] != -1) {
            result.push_back({previous[current], current, distances[current]});
            current = previous[current];
        }
        return result;
    }
};

#endif
