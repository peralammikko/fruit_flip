#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <unordered_set>


using namespace std;

const std::string HELP_TEXT = "S = store id1 i2\nP = print id\n"
                              "C = count id\nD = depth id\n";


std::vector<std::string> split(const std::string& s, const char delimiter, bool ignore_empty = false){
    std::vector<std::string> result;
    std::string tmp = s;

    while(tmp.find(delimiter) != std::string::npos)
    {
        std::string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        if(not (ignore_empty and new_part.empty()))
        {
            result.push_back(new_part);
        }
    }
    if(not (ignore_empty and tmp.empty()))
    {
        result.push_back(tmp);
    }
    return result;
}
int print_count(map<string, vector<string>> v, string id, bool count_self = false) {
    int sum = 0;
    if (count_self)
        sum = 1;
    for (auto sub : v.at(id)) {
        sum += print_count(v, sub, count_self = true);
    }
    return sum;
}

int print_depth(map<string, vector<string>> v, string id) {
    for (auto child : v.at(id)) {
        int depth = 1 + print_depth(v, child);
        return depth;
    }
    return 0;
}

void print_recursive(const map<string, vector<string>>& v, const string& id, const int depth = 0) {
    for (int i = 0; i < depth; i++) {
        cout << "..";
    }
    cout << id << endl;
    for (const string& sub : v.at(id)) {
            print_recursive(v, sub, depth+1);
    }
}

int main()
{
    // TODO: Implement the datastructure here
    map<string, vector<string>> verkko;

    while(true){
        std::string line;
        std::cout << "> ";
        getline(std::cin, line);
        std::vector<std::string> parts = split(line, ' ', true);

        std::string command = parts.at(0);

        if(command == "S" or command == "s"){
            if(parts.size() != 3){
                std::cout << "Erroneous parameters!" << std::endl << HELP_TEXT;
                continue;
            }
            std::string id1 = parts.at(1);
            std::string id2 = parts.at(2);

            // TODO: Implement the command here!
            for (const auto& id : {id1, id2}) {

                if (verkko.count(id) == 0) {
                    verkko[id] = {};
                }
        }
            verkko[id1].push_back(id2);

        } else if(command == "P" or command == "p"){
            if(parts.size() != 2){
                std::cout << "Erroneous parameters!" << std::endl << HELP_TEXT;
                continue;
            }
            std::string id = parts.at(1);

            print_recursive(verkko, id);

            // TODO: Implement the command here!


        } else if(command == "C" or command == "c"){
            if(parts.size() != 2){
                std::cout << "Erroneous parameters!" << std::endl << HELP_TEXT;
                continue;
            }
            std::string id = parts.at(1);

            // TODO: Implement the command here!
            cout << print_count(verkko, id) << endl;

        } else if(command == "D" or command == "d"){
            if(parts.size() != 2){
                std::cout << "Erroneous parameters!" << std::endl << HELP_TEXT;
                continue;
            }
            std::string id = parts.at(1);

            // TODO: Implement the command here!
            cout << print_depth(verkko, id) << endl;

        } else if(command == "Q" or command == "q"){
           return EXIT_SUCCESS;
        } else {
            std::cout << "Erroneous command!" << std::endl << HELP_TEXT;
        }
    }
}
