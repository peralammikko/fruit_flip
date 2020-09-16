#include <cstdlib>
#include <iostream>
#include <vector>


using namespace std;


bool same_values(vector<int> integers) {
    int koko = integers.size();
    int i = 0;
    while (i < koko-1) {
        if (integers.at(i) == integers.at(i+1)) {
            i++;
        } else {
            return false;
        }
    } return true;
}

bool is_ordered_non_strict_ascending(vector<int> integers) {
    int koko = integers.size();
    int i = 0;
    while (i < koko-1) {
        if (integers.at(i) <= integers.at(i+1)) {
            i++;
        } else {
            return false;
        }
    } return true;
}

bool is_arithmetic_series(vector<int> integers) {
    int koko = integers.size();
    int i = 0;
    while (i < koko-2) {
        int erotus1 = integers.at(i+1) - integers.at(i);
        int erotus2 = integers.at(i+2) - integers.at(i+1);
        if (erotus1 == erotus2)
            i++;
        else
            return false;
    }
    return true;
}

bool is_geometric_series(vector<int> integers) {
    int koko = integers.size();
    int i = 0;
    int sum = 0;
    for (int alkio : integers) {
        sum += integers.at(alkio);
    }
    if (sum == 0)
        return false;
    while (i < koko-2) {
        int jako1 = integers.at(i+1) / integers.at(i);
        int jako2 = integers.at(i+2) / integers.at(i+1);
        if (jako1 == jako2)
            i++;
        else
            return false;
    }
    return true;
}

void read_integers(std::vector< int >& ints, int count)
{
    int new_integer = 0;
    for(int i = 0; i < count; ++i)
    {
        std::cin >> new_integer;
        ints.push_back(new_integer);
    }
}

// TODO: Implement your solution here


int main()
{
    std::cout << "How many integers are there? ";
    int how_many = 0;
    std::cin >> how_many;

    std::cout << "Enter the integers: ";
    std::vector<int> integers;
    read_integers(integers, how_many);

    if(same_values(integers))
        std::cout << "All the integers are the same" << std::endl;
    else
        std::cout << "All the integers are not the same" << std::endl;

    if(is_ordered_non_strict_ascending(integers))
        std::cout << "The integers are in a non-strict ascending order" << std::endl;
    else
        std::cout << "The integers are not in a non-strict ascending order" << std::endl;

    if(is_arithmetic_series(integers))
        std::cout << "The integers form an arithmetic series" << std::endl;
    else
        std::cout << "The integers do not form an arithmetic series" << std::endl;

    if(is_geometric_series(integers))
        std::cout << "The integers form a geometric series" << std::endl;
    else
        std::cout << "The integers do not form a geometric series" << std::endl;

    return EXIT_SUCCESS;
}
