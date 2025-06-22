#ifndef PRIM_LIST_HPP
#define PRIM_LIST_HPP

#include "../../GraphsImpl/GraphList.hpp"
#include "../../common/EdgeTriple.hpp"
#include "../../VektorImpl/Vektor.hpp"
#include <iostream>
#include <limits>

class PrimList {
public:
    /**
    * Finds the Minimum Spanning Tree (MST) using Prim's algorithm on a graph represented as an adjacency list.
    * Time complexity: O(V^2) without priority queue.
    */
    static Vektor<EdgeTriple> findMST(const GraphList* graph) {
        const int vertexQuantity = graph->getVertexCount();

        Vektor<bool> inMST = initializeMSTFlags(vertexQuantity);
        Vektor<EdgeTriple> mst;
        inMST[0] = true;

        while (mst.size() < vertexQuantity - 1) {
            EdgeTriple bestEdge = selectMinimumEdge(graph, inMST, vertexQuantity);

            if (bestEdge.from != -1 && bestEdge.to != -1) {
                mst.push_back(bestEdge);
                inMST[bestEdge.to] = true;
            } else {
                std::cerr << "[PRIM] Nie znaleziono kolejnej krawędzi — graf nie jest spójny?\n";
                break;
            }
        }

        return mst;
    }

    static std::string calculateMSTWeightAsString(const Vektor<EdgeTriple>& mst) {
        int totalWeight = 0;
        for (int i = 0; i < mst.size(); ++i) {
            totalWeight += mst[i].weight;
        }
        return std::to_string(totalWeight);
    }

    /**
    * Displays the MST edges and total weight.
    */
    static void displayMST(const Vektor<EdgeTriple>& mst) {
        int total = 0;
        std::cout << "MST edges (PrimList):\n";
        for (int i = 0; i < mst.size(); ++i) {
            std::cout << mst[i].from << " -- " << mst[i].to << " [w=" << mst[i].weight << "]\n";
            total += mst[i].weight;
        }
        std::cout << "Total MST weight: " << total << "\n";
    }

private:
    /**
     * Initializes a boolean vector to mark visited vertices in MST.
     */
    static Vektor<bool> initializeMSTFlags(const int vertexQuantity) {
        Vektor<bool> inMST;
        for (int i = 0; i < vertexQuantity; ++i)
            inMST.push_back(false);
        return inMST;
    }

    /**
     * Selects the minimum-weight edge connecting MST to a new vertex.
     * Works by checking both (u,v) and (v,u) since graph is undirected.
     * Time complexity: O(V^2)
     */
    static EdgeTriple selectMinimumEdge(const GraphList* graph, const Vektor<bool>& inMST, const int vertexQuantity) {
        int minWeight = std::numeric_limits<int>::max();
        EdgeTriple bestEdge = {-1, -1, -1};

        Vektor<std::pair<int, int>> seenPairs;

        for (int u = 0; u < vertexQuantity; ++u) {
            if (!inMST[u]) continue;

            const Vektor<Edge>& neighbors = graph->getNeighbors(u);
            for (int i = 0; i < neighbors.size(); ++i) {
                int v = neighbors[i].dest;
                int weight = neighbors[i].weight;

                if (v == u) continue; // pomijaj pętle

                if (!inMST[v] && weight < minWeight) {
                    bestEdge = {u, v, weight};
                    minWeight = weight;
                }
            }
            for (int v = 0; v < vertexQuantity; ++v) {
                if (v == u || inMST[v]) continue;

                const Vektor<Edge>& incoming = graph->getNeighbors(v);
                for (int j = 0; j < incoming.size(); ++j) {
                    if (incoming[j].dest == u) {
                        int weight = incoming[j].weight;
                        if (weight < minWeight) {
                            bestEdge = {u, v, weight};  // from = u, to = v
                            minWeight = weight;
                        }
                    }
                }
            }
        }
        return bestEdge;
    }
};

#endif
