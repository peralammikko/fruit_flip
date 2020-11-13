#include <iostream>
#include <memory>
#include "cards.hh"

using namespace std;


Cards::Cards(): top_( nullptr ) {
}


void Cards::add(int id) {
    std::shared_ptr<Card_data> new_card 
            = std::make_shared<Card_data>(Card_data{id, top_});
    top_ = new_card;
}

void Cards::print(std::ostream& s) {
   std::shared_ptr<Card_data> to_be_printed = top_;
   int nr = 1;

   while( to_be_printed != 0 ) {
      s << nr << ": " << to_be_printed->data << std::endl;
      to_be_printed = to_be_printed->next;
      ++nr;
   }
}

bool Cards::remove(int &id)
{
    if (top_ == nullptr) {
        return false;
    }
    shared_ptr<Card_data> next_card = top_->next;
    id = next_card->data;
    top_ = next_card;

    return true;
}

void Cards::reverse()
{
    shared_ptr<Card_data> previous = nullptr;
    shared_ptr<Card_data> current_card = top_;

    while (current_card != nullptr) {

        shared_ptr<Card_data> next_card = current_card->next;
        current_card->next = previous;

        previous = current_card;
        current_card = next_card;

    }
    top_ = previous;
}

// Tip for writing code more efficiently:
// Do not write the stubs of the methods remove and reverse by yourself here,
// but open the file cards.hh and click the declaration of the method
// by the right mouse button and select
// Refactor > Add definition in cards.cpp
