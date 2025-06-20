#ifndef UNION_JOIN_SET_HPP
#define UNION_JOIN_SET_HPP

#include "../VektorImpl/Vektor.hpp"

class UnionJoinSet {
    Vektor<int> parent;

public:
    explicit UnionJoinSet(int n) : parent(n) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int u) {
        if (parent[u] != u) parent[u] = find(parent[u]);
        return parent[u];
    }

    void unite(int u, int v) {
        parent[find(u)] = find(v);
    }
};

#endif
