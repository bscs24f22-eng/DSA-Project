
#include <iostream>
#include <limits>
using namespace std;

template <typename T>
class Node
{
public:
    T data;
    Node* next;
    Node(T val) : data(val), next(nullptr) {}
};

template <typename T>
class Queue
{
private:
    Node<T>* frontNode;
    Node<T>* rearNode;
    int count;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

    ~Queue()
    {
        while (!isEmpty())
            dequeue();
    }

    void enqueue(T val)
    {
        Node<T>* newNode = new Node<T>(val);
        if (isEmpty())
            frontNode = rearNode = newNode;
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

        Node<T>* temp = frontNode;
        T val = temp->data;
        frontNode = frontNode->next;

        if (!frontNode)
            rearNode = nullptr;

        delete temp;
        count--;
        return val;
    }

    T front() const
    {
        if (isEmpty())
            return T();
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
        if (rank == 'A') return 1;
        if (rank >= '2' && rank <= '9') return rank - '0';
        if (rank == 'T') return 10;
        if (rank == 'J') return 11;
        if (rank == 'Q') return 12;
        if (rank == 'K') return 13;
        return 0;
    }

    bool isRed() const
    {
        return suit == 'H' || suit == 'D';
    }

    void display() const
    {
        if (isEmpty())
            cout << "[  ]";
        else
            cout << "[" << rank << suit << "]";
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
        char ranks[] = {'A','2','3','4','5','6','7','8','9','T','J','Q','K'};
        char suits[] = {'H','D','C','S'};
        int idx = 0;

        for (int s = 0; s < 4; s++)
            for (int r = 0; r < 13; r++)
                deck[idx++] = Card(ranks[r], suits[s]);

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
        int idx = 0;

        for (int c = 0; c < 4; c++)
            for (int i = 0; i < 7; i++)
                tableau[c].enqueue(deck[idx++]);

        for (int c = 4; c < 8; c++)
            for (int i = 0; i < 6; i++)
                tableau[c].enqueue(deck[idx++]);
    }

    bool canPlaceOnTableau(Card card, Card target)
    {
        if (target.isEmpty()) return true;
        if (card.getRankValue() != target.getRankValue() - 1) return false;
        if (card.isRed() == target.isRed()) return false;
        return true;
    }

    bool canPlaceOnFoundation(Card card, int idx)
    {
        if (foundations[idx].isEmpty())
            return card.rank == 'A';

        Card top = foundations[idx].front();
        return card.suit == top.suit &&
               card.getRankValue() == top.getRankValue() + 1;
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
            freeCells[i] = Card();
        dealCards();
    }

    void display()
    {
        cout << "\nFREECELL (Queue-Based)\n";
        cout << "Moves: " << moves << "\n\n";

        cout << "FREE CELLS: ";
        for (int i = 0; i < 4; i++) freeCells[i].display(), cout << " ";

        cout << "\nFOUNDATIONS: ";
        for (int i = 0; i < 4; i++)
            foundations[i].isEmpty() ? cout << "[  ] " : foundations[i].front().display(), cout << " ";

        cout << "\n\nTABLEAU TOP:\n";
        for (int i = 0; i < 8; i++)
            tableau[i].isEmpty() ? cout << "[  ] " : tableau[i].front().display(), cout << " ";

        cout << "\n\nLIMITATION: Only TOP card visible\n";
    }

    void moveCard(int from, char fType, int to, char tType)
    {
        Card card;
        fType = toupper(fType);
        tType = toupper(tType);

        if (fType == 'T')
            card = tableau[from - 1].dequeue();
        else
        {
            card = freeCells[from - 1];
            freeCells[from - 1] = Card();
        }

        if (tType == 'T')
        {
            Card target = tableau[to - 1].isEmpty() ? Card() : tableau[to - 1].front();
            if (canPlaceOnTableau(card, target))
                tableau[to - 1].enqueue(card), moves++;
            else
                tableau[from - 1].enqueue(card);
        }
        else if (tType == 'F')
        {
            if (freeCells[to - 1].isEmpty())
                freeCells[to - 1] = card, moves++;
            else
                tableau[from - 1].enqueue(card);
        }
        else if (tType == 'O')
        {
            if (canPlaceOnFoundation(card, to - 1))
                foundations[to - 1].enqueue(card), moves++;
            else
                tableau[from - 1].enqueue(card);
        }
    }

    bool checkWin()
    {
        for (int i = 0; i < 4; i++)
            if (foundations[i].size() != 13)
                return false;
        return true;
    }

    void play()
    {
        char ch;
        while (true)
        {
            display();
            if (checkWin()) break;

            cout << "\nM: Move | Q: Quit → ";
            cin >> ch;
            if (toupper(ch) == 'Q') break;

            int f, t;
            char ft, tt;
            cin >> ft >> f >> tt >> t;
            moveCard(f, ft, t, tt);
        }
    }
};

int main()
{
    cout << "FREECELL - Queue Implementation\n";
    cout << "LIMITATIONS:\n";
    cout << "Only top card visible\nSingle-card moves\n\n";

    cin.get();
    FreeCell game;
    game.play();
    return 0;
}