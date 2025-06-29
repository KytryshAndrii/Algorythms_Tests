#ifndef KRUSKAL_LIST_HPP
#define KRUSKAL_LIST_HPP

#include <algorithm>
#include <iostream>

#include "../../GraphsImpl/GraphList.hpp"
#include "../../VektorImpl/Vektor.hpp"
#include "../../common/UnionJoinSet.hpp"
#include "../../SortAlgorithms/SortAlgorithms.hpp"

class KruskalList {
public:
    static Vektor<EdgeTriple> findMST(const GraphList* graph) {
        int vertexQuantity = graph->getVertexCount();
        Vektor<EdgeTriple> allEdges = extractUniqueEdges(graph, vertexQuantity);
        sortEdgesByWeight(allEdges);

        UnionJoinSet unionJoinSet(vertexQuantity);
        Vektor<EdgeTriple> mst;

        for (int i = 0; i < allEdges.size() && mst.size() < vertexQuantity - 1; ++i) {
            auto e = allEdges[i];
            if (unionJoinSet.find(e.from) != unionJoinSet.find(e.to)) {
                mst.push_back(e);
                unionJoinSet.unite(e.from, e.to);
            }
        }

        return mst;
    }

    static std::string calculateMSTWeightAsString(const Vektor<EdgeTriple>& mst) {
        int total = 0;
        for (int i = 0; i < mst.size(); ++i) {
            total += mst[i].weight;
        }
        return std::to_string(total);
    }

    static void displayMST(const Vektor<EdgeTriple>& mst) {
        int total = 0;
        std::cout << "MST edges (KruskalList):\n";
        for (int i = 0; i < mst.size(); ++i) {
            std::cout << mst[i].from << " -- " << mst[i].to << " [weight=" << mst[i].weight << "]\n";
            total += mst[i].weight;
        }
        std::cout << "Total MST weight: " << total << "\n";
    }

private:
    static Vektor<EdgeTriple> extractUniqueEdges(const GraphList* graph, int vertexQuantity) {
        Vektor<EdgeTriple> edges;
        Vektor<std::pair<int, int>> seenEdges;

        for (int u = 0; u < vertexQuantity; ++u) {
            const auto& neighbors = graph->getNeighbors(u);
            for (int i = 0; i < neighbors.size(); ++i) {
                int v = neighbors[i].dest;
                const int w = neighbors[i].weight;

                int a = std::min(u, v);
                int b = std::max(u, v);

                bool exists = false;
                for (int j = 0; j < seenEdges.size(); ++j) {
                    if (seenEdges[j].first == a && seenEdges[j].second == b) {
                        exists = true;
                        break;
                    }
                }

                if (!exists) {
                    edges.push_back({a, b, w});
                    seenEdges.push_back({a, b});
                }
            }
        }

        return edges;
    }

    static void sortEdgesByWeight(Vektor<EdgeTriple>& edges) {
        SortAlgorithms<EdgeTriple>::quickSort(edges);
    }
};

#endif
