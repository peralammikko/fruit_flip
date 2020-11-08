#include "queue.hh"
#include <iostream>
using namespace std;

// Implement the member functions of Queue here

Queue::Queue(unsigned int cycle)
{
    cycle_ = cycle;
}

Queue::~Queue()
{
    Vehicle* currentCar = first_;
    while (first_ != nullptr) {
        Vehicle* nextCar = currentCar->next;
        delete currentCar;
        currentCar = nextCar;
    }
}

void Queue::enqueue(string reg)
{
    Vehicle* newCar = new Vehicle{reg, nullptr};
    newCar->reg_num = reg;
    if (first_ == nullptr) {
        first_ = newCar;
        last_ = newCar;
        carsinQueue_++;
    } else {
        last_->next = newCar;
        last_ = newCar;
        carsinQueue_++;
    }
    if (is_green_ == true) {
        cout << "GREEN: The vehicle " << newCar->reg_num << " need not stop to wait" << endl;
        dequeue();
    }
}

void Queue::dequeue()
{
    Vehicle* temp = first_->next;
    delete first_;
    first_ = temp;
    passableCars_--;
    carsinQueue_--;
    if (passableCars_ == 0) {
        is_green_ = false;
    }

}

void Queue::switch_light()
{
    passableCars_ = cycle_;
    // RED -> GREEN
    if (is_green_ == false) {
        is_green_ = true;
        cout << "GREEN: ";
            if (first_ == nullptr) {
                cout << "No vehicles waiting in traffic lights" << endl;
            } else {
                cout << "Vehicle(s) ";
                while (passableCars_ != 0) {
                    cout << first_->reg_num << " ";
                    dequeue();
                    if (carsinQueue_ == 0) {
                        is_green_ = false;
                        break;
                    }
                }
                cout << "can go on" << endl;
            }
    // GREEN -> RED
    } else {
        is_green_ = false;
        passableCars_ = cycle_;
        print();
    }
}

void Queue::reset_cycle(unsigned int cycle)
{
    cycle_ = cycle;
}

void Queue::print()
{
    Vehicle* printCar = first_;
    // RED
    if (is_green_ == false) {
        cout << "RED: ";
        if (first_ == nullptr) {
            cout << "No vehicles waiting in traffic lights" << endl;
        } else {
            cout << "Vehicle(s) ";
            while (printCar != nullptr) {
                cout << printCar->reg_num << " ";
                printCar = printCar->next;
            }
            cout << "waiting in traffic lights" << endl;
        }
    // GREEN
    } else {
        cout << "GREEN: ";
        if (first_ == nullptr) {
            cout << "No vehicles waiting in traffic lights" << endl;
        }
    }
}
