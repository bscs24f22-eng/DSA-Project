#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

struct card
 {
    int rank;
    int suit;

    card()
     {
        rank = 0;
        suit = 0;
    }

    card(int r, int s)
     {
        rank = r;
        suit = s;
    }
};

struct node
 {
    card data;
    node* next;

    node(card c)
     {
        data = c;
        next = NULL;
    }
};

class linked_list
 {
public:
    node* head;
    node* tail;
    int size;

    linked_list()
     {
        head = NULL;
        tail = NULL;
        size = 0;
    }

    ~linked_list()
     {
        clear();
    }

    bool is_empty()
     {
        return head == NULL;
    }

    void insert_at_tail(card c)
     {
        node* new_node = new node(c);

        if (is_empty())
         {
            head = new_node;
            tail = new_node;
        }
        else
         {
            tail->next = new_node;
            tail = new_node;
        }
        size++;
    }

    void remove_from_tail()
     {
        if (is_empty()) return;

        if (head == tail)
         {
            delete head;
            head = nullptr;
            tail = nullptr;
        }
        else
         {
            node* current = head;
            while (current->next != tail)
             {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        size--;
    }

    card get_tail_data()
     {
        if (tail != nullptr)
         {
            return tail->data;
        }
        return card();
    }

    card get_at_index(int idx)
     {
        node* current = head;
        int count = 0;

        while (current != NULL && count < idx)
         {
            current = current->next;
            count++;
        }

        if (current != NULL)
         {
            return current->data;
        }
        return card();
    }

    void clear()
     {
        while (!is_empty())
         {
            remove_from_tail();
        }
    }

    int get_size()
     {
        return size;
    }
};

struct game_state
 {
    linked_list* tableau[8];
    linked_list* free_cells[4];
    linked_list* foundations[4];
    int score;

    game_state()
     {
        for (int i = 0; i < 8; i++)
         {
            tableau[i] = new linked_list();
        }
        for (int i = 0; i < 4; i++)
         {
            free_cells[i] = new linked_list();
            foundations[i] = new linked_list();
        }
        score = 0;
    }

    ~game_state()
     {
        for (int i = 0; i < 8; i++)
         {
            delete tableau[i];
        }
        for (int i = 0; i < 4; i++)
         {
            delete free_cells[i];
            delete foundations[i];
        }
    }
};

game_state* game;

void shuffle_deck(card deck[], int deck_size)
 {
    srand(time(0));
    for (int i = deck_size - 1; i > 0; i--)
     {
        int j = rand() % (i + 1);
        card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void initialize_game()
 {
    for (int i = 0; i < 8; i++)
     {
        game->tableau[i]->clear();
    }
    for (int i = 0; i < 4; i++)
     {
        game->free_cells[i]->clear();
        game->foundations[i]->clear();
    }

    game->score = 0;

    card deck[52];
    int idx = 0;
    for (int s = 0; s < 4; s++)
     {
        for (int r = 1; r <= 13; r++)
         {
            deck[idx] = card(r, s);
            idx++;
        }
    }

    shuffle_deck(deck, 52);

    idx = 0;
    for (int col = 0; col < 8; col++)
     {
        int cards_in_col = (col < 4) ? 7 : 6;
        for (int i = 0; i < cards_in_col; i++)
         {
            game->tableau[col]->insert_at_tail(deck[idx++]);
        }
    }
}

string get_suit_name(int suit)
 {
    if (suit == 0) return "D";
    if (suit == 1) return "R";
    if (suit == 2) return "S";
    return "E";
}

string get_rank_name(int rank)
 {
    if (rank == 1) return "A";
    if (rank == 11) return "J";
    if (rank == 12) return "Q";
    if (rank == 13) return "K";
    return to_string(rank);
}

string card_to_string(card c)
 {
    if (c.rank == 0) return " ";
    string rank_str = get_rank_name(c.rank);
    string suit_str = get_suit_name(c.suit);

    if (rank_str.length() == 1)
     {
        return rank_str + suit_str + " ";
    }
    else
     {
        return rank_str + suit_str;
    }
}

int get_card_color(const card& c)
 {
    if (c.suit == 0 || c.suit == 1)
     {
        return 0;
    }
    else
     {
        return 1;
    }
}