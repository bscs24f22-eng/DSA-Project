#include "raylib.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>
using namespace std;

template <class T>
class Stack {
public:
    int size, top;
    T *ptr;

    Stack(int a = 100) {
        ptr = new T[a];
        top = -1;
        size = a;
    }

    Stack(const Stack& other) {
        size = other.size;
        top = other.top;
        ptr = new T[size];
        for (int i = 0; i <= top; i++) {
            ptr[i] = other.ptr[i];
        }
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            delete[] ptr;
            size = other.size;
            top = other.top;
            ptr = new T[size];
            for (int i = 0; i <= top; i++) {
                ptr[i] = other.ptr[i];
            }
        }
        return *this;
    }

    bool isEmpty() const {
        return top == -1;
    }

    bool isFull() const {
        return top == size - 1;
    }

    T peek() const {
        if (!isEmpty())
            return ptr[top];
        return T();
    }

    int getSize() const {
        return top + 1;
    }

    T getAt(int index) const {
        if (index >= 0 && index <= top)
            return ptr[index];
        return T();
    }

    bool pop(T &p) {
        if (!isEmpty()) {
            p = ptr[top];
            top--;
            return true;
        }
        return false;
    }

    bool push(T n) {
        if (!isFull()) {
            ptr[++top] = n;
            return true;
        }
        return false;
    }

    void clear() {
        top = -1;
    }

    ~Stack() {
        delete[] ptr;
    }
};

//Card Class
class Card {
private:
    int rank;  // 1=Ace, 2-10, 11=Jack, 12=Queen, 13=King
    int suit;  // 0=Club, 1=Diamond, 2=Heart, 3=Spade
public:
    Card() : rank(0), suit(0) {}
    Card(int r, int s) : rank(r), suit(s) {}

    int getRank() const { return rank; }
    int getSuit() const { return suit; }
    bool isRed() const { return suit == 1 || suit == 2; }
    bool isBlack() const { return suit == 0 || suit == 3; }

    bool canPlaceOnTableau(const Card& other) const {
        return (rank == other.rank - 1) && (isRed() != other.isRed());
    }

    bool canPlaceOnFoundation(const Card& other) const {
        return (suit == other.suit) && (rank == other.rank + 1);
    }

    void draw(int x, int y, bool selected, Texture2D* cardImages, Texture2D cardBack) const {
        Rectangle cardRect = {(float)x, (float)y, 70, 90};

        int imgIdx = suit * 13 + (rank - 1);
        if (imgIdx >= 0 && imgIdx < 52 && cardImages[imgIdx].id) {
            DrawTexturePro(cardImages[imgIdx],
                          {0, 0, (float)cardImages[imgIdx].width, (float)cardImages[imgIdx].height},
                          cardRect, {0, 0}, 0, WHITE);
        } else {
            DrawRectangleRec(cardRect, WHITE);
            DrawRectangleLinesEx(cardRect, 2, BLACK);
        }

        if (selected) DrawRectangleLinesEx(cardRect, 3, YELLOW);
    }
};

//  Foundation Pile
class Foundation {
public:
    Stack<Card> cards;
    int suit;

    Foundation() : cards(52), suit(-1) {}

    bool isEmpty() const { return cards.isEmpty(); }
    int size() const { return cards.getSize(); }
    Card getTopCard() const { return cards.peek(); }

    bool canAddCard(const Card& c) {
        if (isEmpty()) return c.getRank() == 1;
        Card top = getTopCard();
        return c.canPlaceOnFoundation(top);
    }

    void addCard(const Card& c) {
        if (isEmpty()) suit = c.getSuit();
        cards.push(c);
    }

    void draw(int x, int y, Texture2D* cardImages, Texture2D cardBack) const {
        Rectangle r = {(float)x, (float)y, 70, 90};

        if (isEmpty()) {
            DrawRectangleRec(r, Color{25, 40, 25, 255});
            DrawRectangleLinesEx(r, 2, Color{50, 80, 50, 255});
            DrawText("A", x+28, y+35, 24, Color{80, 120, 80, 255});
        } else {
            Card top = getTopCard();
            top.draw(x, y, false, cardImages, cardBack);
        }
    }

    void save(ofstream& f) const {
        f << cards.getSize() << " ";
        for (int i = 0; i < cards.getSize(); i++) {
            Card c = cards.getAt(i);
            f << c.getRank() << " " << c.getSuit() << " ";
        }
    }

    void load(ifstream& f) {
        cards.clear();
        int cnt; f >> cnt;
        for (int i = 0; i < cnt; i++) {
            int r, s;
            f >> r >> s;
            cards.push(Card(r, s));
        }
    }
};

//  Free Cell
class FreeCell {
private:
    Card card;
    bool occupied;
public:
    FreeCell() : occupied(false) {}

    bool isEmpty() const { return !occupied; }
    Card getCard() const { return card; }

    void addCard(const Card& c) {
        card = c;
        occupied = true;
    }

    Card removeCard() {
        Card c = card;
        occupied = false;
        return c;
    }

    void draw(int x, int y, Texture2D* cardImages, Texture2D cardBack) const {
        Rectangle r = {(float)x, (float)y, 70, 90};

        if (isEmpty()) {
            DrawRectangleRec(r, Color{30, 35, 25, 255});
            DrawRectangleLinesEx(r, 2, Color{60, 70, 50, 255});
        } else {
            card.draw(x, y, false, cardImages, cardBack);
        }
    }

    void save(ofstream& f) const {
        f << occupied << " ";
        if (occupied) f << card.getRank() << " " << card.getSuit() << " ";
    }

    void load(ifstream& f) {
        f >> occupied;
        if (occupied) {
            int r, s;
            f >> r >> s;
            card = Card(r, s);
        }
    }
};

// Tableau Column
class TableauColumn {
private:
    Stack<Card> cards;
public:
    TableauColumn() : cards(52) {}

    void addCard(Card c) { cards.push(c); }
    int size() const { return cards.getSize(); }
    bool isEmpty() const { return cards.isEmpty(); }
    Card getTopCard() const { return cards.peek(); }
    Card getCardAt(int idx) const { return cards.getAt(idx); }

    int getMaxMovableSequence(int emptyFreeCells, int emptyColumns) const {
        if (isEmpty()) return 0;

        int maxMove = (emptyFreeCells + 1) * (1 << emptyColumns);

        int validSeq = 1;
        for (int i = cards.getSize() - 1; i > 0; i--) {
            Card curr = cards.getAt(i);
            Card prev = cards.getAt(i - 1);

            if (curr.canPlaceOnTableau(prev)) {
                validSeq++;
            } else {
                break;
            }
        }

        return min(validSeq, maxMove);
    }

    bool isValidSequenceFrom(int startIdx) const {
        if (startIdx < 0 || startIdx >= cards.getSize()) return false;

        for (int i = startIdx; i < cards.getSize() - 1; i++) {
            Card curr = cards.getAt(i);
            Card next = cards.getAt(i + 1);
            if (!next.canPlaceOnTableau(curr)) return false;
        }
        return true;
    }

    Stack<Card> removeCards(int startIdx) {
        Stack<Card> temp(52);
        for (int i = startIdx; i < cards.getSize(); i++) {
            temp.push(cards.getAt(i));
        }

        int numToRemove = cards.getSize() - startIdx;
        for (int i = 0; i < numToRemove; i++) {
            Card dummy;
            cards.pop(dummy);
        }

        return temp;
    }

    void removeTopCard() {
        Card dummy;
        cards.pop(dummy);
    }

    void addCards(Stack<Card>& cardStack) {
        Stack<Card> temp(52);

        while (!cardStack.isEmpty()) {
            Card c;
            cardStack.pop(c);
            temp.push(c);
        }

        while (!temp.isEmpty()) {
            Card c;
            temp.pop(c);
            cards.push(c);
        }
    }

    void draw(int x, int y, int colNum, bool isSel, int selIdx, Texture2D* ci, Texture2D cb) const {
        DrawText(TextFormat("%d", colNum), x+26, y-24, 18, Color{0,0,0,180});
        DrawText(TextFormat("%d", colNum), x+25, y-25, 18, Color{255,215,0,255});

        if (isEmpty()) {
            DrawRectangleLinesEx({(float)x, (float)y, 70, 90}, 2, Color{100,120,80,150});
        } else {
            for (int i = 0; i < cards.getSize(); i++) {
                Card c = cards.getAt(i);
                c.draw(x, y+i*22, isSel && i>=selIdx, ci, cb);
            }
        }
    }

    void save(ofstream& f) const {
        f << cards.getSize() << " ";
        for (int i = 0; i < cards.getSize(); i++) {
            Card c = cards.getAt(i);
            f << c.getRank() << " " << c.getSuit() << " ";
        }
    }

    void load(ifstream& f) {
        cards.clear();
        int cnt; f >> cnt;
        for (int i = 0; i < cnt; i++) {
            int r, s;
            f >> r >> s;
            cards.push(Card(r, s));
        }
    }
};

void DrawTextWithShadow(const char* text, int x, int y, int size, Color shadow, Color main) {
    DrawText(text, x+2, y+2, size, shadow);
    DrawText(text, x, y, size, main);
}

//  FreeCell Game
class FreeCellGame {
private:
    TableauColumn tableau[8];
    FreeCell freeCells[4];
    Foundation foundations[4];
    int moves, score, selCol, selCard;
    bool gameWon;
    string msg;
    float msgTimer;
    Texture2D cardImg[52], cardBack, bg;
    int selType;
    int selIndex;
    float gameTime;
    bool showHint;
    float hintTimer;
    int hintCol, hintCard;

    // Move history for undo
    struct Move {
        int fromType; // 0=tableau, 1=freecell, 2=foundation
        int fromIndex;
        int toType;
        int toIndex;
        int numCards;
        Stack<Card> movedCards;

        Move() : fromType(-1), fromIndex(-1), toType(-1), toIndex(-1), numCards(0), movedCards(52) {}
    };
    Stack<Move> moveHistory;

    void createDeck() {
        Stack<Card> all(52);

        for (int suit = 0; suit < 4; suit++) {
            for (int rank = 1; rank <= 13; rank++) {
                all.push(Card(rank, suit));
            }
        }

        Card* tempArr = new Card[52];
        for (int i = 0; i < 52; i++) {
            tempArr[i] = all.getAt(i);
        }

        for (int i = 51; i > 0; i--) {
            int j = rand() % (i + 1);
            Card temp = tempArr[i];
            tempArr[i] = tempArr[j];
            tempArr[j] = temp;
        }

        all.clear();
        for (int i = 0; i < 52; i++) {
            all.push(tempArr[i]);
        }
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
        int count = 0;
        for (int i = 0; i < 4; i++)
            if (freeCells[i].isEmpty()) count++;
        return count;
    }

    int countEmptyColumns() const {
        int count = 0;
        for (int i = 0; i < 8; i++)
            if (tableau[i].isEmpty()) count++;
        return count;
    }

    void calculateScore() {
        score = 0;

        for (int i = 0; i < 4; i++) {
            score += foundations[i].size() * 10;
        }

        for (int i = 0; i < 4; i++) {
            if (foundations[i].size() == 13) {
                score += 100;
            }
        }

        score -= moves * 2;

        if (score < 0) score = 0;
    }

public:
    FreeCellGame() : moves(0), score(0), gameWon(false), selCol(-1), selCard(-1),
                     msgTimer(0), selType(-1), selIndex(-1), gameTime(0),
                     showHint(false), hintTimer(0), hintCol(-1), hintCard(-1), moveHistory(1000) {
        srand(time(0));
    }

    void loadImages() {
        string suits[] = {"Club", "Diamond", "Heart", "Spade"};
        string ranks[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

        for (int suit = 0; suit < 4; suit++) {
            for (int rank = 0; rank < 13; rank++) {
                int idx = suit * 13 + rank;
                string filename = "Deck Images/" + suits[suit] + " " + ranks[rank] + ".jpeg";
                Image img = LoadImage(filename.c_str());
                if (img.data) {
                    cardImg[idx] = LoadTextureFromImage(img);
                    UnloadImage(img);
                } else {
                    cardImg[idx].id = 0;
                }
            }
        }

        Image back = LoadImage("Deck Images/card_back.jpeg");
        if (!back.data) back = GenImageColor(70, 90, DARKBLUE);
        cardBack = LoadTextureFromImage(back);
        UnloadImage(back);

        Image bgImg = LoadImage("Deck Images/background.jpeg");
        if (!bgImg.data) bgImg = GenImageColor(1020, 750, Color{34,139,34,255});
        bg = LoadTextureFromImage(bgImg);
        UnloadImage(bgImg);
    }

    void unloadImages() {
        for (int i = 0; i < 52; i++)
            if (cardImg[i].id) UnloadTexture(cardImg[i]);
        UnloadTexture(cardBack);
        UnloadTexture(bg);
    }

    void newGame() {
        for (int i = 0; i < 8; i++) tableau[i] = TableauColumn();
        for (int i = 0; i < 4; i++) {
            freeCells[i] = FreeCell();
            foundations[i] = Foundation();
        }
        moves = 0;
        score = 0;
        gameTime = 0;
        gameWon = false;
        selCol = selCard = -1;
        selType = selIndex = -1;
        showHint = false;
        hintTimer = 0;
        hintCol = hintCard = -1;
        moveHistory.clear();
        createDeck();
        msg = "New game started! Move all cards to foundations.";
        msgTimer = 3;
    }

    bool save() {
        ofstream f("freecell_save.dat");
        if (!f) return false;
        f << moves << " " << score << " " << gameTime << " " << gameWon << endl;
        for (int i = 0; i < 8; i++) { tableau[i].save(f); f << endl; }
        for (int i = 0; i < 4; i++) { freeCells[i].save(f); f << endl; }
        for (int i = 0; i < 4; i++) { foundations[i].save(f); f << endl; }
        f.close();
        return true;
    }

    bool load() {
        ifstream f("freecell_save.dat");
        if (!f) return false;
        for (int i = 0; i < 8; i++) tableau[i] = TableauColumn();
        for (int i = 0; i < 4; i++) {
            freeCells[i] = FreeCell();
            foundations[i] = Foundation();
        }
        f >> moves >> score >> gameTime >> gameWon;
        for (int i = 0; i < 8; i++) tableau[i].load(f);
        for (int i = 0; i < 4; i++) freeCells[i].load(f);
        for (int i = 0; i < 4; i++) foundations[i].load(f);
        f.close();
        selCol = selCard = -1;
        selType = selIndex = -1;
        msg = "Game loaded successfully!";
        msgTimer = 2;
        return true;
    }

    void checkWin() {
        int totalCards = 0;
        for (int i = 0; i < 4; i++)
            totalCards += foundations[i].size();

        if (totalCards == 52) {
            gameWon = true;
            calculateScore();
            msg = "CONGRATULATIONS! YOU WON!";
            msgTimer = 5;
        }
    }

    void recordMove(int fromType, int fromIndex, int toType, int toIndex, Stack<Card>& cards) {
        Move m;
        m.fromType = fromType;
        m.fromIndex = fromIndex;
        m.toType = toType;
        m.toIndex = toIndex;
        m.numCards = cards.getSize();

        // Copy cards
        for (int i = 0; i < cards.getSize(); i++) {
            m.movedCards.push(cards.getAt(i));
        }

        moveHistory.push(m);
    }

    void undoMove() {
        if (moveHistory.isEmpty()) {
            msg = "No moves to undo!";
            msgTimer = 2;
            return;
        }

        Move m;
        moveHistory.pop(m);

        // Reverse the cards order for proper restoration
        Stack<Card> reversedCards(52);
        for (int i = 0; i < m.numCards; i++) {
            Card c = m.movedCards.getAt(i);
            reversedCards.push(c);
        }

        // Remove cards from destination
        if (m.toType == 0) { // Tableau
            for (int i = 0; i < m.numCards; i++) {
                tableau[m.toIndex].removeTopCard();
            }
        } else if (m.toType == 1) { // FreeCell
            freeCells[m.toIndex].removeCard();
        } else if (m.toType == 2) { // Foundation
            Card dummy;
            foundations[m.toIndex].cards.pop(dummy);
        }

        // Restore cards to source
        if (m.fromType == 0) { // Tableau
            while (!reversedCards.isEmpty()) {
                Card c;
                reversedCards.pop(c);
                tableau[m.fromIndex].addCard(c);
            }
        } else if (m.fromType == 1) { // FreeCell
            Card c;
            reversedCards.pop(c);
            freeCells[m.fromIndex].addCard(c);
        }

        moves--;
        if (moves < 0) moves = 0;
        calculateScore();

        msg = "Move undone!";
        msgTimer = 1.5;
    }

    void findHint() {
        showHint = false;
        hintCol = hintCard = -1;

        // First, try to find moves to foundation
        for (int c = 0; c < 8; c++) {
            if (!tableau[c].isEmpty()) {
                Card card = tableau[c].getTopCard();
                for (int f = 0; f < 4; f++) {
                    if (foundations[f].canAddCard(card)) {
                        showHint = true;
                        hintCol = c;
                        hintCard = tableau[c].size() - 1;
                        hintTimer = 3.0;
                        msg = "Hint: Move card to foundation!";
                        msgTimer = 2;
                        return;
                    }
                }
            }
        }

        // Try to find moves from freecells to foundation
        for (int fc = 0; fc < 4; fc++) {
            if (!freeCells[fc].isEmpty()) {
                Card card = freeCells[fc].getCard();
                for (int f = 0; f < 4; f++) {
                    if (foundations[f].canAddCard(card)) {
                        showHint = true;
                        msg = "Hint: Move freecell card to foundation!";
                        msgTimer = 2;
                        return;
                    }
                }
            }
        }

        // Find tableau to tableau moves
        for (int c = 0; c < 8; c++) {
            if (!tableau[c].isEmpty()) {
                int emptyFC = countEmptyFreeCells();
                int emptyCol = countEmptyColumns();
                int maxMove = tableau[c].getMaxMovableSequence(emptyFC, emptyCol);

                for (int startIdx = tableau[c].size() - maxMove; startIdx < tableau[c].size(); startIdx++) {
                    if (startIdx < 0) continue;
                    if (!tableau[c].isValidSequenceFrom(startIdx)) continue;

                    Card movingCard = tableau[c].getCardAt(startIdx);

                    for (int target = 0; target < 8; target++) {
                        if (target == c) continue;

                        if (!tableau[target].isEmpty()) {
                            Card targetCard = tableau[target].getTopCard();
                            if (movingCard.canPlaceOnTableau(targetCard)) {
                                showHint = true;
                                hintCol = c;
                                hintCard = startIdx;
                                hintTimer = 3.0;
                                msg = "Hint: Move card sequence!";
                                msgTimer = 2;
                                return;
                            }
                        }
                    }
                }
            }
        }

        msg = "No obvious moves found!";
        msgTimer = 2;
    }

    void autoMoveToFoundation(int fromType, int fromIndex) {
        Card card;
        bool canRemove = false;

        if (fromType == 0 && !tableau[fromIndex].isEmpty()) {
            card = tableau[fromIndex].getTopCard();
            canRemove = true;
        } else if (fromType == 1 && !freeCells[fromIndex].isEmpty()) {
            card = freeCells[fromIndex].getCard();
            canRemove = true;
        }

        if (!canRemove) return;

        for (int i = 0; i < 4; i++) {
            if (foundations[i].canAddCard(card)) {
                if (fromType == 0) tableau[fromIndex].removeTopCard();
                else if (fromType == 1) freeCells[fromIndex].removeCard();

                foundations[i].addCard(card);
                moves++;
                calculateScore();
                checkWin();
                return;
            }
        }
    }

    void handleInput() {
        Vector2 mp = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < 4; i++) {
                int x = 20 + i * 90;
                if (CheckCollisionPointRec(mp, {(float)x, 20, 70, 90})) {
                    if (selType == -1) {
                        if (!freeCells[i].isEmpty()) {
                            selType = 1;
                            selIndex = i;
                        }
                    } else {
                        if (freeCells[i].isEmpty()) {
                            Card card;
                            if (selType == 0 && !tableau[selCol].isEmpty()) {
                                card = tableau[selCol].getTopCard();
                                Stack<Card> temp(52);
                                temp.push(card);
                                recordMove(0, selCol, 1, i, temp);

                                tableau[selCol].removeTopCard();
                                freeCells[i].addCard(card);
                                moves++;
                                calculateScore();
                                checkWin();
                            } else if (selType == 1) {
                                card = freeCells[selIndex].getCard();
                                Stack<Card> temp(52);
                                temp.push(card);
                                recordMove(1, selIndex, 1, i, temp);

                                card = freeCells[selIndex].removeCard();
                                freeCells[i].addCard(card);
                                moves++;
                                calculateScore();
                            }
                        }
                        selType = selIndex = selCol = selCard = -1;
                    }
                    return;
                }
            }

            for (int i = 0; i < 4; i++) {
                int x = 560 + i * 90;
                if (CheckCollisionPointRec(mp, {(float)x, 20, 70, 90})) {
                    if (selType != -1) {
                        Card card;
                        bool canMove = false;

                        if (selType == 0 && !tableau[selCol].isEmpty()) {
                            card = tableau[selCol].getTopCard();
                            canMove = true;
                        } else if (selType == 1) {
                            card = freeCells[selIndex].getCard();
                            canMove = true;
                        }

                        if (canMove && foundations[i].canAddCard(card)) {
                            Stack<Card> temp(52);
                            temp.push(card);

                            if (selType == 0) {
                                recordMove(0, selCol, 2, i, temp);
                                tableau[selCol].removeTopCard();
                            } else if (selType == 1) {
                                recordMove(1, selIndex, 2, i, temp);
                                freeCells[selIndex].removeCard();
                            }

                            foundations[i].addCard(card);
                            moves++;
                            calculateScore();
                            checkWin();
                        }
                        selType = selIndex = selCol = selCard = -1;
                    }
                    return;
                }
            }

            for (int c = 0; c < 8; c++) {
                int x = 100 + c * 110;
                int y = 150;

                if (!tableau[c].isEmpty()) {
                    for (int cd = tableau[c].size()-1; cd >= 0; cd--) {
                        if (CheckCollisionPointRec(mp, {(float)x, (float)(y+cd*22), 70, 90})) {
                            if (selType == -1) {
                                selType = 0;
                                selCol = c;
                                selCard = cd;
                            } else {
                                if (selType == 0) {
                                    int numCards = tableau[selCol].size() - selCard;
                                    int emptyFC = countEmptyFreeCells();
                                    int emptyCol = countEmptyColumns();

                                    // Check if moving to same column
                                    if (selCol == c) {
                                        selType = selIndex = selCol = selCard = -1;
                                        return;
                                    }

                                    // Adjust empty columns if target is empty
                                    if (tableau[c].isEmpty()) {
                                        emptyCol--;
                                    }

                                    int maxMove = (emptyFC + 1) * (1 << (emptyCol > 0 ? emptyCol : 0));

                                    if (numCards <= maxMove && tableau[selCol].isValidSequenceFrom(selCard)) {
                                        Card movingCard = tableau[selCol].getCardAt(selCard);

                                        bool canMove = false;
                                        if (tableau[c].isEmpty()) {
                                            canMove = true; // Can move to empty column
                                        } else {
                                            Card targetCard = tableau[c].getTopCard();
                                            if (movingCard.canPlaceOnTableau(targetCard)) {
                                                canMove = true;
                                            }
                                        }

                                        if (canMove) {
                                            Stack<Card> cards = tableau[selCol].removeCards(selCard);
                                            recordMove(0, selCol, 0, c, cards);

                                            // Need to make a copy for adding
                                            Stack<Card> cardsCopy(52);
                                            for (int i = 0; i < cards.getSize(); i++) {
                                                cardsCopy.push(cards.getAt(i));
                                            }

                                            tableau[c].addCards(cardsCopy);
                                            moves++;
                                            calculateScore();
                                            checkWin();
                                        }
                                    }
                                } else if (selType == 1) {
                                    Card card = freeCells[selIndex].getCard();

                                    bool canMove = false;
                                    if (tableau[c].isEmpty()) {
                                        canMove = true;
                                    } else {
                                        Card targetCard = tableau[c].getTopCard();
                                        if (card.canPlaceOnTableau(targetCard)) {
                                            canMove = true;
                                        }
                                    }

                                    if (canMove) {
                                        Stack<Card> temp(52);
                                        temp.push(card);
                                        recordMove(1, selIndex, 0, c, temp);

                                        freeCells[selIndex].removeCard();
                                        tableau[c].addCard(card);
                                        moves++;
                                        calculateScore();
                                        checkWin();
                                    }
                                }
                                selType = selIndex = selCol = selCard = -1;
                            }
                            return;
                        }
                    }
                } else {
                    if (CheckCollisionPointRec(mp, {(float)x, (float)y, 70, 90})) {
                        if (selType == 0) {
                            int numCards = tableau[selCol].size() - selCard;
                            int emptyFC = countEmptyFreeCells();
                            int emptyCol = countEmptyColumns() - 1;
                            int maxMove = (emptyFC + 1) * (1 << (emptyCol > 0 ? emptyCol : 0));

                            if (numCards <= maxMove && tableau[selCol].isValidSequenceFrom(selCard)) {
                                Stack<Card> cards = tableau[selCol].removeCards(selCard);
                                recordMove(0, selCol, 0, c, cards);

                                Stack<Card> cardsCopy(52);
                                for (int i = 0; i < cards.getSize(); i++) {
                                    cardsCopy.push(cards.getAt(i));
                                }

                                tableau[c].addCards(cardsCopy);
                                moves++;
                                calculateScore();
                                checkWin();
                            }
                        } else if (selType == 1) {
                            Card card = freeCells[selIndex].getCard();
                            Stack<Card> temp(52);
                            temp.push(card);
                            recordMove(1, selIndex, 0, c, temp);

                            card = freeCells[selIndex].removeCard();
                            tableau[c].addCard(card);
                            moves++;
                            calculateScore();
                            checkWin();
                        }
                        selType = selIndex = selCol = selCard = -1;
                    }
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            selType = selIndex = selCol = selCard = -1;
        }

        static double lastClick = 0;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            double currentTime = GetTime();

            if (currentTime - lastClick < 0.3) {
                for (int c = 0; c < 8; c++) {
                    int x = 100 + c * 110;
                    int y = 150;
                    if (!tableau[c].isEmpty()) {
                        int topY = y + (tableau[c].size()-1) * 22;
                        if (CheckCollisionPointRec(mp, {(float)x, (float)topY, 70, 90})) {
                            autoMoveToFoundation(0, c);
                            return;
                        }
                    }
                }

                for (int i = 0; i < 4; i++) {
                    int x = 20 + i * 90;
                    if (CheckCollisionPointRec(mp, {(float)x, 20, 70, 90})) {
                        if (!freeCells[i].isEmpty()) {
                            autoMoveToFoundation(1, i);
                            return;
                        }
                    }
                }
            }
            lastClick = currentTime;
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
            if (save()) { msg = "Game saved successfully!"; msgTimer = 2; }
        }

        if (IsKeyPressed(KEY_H)) {
            findHint();
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) {
            undoMove();
        }
    }

    void update(float dt) {
        if (msgTimer > 0) msgTimer -= dt;
        if (!gameWon) gameTime += dt;
        if (hintTimer > 0) {
            hintTimer -= dt;
            if (hintTimer <= 0) {
                showHint = false;
            }
        }
    }

    void draw() {
        DrawTexturePro(bg, {0,0,(float)bg.width,(float)bg.height},
                      {0,0,1020,750}, {0,0}, 0, WHITE);

        // Draw free cells
        DrawTextWithShadow("Free Cells", 30, 117, 14, Color{0,0,0,200}, WHITE);
        for (int i = 0; i < 4; i++) {
            freeCells[i].draw(20 + i * 90, 20, cardImg, cardBack);
            if (selType == 1 && selIndex == i)
                DrawRectangleLinesEx({(float)(20 + i * 90), 20, 70, 90}, 3, YELLOW);
        }

        // Draw foundations
        DrawTextWithShadow("Foundations", 570, 117, 14, Color{0,0,0,200}, WHITE);
        for (int i = 0; i < 4; i++) {
            foundations[i].draw(560 + i * 90, 20, cardImg, cardBack);
        }

        // Draw score and time boxes
        DrawRectangle(400, 20, 140, 90, Color{40, 30, 20, 230});
        DrawRectangleLinesEx({400, 20, 140, 90}, 3, Color{120, 90, 50, 255});

        DrawTextWithShadow("MOVES", 432, 28, 16, Color{0,0,0,200}, Color{255,215,0,255});
        DrawTextWithShadow(TextFormat("%d", moves), 450, 50, 20, Color{0,0,0,200}, WHITE);

        DrawTextWithShadow("SCORE", 432, 74, 16, Color{0,0,0,200}, Color{255,215,0,255});
        DrawTextWithShadow(TextFormat("%d", score), 450, 96, 20, Color{0,0,0,200}, Color{0,255,100,255});

        // Draw time
        int minutes = (int)gameTime / 60;
        int seconds = (int)gameTime % 60;
        DrawRectangle(920, 20, 90, 50, Color{40, 30, 20, 230});
        DrawRectangleLinesEx({920, 20, 90, 50}, 3, Color{120, 90, 50, 255});
        DrawTextWithShadow("TIME", 938, 26, 16, Color{0,0,0,200}, Color{255,215,0,255});
        DrawTextWithShadow(TextFormat("%02d:%02d", minutes, seconds), 930, 46, 18, Color{0,0,0,200}, Color{100,200,255,255});

        // Draw tableau
        for (int i = 0; i < 8; i++) {
            bool isHintCol = (showHint && hintCol == i);
            tableau[i].draw(100 + i*110, 150, i+1, selType==0 && selCol==i, selCard, cardImg, cardBack);

            // Draw hint highlight
            if (isHintCol && hintCard >= 0) {
                int x = 100 + i * 110;
                int y = 150 + hintCard * 22;
                DrawRectangleLinesEx({(float)x, (float)y, 70, 90}, 4, Color{0, 255, 0, 255});
                DrawRectangleLinesEx({(float)(x-2), (float)(y-2), 74, 94}, 2, Color{255, 255, 0, 255});
            }
        }

        if (msgTimer > 0) {
            int w = MeasureText(msg.c_str(), 18);
            DrawRectangle(510-w/2-15, 700, w+30, 35, Color{0,0,0,230});
            DrawRectangleLinesEx({(float)(510-w/2-15), 700, (float)(w+30), 35}, 2, Color{255,215,0,255});
            DrawTextWithShadow(msg.c_str(), 510-w/2, 708, 18, Color{0,0,0,180}, WHITE);
        }

        DrawTextWithShadow("Left Click: Select/Move | Right Click: Cancel | Double-Click: Auto-Foundation",
                 80, 735, 13, Color{0,0,0,200}, Color{200,200,200,255});
        DrawTextWithShadow("Ctrl+S: Save | Ctrl+Z: Undo | H: Hint | ESC: Menu",
                 250, 752, 13, Color{0,0,0,200}, Color{200,200,200,255});

        if (gameWon) {
            DrawRectangle(0, 0, 1020, 750, Color{0,0,0,230});
            DrawTextWithShadow("CONGRATULATIONS!", 260, 250, 48, Color{0,0,0,255}, Color{255,215,0,255});
            DrawTextWithShadow("YOU WON THE GAME!", 310, 320, 38, Color{0,0,0,255}, YELLOW);
            DrawTextWithShadow(TextFormat("Total Moves: %d", moves), 390, 380, 26, Color{0,0,0,255}, WHITE);
            DrawTextWithShadow(TextFormat("Final Score: %d", score), 390, 420, 26, Color{0,0,0,255}, Color{0,255,100,255});

            int minutes = (int)gameTime / 60;
            int seconds = (int)gameTime % 60;
            DrawTextWithShadow(TextFormat("Time: %02d:%02d", minutes, seconds), 410, 460, 26, Color{0,0,0,255}, Color{100,200,255,255});

            DrawTextWithShadow("Press ESC to return to menu", 350, 510, 22, Color{0,0,0,255}, Color{180,180,180,255});
        }
    }

    bool isWon() const { return gameWon; }
};

enum MenuState { MAIN_MENU, PLAYING, EXIT_GAME };

int main() {
    InitWindow(1020, 750, "FreeCell Solitaire");
    SetTargetFPS(60);

    FreeCellGame game;
    game.loadImages();

    Image menuBgImg = LoadImage("Deck Images/menu_bg.jpeg");
    if (!menuBgImg.data) menuBgImg = GenImageColor(1020, 750, Color{20,20,20,255});
    Texture2D menuBg = LoadTextureFromImage(menuBgImg);
    UnloadImage(menuBgImg);

    MenuState state = MAIN_MENU;
    int menuSel = 0;
    bool saveExists = false;

    while (!WindowShouldClose()) {
        ifstream check("freecell_save.dat");
        saveExists = check.is_open();
        if (check.is_open()) check.close();

        if (state == MAIN_MENU) {
            Vector2 mp = GetMousePosition();

            if (IsKeyPressed(KEY_DOWN)) menuSel = (menuSel+1)%(saveExists?3:2);
            if (IsKeyPressed(KEY_UP)) menuSel = (menuSel-1+(saveExists?3:2))%(saveExists?3:2);

            Rectangle btnContinue = {360, 340, 300, 50};
            Rectangle btnNewGame = {360, 405, 300, 50};
            Rectangle btnExit = {360, 470, 300, 50};

            if (CheckCollisionPointRec(mp, btnContinue) && saveExists) {
                menuSel = 0;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    game.load();
                    state = PLAYING;
                }
            }
            else if (CheckCollisionPointRec(mp, btnNewGame)) {
                menuSel = saveExists ? 1 : 0;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    game.newGame();
                    state = PLAYING;
                }
            }
            else if (CheckCollisionPointRec(mp, btnExit)) {
                menuSel = saveExists ? 2 : 1;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    state = EXIT_GAME;
            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (saveExists) {
                    if (menuSel == 0) { game.load(); state = PLAYING; }
                    else if (menuSel == 1) { game.newGame(); state = PLAYING; }
                    else if (menuSel == 2) state = EXIT_GAME;
                } else {
                    if (menuSel == 0) { game.newGame(); state = PLAYING; }
                    else if (menuSel == 1) state = EXIT_GAME;
                }
            }

            BeginDrawing();
            ClearBackground(BLACK);

            DrawTexturePro(menuBg, {0,0,(float)menuBg.width,(float)menuBg.height},
                          {0,0,1020,750}, {0,0}, 0, WHITE);

            // Draw semi-transparent dark overlay
            DrawRectangle(0, 0, 1020, 750, Color{15,20,15,180});

            // Draw menu background panel
            DrawRectangle(250, 80, 520, 500, Color{25,30,20,240});
            DrawRectangleLinesEx({250, 80, 520, 500}, 4, Color{120,90,50,255});

            DrawTextWithShadow("FREECELL", 335, 110, 68, Color{0,0,0,255}, Color{180,140,70,255});
            DrawTextWithShadow("SOLITAIRE", 290, 190, 68, Color{0,0,0,255}, Color{180,140,70,255});

            if (saveExists) {
                Color cb = (menuSel==0) ? Color{60,80,40,255} : Color{30,40,20,220};
                Color cb2 = (menuSel==0) ? Color{180,140,70,255} : Color{100,120,60,255};
                DrawRectangleRec(btnContinue, cb);
                DrawRectangleLinesEx(btnContinue, 3.0f, cb2);
                DrawTextWithShadow("Continue", 415, 354, 28, Color{0,0,0,200}, Color{220,200,160,255});
            }

            int ngs = saveExists ? 1 : 0;
            Color ngb = (menuSel==ngs) ? Color{60,80,40,255} : Color{30,40,20,220};
            Color ngb2 = (menuSel==ngs) ? Color{180,140,70,255} : Color{100,120,60,255};
            DrawRectangleRec(btnNewGame, ngb);
            DrawRectangleLinesEx(btnNewGame, 3.0f, ngb2);
            DrawTextWithShadow("New Game", 405, 419, 28, Color{0,0,0,200}, Color{220,200,160,255});

            int es = saveExists ? 2 : 1;
            Color eb = (menuSel==es) ? Color{60,80,40,255} : Color{30,40,20,220};
            Color eb2 = (menuSel==es) ? Color{180,140,70,255} : Color{100,120,60,255};
            DrawRectangleRec(btnExit, eb);
            DrawRectangleLinesEx(btnExit, 3.0f, eb2);
            DrawTextWithShadow("Exit", 460, 484, 28, Color{0,0,0,200}, Color{220,200,160,255});

            DrawTextWithShadow("Use Arrow Keys or Mouse to Select", 330, 560, 19, Color{0,0,0,200}, Color{180,160,130,255});

            EndDrawing();
        }
        else if (state == PLAYING) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                game.save();
                state = MAIN_MENU;
                menuSel = 0;
            }

            game.handleInput();
            game.update(GetFrameTime());

            BeginDrawing();
            game.draw();
            EndDrawing();
        }
        else if (state == EXIT_GAME) {
            break;
        }
    }

    game.save();
    game.unloadImages();
    UnloadTexture(menuBg);
    CloseWindow();
    return 0;
}