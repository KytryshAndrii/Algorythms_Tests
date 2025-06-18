#ifndef GRAPH_FACTORY_HPP
#define GRAPH_FACTORY_HPP

#include "../GraphsImpl//GraphList.hpp"
#include "../GraphsImpl/GraphMatrix.hpp"
#include "GraphRepresentingShape.hpp"

inline IGraph* createGraphInstance(GraphRepresentingShape type, int vertices, int edges) {
    switch (type) {
        case LIST:
            return new GraphList(vertices);
        case MATRIX:
            return new GraphMatrix(vertices, edges);
        default:
            std::cerr << "[ERROR] Unknown graph type\n";
        return nullptr;
    }
}

#endif
