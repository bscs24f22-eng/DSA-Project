#ifndef FREECELL_H
#define FREECELL_H

#include <fstream>
#include "Card.h"
#include "Stack.h"   // Your template Stack<T> class
#include "raylib.h"

class FreeCell {
private:
    Card card;
    bool occupied;

public:
    FreeCell() : occupied(false) {}

    // Check if the free cell is empty
    bool isEmpty() const {
        return !occupied;
    }

    // Get the card in the cell
    Card getCard() const {
        return card;
    }

    // Place a card into the cell
    void addCard(const Card& c) {
        card = c;
        occupied = true;
    }

    // Remove and return the card from the cell
    Card removeCard() {
        Card c = card;
        occupied = false;
        return c;
    }

    // Draw the free cell
    void draw(int x, int y, Texture2D* cardImages, Texture2D cardBack) const {
        Rectangle r = {(float)x, (float)y, 70, 90};

        if (isEmpty()) {
            DrawRectangleRec(r, Color{30, 35, 25, 255});
            DrawRectangleLinesEx(r, 2, Color{60, 70, 50, 255});
        } else {
            card.draw(x, y, false, cardImages, cardBack);
        }
    }

    // Save the free cell to file
    void save(std::ofstream& f) const {
        f << occupied << " ";
        if (occupied)
            f << card.getRank() << " " << card.getSuit() << " ";
    }

    // Load the free cell from file
    void load(std::ifstream& f) {
        f >> occupied;
        if (occupied) {
            int r, s;
            f >> r >> s;
            card = Card(r, s);
        }
    }
};

#endif