#ifndef VEKTOR_H
#define VEKTOR_H

#include <stdexcept>

template <typename T>
class Vektor {
public:
    Vektor();
    Vektor(int initialSize);
    Vektor(const Vektor<T>& other);
    Vektor<T>& operator=(const Vektor<T>& other);
    ~Vektor();

    void push_back(const T& value);
    int size() const;
    T& operator[](int index);
    const T& operator[](int index) const;

private:
    T* data;
    int capacity;
    int count;
    void resize(int newCapacity);
};

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
}

#endif
