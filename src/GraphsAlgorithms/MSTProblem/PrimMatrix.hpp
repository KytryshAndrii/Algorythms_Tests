#ifndef PRIM_MATRIX_HPP
#define PRIM_MATRIX_HPP

#include "../../GraphsImpl/GraphMatrix.hpp"
#include "../../common/EdgeTripple.hpp"
#include "../../VektorImpl/Vektor.hpp"
#include <iostream>
#include <limits>

class PrimMatrix {
public:
    /**
     * Finds the Minimum Spanning Tree (MST) using Prim's algorithm on a graph represented as an incidence matrix.
     * Starts from vertex 0 and iteratively selects the smallest weight edge connecting to the MST.
     *
     * Time complexity: O(V * E), where V is the number of vertices and E is the number of edges.
     */
    static Vektor<EdgeTriple> findMST(const GraphMatrix* graph) {
        const int vertexQuantity = graph->getVertexCount();
        const int edgesQuantity = graph->getEdgeCount();

        Vektor<bool> inMST = initializeMSTFlags(vertexQuantity);
        Vektor<EdgeTriple> mst;
        inMST[0] = true;

        while (mst.size() < vertexQuantity - 1) {
            EdgeTriple bestEdge = selectMinimumEdge(graph, inMST, edgesQuantity);

            if (bestEdge.from != -1 && bestEdge.to != -1) {
                mst.push_back(bestEdge);
                inMST[bestEdge.to] = true;
            } else {
                break;
            }
        }

        return mst;
    }

    /**
   * Displays the edges in the MST and their total weight.
   */
    static void displayMST(const Vektor<EdgeTriple>& mst) {
        int total = 0;
        std::cout << "MST edges (PrimMatrix):\n";
        for (int i = 0; i < mst.size(); ++i) {
            std::cout << mst[i].from << " -- " << mst[i].to << " [w=" << mst[i].weight << "]\n";
            total += mst[i].weight;
        }
        std::cout << "Total MST weight: " << total << "\n";
    }

private:
    /**
    * Initializes the boolean flags for tracking inclusion of each vertex in the MST.
    *
    * Time complexity: O(V)
    */
    static Vektor<bool> initializeMSTFlags(const int vertexQuantity) {
        Vektor<bool> inMST;
        for (int i = 0; i < vertexQuantity; ++i)
            inMST.push_back(false);
        return inMST;
    }

    /**
    * Scans all edges to find the minimum weight edge connecting MST with non-MST vertex.
    * Works by checking both (u,v) and (v,u) since graph is undirected.
    *
    * Time complexity: O(E) per iteration => total O(V * E)
    */
    static EdgeTriple selectMinimumEdge(const GraphMatrix* graph, const Vektor<bool>& inMST, const int edgesQuantity) {
        int minWeight = std::numeric_limits<int>::max();
        EdgeTriple bestEdge = {-1, -1, -1};

        for (int i = 0; i < edgesQuantity; ++i) {
            const int u = graph->getFrom(i);
            const int v = graph->getDest(i);
            const int weight = graph->getWeight(i);

            if (inMST[u] && !inMST[v] && weight < minWeight) {
                bestEdge = {u, v, weight};
                minWeight = weight;
            } else if (inMST[v] && !inMST[u] && weight < minWeight) {
                bestEdge = {v, u, weight};
                minWeight = weight;
            }
        }

        return bestEdge;
    }
};

#endif
