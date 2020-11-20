#include "company.hh"
#include <vector>
#include <map>
#include <iostream>

using namespace std;

Company::Company() {}

Company::~Company()
{
    for (auto iter : companyList) {
        delete iter.second;
        companyList.erase(iter.first);
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
    delete new_Person;
}

void Company::addRelation(const std::string &subordinate, const std::string &boss, std::ostream &output)
{
    Employee* idPerson = getPointer(subordinate);
    if (idPerson == nullptr) {printNotFound(subordinate,output);}
    Employee* idsBoss = getPointer(boss);
    int nameCount = companyList.count(subordinate);
    if (nameCount < 1) {
        if (boss == "") {
            idPerson->boss_ = nullptr;
        } else {
            idPerson->boss_ = idsBoss;
            idsBoss->subordinates_.push_back(idPerson);
        }
    }
}

void Company::printGroup(const string &id, const string &group, const IdSet &container, ostream &output) const
{
    int listLength = container.size();
    if (listLength == 0) {
        output << id << " has no " << group << "." << endl;
    } else {
        output << id << " has " << listLength << " " << group << ":" << endl;
        for (auto i : container) {
                cout << i << endl;
        }
    }

}

IdSet Company::VectorToIdSet(const std::vector<Employee *> &container) const
{
    IdSet alphabeticalOrder = {};
    for (auto i : container) {
        alphabeticalOrder.insert(i->id_);
    }
    return alphabeticalOrder;

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

void Company::printEmployees(std::ostream &output) const
{
    for (auto it : companyList) {
        output << it.second->id_ << ", " << it.second->department_ << ", " << it.second->time_in_service_ << std::endl;
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
    if (idsBoss != nullptr) {
        for (auto entity : idsBoss->subordinates_) {
            if (entity->id_ == id) {
                continue;
            }
            colleagueList.insert(entity->id_);
        }
    }
    printGroup(id, "colleagues", colleagueList, output);
}


void Company::printDepartment(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,output); return;}
    Employee* idsBoss = idPerson->boss_;
    set<Employee*> lineList = {};
    set<string> departNames = {};
    while (idsBoss != nullptr) {
        if (idsBoss->department_ == idPerson->department_) {
            idPerson = idsBoss;
            idsBoss = idPerson->boss_;
        } else {
            break;
        }
    }
    listRecursive(idPerson->id_,100, lineList);

    if (idPerson->id_ != id) {
        departNames.insert(idPerson->id_);
    }
    for (auto i : lineList) {
        if (i->department_ != idPerson->department_) {
            continue;
        }
        if (i->id_ == id) {
            continue;
        }
        departNames.insert(i->id_);
    }
    printGroup(id, "department colleagues",departNames,output);

}

void Company::printLongestTimeInLineManagement(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,output); return;}
    set<Employee*> lineList = {};
    listRecursive(id,100,lineList);

    double maxTime = idPerson->time_in_service_;
    string maxTimePerson = idPerson->id_;
    for (auto entity : lineList) {
        if (entity->time_in_service_ > maxTime) {
            maxTime = entity->time_in_service_;
            maxTimePerson = entity->id_;
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
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,output); return;}
    set<Employee*> lineList = {};
    listRecursive(id,100,lineList);

    double minTime = idPerson->time_in_service_;
    string minTimePerson = idPerson->id_;
    for (auto entity : lineList) {
        if (entity->time_in_service_ < minTime) {
            minTime = entity->time_in_service_;
            minTimePerson = entity->id_;
        }
    }
    if (minTimePerson == id) {
        cout << "With the time of " << minTime << ", " << id << " is the shortest-served employee in their line management." << endl;
        return;
    }
    cout << "With the time of " << minTime << ", " << minTimePerson << " is the shortest-served employee in " << id << "'s line management." << endl;
}

void Company::printBossesN(const std::string &id, const int n, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,cout); return;}
    if (n < 1) {output << "Error. Level can't be less than 1." << endl; return;}
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

void Company::printSubordinatesN(const std::string &id, const int n, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,cout); return;}
    if (n < 1) {output << "Error. Level can't be less than 1." << endl; return;}
    set<Employee*> setSubs = {};
    set<string> setIds = {};
    listRecursive(idPerson->id_, n, setSubs);
    for (auto i : setSubs) {
        setIds.insert(i->id_);
    }
    printGroup(id,"subordinates",setIds,output);
}




