#include "book.hh"
#include "date.hh"
#include <string>
#include <iostream>

using namespace std;

Book::Book(string author, string name)
{
    author_ = author;
    name_ = name;
    lainassa_ = false;
}

void Book::print()
{
    cout << author_ << " : " << name_ << endl;
    if (lainassa_ == false) {
        cout << "- available" << endl;
    } else if (lainassa_ == true) {
        cout << "- loaned: ";
        lainauspvm_.print();
        cout << "- to be returned: ";
        palautuspvm_.print();
    }
}

void Book::loan(Date pvm)
{
    if (lainassa_ == true) {
        cout << "Already loaned: cannot be loaned" << endl;
        return;
    } else {
        lainassa_ = true;
        lainauspvm_ = pvm;
        pvm.advance(28);
        palautuspvm_ = pvm;
    }
}

void Book::renew()
{
    if (lainassa_ == true) {
        palautuspvm_.advance(28);
    } else {
        cout << "Not loaned: cannot be renewed" << endl;
        return;
    }
}

void Book::give_back()
{
    lainassa_ = false;
}
