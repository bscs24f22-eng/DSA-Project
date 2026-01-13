
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

void clear_screen()
 {
    system("cls");
}

void display_game()
 {
    clear_screen();
    cout <<endl;
    cout << "========================================"<<endl;
    cout << "   FREECELL GAME (LINKED LIST)         "<<endl;
    cout << "========================================"<<endl;
    cout << "Score: " << game->score <<endl<<endl;

    cout << "FREE CELLS:"<<endl;
    for (int i = 0; i < 4; i++)
     {
        cout << "FC" << i + 1 << ": ";
        if (game->free_cells[i]->is_empty())
         {
            cout << "[ ]";
        }
        else
         {
            cout << "[" << card_to_string(game->free_cells[i]->get_tail_data()) << "]";
        }
        cout << "  ";
    }
    cout <<endl<<endl;

    cout<<"FOUNDATIONS:"<<endl;
    string suit_names[] = {"D", "R", "S", "E"};
    for (int i = 0; i < 4; i++)
     {
        cout << suit_names[i] << ": ";
        if (game->foundations[i]->is_empty())
         {
            cout << "[ ]";
        }
        else
         {
            cout << "[" << card_to_string(game->foundations[i]->get_tail_data()) << "]";
        }
        cout << "  ";
    }
    cout <<endl<<endl;

    cout << "TABLEAU:"<<endl;
    cout << "Col1     Col2     Col3     Col4     Col5     Col6     Col7     Col8"<<endl;

    int max_height = 0;
    for (int i = 0; i < 8; i++)
     {
        if (game->tableau[i]->get_size() > max_height)
         {
            max_height = game->tableau[i]->get_size();
        }
    }

    for (int row = 0; row < max_height; row++)
     {
        for (int col = 0; col < 8; col++)
         {
            if (game->tableau[col]->get_size() > row)
             {
                card c = game->tableau[col]->get_at_index(row);
                cout << "[" << card_to_string(c) << "]";
            }
            else
             {
                cout<<"    ";
            }
            cout<<"    ";
        }
        cout <<endl;
    }
    cout<<endl;
}

bool can_place_on_tableau(card c, int col)
 {
    if (game->tableau[col]->is_empty()) return true;

    card top_card = game->tableau[col]->get_tail_data();

    if (get_card_color(c) == get_card_color(top_card)) return false;
    if (c.rank != top_card.rank - 1) return false;

    return true;
}

bool can_place_on_foundation(card c, int found_idx)
 {
    if (game->foundations[found_idx]->is_empty())
     {
        return c.rank == 1;
    }

    card top_card = game->foundations[found_idx]->get_tail_data();
    if (c.suit != top_card.suit) return false;
    if (c.rank != top_card.rank + 1) return false;

    return true;
}

void move_card()
 {
    cout <<endl<<"=== MOVE CARD ==="<<endl;
    cout<<"Source: C1-C8 (Columns), F1-F4 (Free Cells)"<<endl;
    cout << "Enter source: ";

    string source;
    cin >> source;

    linked_list* source_list = NULL;

    if (source.length() < 2)
     {
        cout<<endl<<"Invalid input! Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    if (source[0] == 'C' || source[0] == 'c')
     {
        int col = source[1] - '1';
        if (col >= 0 && col < 8 && !game->tableau[col]->is_empty())
         {
            source_list = game->tableau[col];
        }
    }
    else if (source[0] == 'F' || source[0] == 'f')
     {
        int fc = source[1] - '1';
        if (fc >= 0 && fc < 4 && !game->free_cells[fc]->is_empty())
         {
            source_list = game->free_cells[fc];
        }
    }

    if (source_list == NULL)
     {
        cout <<endl<<"Invalid source or empty pile! Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    card card_to_move = source_list->get_tail_data();

    cout << "Moving card: "<< card_to_string(card_to_move)<<endl;
    cout << "Destination: C1-C8 (Columns), F1-F4 (Free), D1-D4 (Foundation)"<<endl;
    cout << "Enter destination: ";

    string dest;
    cin >> dest;

    if (dest.length() < 2)
     {
        cout <<endl<<"Invalid input! Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    bool valid_move = false;

    if (dest[0] == 'C' || dest[0] == 'c')
     {
        int col = dest[1] - '1';
        if (col >= 0 && col < 8)
         {
            if (can_place_on_tableau(card_to_move, col))
             {
                source_list->remove_from_tail();
                game->tableau[col]->insert_at_tail(card_to_move);
                game->score += 5;
                valid_move = true;
            }
        }
    }
    else if (dest[0] == 'F' || dest[0] == 'f')
     {
        int fc = dest[1] - '1';
        if (fc >= 0 && fc < 4 && game->free_cells[fc]->is_empty())
         {
            source_list->remove_from_tail();
            game->free_cells[fc]->insert_at_tail(card_to_move);
            valid_move = true;
        }
    }
    else if (dest[0] == 'D' || dest[0] == 'd')
     {
        int found = dest[1] - '1';
        if (found >= 0 && found < 4)
         {
            if (can_place_on_foundation(card_to_move, found))
             {
                source_list->remove_from_tail();
                game->foundations[found]->insert_at_tail(card_to_move);
                game->score += 10;
                valid_move = true;
            }
        }
    }

    if (valid_move)
     {
        cout<<endl<<"Move successful!";
    }
    else
     {
        cout <<endl<<"Invalid move!";
    }

    cout << " Press Enter to continue...";
    cin.ignore();
    cin.get();
}

bool check_win()
 {
    for (int i = 0; i < 4; i++)
     {
        if (game->foundations[i]->get_size() != 13)
         {
            return false;
        }
    }
    return true;
}

int main()
 {
    game = new game_state();

    cout << "========================================"<<endl;
    cout << "   WELCOME TO CONSOLE FREECELL GAME   "<<endl;
    cout << "  Using LINKED LIST Data Structure    "<<endl;
    cout << "========================================"<<endl;
    cout << "Press Enter to start...";
    cin.get();

    initialize_game();

    while (true)
     {
        display_game();

        if (check_win())
         {
            cout <<endl<< "*** CONGRATULATIONS! YOU WON! ***"<<endl;
            cout << "Final Score: " << game->score <<endl;
            cout <<endl<<"Press Enter to exit...";
            cin.get();
            break;
        }

        cout << "Commands:"<<endl;
        cout << "  M - Move card"<<endl;
        cout << "  R - Reset game"<<endl;
        cout << "  Q - Quit"<<endl;
        cout <<endl<<"Enter command: ";

        char command;
        cin >> command;

        if (command == 'M' || command == 'm')
         {
            move_card();
        }
        else if (command == 'R' || command == 'r')
         {
            initialize_game();
            cout <<endl<<"Game reset! Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
        else if (command == 'Q' || command == 'q')
         {
            cout <<endl<<"Thanks for playing.."<<endl;
            break;
        }
        else
         {
            cout <<"Invalid command! Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    delete game;
    return 0;
}