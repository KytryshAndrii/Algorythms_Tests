#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include "../VektorImpl/Vektor.hpp"

template <typename T>
class MinHeap {
    Vektor<T> heap;

public:
    MinHeap() {}

    bool empty() const {
        return heap.size() == 0;
    }

    int size() const {
        return heap.size();
    }

    const T& top() const {
        return heap[0];
    }

    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    void pop() {
        if (empty()) return;
        heap[0] = heap[heap.size() - 1];
        heap.removeAt(heap.size() - 1);
        heapifyDown(0);
    }

private:
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] < heap[parent]) {
                T tmp = heap[index];
                heap[index] = heap[parent];
                heap[parent] = tmp;
                index = parent;
            } else break;
        }
    }

    void heapifyDown(int index) {
        int n = heap.size();
        while (true) {
            int smallest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < n && heap[left] < heap[smallest]) smallest = left;
            if (right < n && heap[right] < heap[smallest]) smallest = right;

            if (smallest == index) break;

            T tmp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = tmp;
            index = smallest;
        }
    }
};

#endif
