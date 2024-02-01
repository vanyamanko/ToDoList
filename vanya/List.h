#ifndef LIST_H
#define LIST_H
#include <iostream>
#include "Exception.h"
template<typename T>
class List {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& d, Node* n = nullptr, Node* p = nullptr) : data(d), next(n), prev(p) {}
    };

    class Iterator {
    Node* node;
    explicit Iterator(Node* n) : node(n) {}

    bool operator==(const Iterator& other) const {
        return node == other.node;
    }

    bool operator!=(const Iterator& other) const {
        return node != other.node;
    }

    Iterator& operator++() {
        node = node->next;
        return *this;
    }

    Iterator operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    Iterator& operator--() {
        node = node->prev;
        return *this;
    }

    Iterator operator--(int) {
        Iterator temp = *this;
        --(*this);
        return temp;
    }

    T& operator*() const {
        return node->data;
    }
};

Iterator begin() const {
    return Iterator(head);
}

Iterator end() const {
    return Iterator(nullptr);
}

    Node* head;
    Node* tail;
    int size;

public:
    explicit List(int initialSize);
    List();
    ~List();

    void pushBack(const T& value);
    void popBack();
    void pushFront(const T& value);
    void popFront();
    int getSize() const;

    T& operator[](int index);
    const T& operator[](int index) const;

    void clear();
    T* data();
    void resize(int newSize);
};

#include "List.cpp"

#endif
