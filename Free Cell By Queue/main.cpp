#include <iostream>
#include <limits>
using namespace std;

template <typename T>
class Node
{
public:
    T data;
    Node *next;
    Node(T val) : data(val), next(nullptr) {}
};

template <typename T>
class Queue
{
private:
    Node<T> *frontNode;
    Node<T> *rearNode;
    int count;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

    ~Queue()
    {
        while (!isEmpty())
        {
            dequeue();
        }
    }

    void enqueue(T val)
    {
        Node<T> *newNode = new Node<T>(val);
        if (isEmpty())
        {
            frontNode = rearNode = newNode;
        }
        else
        {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    T dequeue()
    {
        if (isEmpty())
        {
            cout << "Queue is empty!" << endl;
            return T();
        }
        Node<T> *temp = frontNode;
        T val = temp->data;
        frontNode = frontNode->next;
        if (frontNode == nullptr)
        {
            rearNode = nullptr;
        }
        delete temp;
        count--;
        return val;
    }

    T front() const
    {
        if (isEmpty())
        {
            return T();
        }
        return frontNode->data;
    }

    bool isEmpty() const
    {
        return frontNode == nullptr;
    }

    int size() const
    {
        return count;
    }
};

struct Card
{
    char rank;
    char suit;

    Card() : rank('0'), suit('0') {}
    Card(char r, char s) : rank(r), suit(s) {}

    bool isEmpty() const
    {
        return rank == '0';
    }

    int getRankValue() const
    {
        if (rank == 'A')
            return 1;
        if (rank >= '2' && rank <= '9')
            return rank - '0';
        if (rank == 'T')
            return 10;
        if (rank == 'J')
            return 11;
        if (rank == 'Q')
            return 12;
        if (rank == 'K')
            return 13;
        return 0;
    }

    bool isRed() const
    {
        return suit == 'H' || suit == 'D';
    }

    void display() const
    {
        if (isEmpty())
        {
            cout << "[  ]";
        }
        else
        {
            cout << "[" << rank << suit << "]";
        }
    }
};

class FreeCell
{
private:
    Queue<Card> tableau[8];
    Card freeCells[4];
    Queue<Card> foundations[4];
    int moves;

    void initializeDeck(Card deck[52])
    {
        char ranks[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
        char suits[] = {'H', 'D', 'C', 'S'};
        int idx = 0;

        for (int s = 0; s < 4; s++)
        {
            for (int r = 0; r < 13; r++)
            {
                deck[idx++] = Card(ranks[r], suits[s]);
            }
        }

        for (int i = 0; i < 52; i++)
        {
            int j = (i * 17 + 29) % 52;
            Card temp = deck[i];
            deck[i] = deck[j];
            deck[j] = temp;
        }
    }

    void dealCards()
    {
        Card deck[52];
        initializeDeck(deck);

        int cardIdx = 0;
        for (int col = 0; col < 4; col++)
        {
            for (int i = 0; i < 7; i++)
            {
                tableau[col].enqueue(deck[cardIdx++]);
            }
        }

        for (int col = 4; col < 8; col++)
        {
            for (int i = 0; i < 6; i++)
            {
                tableau[col].enqueue(deck[cardIdx++]);
            }
        }
    }

    bool canPlaceOnTableau(Card card, Card target)
    {
        if (target.isEmpty())
            return true;
        if (card.getRankValue() != target.getRankValue() - 1)
            return false;
        if (card.isRed() == target.isRed())
            return false;
        return true;
    }

    bool canPlaceOnFoundation(Card card, int foundIdx)
    {
        if (card.isEmpty())
            return false;

        if (foundations[foundIdx].isEmpty())
        {
            return card.rank == 'A';
        }

        Card topCard = foundations[foundIdx].front();

        return (card.suit == topCard.suit &&
                card.getRankValue() == topCard.getRankValue() + 1);
    }

    void clearInputBuffer()
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    FreeCell() : moves(0)
    {
        for (int i = 0; i < 4; i++)
        {
            freeCells[i] = Card();
        }
        dealCards();
    }

    void display()
    {
        cout << "\n"
             << endl;
        cout << "         FREECELL (Queue-Based)" << endl;
        cout << "\n"
             << endl;
        cout << "Moves: " << moves << endl;

        cout << "\nFREE CELLS:  ";
        for (int i = 0; i < 4; i++)
        {
            freeCells[i].display();
            cout << " ";
        }

        cout << "    FOUNDATIONS: ";
        for (int i = 0; i < 4; i++)
        {
            if (foundations[i].isEmpty())
            {
                cout << "[  ] ";
            }
            else
            {
                foundations[i].front().display();
                cout << " ";
            }
        }

        cout << "\n\nTABLEAU:" << endl;
        cout << "  1    2    3    4    5    6    7    8" << endl;
        cout << "----------------------------------------" << endl;

        cout << "TOP: ";
        for (int i = 0; i < 8; i++)
        {
            if (tableau[i].isEmpty())
            {
                cout << "[  ] ";
            }
            else
            {
                tableau[i].front().display();
                cout << " ";
            }
        }

        cout << "\nSIZE:";
        for (int i = 0; i < 8; i++)
        {
            cout << " (" << tableau[i].size() << ")  ";
        }

        cout << "\n\n LIMITATION: Only TOP card of each column visible!" << endl;
    }

    void moveCard(int from, char fromType, int to, char toType)
    {
        Card card;

        fromType = toupper(fromType);
        toType = toupper(toType);

        if (fromType != 'T' && fromType != 'F')
        {
            cout << "Invalid source type! Use 'T' for Tableau or 'F' for FreeCell." << endl;
            return;
        }

        if (toType != 'T' && toType != 'F' && toType != 'O')
        {
            cout << "Invalid destination type! Use 'T' (Tableau), 'F' (FreeCell), or 'O' (Foundation)." << endl;
            return;
        }

        if (fromType == 'T')
        {
            if (from < 1 || from > 8)
            {
                cout << " Invalid tableau column! Must be between 1-8." << endl;
                return;
            }
            if (tableau[from - 1].isEmpty())
            {
                cout << " Tableau column " << from << " is empty!" << endl;
                return;
            }
            card = tableau[from - 1].dequeue();
        }
        else if (fromType == 'F')
        {
            if (from < 1 || from > 4)
            {
                cout << "Invalid free cell! Must be between 1-4." << endl;
                return;
            }
            if (freeCells[from - 1].isEmpty())
            {
                cout << "Free cell " << from << " is empty!" << endl;
                return;
            }
            card = freeCells[from - 1];
            freeCells[from - 1] = Card();
        }

        if (toType == 'T')
        {
            if (to < 1 || to > 8)
            {
                cout << "Invalid destination tableau! Must be between 1-8." << endl;

                if (fromType == 'T')
                    tableau[from - 1].enqueue(card);
                else
                    freeCells[from - 1] = card;
                return;
            }

            Card target = tableau[to - 1].isEmpty() ? Card() : tableau[to - 1].front();

            if (canPlaceOnTableau(card, target))
            {
                tableau[to - 1].enqueue(card);
                moves++;
                cout << "Card moved successfully to Tableau " << to << "!" << endl;
            }
            else
            {
                cout << "Invalid move! Card must be opposite color and one rank lower." << endl;

                if (fromType == 'T')
                    tableau[from - 1].enqueue(card);
                else
                    freeCells[from - 1] = card;
            }
        }
        else if (toType == 'F')
        {
            if (to < 1 || to > 4)
            {
                cout << "Invalid free cell! Must be between 1-4." << endl;
                if (fromType == 'T')
                    tableau[from - 1].enqueue(card);
                else
                    freeCells[from - 1] = card;
                return;
            }

            if (freeCells[to - 1].isEmpty())
            {
                freeCells[to - 1] = card;
                moves++;
                cout << "Card moved to Free Cell " << to << "!" << endl;
            }
            else
            {
                cout << "Free cell " << to << " is already occupied!" << endl;
                if (fromType == 'T')
                    tableau[from - 1].enqueue(card);
                else
                    freeCells[from - 1] = card;
            }
        }
        else if (toType == 'O')
        {
            if (to < 1 || to > 4)
            {
                cout << "Invalid foundation! Must be between 1-4." << endl;
                if (fromType == 'T')
                    tableau[from - 1].enqueue(card);
                else
                    freeCells[from - 1] = card;
                return;
            }

            if (canPlaceOnFoundation(card, to - 1))
            {
                foundations[to - 1].enqueue(card);
                moves++;
                cout << " Card moved to Foundation " << to << "!" << endl;
            }
            else
            {
                cout << "Invalid foundation move! Must build Ace→King in same suit." << endl;
                if (fromType == 'T')
                    tableau[from - 1].enqueue(card);
                else
                    freeCells[from - 1] = card;
            }
        }
    }

    bool checkWin()
    {
        for (int i = 0; i < 4; i++)
        {
            if (foundations[i].size() != 13)
            {
                return false;
            }
        }
        return true;
    }

    void play()
    {
        char choice;

        while (true)
        {
            display();

            if (checkWin())
            {
                cout << "\n CONGRATULATIONS! YOU WON! " << endl;
                cout << "Total moves: " << moves << endl;
                break;
            }

            cout << "\nCOMMANDS:" << endl;
            cout << "M - Move card | Q - Quit" << endl;
            cout << "\nEnter command: ";
            cin >> choice;

            if (cin.fail())
            {
                cout << " Invalid input! Please enter a character." << endl;
                clearInputBuffer();
                continue;
            }

            choice = toupper(choice);

            if (choice == 'Q')
            {
                cout << "Thanks for playing! Final moves: " << moves << endl;
                break;
            }

            if (choice == 'M')
            {
                int from, to;
                char fromType, toType;

                cout << "\nMove FROM (T=Tableau, F=FreeCell): ";
                cin >> fromType >> from;

                if (cin.fail())
                {
                    cout << "Invalid input! Expected format: T 3 or F 1" << endl;
                    clearInputBuffer();
                    continue;
                }

                cout << "Move TO (T=Tableau, F=FreeCell, O=Foundation): ";
                cin >> toType >> to;

                if (cin.fail())
                {
                    cout << " Invalid input! Expected format: T 5, F 2, or O 1" << endl;
                    clearInputBuffer();
                    continue;
                }

                moveCard(from, fromType, to, toType);
            }
            else
            {
                cout << "Invalid command! Use 'M' to move or 'Q' to quit." << endl;
            }
        }
    }
};

int main()
{
    cout << "\n"
         << endl;
    cout << "\n"
         << endl;
    cout << " FREECELL - Queue Implementation      " << endl;
    cout << "\n"
         << endl;
    cout << "\nLIMITATIONS:                      " << endl;
    cout << "  Only TOP card visible per column   " << endl;
    cout << "  Single card moves only            " << endl;
    cout << "  No sequence moves                   " << endl;
    cout << "  Much harder than standard FreeCell  " << endl;
    cout << "\n"<< endl;

    cout << "HOW TO PLAY:" << endl;
    cout << "1. Move cards: T (Tableau) or F (FreeCell) + number" << endl;
    cout << "2. Example: 'T 3' means Tableau column 3" << endl;
    cout << "3. Foundations (O): Build Ace--->King same suit" << endl;
    cout << "4. Tableau: Alternate colors, descending rank" << endl;

    cout << "\nPress ENTER to start...";
    cin.ignore();
    cin.get();

    FreeCell game;
    game.play();

    return 0;
}