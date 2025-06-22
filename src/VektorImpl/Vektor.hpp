#ifndef VEKTOR_H
#define VEKTOR_H

#include <stdexcept>
#include <algorithm>

template <typename T>
class Vektor {
public:
    Vektor();
    Vektor(int initialSize);
    Vektor(const Vektor<T>& other);
    Vektor<T>& operator=(const Vektor<T>& other);
    ~Vektor();

    void push_back(const T& value);
    void insertEnd(const T* first, const T* last);

    int size() const;
    bool empty() const;

    void removeAt(int index);
    T& operator[](int index);
    const T& operator[](int index) const;

    // Iterator support
    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;

private:
    T* data;
    int capacity;
    int count;

    void resize(int newCapacity);
};

// --- Implementation ---

template <typename T>
Vektor<T>::Vektor() : capacity(10), count(0) {
    data = new T[capacity];
}

template <typename T>
Vektor<T>::Vektor(int initialSize)
    : capacity(initialSize), count(initialSize) {
    data = new T[capacity];
    for (int i = 0; i < count; ++i) {
        data[i] = T();
    }
}

template <typename T>
Vektor<T>::Vektor(const Vektor<T>& other)
    : capacity(other.capacity), count(other.count) {
    data = new T[capacity];
    for (int i = 0; i < count; i++) {
        data[i] = other.data[i];
    }
}

template <typename T>
Vektor<T>& Vektor<T>::operator=(const Vektor<T>& other) {
    if (this != &other) {
        delete[] data;
        capacity = other.capacity;
        count = other.count;
        data = new T[capacity];
        for (int i = 0; i < count; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template <typename T>
Vektor<T>::~Vektor() {
    delete[] data;
}

template <typename T>
int Vektor<T>::size() const {
    return count;
}

template <typename T>
bool Vektor<T>::empty() const {
    return count == 0;
}

template <typename T>
T& Vektor<T>::operator[](int index) {
    if (index < 0 || index >= count)
        throw std::out_of_range("Index out of range");
    return data[index];
}

template <typename T>
const T& Vektor<T>::operator[](int index) const {
    if (index < 0 || index >= count)
        throw std::out_of_range("Index out of range");
    return data[index];
}

template <typename T>
void Vektor<T>::push_back(const T& value) {
    if (count >= capacity) {
        capacity++;
        resize(static_cast<int>(capacity * 1.5) + 1);
    }
    data[count++] = value;
}

template <typename T>
void Vektor<T>::resize(int newCapacity) {
    T* newData = new T[newCapacity];
    for (int i = 0; i < count; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
void Vektor<T>::removeAt(int index) {
    if (index < 0 || index >= count)
        throw std::out_of_range("Index out of range in removeAt");
    for (int i = index; i < count - 1; ++i) {
        data[i] = data[i + 1];
    }
    --count;
}

// --- Iterator methods ---

template <typename T>
T* Vektor<T>::begin() {
    return data;
}

template <typename T>
T* Vektor<T>::end() {
    return data + count;
}

template <typename T>
const T* Vektor<T>::begin() const {
    return data;
}

template <typename T>
const T* Vektor<T>::end() const {
    return data + count;
}

// --- Bulk insert ---

template <typename T>
void Vektor<T>::insertEnd(const T* first, const T* last) {
    int insertCount = static_cast<int>(last - first);
    if (count + insertCount > capacity)
        resize((count + insertCount) * 2);

    std::copy(first, last, data + count);
    count += insertCount;
}

#endif
