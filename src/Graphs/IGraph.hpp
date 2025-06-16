#ifndef IGRAPH_HPP
#define IGRAPH_HPP

class IGraph {
public:
    virtual void addEdge(int from, int to, int weight) = 0;
    virtual void display() const = 0;
    virtual int getVertexCount() const = 0;
    virtual int getEdgeCount() const = 0;

    virtual ~IGraph() = default;
};

#endif
