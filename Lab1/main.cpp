#include "Books and chars.h"
#include "Node.h"

using namespace std;

int main()
{   //keeping all vertices in case a vertice was not connected so it would be unaccessible otherwise
    vector<Node<Book> *> vertices; //change type here
    string input = "";
    Node<Book>::allNodes = 0;  //change type here
    int *pAllNodes = &Node<Book>::allNodes;  //change type here

    while (input != "exit")
    {
        cout << "Enter \"new\" to create a node, \"out\" to output all nodes and edges, \"graph\" to output a spanning tree, \"exit\" to finish the program\n";
        cin >> input;

        if (input == "graph")
        {
            vertices[0]->spanningTree();
            cout << "\n";
        }
        else

        if (input == "out")
            for (int i = 0; i < vertices.size(); i++)
                cout << "#" << i << " " << *vertices[i];
        else

        if (input == "new")
        {
            vertices.push_back(new Node<Book>); //change type here
            bool found = false;
            for (int i = 0 ; i < vertices.size() - 1; i++)  //comment the whole cycle if type in Node is not Book; checking for characters in other books
            {                                  //- 1 because also excluding last added node
                for (int j = 0; j < vertices[vertices.size()-1]->characterAmountGet(); j++)  //for each character in last added book
                {
                    string charName = vertices[vertices.size()-1]->characterNameGet(j);  //get a character in last added book
                    found = vertices[i]->characterCompare(charName);  //and compare with all characters in other books
                    if (found)
                    {
                        Book temp = vertices[vertices.size()-1]->getBook();  //take out the last added book
                        vertices[i]->addBook(temp);  //and put it to the series
                        vertices.pop_back();
                        cout << "New book of the series\n";
                        *pAllNodes--;
                        break;
                    }
                }
                if (found)
                    break;
            }
            if (!found)
            {
                int connectingTo = vertices[vertices.size()-1]->makeEdge(); //try to make an edge
                while (connectingTo != -1)  //out of cycle if used declined
                {
                    if (connectingTo >=0)  //make an two-ways edge
                    {
                        vertices[vertices.size()-1]->connection(vertices[connectingTo]);
                        vertices[connectingTo]->connection(vertices[vertices.size()-1]);
                    }

                    connectingTo = vertices[vertices.size()-1]->makeEdge();  //try to make an edge again
                }
            }
        }
        else
        {
            cout << "Incorrect input, try again";
            continue;
        }
    }
}
