#include <classes.h>
#include <clocale>

using namespace std;

void dfs(int pos, vector<int> *edgeList, vector<bool> isVisited, int *allNodes)
{
    for (int i = 0; i < edgeList[0].size(); i++)
    {

        if (pos == edgeList[0][i] && !isVisited[edgeList[1][i]])
        {
            cout << edgeList[0][i] << "-" << edgeList[1][i] << "\n";
            isVisited[edgeList[0][i]] = true;
            dfs(edgeList[1][i], edgeList, isVisited, allNodes);
            break;
        }
        else
        if (pos == edgeList[1][i] && !isVisited[edgeList[0][i]])
        {
            cout << edgeList[1][i] << "-" << edgeList[0][i] << "\n";
            isVisited[edgeList[1][i]] = true;
            dfs(edgeList[0][i], edgeList, isVisited, allNodes);
            break;
        }
    }
}
void spanningTree(int pos, vector<int> *edgeList, vector<bool> isVisited, int *allNodes)
{
    for (int i = 0; i < *allNodes; i++)
        isVisited.push_back(false);

    isVisited[pos] = true;

    dfs(pos, edgeList, isVisited, allNodes);

    isVisited.clear();
}

int main()
{
    Node<Book>::allNodes = 0;  //change
    int *pAllNodes = &Node<Book>::allNodes; //change
    vector<Node<Book> > vertices;
    vector<int> edgeList[2];
    vector<bool> isVisited;
    string input;
    setlocale(LC_ALL, "Russian");

    cout << "Введите \"new\" для создания узла, \"out\" для вывода вершин и связей, \"graph\" для вывода узлов остовного дерева, \"exit\" для выхода из программы\n";
    cin >> input;

    int i = 0;
    while (input != "exit")
    {
        if (input == "graph")
        {
            int pos = 0;
            spanningTree(pos, edgeList, isVisited, pAllNodes);
        }
        else

        if (input == "out")
            for (int k = 0; k < *pAllNodes; k++)
            {
                cout << "#" << k << " " << vertices[k] << "Связан с:";
                for (int j = 0; j < *pAllNodes-1; j++)
                if (edgeList[0][j] == k)
                    cout << " #" << edgeList[1][j];
                else

                if (edgeList[1][j] == k)
                    cout << " #" << edgeList[0][j];
                cout << "\n";
            }
        else

        if (input == "new")
        {
            Node<Book> newNode; //change
            vertices.push_back(newNode);
            Book i0; //change
            cout << "Введите информацию вершины: \n";
            /*cin >> i0;*/                                  //here if anything except Book
            Node<Book> *temp = i0.addBook(&vertices[i]);    //for Book
            if (temp != &vertices[i])                       //for Book
            {                                               //for Book
                vertices.pop_back();                        //for Book
                i--;                                        //for Book
                *pAllNodes = *pAllNodes - 1;                //for Book
                cout << "Новая книга серии\n";              //for Book
            }                                               //for Book
            vertices[i].filling(i0);

            bool exit = false;
            int connect;
            if (i > 0)
            while (!exit)
            {
                cout << "Связать вершину?(y/n)\n";
                cin >> input;
                if (input == "y")
                {
                    cout << "С какой вершиной связать? 0 - " << i-1 << "\n";
                    cin >> connect;
                    if (connect < i)
                    {
                        bool check = false;
                        for (int j = 0; j < edgeList[0].size(); j++)
                            if ((edgeList[0][j] == connect && edgeList[1][j] == i) || (edgeList[0][j] == i && edgeList[1][j] == connect))
                            {
                                cout << "Вершины уже связаны\n";
                                check = true;
                                break;
                            }

                        if (!check)
                        {
                            edgeList[0].push_back(i);
                            edgeList[1].push_back(connect);
                        }

                    }
                    else

                    cout << "Некорректная вершина, пробуем сначала\n";
                }
                else

                if (input == "n")
                    exit = true;
                else

                cout << "Некорректный ввод, начинаем сначала\n";
            }
            i++;
        }
        else
        {
            cout << "Некорректный ввод, попробуйте ещё раз";
            return 0;
        }

        cout << "Введите \"new\" для создания узла, \"out\" для вывода вершин и связей, \"graph\" для вывода узлов остовного дерева, \"exit\" для выхода из программы\n";
        cin >> input;
    }
}
