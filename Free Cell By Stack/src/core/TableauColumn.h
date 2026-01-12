#ifndef TABLEAUCOLUMN_H
#define TABLEAUCOLUMN_H

#include <iostream>
#include "Stack.h"
#include "Card.h"

using namespace std;

class TableauColumn {
private:
    Stack<Card> cards;  // Stack of cards in this column

public:
    TableauColumn() : cards(13) {}  // Max 13 cards per column (initially empty)

    // Add a card on top
    void addCard(const Card &c) {
        cards.push(c);
    }

    // Remove the top card
    Card removeTop() {
        if (!cards.isEmpty()) return cards.pop();
        return Card(); // return default card if empty
    }

    // Get top card without removing
    Card getTop() const {
        if (!cards.isEmpty()) return cards.peek();
        return Card(); // default
    }

    // Get number of cards in this column
    int size() const {
        return cards.size();
    }

    // Check if column is empty
    bool isEmpty() const {
        return cards.isEmpty();
    }

    // Access a card at specific index (0 = bottom, size-1 = top)
    Card getCardAt(int index) const {
        if (index >= 0 && index < cards.size())
            return cards.getAt(index);
        return Card(); // default
    }

    // Move N cards from top and return as Stack
    Stack<Card> removeTopN(int n) {
        Stack<Card> temp(n);
        for (int i = 0; i < n && !cards.isEmpty(); i++)
            temp.push(cards.pop());
        return temp;
    }

    // Add multiple cards from a stack (in order)
    void addStack(Stack<Card> &stack) {
        int n = stack.size();
        for (int i = 0; i < n; i++) {
            cards.push(stack.getAt(i));
        }
    }

    // Save and load for file handling
    void save(ofstream &f) const {
        f << cards.size() << " ";
        for (int i = 0; i < cards.size(); i++) {
            Card c = cards.getAt(i);
            f << c.rank << " " << c.suit << " ";
        }
    }

    void load(ifstream &f) {
        int n; f >> n;
        cards.clear();
        for (int i = 0; i < n; i++) {
            int r, s; f >> r >> s;
            cards.push(Card(r, s));
        }
    }
};

#endif