#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename T>
class Node
{
private:
    vector<T> content;
    vector<Node<T> *> connectedTo;
    int thisNode;
    bool checked = false; //for spanning tree
public:
    static int allNodes;
    Node()
    {
        thisNode = allNodes;
        allNodes++;
        T newContent;
        cin >> newContent;
        content.push_back(newContent);
    }
    int makeEdge()
    {
        int connectTo;
        string input;
        if (thisNode > 0)
        {
            cout << "Connect node?(y/n)\n";
            cin >> input;
            if (input == "y")
            {
                cout << "Connect with which node? 0 - " << allNodes-1 << "\n";
                cin >> connectTo;
                if (connectTo < allNodes)
                {
                    bool check = false;
                    for (int i = 0; i < connectedTo.size(); i++)
                        if (connectedTo[i]->getNumber() == connectTo)
                        {
                            cout << "Nodes are connected\n";
                            check = true;
                            break;
                        }

                    if (!check)
                    {
                        return connectTo;
                    }

                }
                else

                cout << "Node does not exist\n";
            }
            else

            if (input == "n")
                return -1;

            cout << "Incorrect input, try again\n";
            return -2;
        }
    }
    void connection(Node<T> *connecting)
    {
        connectedTo.push_back(connecting);
    }
    void spanningTree()
    {
        if (!checked)
        {
            if (thisNode == 0)
            cout << thisNode;

            else
            cout << " - " << thisNode;
            checked = true;
            for (int i = 0; i < connectedTo.size(); i++)
            connectedTo[i]->spanningTree();
        }
    }
    void connections()
    {
        for (int i = 0; i < connectedTo.size(); i++)
            cout << " #" << connectedTo[i]->getNumber();
        cout << "\n";
    }
    int getNumber()
    {
        return thisNode;
    }
    friend ostream& operator<<(ostream& os, Node<T> &out)
    {
        os << out.content[0] << "\n";
        for (int i = 1; i < out.content.size(); i++)
            os << "   " << out.content[i] << "\n";
        os << "Connected to:";
        out.connections();
        return os;
    }
};

template <>
class Node<Book>
{
    private:
    vector<Book> content;
    vector<Node<Book> *> connectedTo;
    int thisNode;
    bool checked = false; //for spanning tree
public:
    static int allNodes;
    Node()
    {
        thisNode = allNodes;
        allNodes++;
        Book newContent;
        newContent.newBook(); //different here, operator >> overload didn't work for some reason
        content.push_back(newContent);
    }
    int characterAmountGet()
    {
        return content[0].characters.size();
    }
    string characterNameGet(int j)
    {
        return content[0].characters[j]->getName();
    }
    bool characterCompare(string comparingChar)
    {
        for (int i = 0; i < content.size(); i++)
        {
            for (int j = 0; j < content[i].characters.size(); j++)
            {
                if (content[i].characters[j]->getName() == comparingChar)
                    return true;
            }
        }
        return false;
    }
    Book getBook()
    {
        return content[0];
    }
    void addBook(Book addBook)
    {
        content.push_back(addBook);
    }
    int makeEdge()
    {
        int connectTo;
        string input;
        if (thisNode > 0)
        {
            cout << "Connect node?(y/n)\n";
            cin >> input;
            if (input == "y")
            {
                cout << "Connect with which node? 0 - " << allNodes - 2 << "\n";
                cin >> connectTo;
                if (connectTo < allNodes - 1)
                {
                    bool check = false;
                    for (int i = 0; i < connectedTo.size(); i++)
                        if (connectedTo[i]->getNumber() == connectTo)
                        {
                            cout << "Nodes are connected\n";
                            check = true;
                            return -2;
                        }

                    if (!check)
                    {
                        return connectTo;
                    }

                }
                else
                {
                    cout << "Node does not exist\n";
                    return -2;
                }
            }
            else

            if (input == "n")
                return -1;
            else
            {
                cout << "Incorrect input, try again\n";
                return -2;
            }
        }
        return -1;
    }
    void connection(Node<Book> *connecting)
    {
        connectedTo.push_back(connecting);
    }
    void sortSeries()
    {
        Book buffer;

        for (int i = 0; i < content.size(); i++)
            for (int j = i; j < content.size(); j++)

                if (content[i] < content[j])
                {
                    buffer = content[i];
                    content[i] = content[j];
                    content[j] = buffer;
                }
    }
    void spanningTree()
    {
        if (!checked)
        {
            if (thisNode == 0)
            cout << thisNode;

            else
            cout << " - " << thisNode;
            checked = true;
            for (int i = 0; i < connectedTo.size(); i++)
            connectedTo[i]->spanningTree();
        }
    }
    void connections()
    {
        for (int i = 0; i < connectedTo.size(); i++)
            cout << " #" << connectedTo[i]->getNumber();
        cout << "\n";
    }
    int getNumber()
    {
        return thisNode;
    }
    friend ostream& operator<<(ostream& os, Node<Book> &out)
    {
        os << out.content[0] << "\n";
        for (int i = 1; i < out.content.size(); i++)
            os << "   " << out.content[i] << "\n";
        os << "Connected to:";
        out.connections();
        return os;
    }
};
template <typename T>
int Node<T>::allNodes;
template <>
int Node<Book>::allNodes;
