#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

template <typename T>
class Node;

class Book;

class Character
{
private:
    string name;
    vector<int> importance;
    vector<Book*> inBook;
    Node<Book> *father;
public:
    void newBook(string charBuffer, Book *from, Node<Book> *fath)
    {
        name = charBuffer;
        addBook(from);
    }
    void addBook(Book *from)
    {
        cout << "Введите важность персонажа (0 - главный, 1 = второстепенный, 2 - эпизодический): ";
        int  imp;
        cin >> imp;
        importance.push_back(imp);
        inBook.push_back(from);
    }
    string getName()
    {
        return name;
    }
    Node<Book> *getFather()
    {
        return father;
    }
};

class Book
{
private:
    string name;
    string author;
    int day;
    int month;
    int year;
    int pages;
    string annotation;
    string charName;
    vector<Character*> characters;
    queue<Character*> allChars;
    Node<Book> *tiedTo;
public:
    void *addChar(string charName)
    {
        Character *queueBuffer;

        for (int i = allChars.size(); i>0; i--)
        {
            queueBuffer = allChars.front();
            allChars.pop();
            string charBuffer = queueBuffer->getName();
            allChars.push(queueBuffer);

            if (charName == charBuffer)
            {
                queueBuffer->addBook(this);
                tiedTo = queueBuffer->getFather();
                characters.push_back(queueBuffer);
                return 0;
            }

        }

        Character *newChar = new Character;
        newChar->newBook(charName, this, tiedTo);
        characters.push_back(queueBuffer);
    }
    Node<Book> *addBook(Node<Book> *connection)
    {
        tiedTo = connection;
        cout << "Введите название книги: ";
        cin >> name;
        cout << "Автора: ";
        cin >> author;
        cout << "День издания: ";
        cin >> day;
        cout << "Месяц: ";
        cin >> month;
        cout << "Год: ";
        cin >> year;
        cout << "Кол-во страниц в книге: ";
        cin >> pages;
        cout << "Короткая аннотация: ";
        cin >> annotation;
        cout << "Вводите персонажей, когда закончите, введите \"0\":\n";
        cin >> charName;

        while (charName !="0")
        {
            addChar(charName);
            cin >> charName;
        }

        return tiedTo;
    }
    int getYear()
    {
        return year;
    }
    int getMonth()
    {
        return month;
    }
    int getDay()
    {
        return day;
    }
    bool operator <(Book compare)
    {
        int comYear = compare.getYear();
        int comMonth = compare.getMonth();
        int comDay = compare.getDay();

        if (year < comYear)
            return true;
        else

        if (year > comYear)
            return false;
        else

        if (month < comMonth)
            return true;
        else

        if (month > comMonth)
            return false;
        else

        if (day < comDay)
            return true;
        else

        if (day >= comDay)
            return false;
    }
};

template <typename T>
class Node
{
private:
    vector<T> content;
    int thisNode;
public:
    static int allNodes;
    static vector<int> edgeList[2];
    static vector<bool> isVisited;
    Node()
    {
        thisNode = allNodes;
    }
    void filling(T filled)
    {
        content.push_back(filled);
    }
    void sortSeries()
    {
        T buffer;

        for (int i = 0; i < content.size(); i++)
            for (int j = i; j < content.size(); j++)

                if (content[i] < content[j])
                {
                    buffer = content[i];
                    content[i] = content[j];
                    content[j] = buffer;
                }
    }
    static void dfs(int pos)
    {
        for (int i = 0; i < edgeList[0].size(); i++)
        {

            if (pos == edgeList[0][i] && !isVisited[edgeList[1][i]])
            {
                cout << edgeList[0][i] << "-" << edgeList[1][i];
                isVisited[edgeList[0][i]] = true;
                dfs(edgeList[1][i]);
            }
        }
    }
    static void spanningTree(int pos)
    {
        for (int i = 0; i < allNodes; i++)
            isVisited.push_back(false);

        Node::dfs(pos);

        isVisited.clear();
    }
};
template <typename T>
int Node<T>::allNodes;
template <typename T>
vector<int> Node<T>::edgeList[2];
template <typename T>
vector<bool> Node<T>::isVisited;
