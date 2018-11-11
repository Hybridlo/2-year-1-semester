#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Book;

class Character
{
private:
    string name;
    int importance = 0;
public:
    void newChar(string charName)
    {
        name = charName;
        cout << "Enter character importance (0 - main, 1 - secondary, 2 - background): ";
        while (true)
        {
            cin >> importance;
            if (importance < 0 || importance > 2)
            {
                cout << "Wrong input, enter valid importance";
                importance = 0;
            }
            else
                break;
        }
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
public:
    vector<Character*> characters;
    void addChar(string charName)
    {
        Character *newChar = new Character;
        newChar->newChar(charName);
        characters.push_back(newChar);
    }
    void newBook()
    {
        cout << "Enter name of book: ";
        cin >> name;
        cout << "Author: ";
        cin >> author;
        cout << "Release date: ";
        cin >> day;
        cout << "Month: ";
        cin >> month;
        cout << "Year: ";
        cin >> year;
        cout << "Amount of pages: ";
        cin >> pages;
        cout << "Short annotation: ";
        cin >> annotation;
        cout << "Enter characters, when done enter  \"0\":\n";
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

        if (day >= comDay)
            return false;
    }
    friend ostream& operator<<(ostream& os, const Book &out)
    {
        os << " " << out.name << " ";
        return os;
    }
};
