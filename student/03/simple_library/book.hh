#ifndef BOOK_HH
#define BOOK_HH
#include "date.hh"
#include <string>
#include <iostream>

using namespace std;

class Book
{
public:
    Book();
    Book(string author, string name);
    void print();
    void loan(Date pvm);
    void renew();
    void give_back();

private:
    string author_;
    string name_;
    bool lainassa_;
    Date lainauspvm_;
    Date palautuspvm_;

};

#endif // BOOK_HH
