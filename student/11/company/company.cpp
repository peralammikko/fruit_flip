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
        output << "Error. Employee already added." << endl;
    } else {
        new_Person->id_ = id;
        new_Person->department_ = dep;
        new_Person->time_in_service_ = time;
        companyList.insert({id, {new_Person}});
    }

}

void Company::printGroup(const string &id, const string &group, const IdSet &container, ostream &output) const
{
    int listLength = container.size();
    if (listLength == 0) {
        output << id << " has no " << group << "." << endl;
    } else {
        output << id << " has " << listLength << " " << group << "." << endl;
        for (auto i : container) {
                cout << i << endl;
        }
    }

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
    if (idPerson == nullptr) {printNotFound(id, cout); return;}
    vector<Employee*> subsVector = idPerson->subordinates_;
    IdSet subSet = VectorToIdSet(subsVector);
    string subsString = "subordinates";
    printGroup(id, subsString, subSet, cout);
}

void Company::printColleagues(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id, cout); return;}

    Employee* idsBoss = idPerson->boss_;
    set<string> colleagueList = {};
    for (auto entity : companyList) {
        if (entity.second->boss_ == idsBoss) {
            if (entity.second->id_ == id) {
                continue;
            } else if (entity.second->boss_ == nullptr) {
                continue;
            }
            colleagueList.insert(entity.second->id_);
        }
    }
    string empString = "colleagues";
    printGroup(id, empString, colleagueList, cout);
}

void Company::printDepartment(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,cout); return;}

}

void Company::printLongestTimeInLineManagement(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,cout); return;}
    double maxTime = idPerson->time_in_service_;
    string maxTimePerson = "";
    for (auto sub : idPerson->subordinates_) {
        if (sub->time_in_service_ > maxTime) {
            maxTime = sub->time_in_service_;
            maxTimePerson = sub->id_;
        }
    }
    if (idPerson->time_in_service_ == maxTime) {
        cout << "With the time of " << idPerson->time_in_service_ << ", " << id << " is the longest-served employee in their line management." << endl;
        return;
    }
    cout << "With the time of " << maxTime << ", " << maxTimePerson << " is the longest-served employee in " << id << "'s line management." << endl;

}

void Company::printShortestTimeInLineManagement(const std::string &id, std::ostream &output) const
{

}

void Company::printBossesN(const std::string &id, const int n, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,cout); return;}
    set<string> setBosses = {};
    int i = 0;
    string bossString = "bosses";
    while (i < n) {
        if (idPerson->boss_ == nullptr) {break;}
            string bossName = idPerson->boss_->id_;
            setBosses.insert(bossName);
            idPerson = idPerson->boss_;
            i++;
    }
    printGroup(id, bossString, setBosses, cout);
}

void Company::printSubordinatesN(const std::string &id, const int n, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,cout); return;}
    set<string> setSubs = {};
    int i = 0;
    string subString = "subordinates";
    while (i < n) {
        for (auto entity : idPerson->subordinates_) {
            setSubs.insert(entity->id_);
        }
    }
}




