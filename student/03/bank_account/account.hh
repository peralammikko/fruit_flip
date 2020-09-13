#ifndef ACCOUNT_HH
#define ACCOUNT_HH

#include <string>

using namespace std;

class Account
{
public:
    // Constructor
    Account(const std::string& owner, bool has_credit = false);

    // More methods
    int getbalance() const;

    void print() const;

    void set_credit_limit(int raja);

    bool check_balance(int otto);

    void save_money(int pano);

    void take_money(int otto);

    void transfer_to(Account ottaja, int siirto);

private:
    // Generates IBAN (based on running_number_ below).
    // Allows no more than 99 accounts.
    void generate_iban();
    int saldo_ = 0;
    string omistaja_;
    bool luotto_;
    int luottoraja_;
    string iban_;

    // Used for generating IBAN.
    // Static keyword means that the value of running_number_ is the same for
    // all Account objects.
    // In other words, running_number_ is a class-wide attribute, there is
    // no own copies of it for each object of the class.
    static int running_number_;

    // More attributes/methods

};

#endif // ACCOUNT_HH
