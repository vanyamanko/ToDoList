#ifndef LIST_CPP
#define LIST_CPP

#include "List.h"
#include <QString>

template<typename T>
List<T>::List(int initialSize) : List() {
    resize(initialSize);
}

template<typename T>
List<T>::List() : head(nullptr), tail(nullptr), size(0) {}

template<typename T>
List<T>::~List() {
    clear();
}

template<typename T>
void List<T>::pushBack(const T& value) {
    Node* newNode = new Node(value);

    if (size == 0) {
        head = tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }

    size++;
}

template<typename T>
void List<T>::popBack() {
    if (size == 0) {
        try {
           throw Exception(1, "Cannot popBack(): List is empty");
        } catch (const Exception& e) {
            e.show();
            return;
        }
    } else if (size == 1) {
        delete tail;
        head = tail = nullptr;
    } else {
        Node* prevNode = tail->prev;
        prevNode->next = nullptr;
        delete tail;
        tail = prevNode;
    }

    size--;
}

template<typename T>
void List<T>::pushFront(const T& value) {
    Node* newNode = new Node(value);

    if (size == 0) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }

    size++;
}

template<typename T>
void List<T>::popFront() {
    if (size == 0) {
        try {
            throw Exception(2, "Cannot popFront(): List is empty");
        } catch (const Exception& e) {
            e.show();
            return;
        }
    } else if (size == 1) {
        delete head;
        head = tail = nullptr;
    } else {
        Node* nextNode = head->next;
        nextNode->prev = nullptr;
        delete head;
        head = nextNode;
    }

    size--;
}

template<typename T>
int List<T>::getSize() const {
    return size;
}

template<typename T>
T& List<T>::operator[](int index) {
    Node* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }

    return current->data;
}

template<typename T>
const T& List<T>::operator[](int index) const {

    Node* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }

    return current->data;
}

template<typename T>
void List<T>::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }

    head = tail = nullptr;
    size = 0;
}

template<typename T>
T* List<T>::data() {
    if (size == 0) {
        return nullptr;
    }

    T* dataArray = new T[size];
    Node* current = head;
    int index = 0;

    while (current != nullptr) {
        dataArray[index] = current->data;
        current = current->next;
        index++;
    }

    return dataArray;
}

template<typename T>
void List<T>::resize(int newSize) {
    if(newSize < 0)
    {
        try {
            throw Exception(3, "Cannot resize()");
        } catch (const Exception& e) {
            e.show();
            return;
        }
    }

    if (newSize == size) {
        return;
    }

    if (newSize < size) {
        while (size > newSize) {
            popBack();
        }
    } else {
        while (size < newSize) {
            pushBack(T());
        }
    }
}
#endif
