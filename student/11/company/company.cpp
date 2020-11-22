#include "company.hh"
#include <vector>
#include <map>
#include <iostream>

using namespace std;

Company::Company() {}

// Luokan destructor. Käy läpi jokaisen tietokannan jäsenen ja deletoi struct-rakenteen
// (esimies-alaissuhten) ja poistaa sitten kyseisen mapin tietoparin.
Company::~Company()
{
    map<string, Employee*>::iterator iter = companyList.begin();
    if (iter != companyList.end()) {
        delete iter->second;
        companyList.erase(iter);
    }
}

// Etsii ja palauttaa tietokannasta osoittimen annettuun henkilöön (ID).
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

// Tulostaa virheviestin tilanteessa, jossa käyttäjän syöttämää henkilöä (ID) ei löydy tietokannasta.
void Company::printNotFound(const string &id, ostream &output) const
{
        output << "Error. " << id << " not found." << endl;
}

/* Luo uudelle henkilölle Emploee-rakenteen ja lisää henkilön ohjelman tietokantaan.
 * Tarkistaa aluksi getPointer-funktion avulla, onko henkilö jo aiemmin lisätty.
 * Mikäli henkilö on jo olemassa, tulostaa ohjelma virheviestin. Muussa tapauksessa
 * varataan uusi dynaaminen muuttuja, jolle syötetään tarvittavat tiedot, jotka saadaan
 * funktion parametreista. Lopulta lisätään uusi työntekijä firman map-rakenteella
 * toimivaan tietokantaan avaimena nimi (id) ja arvona työntekijän tiedot (Employee-tietue).
*/

void Company::addNewEmployee(const std::string &id, const std::string &dep, const double &time, std::ostream &output)
{
    Employee* new_Person = getPointer(id);
    if (new_Person != nullptr) {
        output << "Error. Employee already added." << endl;
    } else {
        new_Person = new Employee{};
        new_Person->id_ = id;
        new_Person->department_ = dep;
        new_Person->time_in_service_ = time;
        companyList.insert({id, {new_Person}});
    }
}

/* Luodaan henkilöiden välille esihenkilö-alaissuhteita. Funktio tarkastaa ensin,
 * löytyykö henkilö tietokannasta (ja tulostaa virheviestin jos ei löydy).
 * Tämän jälkeen funktio tarkastaa parametreista, onko henkilölle määritelty esimiestä.
 * Jos boss-parametri on tyhjä, merkitään että henkilöllä ei ole esimiestä.
 * Muussa tapauksessa etsitään esimies tietokannasta getPointer-funktiolla
 * (ja tulostetaan virheviesti jos esimiestä ei löydy). Tämän jälkeen alainen yhdistetään
 * esimieheen ja subordinate esimiehen alaiseksi.
*/
void Company::addRelation(const std::string &subordinate, const std::string &boss, std::ostream &output)
{
    Employee* idPerson = getPointer(subordinate);
    if (idPerson == nullptr) {printNotFound(subordinate, output); return;}

    if (boss == "") {
        idPerson->boss_ = nullptr;
    } else {
        Employee* idsBoss = getPointer(boss);
        if (idsBoss == nullptr) {return;}
        idPerson->boss_ = idsBoss;
        idsBoss->subordinates_.push_back(idPerson);
    }
}

/* Tulostaa säiliössä olevan datan aakkosjärjestyksessä.
 * Parametrit:
 * (1) id = käyttäjän syöttämä henkilö
 * (2) group = henkilöön liittyvä ryhmä, jota käyttäjä haluaa tietää (alaiset/esimiehet/kolleegat tms.)
 * (3) container = datasäiliö muotoa set<string>
 * (4) output = tulostuskomento
*/
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

/* Siirtää vektorissa olevat yksitellen string-tyyppiset tiedot set-säiliöön,
 * jonka jälkeen se palautuu funktioon aakkosjärjestyksessä.
 * Käyttää header filessa määriteltyä IdSet -muotoa, joka on <set>string -säiliö.
 *
 * Parametri: container = struct-tietueita sisältävä vektori
*/
IdSet Company::VectorToIdSet(const std::vector<Employee *> &container) const
{
    IdSet alphabeticalOrder = {};
    for (auto i : container) {
        alphabeticalOrder.insert(i->id_);
    }
    return alphabeticalOrder;

}

/* Hierarkian etsimiseen ja tallentamiseen käytettävä apufunktio.
 * Etsii esimies-alaissuhteiden puusta pyydetyn henkilön ja listaa rekursiivisesti
 * kaikki henkilön suorat ja epäsuorat alaiset set-säiliöön. Funktio kutsuu itseään
 * uudestaan siis jokaisen alaisen kohdalla ja listaa alaisia niin kauan, kunnes alaisia ei
 * enää löydy tai hierarkiataso n saavutetaan.
 *
 * Henkilötietojen osoittimet tallentuvat set-säiliöön, jolloin ne tallentuvat
 * aakkosjärjestykseen, ja jotka myöhemmin palautuvat viitteenä alkuperäiseen funktioon.
 * Parametrit:
 * (1) id = käyttäjän syöttämän henkilön nimi
 * (2) depth = käyttäjän toivoma hierarkiatason syvyys
 * (3) list = lista henkilön id alaisten tiedoista
 * (4) tempDepth = rekursiossa käytettävä apuparametri syvyyden saavuttamiseksi
*/

void Company::listRecursive(const string &id, const int &depth, set<Employee*> &list, int tempDepth = 0) const
{
    Employee* idPerson = getPointer(id);
    if (tempDepth < depth) {
        for (auto sub : idPerson->subordinates_) {
            list.insert(sub);
            listRecursive(sub->id_, depth, list, tempDepth+1);
        }
    }
}


// Tulostaa kaikkien tietokannassa olevien henkilöiden nimet, osastot ja palvelusajat.
void Company::printEmployees(std::ostream &output) const
{
    for (auto it : companyList) {
        output << it.second->id_ << ", " << it.second->department_ << ", " << it.second->time_in_service_ << std::endl;
    }
}


/* Etsii ja tulostaa henkilön lähimmän esimiehen.
 * Henkilö ja tämän esimies etsitään tietokannasta, jonka jälkeen
 * tulostus tapahtuu printGroup -apufunktion avulla. Mikäli henkilöllä ei ole
 * esimiestä, apufunktio tulostaa "<id> has no bosses."
*/
void Company::printBoss(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id, output); return;}
    Employee* idsBoss = idPerson->boss_;
    IdSet bossId = {};
    if (idsBoss != nullptr) {
        bossId.insert(idsBoss->id_);
    }
    printGroup(id,"bosses",bossId, output);
}


/* Etsii ja tulostaa henkilön lähimmät (suorat) alaiset.
 * Henkilö etsitään tietokannasta ja tälle merkatut suorat alaiset tallennetaan vektoriin,
 * jonka sisältö siirretään VectorToIdSet -apufunktion avulla aakkosjärjestykseen set-säiliöön
 * tulostusta varten (jonka hoitaa apufunktio printGroup). Mikäli henkilöllä ei ole suoria
 * alaisia, ohjelma tulostaa "<id> has no subordinates."
*/
void Company::printSubordinates(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id, cout); return;}
    vector<Employee*> subsVector = idPerson->subordinates_;
    IdSet subSet = VectorToIdSet(subsVector);
    printGroup(id, "subordinates", subSet, output);
}


/* Etsii ja tulostaa henkilön <id> kollegat eli saman
 * esimiehen alla (ja samalla hierarkiatasolla) toimivat työntekijät.
 * Etsii tietokannasta henkilön <id> esimiehen ja listaa tämän suorat alaiset
 * IdSet-säiliöön (listaamatta itse henkilöä <id> omaksi kollegakseen), joka
 * tulostetaan apufunktio printGroupin avulla. Jos henkilöllä ei ole esimiestä
 * tai muita samantasoisia työntekijöitä, ohjelma tulostaa "<id> has no colleagues."
*/
void Company::printColleagues(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id, cout); return;}

    Employee* idsBoss = idPerson->boss_;
    IdSet colleagueList = {};
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

/* Etsii ja tulostaa henkilön <id> osastolla olevat työntekijät.
 * Funktio etsii ensin henkilön <id> korkeimman esimiehen, jolla on sama osasto kuin
 * <id>:llä liikkumalla ylöspäin hierarkiassa. Korkeimman osaston esimiehen löydyttyä
 * kutsutaan apufunktiota listRecursive, joka listaa kaikki kyseisen henkilön suorat
 * ja epäsuorat alaiset, kunnes alaisia ei enää löydy.
 *
 * Funktioon palautuu lista kaikista alaisten tiedoista, joka sitten käydään
 * läpi vertaamalla listan henkilöiden osastoa alkuperäisen henkilön
 * <id> osastoon. Samalla osastolla työskentelevien nimet lisätään IdSet-säiliöön
 * departNames, ja samoin itse <id> jos henkilö ei ole osastonsa korkein esimies.
 * Säiliön tiedot tulostetaan apufunktion printGroup avulla.
*/
void Company::printDepartment(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,output); return;}
    Employee* idsBoss = idPerson->boss_;
    set<Employee*> lineList = {};
    IdSet departNames = {};
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


/* Tulostaa henkilön <id> linjaorganisaatiosta (henkilöstä alaspäin muodostuva hierarkia)
 * pisimpään palvelleen henkilön nimen ja palvelusajan.
 * Funktio ensin etsii linkitetyistä tietueista henkilön <id> kaikki alaiset listRecursive -apufunktion avulla.
 * Apufunktio palauttaa listan kaikista suorista ja epäsuorista alaisista, ja alkaa vertailemaan
 * työntekijöiden palvelusaikoja (alkuun vertailuarvoksi annettu <id>:n palvelusaika).
*/
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


/* Tulostaa henkilön <id> linjaorganisaatiosta (henkilöstä alaspäin muodostuva hierarkia)
 * lyhimpään palvelleen henkilön nimen ja palvelusajan.
 * Funktio ensin etsii linkitetyistä tietyeista henkilön <id> kaikki alaiset listRecursive -apufunktion avulla.
 * Apufunktio palauttaa listan kaikista suorista ja epäsuorista alaisista ja alkaa vertailemaan
 * työntekijöiden palvelusaikoja (alkuun vertailuarvona <id>:n palvelusaika).
*/
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


/* Etsii ja tulostaa henkiön <id> kaikki esimiehet annetulle hierarkiatasolle n asti.
 * Luo IdSet-säiliön henkilön esimiehiä varten, johon esimiehet voidaan tallentaa aakkosjärjestyksessä.
 * Funktio etsii henkilön <id> esimiehen ja lisää esimiehen nimen säiliöön. Tämän jälkeen
 * funktio siirtyy hierarkiassa yhden tason ylemmäksi ja etsii <id>:n esimiehen esimiehen jne
 * kunnes ylempää esimiestä ei enää löydy tai taso n saavutetaan. Säiliön tiedot tulostetaan
 * apufunktion printGroup avulla.
 *
 * Tulostaa virheviestin, mikäli henkilöä <id> ei löydy tai taso n on liian pieni (alle 1).
*/
void Company::printBossesN(const std::string &id, const int n, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,cout); return;}
    if (n < 1) {output << "Error. Level can't be less than 1." << endl; return;}
    IdSet setBosses = {};
    int i = 0;
    while (i < n) {
        if (idPerson->boss_ == nullptr) {
            break;
        }
        string bossName = idPerson->boss_->id_;
        setBosses.insert(bossName);
        idPerson = idPerson->boss_;
        i++;
    }
    printGroup(id, "bosses", setBosses, output);
}


/* Etsii ja tulostaa henkilön <id> kaikki alaiset annetulle hierarkiatasolle n asti.
 * Funtio etsii ensin apufunktio listRecursiven avulla kaikki henkilön <id> suorat alaiset, jonka
 * jälkeen apufunktio siirtyy yhden hierarkiatason alemmaksi ja jatkaa listaamista kunnes
 * hierarkiataso n saavutetaan tai alaisia ei enää löydy. Apufunktio palauttaa siis listan kaikista
 * henkilön <id> suorista ja epäsuorista alaisista, joiden nimet lisätään IdSet -säiliöön
 * aakkosjärjestyksessä. Nimet tulostetaan apufunktion printGroup avulla.
*/
void Company::printSubordinatesN(const std::string &id, const int n, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,cout); return;}
    if (n < 1) {output << "Error. Level can't be less than 1." << endl; return;}
    set<Employee*> setSubs = {};
    IdSet subIds = {};
    listRecursive(idPerson->id_, n, setSubs);
    for (auto i : setSubs) {
        subIds.insert(i->id_);
    }
    printGroup(id,"subordinates",subIds,output);
}




