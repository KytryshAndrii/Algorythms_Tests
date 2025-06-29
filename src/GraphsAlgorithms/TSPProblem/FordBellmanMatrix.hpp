#ifndef FORD_BELLMAN_MATRIX_HPP
#define FORD_BELLMAN_MATRIX_HPP

#include <limits>
#include <iostream>

#include "../../VektorImpl/Vektor.hpp"
#include "../../common/EdgeTriple.hpp"
#include "../../GraphsImpl/GraphMatrix.hpp"

class FordBellmanMatrix {
public:
    /**
     * Computes shortest paths from the source vertex using the Ford-Bellman algorithm
     * on a graph represented by an incidence matrix.
     * Time complexity: O(V * E)
     */
    static Vektor<EdgeTriple> findShortestPaths(const GraphMatrix* graph, int source = 0, int destination = -1) {
        const int vertexQuantity = graph->getVertexCount();
        const int edgeQuantity = graph->getEdgeCount();

        Vektor<int> distances(vertexQuantity);
        Vektor<int> previous(vertexQuantity);

        for (int i = 0; i < vertexQuantity; ++i) {
            distances[i] = std::numeric_limits<int>::max();
            previous[i] = -1;
        }

        distances[source] = 0;

        for (int i = 0; i < vertexQuantity - 1; ++i) {
            for (int e = 0; e < edgeQuantity; ++e) {
                int from = graph->getFrom(e);
                int to = graph->getDest(e);
                int weight = graph->getWeight(e);

                if (distances[from] != std::numeric_limits<int>::max() &&
                    distances[from] + weight < distances[to]) {
                    distances[to] = distances[from] + weight;
                    previous[to] = from;
                }
            }
        }

        // Negative cycle check (optional)
        for (int e = 0; e < edgeQuantity; ++e) {
            int from = graph->getFrom(e);
            int to = graph->getDest(e);
            int weight = graph->getWeight(e);
            if (distances[from] != std::numeric_limits<int>::max() &&
                distances[from] + weight < distances[to]) {
                std::cerr << "[ERROR] Graph contains a negative-weight cycle.\n";
                return Vektor<EdgeTriple>();
            }
        }

        if (destination == -1)
            return reconstructAllPaths(previous, distances, source);
        return reconstructSinglePath(previous, distances, source, destination);
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
        int total = 0;
        std::cout << "Ford-Bellman (Matrix) shortest paths from source " << source << ":\n";
        for (int i = paths.size() - 1; i >= 0; --i) {
            const auto&[from, to, weight] = paths[i];
            std::cout << from << " -> " << to << " [weight=" << weight << "]\n";
            total += weight;
        }
        std::cout << "Total Path weight: " << total << "\n";
    }

private:
    static Vektor<EdgeTriple> reconstructAllPaths(const Vektor<int>& previous, const Vektor<int>& distances, int source) {
        Vektor<EdgeTriple> result;
        for (int v = 0; v < previous.size(); ++v) {
            if (v != source && previous[v] != -1 && distances[v] != std::numeric_limits<int>::max()) {
                result.push_back({previous[v], v, distances[v]});
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
