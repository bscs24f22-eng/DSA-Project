#ifndef TEAMMATE1_DS_H
#define TEAMMATE1_DS_H

#include <iostream>
#include <limits>
using namespace std;

/* ================= NODE ================= */
template <typename T>
class Node
{
public:
    T data;
    Node* next;
    Node(T val) : data(val), next(nullptr) {}
};

/* ================= QUEUE ================= */
template <typename T>
class Queue
{
private:
    Node<T>* frontNode;
    Node<T>* rearNode;
    int count;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

    ~Queue()
    {
        while (!isEmpty())
            dequeue();
    }

    void enqueue(T val)
    {
        Node<T>* newNode = new Node<T>(val);
        if (isEmpty())
            frontNode = rearNode = newNode;
        else
        {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    T dequeue()
    {
        if (isEmpty())
        {
            cout << "Queue is empty!" << endl;
            return T();
        }

        Node<T>* temp = frontNode;
        T val = temp->data;
        frontNode = frontNode->next;

        if (!frontNode)
            rearNode = nullptr;

        delete temp;
        count--;
        return val;
    }

    T front() const
    {
        if (isEmpty())
            return T();
        return frontNode->data;
    }

    bool isEmpty() const
    {
        return frontNode == nullptr;
    }

    int size() const
    {
        return count;
    }
};

/* ================= CARD ================= */
struct Card
{
    char rank;
    char suit;

    Card() : rank('0'), suit('0') {}
    Card(char r, char s) : rank(r), suit(s) {}

    bool isEmpty() const
    {
        return rank == '0';
    }

    int getRankValue() const
    {
        if (rank == 'A') return 1;
        if (rank >= '2' && rank <= '9') return rank - '0';
        if (rank == 'T') return 10;
        if (rank == 'J') return 11;
        if (rank == 'Q') return 12;
        if (rank == 'K') return 13;
        return 0;
    }

    bool isRed() const
    {
        return suit == 'H' || suit == 'D';
    }

    void display() const
    {
        if (isEmpty())
            cout << "[  ]";
        else
            cout << "[" << rank << suit << "]";
    }
};

#endif
