#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include "../VektorImpl/Vektor.hpp"
#include <random>
#include <type_traits>

#include "../SortAlgorithms/SortAlgorithms.hpp"

class RandomNumbersToSortGenerator {
public:

    /**
    * Generates a vector of random values of given length and range.
    * Uses uniform distribution: integer for integral types, real for floating types.
    */
    template <typename T>
    static Vektor<T> generateDataToSort(int dataLength, T minValue = T(0), T maxValue = T(1000)) {
        Vektor<T> dataVector;
        std::random_device rd;
        std::mt19937 generator(rd());
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<T> dist(minValue, maxValue);
            for (int idx = 0; idx < dataLength; ++idx) {
                dataVector.push_back(dist(generator));
            }
        } else {
            std::uniform_real_distribution<T> dist(minValue, maxValue);
            for (int idx = 0; idx < dataLength; ++idx) {
                dataVector.push_back(dist(generator));
            }
        }
        return dataVector;
    }

    /**
     * Generates data and returns it sorted in ascending order.
     * Internally generates random data and applies quicksort.
     */
    template <typename T>
    static Vektor<T> generateSortedData(int dataLength, T minValue = T(0), T maxValue = T(1000)) {
        Vektor<T> result = generateDataToSort<T>(dataLength, minValue, maxValue);
        SortAlgorithms<T>::quickSort(result);
        return result;
    }

    /**
    * Generates data and returns it sorted in descending order.
    * Generates random data, sorts ascending, then reverses the sequence.
    */
    template <typename T>
    static Vektor<T> generateReverseSortedData(int dataLength, T minValue = T(0), T maxValue = T(1000)) {
        Vektor<T> result = generateDataToSort<T>(dataLength, minValue, maxValue);
        SortAlgorithms<T>::quickSort(result);
        int n = result.size();
        for (int front = 0, back = n - 1; front < back; ++front, --back) {
            std::swap(result[front], result[back]);
        }
        return result;
    }

    /**
     * Generates data with a specified fraction of the prefix already sorted.
     * Sorts only the first sortedFraction * dataLength elements.
     */
    template <typename T>
    static Vektor<T> generatePartiallySortedData(int dataLength, T minValue, T maxValue, double sortedFraction) {
        Vektor<T> result = generateDataToSort<T>(dataLength, minValue, maxValue);
        int sortedCount = static_cast<int>(dataLength * sortedFraction);
        if (sortedCount > 0) {
            Vektor<T> prefix;
            for (int i = 0; i < sortedCount; ++i) {
                prefix.push_back(result[i]);
            }
            SortAlgorithms<T>::quickSort(prefix);
            for (int i = 0; i < sortedCount; ++i) {
                result[i] = prefix[i];
            }
        }
        return result;
    }

    /// Convenience: generates data with 33% of elements sorted.
    template <typename T>
    static Vektor<T> generate33PercentSortedData(int dataLength, T minValue = T(0), T maxValue = T(1000)) {
        return generatePartiallySortedData<T>(dataLength, minValue, maxValue, 0.33);
    }

    /// Convenience: generates data with 66% of elements sorted.
    template <typename T>
    static Vektor<T> generate66PercentSortedData(int dataLength, T minValue = T(0), T maxValue = T(1000)) {
        return generatePartiallySortedData<T>(dataLength, minValue, maxValue, 0.66);
    }
};

#endif
