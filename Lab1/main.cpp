#include <classes.h>
#include <clocale>

using namespace std;



int main()
{
    Node<int>::allNodes = 0;
    Node<double>::allNodes = 0;
    Node<string>::allNodes = 0;
    Node<Book>::allNodes = 0;
    string input, type;
    setlocale(LC_ALL, "Russian");
    vector<Node<int> > vertices0;
    vector<Node<double> > vertices1;
    vector<Node<string> > vertices2;
    vector<Node<Book> > vertices3;
    cout << "������� ��� ��������� ����, 0 - ����� �����, 1 - ������� �����, 2 - ������, 3 - �����\n";
    cin >> type;
    if (type != "0" && type != "1" && type != "2" && type != "3")
    {
        cout << "������������ ����, ����� ���������";
        return 0;
    }

    cout << "������� \"new\" ��� �������� ����, \"graph\" ��� ������ ����� ��������� ������, \"exit\" ��� ������ �� ���������\n";
    cin >> input;

    int i = 0;
    int pos = 0;
    if (type == "0")
    while (input != "exit")
    {
        if (input == "graph")
            Node<int>::spanningTree(pos);
        else

        if (input == "new")
        {
            Node<int> newNode0;
            vertices0.push_back(newNode0);
            int i0;
            Node<int>::allNodes++;
            cout << "������� �����: ";
            cin >> i0;
            vertices0[i].filling(i0);

            bool exit = false;
            int connect;
            if (i > 0)
            while (!exit)
            {
                cout << "������� �������?(y/n)\n";
                cin >> input;
                if (input == "y")
                {
                    cout << "� ����� �������� �������? 0 - " << i-1 << "\n";
                    cin >> connect;
                    if (connect < i)
                    {
                        for (int j = 0; j < Node<int>::edgeList[0].size(); j++)
                            if ((Node<int>::edgeList[0][j] == connect && Node<int>::edgeList[1][j] == i) || (Node<int>::edgeList[0][j] == i && Node<int>::edgeList[1][j] == connect))
                            {
                                cout << "������� ��� �������\n";
                                break;
                            }

                        Node<int>::edgeList[0].push_back(i);
                        Node<int>::edgeList[1].push_back(connect);
                    }
                    else

                    cout << "������������ �������, ������� �������\n";
                }
                else

                if (input == "n")
                    exit = true;
                else

                cout << "������������ ����, �������� �������\n";
            }
            i++;
        }
        else
        {
            cout << "������������ ����, ���������� ��� ���";
            return 0;
        }

        cout << "������� \"new\" ��� �������� ����, \"graph\" ��� ������ ����� ��������� ������, \"exit\" ��� ������ �� ���������\n";
        cin >> input;
    }

    if (type == "1")
    while (input != "exit")
    {
        if (input == "graph")
            Node<double>::spanningTree(pos);
        else

        if (input == "new")
        {
            Node<double> newNode1;
            vertices1.push_back(newNode1);
            double i1;
            Node<double>::allNodes++;
            cout << "������� ����� ����� �����: ";
            cin >> i1;
            vertices1[i].filling(i1);

            bool exit = false;
            int connect;
            if (i > 0)
            while (!exit)
            {
                cout << "������� �������?(y/n)\n";
                cin >> input;
                if (input == "y")
                {
                    cout << "� ����� �������� �������? 0 - " << i-1 << "\n";
                    cin >> connect;
                    if (connect < i)
                    {
                        for (int j = 0; j < Node<double>::edgeList[0].size(); j++)
                            if ((Node<double>::edgeList[0][j] == connect && Node<double>::edgeList[1][j] == i) || (Node<double>::edgeList[0][j] == i && Node<double>::edgeList[1][j] == connect))
                            {
                                cout << "������� ��� �������\n";
                                break;
                            }

                        Node<double>::edgeList[0].push_back(i);
                        Node<double>::edgeList[1].push_back(connect);
                    }
                    else

                    cout << "������������ �������, ������� �������\n";
                }
                else

                if (input == "n")
                    exit = true;
                else

                cout << "������������ ����, �������� �������\n";
            }
            i++;
        }
        else
        {
            cout << "������������ ����, ���������� ��� ���";
            return 0;
        }

        cout << "������� \"new\" ��� �������� ����, \"graph\" ��� ������ ����� ��������� ������, \"exit\" ��� ������ �� ���������\n";
        cin >> input;
    }

    if (type == "2")
    while (input != "exit")
    {
        if (input == "graph")
            Node<string>::spanningTree(pos);
        else

        if (input == "new")
        {
            Node<string> newNode2;
            vertices2.push_back(newNode2);
            string i2;
            Node<string>::allNodes++;
            cout << "������� �����: ";
            cin >> i2;
            vertices2[i].filling(i2);

            bool exit = false;
            int connect;
            if (i > 0)
            while (!exit)
            {
                cout << "������� �������?(y/n)\n";
                cin >> input;
                if (input == "y")
                {
                    cout << "� ����� �������� �������? 0 - " << i-1 << "\n";
                    cin >> connect;
                    if (connect < i)
                    {
                        for (int j = 0; j < Node<string>::edgeList[0].size(); j++)
                            if ((Node<string>::edgeList[0][j] == connect && Node<string>::edgeList[1][j] == i) || (Node<string>::edgeList[0][j] == i && Node<string>::edgeList[1][j] == connect))
                            {
                                cout << "������� ��� �������\n";
                                break;
                            }

                        Node<string>::edgeList[0].push_back(i);
                        Node<string>::edgeList[1].push_back(connect);
                    }
                    else

                    cout << "������������ �������, ������� �������\n";
                }
                else

                if (input == "n")
                    exit = true;
                else

                cout << "������������ ����, �������� �������\n";
            }
            i++;
        }
        else
        {
            cout << "������������ ����, ���������� ��� ���";
            return 0;
        }

        cout << "������� \"new\" ��� �������� ����, \"graph\" ��� ������ ����� ��������� ������, \"exit\" ��� ������ �� ���������\n";
        cin >> input;
    }

    if (type == "3")
    while (input != "exit")
    {
        if (input == "graph")
            Node<Book>::spanningTree(pos);
        else

        if (input == "new")
        {
            Node<Book> newNode3;
            vertices3.push_back(newNode3);
            Book i3;
            Node<Book>::allNodes++;
            Node<Book> *temp = i3.addBook(&vertices3[i]);
            if (temp != &vertices3[i])
            {
                vertices3.pop_back();
                Node<Book>::allNodes--;
                cout << "������� ����� ���������\n";
            }
            temp->filling(i3);

            bool exit = false;
            int connect;
            if (i > 0)
            while (!exit)
            {
                cout << "������� �������?(y/n)\n";
                cin >> input;
                if (input == "y")
                {
                    cout << "� ����� �������� �������? 0 - " << i-1 << "\n";
                    cin >> connect;
                    if (connect < i)
                    {
                        for (int j = 0; j < Node<Book>::edgeList[0].size(); j++)
                            if ((Node<Book>::edgeList[0][j] == connect && Node<Book>::edgeList[1][j] == i) || (Node<Book>::edgeList[0][j] == i && Node<Book>::edgeList[1][j] == connect))
                            {
                                cout << "������� ��� �������\n";
                                break;
                            }

                        Node<Book>::edgeList[0].push_back(i);
                        Node<Book>::edgeList[1].push_back(connect);
                    }
                    else

                    cout << "������������ �������, ������� �������\n";
                }
                else

                if (input == "n")
                    exit = true;
                else

                cout << "������������ ����, �������� �������\n";
            }
            i++;
        }
        else
        {
            cout << "������������ ����, ���������� ��� ���";
            return 0;
        }

        cout << "������� \"new\" ��� �������� ����, \"graph\" ��� ������ ����� ��������� ������, \"exit\" ��� ������ �� ���������\n";
        cin >> input;
    }
}
