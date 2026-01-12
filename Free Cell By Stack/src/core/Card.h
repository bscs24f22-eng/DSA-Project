#ifndef CARD_H
#define CARD_H

#include "raylib.h"

// Card Class
class Card {
private:
    int rank;  // 1=Ace, 2–10, 11=Jack, 12=Queen, 13=King
    int suit;  // 0=Club, 1=Diamond, 2=Heart, 3=Spade

public:
    Card() : rank(0), suit(0) {}
    Card(int r, int s) : rank(r), suit(s) {}

    int getRank() const { return rank; }
    int getSuit() const { return suit; }

    bool isRed() const {
        return suit == 1 || suit == 2;
    }

    bool isBlack() const {
        return suit == 0 || suit == 3;
    }

    // FreeCell tableau rule
    bool canPlaceOnTableau(const Card& other) const {
        return (rank == other.rank - 1) && (isRed() != other.isRed());
    }

    // Foundation rule
    bool canPlaceOnFoundation(const Card& other) const {
        return (suit == other.suit) && (rank == other.rank + 1);
    }

    void draw(int x, int y, bool selected,
              Texture2D* cardImages,
              Texture2D cardBack) const
    {
        Rectangle cardRect = {(float)x, (float)y, 70, 90};

        int imgIdx = suit * 13 + (rank - 1);
        if (imgIdx >= 0 && imgIdx < 52 && cardImages[imgIdx].id) {
            DrawTexturePro(
                cardImages[imgIdx],
                {0, 0,
                 (float)cardImages[imgIdx].width,
                 (float)cardImages[imgIdx].height},
                cardRect,
                {0, 0},
                0,
                WHITE
            );
        } else {
            DrawRectangleRec(cardRect, WHITE);
            DrawRectangleLinesEx(cardRect, 2, BLACK);
        }

        if (selected) {
            DrawRectangleLinesEx(cardRect, 3, YELLOW);
        }
    }
};

#endif