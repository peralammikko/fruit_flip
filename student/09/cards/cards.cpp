#include "cards.hh"

using namespace std;

// TODO: Implement the methods here
Cards::Cards(): top_(nullptr) {

}


void Cards::add(int id)
{
    Card_data* newCard = new Card_data;
    newCard->data = id;
    newCard->next = top_;
    top_ = newCard;
}

void Cards::print_from_top_to_bottom(std::ostream &s)
{
    int i = 1;
    Card_data* printCard = top_;
    while (printCard != nullptr) {
        cout << i << ": " << printCard->data << endl;
        i++;
        printCard = printCard->next;
    }
}

bool Cards::remove(int &id)
{
    if (top_ == nullptr) {
        return false;
    }
    id = top_->data;
    Card_data* temp = top_->next;
    delete top_;
    top_ = temp;
}

bool Cards::bottom_to_top()
{
    if (top_ == nullptr) {
        return false;
    }
    if (top_->next == nullptr) {
        return true;
    }

    Card_data* secondLast = nullptr;
    Card_data* last = top_;

    while (last->next != nullptr) {
        secondLast = last;
        last = last->next;
    }

    last->next = top_;
    top_ = last;

    secondLast->next = nullptr;

    return true;

}

bool Cards::top_to_bottom()
{
    if (top_ == nullptr) {
        return false;
    }
    if (top_->next == nullptr) {
        return true;
    }

    Card_data* secondFromTop = top_->next;
    Card_data* last = top_;

    while (last->next != nullptr) {
        last = last->next;
    }

    last->next = top_;
    top_->next = nullptr;

    top_ = secondFromTop;

    return true;

}

void Cards::print_from_bottom_to_top(std::ostream &s)
{
    Card_data* printCard = top_;
    int i = 1;

    while (printCard != nullptr) {
        cout << i << ": " << printCard->data << endl;
        i++;
        printCard = printCard->next;
    }

}

Cards::~Cards()
{
    Card_data* current = top_;

    while (current != nullptr) {
        Card_data* temp = current->next;
        delete current;
        current = temp;
    }
}
