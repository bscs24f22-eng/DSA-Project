#pragma once
#include <string>
using namespace std;

class Card {
private:
    int rank;
    bool faceUp;

public:
    Card() : rank(0), faceUp(false) {}
    
    Card(int r) : rank(r), faceUp(false) {}

    int getRank() const { return rank; }
    
    bool isFaceUp() const { return faceUp; }
    
    void flip() { faceUp = true; }
    
    void flipDown() { faceUp = false; }

    string toString() const {
        if (!faceUp) return "[##]";
        
        string rankStr;
        if (rank == 1) rankStr = "A";
        else if (rank == 11) rankStr = "J";
        else if (rank == 12) rankStr = "Q";
        else if (rank == 13) rankStr = "K";
        else rankStr = to_string(rank);
        
        return "[" + rankStr + "]";
    }

    bool canPlaceOn(const Card& other) const {
        return (this->rank == other.rank - 1);
    }
};