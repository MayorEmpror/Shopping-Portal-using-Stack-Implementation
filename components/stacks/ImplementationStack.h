#ifndef STACKS_H
#define STACKS_H

#include <iostream>
#include <stdexcept>  // for std::out_of_range
#include <cstddef>    // for size_t

// ===============================
//  ARRAY-BASED STACK (C++03)
// ===============================
template <typename T>
class ArrayStack {
private:
    T* data;
    std::size_t size;
    std::size_t capacity;

    void grow(std::size_t newCap) {
        if (newCap <= capacity) return;
        if (newCap < capacity * 2) newCap = capacity * 2;
        T* newData = new T[newCap];
        for (std::size_t i = 0; i < size; ++i)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCap;
    }

public:
    ArrayStack()
        : data(NULL), size(0), capacity(0) {}

    explicit ArrayStack(std::size_t cap)
        : data(new T[cap]), size(0), capacity(cap) {}

    ArrayStack(const ArrayStack& other)
        : data(new T[other.capacity]), size(other.size), capacity(other.capacity) {
        for (std::size_t i = 0; i < size; ++i)
            data[i] = other.data[i];
    }

    ArrayStack& operator=(const ArrayStack& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            data = new T[capacity];
            for (std::size_t i = 0; i < size; ++i)
                data[i] = other.data[i];
        }
        return *this;
    }

    ~ArrayStack() {
        delete[] data;
    }

    void push(const T& value) {
        if (size == capacity)
            grow(capacity == 0 ? 4 : capacity * 2);
        data[size++] = value;
    }

    void pop() {
        if (empty())
            throw std::out_of_range("ArrayStack::pop(): empty stack");
        --size;
    }

    T top() const {
        if (empty())
            throw std::out_of_range("ArrayStack::top(): empty stack");
        return data[size - 1];
    }

    bool empty() const { return size == 0; }
    std::size_t getSize() const { return size; }

    void clear() { size = 0; }
};


// ===============================
//  LINKED-LIST-BASED STACK (C++03)
// ===============================
template <typename T>
class LinkedStack {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v, Node* n = NULL) : value(v), next(n) {}
    };

    Node* head;
    std::size_t size;

    void copyFrom(const LinkedStack& other) {
        if (other.head == NULL) {
            head = NULL;
            size = 0;
            return;
        }
        // Copy nodes preserving order
        Node* src = other.head;
        Node* tail = new Node(src->value);
        head = tail;
        src = src->next;
        while (src != NULL) {
            tail->next = new Node(src->value);
            tail = tail->next;
            src = src->next;
        }
        size = other.size;
    }

public:
    LinkedStack() : head(NULL), size(0) {}

    LinkedStack(const LinkedStack& other) : head(NULL), size(0) {
        copyFrom(other);
    }

    LinkedStack& operator=(const LinkedStack& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    ~LinkedStack() {
        clear();
    }

    void push(const T& value) {
        head = new Node(value, head);
        ++size;
    }

    void pop() {
        if (empty())
            throw std::out_of_range("LinkedStack::pop(): empty stack");
        Node* temp = head;
        head = head->next;
        delete temp;
        --size;
    }

    T top() const {
        if (empty())
            throw std::out_of_range("LinkedStack::top(): empty stack");
        return head->value;
    }

    bool empty() const { return size == 0; }
    std::size_t getSize() const { return size; }

    void clear() {
        while (head != NULL) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }
};

#endif // STACKS_H
