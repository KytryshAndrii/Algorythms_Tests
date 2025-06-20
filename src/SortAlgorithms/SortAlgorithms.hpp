#ifndef SORTALGORITHMS_H
#define SORTALGORITHMS_H

#include "../VektorImpl/Vektor.hpp"
#include <random>

template <typename T>
class SortAlgorithms {
public:

    /**
     * Sorts the vector using simple insertion sort.
     * Runs in O(n^2) time on average, suitable for small datasets.
     */
    static void insertionSort(Vektor<T>& data_to_sort) {
        const int n = data_to_sort.size();
        for (int i = 1; i < n; i++) {
            T key = data_to_sort[i];
            int j = i - 1;
            while (j >= 0 && data_to_sort[j] > key) {
                data_to_sort[j + 1] = data_to_sort[j];
                j--;
            }
            data_to_sort[j + 1] = key;
        }
    }

    /**
     * Sorts the vector using binary insertion sort.
     * Uses binary search to locate insertion point, reducing comparisons.
     */
    static void binaryInsertionSort(Vektor<T>& data_to_sort) {
        const int n = data_to_sort.size();
        for (int i = 1; i < n; i++) {
            T key = data_to_sort[i];
            int left = 0, right = i - 1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (data_to_sort[mid] <= key)
                    left = mid + 1;
                else
                    right = mid - 1;
            }
            for (int j = i; j > left; j--) {
                data_to_sort[j] = data_to_sort[j - 1];
            }
            data_to_sort[left] = key;
        }
    }

    /**
     * Sorts the vector using heap sort.
     * Builds a max heap then repeatedly extracts the maximum element.
     */
    static void heapSort(Vektor<T>& data_to_sort) {
        const int n = data_to_sort.size();
        buildMaxHeap(data_to_sort);
        for (int i = n - 1; i >= 1; i--) {
            T temp = data_to_sort[0];
            data_to_sort[0] = data_to_sort[i];
            data_to_sort[i] = temp;
            maxHeapify(data_to_sort, 0, i);
        }
    }

    /**
     * Sorts the vector using randomized quick sort.
     * Average O(n log n), uses randomized pivot selection.
     */
    static void quickSort(Vektor<T>& data_to_sort) {
        if (data_to_sort.size() > 1)
            recurciveQuickSort(data_to_sort, 0, data_to_sort.size() - 1);
    }

private:
    /**
    * Ensures subtree rooted at index i satisfies max-heap property.
    */
    static void maxHeapify(Vektor<T>& data_to_sort, int i, int heapSize) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < heapSize && data_to_sort[left] > data_to_sort[largest])
            largest = left;
        if (right < heapSize && data_to_sort[right] > data_to_sort[largest])
            largest = right;
        if (largest != i) {
            T temp = data_to_sort[i];
            data_to_sort[i] = data_to_sort[largest];
            data_to_sort[largest] = temp;
            maxHeapify(data_to_sort, largest, heapSize);
        }
    }

    /**
     * Builds a max-heap from the unsorted vector.
     */
    static void buildMaxHeap(Vektor<T>& data_to_sort) {
        const int n = data_to_sort.size();
        for (int i = n / 2 - 1; i >= 0; i--)
            maxHeapify(data_to_sort, i, n);
    }

    /**
    * Partitions the array around a random pivot and returns pivot index.
    */
    static int partition(Vektor<T>& data_to_sort, int l, int r) {
        thread_local std::mt19937 gen{std::random_device{}()};
        std::uniform_int_distribution<int> dist(l, r);
        int pivotIndex = dist(gen);

        std::swap(data_to_sort[pivotIndex], data_to_sort[r]);
        T pivot = data_to_sort[r];

        int i = l - 1;
        for (int j = l; j < r; ++j) {
            if (data_to_sort[j] <= pivot) {
                ++i;
                std::swap(data_to_sort[i], data_to_sort[j]);
            }
        }
        std::swap(data_to_sort[i + 1], data_to_sort[r]);
        return i + 1;
    }

    /**
    * Recursively applies quick sort to subranges.
    */
    static void recurciveQuickSort(Vektor<T>& data_to_sort, int l, int r) {
        if (l < r) {
            int p = partition(data_to_sort, l, r);
            recurciveQuickSort(data_to_sort, l, p - 1);
            recurciveQuickSort(data_to_sort, p + 1, r);
        }
    }
};

#endif
