#include <iostream>
#include <vertor>
#include <string>
#include <queue>

class Character
{
private:
    string name;
    vector<int> importance;
    vector<*Book> inBook;
public:
    void newBook(string charBuffer, Book *from)
    {
        name = charBuffer;
        addBook(from);
    }
    void addBook(Book *from)
    {
        cout << "������� �������� ��������� (0 - �������, 1 = ��������������, 2 - �������������):";
        int  imp;
        cin << imp;
        importance.insert(importance.end(), imp);
        inBook.insert(inBook.end(), from);
    }
    string getName()
    {
        return name;
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
    vector<*Character> characters;
    queue<string> allChars;
public:
    void addChar(string charName)
    {
        *Character queueBuffer;
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
                characters.insert(characters.end(), queueBuffer);
            }

        }

        if (!inAnotherBook)
        {
            Character *newChar = new Character;
            newChar->newBook(charName, this);
            characters.insert(characters.end(), queueBuffer);
        }
    }
    bool addBook()
    {
        cout << "������� �������� �����:";
        cin >> name;
        cout << "������:";
        cin >> author;
        cout << "���� �������";
        cin >> day;
        cout << "�����";
        cin >> month;
        cout << "���";
        cin >> year;
        cout << "���-�� ������� � �����";
        cin >> pages;
        cout << "�������� ���������";
        cin >> annotation;
        cout << "������� ����������, ����� ���������, ������� \"0\":";
        cin >> charName;

        while (charName !="0")
        {
            addChar(charName);
            cin >> charName;
        }
    }
};
