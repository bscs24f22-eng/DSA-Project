#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <fstream>
#include "Card.h"
#include "Stack.h"   // Your template Stack<T> class
#include "raylib.h"

// Foundation Pile Class
class Foundation {
public:
    Stack<Card> cards;
    int suit;   // Suit of the foundation (-1 when empty)

    Foundation() : cards(52), suit(-1) {}

    bool isEmpty() const {
        return cards.isEmpty();
    }

    int size() const {
        return cards.getSize();
    }

    Card getTopCard() const {
        return cards.peek();
    }

    bool canAddCard(const Card& c) {
        if (isEmpty())
            return c.getRank() == 1;   // Ace starts foundation

        Card top = getTopCard();
        return c.canPlaceOnFoundation(top);
    }

    void addCard(const Card& c) {
        if (isEmpty())
            suit = c.getSuit();
        cards.push(c);
    }

    void draw(int x, int y,
              Texture2D* cardImages,
              Texture2D cardBack) const
    {
        Rectangle r = {(float)x, (float)y, 70, 90};

        if (isEmpty()) {
            DrawRectangleRec(r, Color{25, 40, 25, 255});
            DrawRectangleLinesEx(r, 2, Color{50, 80, 50, 255});
            DrawText("A", x + 28, y + 35, 24, Color{80, 120, 80, 255});
        } else {
            Card top = getTopCard();
            top.draw(x, y, false, cardImages, cardBack);
        }
    }

    void save(std::ofstream& f) const {
        f << cards.getSize() << " ";
        for (int i = 0; i < cards.getSize(); i++) {
            Card c = cards.getAt(i);
            f << c.getRank() << " " << c.getSuit() << " ";
        }
    }

    void load(std::ifstream& f) {
        cards.clear();
        suit = -1;

        int cnt;
        f >> cnt;
        for (int i = 0; i < cnt; i++) {
            int r, s;
            f >> r >> s;
            addCard(Card(r, s));
        }
    }
};

#endif