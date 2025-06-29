#ifndef DIJKSTRA_MATRIX_HPP
#define DIJKSTRA_MATRIX_HPP

#include <limits>
#include <iostream>

#include "../../VektorImpl/Vektor.hpp"
#include "../../common/EdgeTriple.hpp"
#include "../../GraphsImpl/GraphMatrix.hpp"

class DijkstraMatrix {
public:
    /**
     * Computes shortest paths from a source vertex using Dijkstra's algorithm
     * on a directed graph stored in an incidence matrix.
     * Time complexity: O(V^2)
     */
    static Vektor<EdgeTriple> findShortestPaths(const GraphMatrix* graph, int source= 0,  int destination = -1) {
        const int vertexQuantity = graph->getVertexCount();
        const int edgeQuantity = graph->getEdgeCount();

        Vektor<int> distances = initializeTSPDistances(vertexQuantity);
        Vektor<int> previous = initializeTSPPrevious(vertexQuantity);
        Vektor<bool> visited = initializeTSPVisited(vertexQuantity);

        distances[source] = 0;

        for (int i = 0; i < vertexQuantity - 1; ++i) {
            const int currentVertex = getMinDistanceVertex(distances, visited);
            if (currentVertex == -1) break;
            visited[currentVertex] = true;

            for (int edgeIndex = 0; edgeIndex < edgeQuantity; ++edgeIndex) {
                if (graph->getFrom(edgeIndex) == currentVertex) {
                    int neighbor = graph->getDest(edgeIndex);
                    int weight = graph->getWeight(edgeIndex);

                    if (!visited[neighbor] && distances[currentVertex] != std::numeric_limits<int>::max() &&
                        distances[currentVertex] + weight < distances[neighbor]) {
                        distances[neighbor] = distances[currentVertex] + weight;
                        previous[neighbor] = currentVertex;
                    }
                }
            }
        }

        if (destination != -1)
            return reconstructSinglePath(previous, distances, source, destination);

        return reconstructPaths(previous, distances);
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
     * Displays all shortest paths computed using Dijkstra's algorithm.
     */
    static void displayShortestPaths(const Vektor<EdgeTriple>& result,  int source) {
        std::cout << "Dijkstra (Matrix) shortest paths from source " << source << ":\n";
        for (int i = 0; i < result.size(); ++i) {
            std::cout << result[i].from << " -> " << result[i].to
                      << " [weight=" << result[i].weight << "]\n";
        }
    }

private:

    static Vektor<bool> initializeTSPVisited(const int vertexQuantity) {
        Vektor<bool> visited;
        for (int i = 0; i < vertexQuantity; ++i)
            visited.push_back(false);
        return visited;
    }

    static Vektor<int> initializeTSPDistances(const int vertexQuantity) {
        Vektor<int> distances;
        for (int i = 0; i < vertexQuantity; ++i)
            distances.push_back(std::numeric_limits<int>::max());
        return distances;
    }

    static Vektor<int> initializeTSPPrevious(const int vertexQuantity) {
        Vektor<int>  previous;
        for (int i = 0; i < vertexQuantity; ++i)
            previous.push_back(-1);
        return  previous;
    }

    /**
     * Returns the index of the unvisited vertex with the smallest distance.
     * Time complexity: O(V)
     */
    static int getMinDistanceVertex(const Vektor<int>& distances, const Vektor<bool>& visited) {
        int minValue = std::numeric_limits<int>::max();
        int minIndex = -1;
        for (int i = 0; i < distances.size(); ++i) {
            if (!visited[i] && distances[i] <= minValue) {
                minValue = distances[i];
                minIndex = i;
            }
        }
        return minIndex;
    }

    /**
     * Reconstructs the shortest paths from the parent vector.
     * Time complexity: O(V)
     */
    static Vektor<EdgeTriple> reconstructPaths(const Vektor<int>& previous, const Vektor<int>& distances) {
        Vektor<EdgeTriple> result;
        for (int vertex = 0; vertex < previous.size(); ++vertex) {
            if (previous[vertex] != -1 && distances[vertex] != std::numeric_limits<int>::max()) {
                result.push_back({previous[vertex], vertex, distances[vertex]});
            }
        }
        return result;
    }

    /**
    * Reconstructs the path from source to destination only.
    * Time complexity: O(V)
    */
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
