#ifndef ALGORITHMSUTILS_H
#define ALGORITHMSUTILS_H

#include "../VektorImpl/Vektor.hpp"

template <typename T>
class AlgorithmsUtils {
public:
    static bool isSorted(const Vektor<T>& data_to_sort) {
        const int n = data_to_sort.size();
        for (int i = 0; i < n - 1; i++) {
            if (data_to_sort[i] > data_to_sort[i + 1])
                return false;
        }
        return true;
    }

};

#endif
