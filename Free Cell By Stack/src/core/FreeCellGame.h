#ifndef FREECELLGAME_H
#define FREECELLGAME_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include "raylib.h"

#include "Card.h"
#include "Stack.h"
#include "FreeCell.h"
#include "Foundation.h"
#include "TableauColumn.h"

using namespace std;

class FreeCellGame {
private:
    TableauColumn tableau[8];
    FreeCell freeCells[4];
    Foundation foundations[4];

    int moves;
    int score;
    int selCol, selCard;
    bool gameWon;
    string msg;
    float msgTimer;
    Texture2D cardImg[52];
    Texture2D cardBack;
    Texture2D bg;

    int selType; // 0=tableau, 1=freecell
    int selIndex;

    float gameTime;

    // Move history for undo
    struct Move {
        int fromType, fromIndex;
        int toType, toIndex;
        int numCards;
        Stack<Card> movedCards;
        Move() : fromType(-1), fromIndex(-1), toType(-1), toIndex(-1), numCards(0), movedCards(52) {}
    };
    Stack<Move> moveHistory;

    // Internal functions
    void createDeck() {
        Stack<Card> all(52);
        for (int suit = 0; suit < 4; suit++)
            for (int rank = 1; rank <= 13; rank++)
                all.push(Card(rank, suit));

        // Shuffle deck
        Card* tempArr = new Card[52];
        for (int i = 0; i < 52; i++) tempArr[i] = all.getAt(i);

        for (int i = 51; i > 0; i--) {
            int j = rand() % (i+1);
            swap(tempArr[i], tempArr[j]);
        }

        all.clear();
        for (int i = 0; i < 52; i++) all.push(tempArr[i]);
        delete[] tempArr;

        int cardIdx = 0;
        for (int c = 0; c < 8; c++) {
            int numCards = (c < 4) ? 7 : 6;
            for (int i = 0; i < numCards; i++) {
                tableau[c].addCard(all.getAt(cardIdx++));
            }
        }
    }

    int countEmptyFreeCells() const {
        int cnt = 0;
        for (int i = 0; i < 4; i++) if (freeCells[i].isEmpty()) cnt++;
        return cnt;
    }

    int countEmptyColumns() const {
        int cnt = 0;
        for (int i = 0; i < 8; i++) if (tableau[i].isEmpty()) cnt++;
        return cnt;
    }

    void calculateScore() {
        score = 0;
        for (int i = 0; i < 4; i++) score += foundations[i].size() * 10;
        for (int i = 0; i < 4; i++) if (foundations[i].size() == 13) score += 100;
        score -= moves * 2;
        if (score < 0) score = 0;
    }

public:
    FreeCellGame() : moves(0), score(0), selCol(-1), selCard(-1),
                     selType(-1), selIndex(-1), gameWon(false), msgTimer(0),
                     gameTime(0), moveHistory(1000) {
        srand(time(0));
    }

    void loadImages() {
        string suits[] = {"Club","Diamond","Heart","Spade"};
        string ranks[] = {"Ace","2","3","4","5","6","7","8","9","10","J","Q","K"};
        for (int s=0; s<4; s++)
            for (int r=0; r<13; r++) {
                int idx = s*13+r;
                string filename = "Deck Images/" + suits[s] + " " + ranks[r] + ".jpeg";
                Image img = LoadImage(filename.c_str());
                if (img.data) {
                    cardImg[idx] = LoadTextureFromImage(img);
                    UnloadImage(img);
                } else cardImg[idx].id = 0;
            }
        Image back = LoadImage("Deck Images/card_back.jpeg");
        if (!back.data) back = GenImageColor(70,90,DARKBLUE);
        cardBack = LoadTextureFromImage(back);
        UnloadImage(back);

        Image bgImg = LoadImage("Deck Images/background.jpeg");
        if (!bgImg.data) bgImg = GenImageColor(1020,750,Color{34,139,34,255});
        bg = LoadTextureFromImage(bgImg);
        UnloadImage(bgImg);
    }

    void unloadImages() {
        for (int i=0;i<52;i++) if (cardImg[i].id) UnloadTexture(cardImg[i]);
        UnloadTexture(cardBack);
        UnloadTexture(bg);
    }

    void newGame() {
        for (int i=0;i<8;i++) tableau[i] = TableauColumn();
        for (int i=0;i<4;i++) { freeCells[i] = FreeCell(); foundations[i] = Foundation(); }
        moves = 0; score = 0; selCol = selCard = -1; selType = selIndex = -1;
        gameWon = false; gameTime = 0; moveHistory.clear();
        createDeck();
        msg = "New game started!";
        msgTimer = 3;
    }

    bool save() {
        ofstream f("freecell_save.dat");
        if (!f) return false;
        f << moves << " " << score << " " << gameTime << " " << gameWon << endl;
        for (int i=0;i<8;i++){ tableau[i].save(f); f << endl; }
        for (int i=0;i<4;i++){ freeCells[i].save(f); f << endl; }
        for (int i=0;i<4;i++){ foundations[i].save(f); f << endl; }
        f.close();
        return true;
    }

    bool load() {
        ifstream f("freecell_save.dat"); if (!f) return false;
        for (int i=0;i<8;i++) tableau[i] = TableauColumn();
        for (int i=0;i<4;i++) { freeCells[i] = FreeCell(); foundations[i] = Foundation(); }
        f >> moves >> score >> gameTime >> gameWon;
        for (int i=0;i<8;i++) tableau[i].load(f);
        for (int i=0;i<4;i++) freeCells[i].load(f);
        for (int i=0;i<4;i++) foundations[i].load(f);
        f.close();
        selCol = selCard = -1; selType = selIndex = -1;
        msg = "Game loaded successfully!";
        msgTimer = 2;
        return true;
    }

    bool isWon() const { return gameWon; }

    // For brevity, handleInput(), update(), draw(), checkWin(), undoMove() 
    // should be implemented in a similar modular fashion as your main code.
};

#endif