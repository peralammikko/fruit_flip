#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
// TODO: Implement split function here
// Do not change main function

vector<string> split(string line, char separator, bool totuus = false) {
    int pituus = line.length();
    string text = "";
    vector<string> merkit;
    int i = 0;
    while (i < pituus) {
        char merkki = line.at(i);
        if (merkki != separator) {
            text += merkki;
            i++;
            if (i == pituus) {
                merkit.push_back(text);
            }
        } else if (merkki == separator) {
            if (i==0) {
                merkit.push_back(" ");
                i++;
            } else if (line.at(i-1) == separator) {
                merkit.push_back(" ");
                i++;
                if (i == pituus) {
                merkit.push_back(" ");
                }
            } else {
                merkit.push_back(text);
                text = "";
                i++;
                if (i == pituus) {
                    merkit.push_back(" "); }
            }
        }
    }
    if (totuus == true) {
            merkit.erase(remove(merkit.begin(),merkit.end(), " "),merkit.end());
        }
    return merkit;
}


int main()
{
    std::string line = "";
    std::cout << "Enter a string: ";
    getline(std::cin, line);
    std::cout << "Enter the separator character: ";
    char separator = getchar();

    std::vector< std::string > parts  = split(line, separator);
    std::cout << "Splitted string including empty parts: " << std::endl;
    for( auto part : parts ) {
        std::cout << part << std::endl;
    }

    std::vector< std::string > parts_no_empty  = split(line, separator, true);
    std::cout << "Splitted string ignoring empty parts: " << std::endl;
    for( auto part : parts_no_empty ) {
        std::cout << part << std::endl;
    }
}
