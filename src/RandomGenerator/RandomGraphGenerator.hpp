#ifndef RANDOM_GRAPH_GENERATOR_HPP
#define RANDOM_GRAPH_GENERATOR_HPP

#include <random>
#include <utility>
#include <algorithm>
#include <ctime>

#include "../common/EdgeTriple.hpp"
#include "../VektorImpl/Vektor.hpp"
#include "../common/GraphFactory.hpp"
#include "../common/GraphRepresentingShape.hpp"

class RandomGraphGenerator {
    std::mt19937 rng;

public:

    /**
     * Constructor seeds the RNG with current time.
     */
    RandomGraphGenerator() {
        rng.seed(static_cast<unsigned>(time(nullptr)));
        std::cout << "[LOG] RandomGraphGenerator initialized with seed from time.\n";
    }

    /**
     * Generates a random graph with 25% density.
     */
    Vektor<EdgeTriple> generateGraph25(int v) {
        std::cout << "[LOG] Generating graph with 25% density and " << v << " vertices.\n";
        return generateGraphWithDensity(v, 25.0);
    }

    /**
     * Generates a random graph with 50% density.
     */
    Vektor<EdgeTriple> generateGraph50(int v) {
        std::cout << "[LOG] Generating graph with 50% density and " << v << " vertices.\n";
        return generateGraphWithDensity(v, 50.0);
    }

    /**
     * Generates a random graph with 99% density.
     */
    Vektor<EdgeTriple> generateGraph99(int v) {
         std::cout << "[LOG] Generating graph with 99% density and " << v << " vertices.\n";
        return generateGraphWithDensity(v, 99.0);
    }

    /**
     * Generates a graph (with specific density) and inserts it into an IGraph structure.
     * Time complexity: O(E), where E is target number of edges.
     */
    IGraph* generateGraphInStructure(int v, double density, GraphRepresentingShape type) {
        std::cout << "[LOG] Generating graph with " << density << "% density and " << v << " vertices into structure.\n";
        Vektor<EdgeTriple> edges = generateGraphWithDensity(v, density);
        int maxEdges = v * (v - 1) / 2;

        IGraph* graph = createGraphInstance(type, v, maxEdges);
        if (!graph) {
            std::cerr << "[ERROR] Graph creation failed.\n";
            return nullptr;
        }

        std::cout << "[LOG] Created graph instance. Inserting " << edges.size() << " edges...\n";
        for (int i = 0; i < edges.size(); ++i) {
            graph->addEdge(edges[i].from, edges[i].to, edges[i].weight);
        }
        std::cout << "[LOG] Graph generation into structure complete.\n";
        return graph;
    }

    /**
    * Logs the actual density of a generated graph and compares it to the intended density.
    */
    static void logActualDensity(int vertexCount, int edgeCount, double targetDensity) {
        int maxEdges = vertexCount * (vertexCount - 1) / 2;
        double actualDensity = (100.0 * edgeCount) / maxEdges;
        std::cout << "[LOG] Actual density: " << actualDensity << "%\n";

        if (std::abs(actualDensity - targetDensity) > 1.0) {
            std::cerr << "[WARNING] Density deviates from target: expected " << targetDensity
                      << "% but got " << actualDensity << "%\n";
        }
    }



private:

    /**
    * Returns a random vertex from 0 to n-1, optionally excluding one vertex.
    */
    int getRandomVertex(int n, int exclude = -1) {
        std::uniform_int_distribution<int> dist(0, n - 1);
        int v;
        do {
            v = dist(rng);
        } while (v == exclude);
        return v;
    }

    /**
     * Returns a random edge weight in range [minW, maxW].
     */
    int getRandomWeight(int minW = 0, int maxW = 100) {
        std::uniform_int_distribution<int> dist(minW, maxW);
        return dist(rng);
    }

    /**
    * Checks if an edge (u,v) or (v,u) already exists in edgeList.
    */
    static bool edgeExists(const Vektor<std::pair<int, int>>& edgeList, int u, int v) {
        int a = std::min(u, v);
        int b = std::max(u, v);
        for (int i = 0; i < edgeList.size(); ++i) {
            if (edgeList[i].first == a && edgeList[i].second == b) {
                return true;
            }
        }
        return false;
    }

    /**
     * Adds edge (u,v) to the seen set, ensuring consistent ordering.
     */
    static void addEdgeRecord(Vektor<std::pair<int, int>>& edgeList, int u, int v) {
        edgeList.push_back({std::min(u, v), std::max(u, v)});
    }

    /**
     * Generates a random spanning tree using randomized Prim-like logic.
     * Ensures connectivity. Time complexity: O(V)
     */
    Vektor<EdgeTriple> generateSpanningTree(int vertexCount) {
        std::cout << "[LOG] Generating spanning tree...\n";
        Vektor<EdgeTriple> edges;
        Vektor<int> connected;
        Vektor<int> unconnected;

        for (int i = 0; i < vertexCount; ++i) unconnected.push_back(i);

        int first = getRandomVertex(vertexCount);
        connected.push_back(first);
        for (int i = 0; i < unconnected.size(); ++i) {
            if (unconnected[i] == first) {
                unconnected.removeAt(i);
                break;
            }
        }

        while (unconnected.size() > 0) {
            int u = connected[getRandomVertex(connected.size())];
            int v = unconnected[getRandomVertex(unconnected.size())];
            int weight = getRandomWeight();

            edges.push_back({u, v, weight});
            connected.push_back(v);
            for (int i = 0; i < unconnected.size(); ++i) {
                if (unconnected[i] == v) {
                    unconnected.removeAt(i);
                    break;
                }
            }
        }

        std::cout << "[LOG] Spanning tree generated with " << edges.size() << " edges.\n";
        return edges;
    }

    /**
     * Generates all unique possible undirected edges for a complete graph.
     * Time complexity: O(V^2)
     */
    Vektor<EdgeTriple> generateAllPossibleEdges(int vertexCount) {
        Vektor<EdgeTriple> allEdges;
        for (int u = 0; u < vertexCount; ++u) {
            for (int v = u + 1; v < vertexCount; ++v) {
                allEdges.push_back({u, v, getRandomWeight()});
            }
        }
        return allEdges;
    }

    /**
     * Generates a graph with desired density by:
     * - creating a random spanning tree (to guarantee connectivity)
     * - adding additional random edges until target density is met
     * Time complexity: O(V + E + V^2) worst-case due to shuffle/search
    */
    Vektor<EdgeTriple> generateGraphWithDensity(int vertexCount, double densityPercent) {
        int maxEdges = vertexCount * (vertexCount - 1) / 2;
        int targetEdgeCount = static_cast<int>(densityPercent * maxEdges / 100.0);
        std::cout << "[LOG] Generating graph with " << targetEdgeCount << " edges (target density).\n";

        Vektor<EdgeTriple> spanningTree = generateSpanningTree(vertexCount);
        Vektor<EdgeTriple> graph = spanningTree;
        Vektor<std::pair<int, int>> usedEdges;

        for (int i = 0; i < spanningTree.size(); ++i) {
            addEdgeRecord(usedEdges, spanningTree[i].from, spanningTree[i].to);
        }

        Vektor<EdgeTriple> allEdges = generateAllPossibleEdges(vertexCount);
        std::shuffle(&allEdges[0], &allEdges[0] + allEdges.size(), rng);

        for (int i = 0; i < allEdges.size() && graph.size() < targetEdgeCount; ++i) {
            int a = allEdges[i].from;
            int b = allEdges[i].to;
            if (edgeExists(usedEdges, a, b)) continue;

            graph.push_back(allEdges[i]);
            addEdgeRecord(usedEdges, a, b);
        }

        std::cout << "[LOG] Final graph has " << graph.size() << " edges.\n";
        return graph;
    }
};

#endif
