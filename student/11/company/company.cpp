#include "company.hh"
#include <vector>
#include <map>
#include <iostream>

using namespace std;

/* 11.4 (P) Firma
 * Ohjelma firman työntekijöistä ja esimies-alaissuhteista.
 * Ohjelma lukee tekstitiedostossa olevat henkilötiedot
 * ja luo niistä tietorakenteen, johon tallennetaan tekstiriveiltä
 * löytyvät työntekijän nimi, osasto ja palvelusaika.
 * Henkilötietojen jälkeen rivillä lukee vielä työntekijän lähin esimies.
 * Työntekijä-esimiessuhde tallennetaan dynaamisesti osoittimina molempien
 *  henkilön tietoihin, esimiehelle yhdeksi hänen alaisista ja työntekijälle esimieheksi.
 *
 * Käyttäjä voi tarkastella firman työntekijöiden tietoja ja esimies-alaissuhteita erilaisin komennoin.
 * Toiminnot:
 * Käyttäjä voi...
 * (1) tulostaa firman työntekijöiden nimet, osastot ja palvelusajat
 * (2) tulostaa halutun henkilön kollegoiden, alaisten tai esimiesten nimet
 * (3) tulostaa halutun työntekijän osaston työntekijöiden nimet
 * (4) tulostaa pisimpään ja lyhimpään palvelleen työntekijän nimen ja palvelusajan
 *
 * Tarkemmat komennot toimintojen suorittamiseksi löytyvät tiedostosta cli.hh.
 *
 * Ohjelman kirjoittaja
 * Nimi: Mikko Perälä
 * Opiskelijanumero: 256960
 * Käyttäjätunnus: peralam
 * E-Mail: mikko.perala@tuni.fi
 *
 * Huomioita ohjelmasta:
 * Rivi 89 (new_Person = new Employee;) ilmenee valgrind-testissa (QT sekä commandline) muistiongelmana:
 * (1688 bytes in 2 blocks are definitely lost in in loss record 3 of 3).
 * Uuden Employee-rakenteen voisi oletettavasti määrittää muodossa new Employee(id, dep, time, nullptr, {}),
 * johon QT Creator antaakin errorin "no matching constructor for Employee".
 * Yritimme Kooditorion assarin (Joel) kanssa korjata tätä muistiongelmaa, mutta lopulta
 * ainut ratkaisu olisi ollut vaihtaa kaikki koodin osoittimet smart pointereihin, joka
 * olisi ollut hieman työlästä muuten jo valmiissa ja toimivassa ohjelmassa.
 * Toivottavasti ongelma ei osoittaudu liian merkittäväksi.
*/

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
        new_Person = new Employee();
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
        for (string name : container) {
                cout << name << endl;
        }
    }

}

/* Siirtää vektorissa olevat yksitellen string-tyyppiset tiedot set-säiliöön,
 * jonka jälkeen se palautuu funktioon aakkosjärjestyksessä.
 * Käyttää header filessa määriteltyä IdSet -muotoa, joka on <set>string -säiliö.
 *
 * Parametri: container = struct-tietueita sisältävä vektori
*/
IdSet Company::VectorToIdSet(const std::vector<Employee*> &container) const
{
    IdSet alphabeticalOrder = {};
    for (Employee* i : container) {
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
 * (2) list = lista henkilön id alaisten tiedoista
 * (3) noDepthLimit = totuusarvo hierarkiatasojen rajaamiseksi.
 * false -> työntekijöitä listataan hierarkiaketjun loppuun saakka (kunnes alaisia ei enää löydy)
 * true -> työntekijöitä listataan hierarkiatasolle N (>1) saakka.
 * (4) depth = käyttäjän toivoma hierarkiataso N (n=1: alainen, n=2:alaisen alainen, jne).
 * (5) tempDepth = rekursiossa käytettävä apuparametri halutun syvyyden saavuttamiseksi
*/

void Company::listRecursive(const string &id, set<Employee*> &list, bool noDepthLimit = false, const int &depth = 1, int tempDepth = 0) const
{
    Employee* idPerson = getPointer(id);
    if (tempDepth < depth) {
        for (Employee* sub : idPerson->subordinates_) {
            list.insert(sub);
            if (noDepthLimit == false) {
                tempDepth = tempDepth+1;
            }
            listRecursive(sub->id_, list, noDepthLimit, depth, tempDepth);
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
        for (Employee* entity : idsBoss->subordinates_) {
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
 * departNames, ja itse <id>:n nimi vältetään (ei voi olla oma kollegansa).
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
    listRecursive(idPerson->id_, lineList, true);

    // idPerson on nyt osaston korkein esimies, jonka kaikki suorat ja epäsuorat alaiset
    // haettiin listRecursive-funktiolla. Lisätään kyseisen esimiehen nimi <id>:n kollegoiden listaan.
    // Käydään palautettu lista läpi yksitellen verraten, onko listattujen työntekijöiden osasto
    // sama kuin <id>:n oma ja hypätään itse <id>:n yli osastokollegoita lueteltaessa.
    if (idPerson->id_ != id) {
        departNames.insert(idPerson->id_);
    }
    for (Employee* entity : lineList) {
        if (entity->department_ != idPerson->department_) {
            continue;
        }
        if (entity->id_ == id) {
            continue;
        }
        departNames.insert(entity->id_);
    }
    printGroup(id, "department colleagues",departNames,output);
}

/* Pisimmän tai lyhimmän palvelusajan löytämiseen käytettävä apufunktio.
 * Funktio ensin etsii linkitetyistä tietueista henkilön <id> kaikki alaiset listRecursive -apufunktion avulla.
 * Apufunktio palauttaa listan kaikista suorista ja epäsuorista alaisista, ja alkaa vertailemaan
 * työntekijöiden palvelusaikoja (alkuun vertailuarvoksi annettu <id>:n palvelusaika) riippuen siitä,
 * etsitäänkö lyhimpään vai pisimpään palvellutta työntekijää. Kyseisen henkilön nimi ja aika tallennetaan
 * tietopariin ja palautetaan alkuperäiseen funktioon tulostusta varten.
*/
void Company::findMinMaxTime(Employee* &id, bool findShortestTime = false) const {

    set<Employee*> lineList = {};
    listRecursive(id->id_, lineList, true);
    pair<string, double> result = {};
    string estTime = "longest";
    if (findShortestTime == true) {
        estTime = "shortest";
        double minTime = id->time_in_service_;
        string minTimePerson = id->id_;
        for (Employee* entity : lineList) {
            if (entity->time_in_service_ < minTime) {
                minTime = entity->time_in_service_;
                minTimePerson = entity->id_;
            }
        }
        result = make_pair(minTimePerson,minTime);
    } else {
        double maxTime = id->time_in_service_;
        string maxTimePerson = id->id_;
        for (Employee* entity: lineList) {
            if (entity->time_in_service_ > maxTime) {
                maxTime = entity->time_in_service_;
                maxTimePerson = entity->id_;
            }
        }
        result = make_pair(maxTimePerson, maxTime);
    }
    if (result.first == id->id_) {
        cout << "With the time of " << result.second << ", " << id->id_ << " is the " << estTime << "-served employee in their line management." << endl;
        return;
    }
    cout << "With the time of " << result.second << ", " << result.first << " is the " << estTime << "-served employee in " << id->id_ << "'s line management." << endl;
}


/* Tulostaa henkilön <id> linjaorganisaatiosta (henkilöstä alaspäin muodostuva hierarkia)
 * pisimpään palvelleen henkilön nimen ja palvelusajan.
 * Henkilöiden aikojen vertailu tapahtuu apufunktiossa findMinMaxTime, josta palautuu tieto-
 * pari pisimpään palvelleesta työntekijästä (nimi + aika).
*/
void Company::printLongestTimeInLineManagement(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,output); return;}
    findMinMaxTime(idPerson, false);
}


/* Tulostaa henkilön <id> linjaorganisaatiosta (henkilöstä alaspäin muodostuva hierarkia)
 * lyhimpään palvelleen henkilön nimen ja palvelusajan.
 * Henkilöiden aikojen vertailu tapahtuu apufunktiossa findMinMaxTime, josta palautuu tieto
 * lyhimpään palvelleesta työntekijästä (nimi + aika).
*/
void Company::printShortestTimeInLineManagement(const std::string &id, std::ostream &output) const
{
    Employee* idPerson = getPointer(id);
    if (idPerson == nullptr) {printNotFound(id,output); return;}
    findMinMaxTime(idPerson, true);
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
    listRecursive(idPerson->id_, setSubs, false, n);
    for (Employee* subordinate : setSubs) {
        subIds.insert(subordinate->id_);
    }
    printGroup(id,"subordinates",subIds,output);
}




