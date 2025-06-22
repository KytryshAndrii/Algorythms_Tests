#ifndef EDGETRIPPLE_HPP
#define EDGETRIPPLE_HPP

struct EdgeTriple {
    int from;
    int to;
    int weight;

    bool operator<=(const EdgeTriple& other) const {
        return weight <= other.weight;
    }
};

#endif
