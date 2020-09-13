#include "account.hh"
#include <iostream>
#include <string>

using namespace std;

Account::Account(const std::string& owner, bool has_credit):
    omistaja_(owner), luotto_(has_credit)
{
    saldo_ = 0;
    generate_iban();

}

int Account::getbalance() const
{
    return saldo_;
}

// Setting initial value for the static attribute running_number_
int Account::running_number_ = 0;

void Account::print() const {
    cout << omistaja_ << " : " << iban_ << " : " << saldo_ << endl;
}

void Account::set_credit_limit(int raja)
{
    if (luotto_ == true) {
        luottoraja_ = raja;
    } else {
        cout << "Cannot set credit limit: the account has no credit card" << endl;
    }
}

bool Account::check_balance(int otto)
{
    if (luotto_ == false) {
        if (otto > saldo_) {
            cout << "Cannot take money: balance underflow" << endl;
            return false;
        } else {
            return true;
        }
    } else {
        if (saldo_ - otto < -luottoraja_) {
            cout << "Cannot take money: credit limit overflow" << endl;
            return false;
        } else {
            return true;
        }
    }
}

void Account::save_money(int pano)
{
    saldo_ = saldo_ + pano;
}

void Account::take_money(int otto)
{
    if (check_balance(otto) == true) {
       saldo_ = saldo_ - otto;
    }
}

void Account::transfer_to(Account ottaja, int siirto)
{
    if (check_balance(siirto) == true) {
        take_money(siirto);
        ottaja.save_money(siirto);
        cout << siirto << " euros taken: new balance of " << ottaja.iban_ << " is " << ottaja.saldo_ << " euros" << endl;
    } else {
        cout << "Transfer from " << iban_ << " failed" << endl;
    }
}

void Account::generate_iban()
{
    ++running_number_;
    std::string suffix = "";
    if(running_number_ < 10)
    {
        suffix.append("0");
    }
    else if(running_number_ > 99)
    {
        std::cout << "Too many accounts" << std::endl;
    }
    suffix.append(std::to_string(running_number_));

    iban_ = "FI00 1234 ";
    iban_.append(suffix);
}
