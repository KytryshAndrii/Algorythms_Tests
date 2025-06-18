#ifndef RANDOM_GRAPH_GENERATOR_HPP
#define RANDOM_GRAPH_GENERATOR_HPP

#include <random>
#include <utility>
#include <algorithm>
#include <ctime>

#include "../common/EdgeTripple.hpp"
#include "../VektorImpl/Vektor.hpp"
#include "../common/GraphFactory.hpp"
#include "../common/GraphRepresentingShape.hpp"

class RandomGraphGenerator {
    std::mt19937 rng;

public:
    RandomGraphGenerator() {
        rng.seed(static_cast<unsigned>(time(nullptr)));
        std::cout << "[LOG] RandomGraphGenerator initialized with seed from time.\n";
    }

    Vektor<EdgeTriple> generateGraph25(int v) {
        std::cout << "[LOG] Generating graph with 25% density and " << v << " vertices.\n";
        return generateGraphWithDensity(v, 25.0);
    }
    Vektor<EdgeTriple> generateGraph50(int v) {
        std::cout << "[LOG] Generating graph with 50% density and " << v << " vertices.\n";
        return generateGraphWithDensity(v, 50.0);
    }
    Vektor<EdgeTriple> generateGraph99(int v) {
         std::cout << "[LOG] Generating graph with 99% density and " << v << " vertices.\n";
        return generateGraphWithDensity(v, 99.0);
    }

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


private:
    int getRandomVertex(int n, int exclude = -1) {
        std::uniform_int_distribution<int> dist(0, n - 1);
        int v;
        do {
            v = dist(rng);
        } while (v == exclude);
        return v;
    }

    int getRandomWeight(int minW = 0, int maxW = 100) {
        std::uniform_int_distribution<int> dist(minW, maxW);
        return dist(rng);
    }

    bool edgeExists(const Vektor<std::pair<int, int>>& edgeList, int u, int v) {
        int a = std::min(u, v);
        int b = std::max(u, v);
        for (int i = 0; i < edgeList.size(); ++i) {
            if (edgeList[i].first == a && edgeList[i].second == b) {
                return true;
            }
        }
        return false;
    }

    void addEdgeRecord(Vektor<std::pair<int, int>>& edgeList, int u, int v) {
        edgeList.push_back({std::min(u, v), std::max(u, v)});
    }

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

    Vektor<EdgeTriple> generateAllPossibleEdges(int vertexCount) {
        Vektor<EdgeTriple> allEdges;
        for (int u = 0; u < vertexCount; ++u) {
            for (int v = u + 1; v < vertexCount; ++v) {
                allEdges.push_back({u, v, getRandomWeight()});
            }
        }
        return allEdges;
    }

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
