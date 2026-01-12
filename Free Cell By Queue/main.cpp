#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

struct card {
    int rank;
    int suit;

    card() : rank(0), suit(0) {}
    card(int r, int s) : rank(r), suit(s) {}
};

template <class t>
class que {
public:
    int size, frnt, rare;
    t* ptr;

    que(int a) {
        ptr = new t[a];
        size = a;
        frnt = 0;
        rare = 0;
    }

    ~que() {
        delete[] ptr;
    }

    bool isempty() { return frnt == rare; }
    bool isfull() { return frnt == size; }

    void enqu(t m) {
        if (!isfull()) ptr[frnt++] = m;
    }

    void dequ() {
        if (!isempty()) rare++;
    }

    int get_size() { return frnt - rare; }

    t get_top() {
        if (!isempty()) return ptr[frnt - 1];
        return t();
    }

    void remove_top() {
        if (!isempty()) frnt--;
    }

    t get_at_index(int idx) {
        if (idx >= rare && idx < frnt) return ptr[idx];
        return t();
    }

    void clear() {
        frnt = rare = 0;
    }
};

struct game_state {
    que<card>* tableau[8];
    que<card>* free_cells[4];
    que<card>* foundations[4];
    int score;

    game_state() {
        for (int i = 0; i < 8; i++)
            tableau[i] = new que<card>(52);

        for (int i = 0; i < 4; i++) {
            free_cells[i] = new que<card>(13);
            foundations[i] = new que<card>(13);
        }
        score = 0;
    }

    ~game_state() {
        for (int i = 0; i < 8; i++) delete tableau[i];
        for (int i = 0; i < 4; i++) {
            delete free_cells[i];
            delete foundations[i];
        }
    }
};

game_state* game;
void shuffle_deck(card deck[], int deck_size) {
    srand(time(0));
    for (int i = deck_size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(deck[i], deck[j]);
    }
}

int get_card_color(const card& c) {
    return (c.suit == 0 || c.suit == 1) ? 0 : 1;
}

bool can_place_on_tableau(card c, int col) {
    if (game->tableau[col]->isempty()) return true;

    card top = game->tableau[col]->get_top();
    return get_card_color(c) != get_card_color(top) &&
           c.rank == top.rank - 1;
}

bool can_place_on_foundation(card c, int idx) {
    if (game->foundations[idx]->isempty())
        return c.rank == 1;

    card top = game->foundations[idx]->get_top();
    return c.suit == top.suit && c.rank == top.rank + 1;
}
