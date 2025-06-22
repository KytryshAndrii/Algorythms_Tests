#ifndef KRUSKAL_MATRIX_HPP
#define KRUSKAL_MATRIX_HPP

#include <iostream>

#include "../../common/EdgeTriple.hpp"
#include "../../GraphsImpl/GraphMatrix.hpp"
#include "../../VektorImpl/Vektor.hpp"
#include "../../common/UnionJoinSet.hpp"
#include "../../SortAlgorithms/SortAlgorithms.hpp"

class KruskalMatrix {
public:
    static Vektor<EdgeTriple> findMST(const GraphMatrix* graph) {
        const int vertexQuantity = graph->getVertexCount();
        Vektor<EdgeTriple> allEdges = extractAllEdges(graph);
        sortEdgesByWeight(allEdges);

        UnionJoinSet unionJoinSet(vertexQuantity);
        Vektor<EdgeTriple> mst;

        for (int i = 0; i < allEdges.size() && mst.size() < vertexQuantity - 1; ++i) {
            const auto& edge = allEdges[i];
            if (unionJoinSet.find(edge.from) != unionJoinSet.find(edge.to)) {
                mst.push_back(edge);
                unionJoinSet.unite(edge.from, edge.to);
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
        std::cout << "MST edges:\n";
        for (int i = 0; i < mst.size(); ++i) {
            std::cout << mst[i].from << " -- " << mst[i].to << " [w=" << mst[i].weight << "]\n";
            total += mst[i].weight;
        }
        std::cout << "Total MST weight: " << total << "\n";
    }

private:
    static Vektor<EdgeTriple> extractAllEdges(const GraphMatrix* graph) {
        const int edgeQuantity = graph->getEdgeCount();
        Vektor<EdgeTriple> edges;
        Vektor<std::pair<int, int>> seenPairs;

        for (int i = 0; i < edgeQuantity; ++i) {
            int u = graph->getFrom(i);
            int v = graph->getDest(i);
            int w = graph->getWeight(i);

            int a = std::min(u, v);
            int b = std::max(u, v);

            bool exists = false;
            for (int j = 0; j < seenPairs.size(); ++j) {
                if (seenPairs[j].first == a && seenPairs[j].second == b) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                edges.push_back({a, b, w});
                seenPairs.push_back({a, b});
            }
        }

        return edges;
    }


    static void sortEdgesByWeight(Vektor<EdgeTriple>& edges) {
        SortAlgorithms<EdgeTriple>::quickSort(edges);
    }
};

#endif
