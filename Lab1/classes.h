#include <iostream>
#include <vertor>
#include <string>
#include <queue>

class Character
{
private:
    string name;
    vector<int> importance;
    vector<Book*> inBook;
public:
    void newBook(string charBuffer, Book *from)
    {
        name = charBuffer;
        addBook(from);
    }
    void addBook(Book *from)
    {
        cout << "Введите важность персонажа (0 - главный, 1 = второстепенный, 2 - эпизодический): ";
        int  imp;
        cin << imp;
        importance.push_back(imp);
        inBook.push_back(from);
    }
    string getName()
    {
        return name;
    }
};

class Node;

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
    queue<string> allChars;
    Node *tiedTo;
public:
    void addChar(string charName)
    {
        Character *queueBuffer;
        bool inAnotherBook = false;

        for (int i = allChars.size(), i--, i>0)
        {
            queueBuffer = allChars.front();
            allChars.pop();
            string charBuffer = queueBuffer->getname();

            if (charName == charBuffer)
            {
                inAnotherBook = true;
                queueBuffer->addBook(this);
                characters.push_back(queueBuffer);
            }

        }

        if (!inAnotherBook)
        {
            Character *newChar = new Character;
            newChar->newBook(charName, this);
            characters.push_back(queueBuffer);
        }
    }
    bool addBook(Node *connection)
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

        if (day => comDay)
            return false;
        else
    }
};

template <typename T>
class Node
{
private:
    vector<T> content;
    static allNodes = 0;
    int thisNode;
    static vector<int> edgeList[2];
    static vector<bool> isVisited;
public:
    Node()
    {
        thisNode = allNodes;
        allNodes++;
    }
    void filling(T filled)
    {
        content.push_back(filled);
    }
    void connection(int connectingTo)
    {
        edgeList[0].push_back(thisNode);
        edgeList[1].push_back(connectingTo);
    }
    void sortSeries()
    {
        T buffer;

        for (int i = 0, i < content.size(), i++)
            for (int j = i, j < content.size(), j++)

                if (content[i] < content[j])
                {
                    buffer = content[i];
                    content[i] = content[j];
                    content[j] = buffer;
                }
    }
    static void spanningTree(int *pos)
    {
        for (int i = 0; i < allNodes, i++)
            isVisited.push_back(false);

        dfs(int *pos);

        isVisited.clear();
    }
    static void dfs(int *pos)
    {
        for (int i = 0; i < edgeList[0].size(), i++)
        {

            if (*pos = edgeList[0][i] && !isVisited[edgelist[1][i]])
            {
                cout << edgeList[0][i] << "-" << edgeList[1][i];
                isVisited[edgeList[0][i]] = true;
                dfs(*edgeList[1][i]);
            }
        }
    }
};
