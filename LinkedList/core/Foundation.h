#pragma once
#include "Linklist.h"
#include "Card.h"

class Column {
private:
    LinkedList<Card> cards;

public:
    Column() {}

    void addCard(Card c) {
        cards.insertAtTail(c);
    }

    int size() const {
        return cards.size();
    }

    bool isEmpty() const {
        return cards.isEmpty();
    }

    Card getTopCard() {
        return cards.getTail();
    }

    void flipTopCard() {
        if (!isEmpty()) {
            Node<Card>* topNode = cards.getNodeAt(cards.size() - 1);
            if (topNode) {
                topNode->data.flip();
            }
        }
    }

    bool canPlaceCards(int startIdx) {
        if (startIdx < 0 || startIdx >= cards.size()) return false;

        for (int i = startIdx; i < cards.size() - 1; i++) {
            Node<Card>* curr = cards.getNodeAt(i);
            Node<Card>* next = cards.getNodeAt(i + 1);
            
            if (!curr || !next) return false;
            if (!curr->data.isFaceUp()) return false;
            if (!next->data.canPlaceOn(curr->data)) return false;
        }
        
        Node<Card>* last = cards.getNodeAt(cards.size() - 1);
        if (!last || !last->data.isFaceUp()) return false;
        
        return true;
    }

    bool checkCompleteSequence() {
        if (cards.size() < 13) return false;

        int idx = cards.size() - 13;
        Node<Card>* node = cards.getNodeAt(idx);
        
        if (!node || !node->data.isFaceUp() || node->data.getRank() != 13)
            return false;

        for (int i = 0; i < 12; i++) {
            Node<Card>* curr = cards.getNodeAt(idx + i);
            Node<Card>* next = cards.getNodeAt(idx + i + 1);
            
            if (!curr || !next) return false;
            if (!next->data.canPlaceOn(curr->data)) return false;
        }

        cards.removeRange(idx, cards.size() - 1);
        flipTopCard();
        return true;
    }

    LinkedList<Card> removeCards(int startIdx) {
        return cards.extractRange(startIdx, cards.size() - 1);
    }

    void removeCardsRange(int startIdx) {
        cards.removeRange(startIdx, cards.size() - 1);
    }

    void addCards(LinkedList<Card>& cardList) {
        cards.appendList(cardList);
    }

    void display(int colNum) const {
        cout << "Col " << colNum << ": ";
        
        if (cards.isEmpty()) {
            cout << "[Empty]";
        } else {
            for (int i = 0; i < cards.size(); i++) {
                Node<Card>* node = cards.getNodeAt(i);
                if (node) {
                    cout << node->data.toString() << " ";
                }
            }
        }
        cout << endl;
    }
};