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
    Employee* idsBoss = idPerson->boss_;
    set<string> bossId = {};
    if (idsBoss != nullptr) {
        bossId.insert(idsBoss->id_);
    }
    printGroup(id,"bosses",bossId, output);
}

void Company::printSubordinates(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id, cout); return;}
    vector<Employee*> subsVector = idPerson->subordinates_;
    IdSet subSet = VectorToIdSet(subsVector);
    printGroup(id, "subordinates", subSet, output);
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
    printGroup(id, "colleagues", colleagueList, output);
}



void Company::printDepartment(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,output); return;}

    // ?

}

void Company::printLongestTimeInLineManagement(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,output); return;}
    double maxTime = idPerson->time_in_service_;
    string maxTimePerson = idPerson->id_;
    while (idPerson->subordinates_.size() != 0) {
        for (auto sub : idPerson->subordinates_) {
            if (sub->time_in_service_ > maxTime) {
                maxTime = sub->time_in_service_;
                maxTimePerson = sub->id_;
            }
            idPerson = sub;
        }
    }
    if (maxTimePerson == id) {
        cout << "With the time of " << maxTime << ", " << id << " is the longest-served employee in their line management." << endl;
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
    while (i < n) {
        if (idPerson->boss_ == nullptr) {break;}
            string bossName = idPerson->boss_->id_;
            setBosses.insert(bossName);
            idPerson = idPerson->boss_;
            i++;
    }
    printGroup(id, "bosses", setBosses, output);
}

void Company::listRecursive(const string &id, const int &depth, set<Employee*> &lista, int tempDepth = 0) const
{
    Employee* idPerson = getPointer(id);
    if (tempDepth < depth) {
        for (auto sub : idPerson->subordinates_) {
            lista.insert(sub);
            listRecursive(sub->id_, depth, lista, tempDepth+1);
        }
    }
}

void Company::printSubordinatesN(const std::string &id, const int n, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,cout); return;}
    set<Employee*> setSubs = {};
    set<string> setIds = {};
    listRecursive(idPerson->id_, n, setSubs);
    for (auto i : setSubs) {
        setIds.insert(i->id_);
    }
    printGroup(id,"subordinates",setIds,output);
}




