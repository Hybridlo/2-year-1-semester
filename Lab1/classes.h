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
    Character *newBook(string charBuffer, Book *from, Node<Book> *fath)
    {
        name = charBuffer;
        father = fath;
        addBook(from);
        return this;
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
    Node<Book> *tiedTo;
public:
    static queue<Character*> allChars;
    void addChar(string charName)
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
                return;
            }

        }

        Character *newChar = new Character;
        queueBuffer = newChar->newBook(charName, this, tiedTo);
        characters.push_back(queueBuffer);
        allChars.push(queueBuffer);
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
    friend ostream& operator<<(ostream& os, const Book &out)
    {
        os << " " << out.name << " ";
        return os;
    }
};

queue<Character*> Book::allChars;

template <typename T>
class Node
{
private:
    vector<T> content;
    int thisNode;
public:
    static int allNodes;
    Node()
    {
        thisNode = allNodes;
        allNodes++;
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
    friend ostream& operator<<(ostream& os, const Node<T> &out)
    {
        os << out.content[0] << "\n";
        for (int i = 1; i < out.content.size(); i++)
            os << "   " << out.content[i] << "\n";
        return os;
    }
};
template <typename T>
int Node<T>::allNodes;
