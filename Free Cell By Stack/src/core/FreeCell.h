#pragma once
#include <iostream>
using namespace std;

template <class T>
class Node {
public:
    T data;
    Node<T>* next;
    Node<T>* prev;

    Node(T d) {
        data = d;
        next = nullptr;
        prev = nullptr;
    }
};

template <class T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int count;

public:
    LinkedList() {
        head = tail = nullptr;
        count = 0;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    int size() const {
        return count;
    }

    Node<T>* getNodeAt(int index) const {
        if (index < 0 || index >= count) return nullptr;
        Node<T>* temp = head;
        for (int i = 0; i < index; i++)
            temp = temp->next;
        return temp;
    }

    void insertAtBeginning(T value) {
        Node<T>* newNode = new Node<T>(value);
        if (isEmpty()) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    void insertAtTail(T value) {
        Node<T>* newNode = new Node<T>(value);
        if (isEmpty()) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    void deleteFromBeginning() {
        if (isEmpty()) return;
        Node<T>* temp = head;

        if (head == tail) {
            head = tail = nullptr;
        }
        else {
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
        count--;
    }

    void deleteFromTail() {
        if (isEmpty()) return;
        Node<T>* temp = tail;

        if (head == tail) {
            head = tail = nullptr;
        }
        else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete temp;
        count--;
    }

    void removeRange(int start, int end) {
        if (start < 0 || end >= count || start > end) return;

        Node<T>* startNode = getNodeAt(start);
        Node<T>* endNode = getNodeAt(end);

        if (startNode->prev)
            startNode->prev->next = endNode->next;
        else
            head = endNode->next;

        if (endNode->next)
            endNode->next->prev = startNode->prev;
        else
            tail = startNode->prev;

        Node<T>* curr = startNode;
        for (int i = start; i <= end; i++) {
            Node<T>* next = curr->next;
            delete curr;
            curr = next;
            count--;
        }
    }

    LinkedList<T> extractRange(int start, int end) {
        LinkedList<T> newList;
        if (start < 0 || end >= count || start > end) return newList;

        Node<T>* startNode = getNodeAt(start);
        Node<T>* endNode = getNodeAt(end);

        for (Node<T>* curr = startNode; curr != endNode->next; curr = curr->next)
            newList.insertAtTail(curr->data);

        return newList;
    }

    void appendList(const LinkedList<T>& other) {
        Node<T>* curr = other.head;
        while (curr != nullptr) {
            insertAtTail(curr->data);
            curr = curr->next;
        }
    }

    T getFront() const {
        if (isEmpty()) return T();
        return head->data;
    }

    T getTail() const {
        if (isEmpty()) return T();
        return tail->data;
    }

    void printForward() const {
        Node<T>* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    ~LinkedList() {
        while (!isEmpty())
            deleteFromBeginning();
    }
};