#include "company.hh"
#include <vector>
#include <map>
#include <iostream>

using namespace std;

Company::Company() {}

Company::~Company()
{

}

void Company::printNotFound(const string &id, ostream &output) const
{
        output << "Error. " << id << " not found." << endl;
}

void Company::addNewEmployee(const std::string &id, const std::string &dep, const double &time, std::ostream &output)
{
    Employee* new_Person = new Employee;
    Employee* checkPerson = getPointer(id);
    if (checkPerson != nullptr) {
        cout << "Error. Employee already added." << endl;
    } else {
        new_Person->id_ = id;
        new_Person->department_ = dep;
        new_Person->time_in_service_ = time;
        companyList.insert({id, {new_Person}});
    }

}

void Company::printGroup(const string &id, const string &group, const IdSet &container, ostream &output) const
{
    vector<string> tempVector = {};

}

Employee *Company::getPointer(const std::string &id) const
{
    Employee* personPointer = nullptr;
    for (auto person : companyList) {
        if (person.first == id) {
            personPointer = person.second;
        }
    }
    return personPointer;
}

IdSet Company::VectorToIdSet(const std::vector<Employee *> &container) const
{
    IdSet alphabeticalOrder = {};
    for (auto i : container) {
        alphabeticalOrder.insert(i->id_);
    }
    return alphabeticalOrder;

}

void Company::printEmployees(std::ostream &output) const
{
    for (auto it : companyList) {
        output << it.second->id_ << ", " << it.second->department_ << ", " << it.second->time_in_service_ << std::endl;
    }
}

void Company::addRelation(const std::string &subordinate, const std::string &boss, std::ostream &output)
{
    Employee* idPerson = getPointer(subordinate);
    Employee* idsBoss = getPointer(boss);
    if (boss == "") {
        idPerson->boss_ = nullptr;
    } else {
        idPerson->boss_ = idsBoss;
        idsBoss->subordinates_.push_back(idPerson);
    }

}

void Company::printBoss(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,cout); return;}
    Employee* idBoss = idPerson->boss_;
    if (idBoss == nullptr) {
        cout << id << " has no bosses." << endl;
    } else {
        cout << id << " has 1 bosses." << endl;
        cout << idBoss->id_ << endl;
    }
}

void Company::printSubordinates(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    printNotFound(id, cout);
    if (idPerson == nullptr) {return;}
    vector<Employee*> subsVector = idPerson->subordinates_;
    IdSet subSet = VectorToIdSet(subsVector);
    int subCount = subSet.size();
    if (subCount > 1) {
        cout << id << " has " << subCount << " subordinates." << endl;
        for (auto i : subSet) {
            cout << i << endl;
        }
    } else {
        cout << id << " has no subordinates." << endl;
    }
}

void Company::printColleagues(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    printNotFound(id, cout);
    if (idPerson == nullptr) {return;}
    Employee* idsBoss = idPerson->boss_;
    vector<string> colleagueList = {};
    for (auto entity : companyList) {
        if (entity.second->boss_ == idsBoss) {
            if (entity.second->id_ == id) {
                continue;
            } else if (entity.second->boss_ == nullptr) {
                continue;
            }
            colleagueList.push_back(entity.second->id_);
        }
    }
    int listLength = colleagueList.size();
    if (listLength == 0) {
        cout << id << " has no colleagues." << endl;
    } else {
        cout << id << " has " << listLength << " colleagues:" << endl;
        for (auto name : colleagueList) {
            cout << name << endl;
        }
    }
}

void Company::printDepartment(const std::string &id, std::ostream &output) const
{

}

void Company::printLongestTimeInLineManagement(const std::string &id, std::ostream &output) const
{

}

void Company::printShortestTimeInLineManagement(const std::string &id, std::ostream &output) const
{

}

void Company::printBossesN(const std::string &id, const int n, std::ostream &output) const
{

}

void Company::printSubordinatesN(const std::string &id, const int n, std::ostream &output) const
{

}




