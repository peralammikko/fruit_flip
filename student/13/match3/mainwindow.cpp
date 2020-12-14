#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QDebug>
#include <QPixmap>
#include <vector>

/* 13.2 (P) Match3
 * fruit_flip on peli, jossa tarkoituksena on yhdistellä värillisiä ruutuja eli hedelmiä.
 * Pelissä pelaaja voi liikutella ruutuja ja siten luoda kolmen tai useamman samanvärisen ruudun suoria
 * poistaakseen ne peliruudukosta. Pelin ideana on, että ruudukko on sattumanvaraisesti generoitu, jolloin haasteena
 * on liikutella ruutuja "matchien" eli värisuorien maksimoimiseksi. Matchit voivat olla pystysuoria
 * (3+ samanväristä ruutua päällekäin) tai vaakasuoria (3+ samanväristä ruutua vierekkäin).
 * Värisuoran tapahtuessa suoran ruudut poistuvat pelistä, joka aiheuttaa niiden yläpuolella
 * olevien hedelmien tippumisen. Kun ruudukko on lopulta tyhjä, pelaaja on voittanut pelin.
 *
 * Pelaaja klikkaa peliruudukkossa olevia värikkäitä ruutuja eli hedelmiä vaihdellakseen niiden paikkoja.
 * Pelissä on sallittua siirtää vain vierekkäisiä ruutuja neljässä pääsuunnassa (ylös, alas, vasen, oikea).
 * Klikattuaan kahta vierekkäistä ruutua pelaaja vahvistaa siirron painamalla nappia "Move", jolloin
 * siirto tapahtuu mikäli se on laillinen (vierekkäin, ei ruudukon ulkopuolella). Peliin on lisätty joitakin
 * lisäasetuksia, joita pelaaja pystyy säätelemään muuttaakseen pelin sääntöjä, kuten tippuvien ruutujen viiveen poisto,
 * jatkuva pelitila tai ruudukon koon muuttaminen. Painamalla peli-ikkunasta löytyviä "?"
 * -nappeja pelaajan on mahdollista saada lyhyitä ohjeita toiminnallisuuksiin ja asetuksiin liityen.
 *
 * Tarkempi dokumentaatio ohjelman toiminnasta ja ominaisuuksista löytyy tiedostosta instructions.txt.
 * Määrittely ja kuvaukset funktioiden toiminnasta löytyvät tiedostoista mainwindow.hh ja mainwindow.cpp.
 *
 * Ohjelman kirjoittaja
 * Nimi: Mikko Perälä
 * Opiskelijanumero: 256960
 * Käyttäjätunnus: peralam
 * E-Mail: mikko.perala@tuni.fi
 *
 * Huomioita ohjelmasta:
 * Timer-widget aiheuttaa muistiongelmia, ratkaisua ei löydetty.
 * Ylisuuren ruudukon (yli 15 saraketta/riviä) saattaa hidastaa konetta ja ohjelman toimintaa huomattavasti.
*/

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // We need a graphics scene in which to draw rectangles
    scene_ = new QGraphicsScene(this);

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a fruit is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    int seed = time(0); // You can change seed value for testing purposes
    randomEng_.seed(seed);
    distr_ = std::uniform_int_distribution<int>(0, NUMBER_OF_FRUITS - 2);
    distr_(randomEng_); // Wiping out the first random number (which is almost always 0)

    ui->delayCheck->setChecked(true);
    init_game();
    connect(timer_, &QTimer::timeout, this, &MainWindow::on_timeout);
    msg_.addButton(QMessageBox::Ok);

    // More code perhaps needed
}

MainWindow::~MainWindow()
{
    grid_.clear();
    fruitGrid_.clear();
    delete timer_;
    delete scene_;
    delete ui;
}

// Alustaa pelin: pelilauta, otsikot, asetukset, kello ja pistetaulu
void MainWindow::init_game()
{
    ui->startPointLine->clear();
    ui->endPointLine->clear();
    checkSettings();
    init_grids();
    init_titles();
    init_score();
    init_timer();
}

// Tarkistetaan asetukset pelin alustusta varten.
void MainWindow::checkSettings()
{
    on_delayCheck_stateChanged(ui->delayCheck->checkState());
    on_refillCheck_stateChanged();
}

// Luo kirjaimet ja numerot ruudukkoa varten
void MainWindow::init_titles()
{
    // Displaying column titles, starting from A
    for(int i = 0, letter = 'A'; i < GRID_ROW_SIZE_+1; ++i, ++letter)
    {
        int shift = 5;
        QString letter_string = "";
        letter_string.append(letter);
        QLabel* label = new QLabel(letter_string, this);
        label->setGeometry(LEFT_MARGIN + shift + i * SQUARE_SIDE,
                           TOP_MARGIN - SQUARE_SIDE,
                           SQUARE_SIDE, SQUARE_SIDE);
    }

    // Displaying row titles, starting from A
    for(int i = 0, letter = 'A'; i < GRID_ROW_SIZE_+1; ++i, ++letter)
    {
        QString letter_string = "";
        letter_string.append(letter);
        QLabel* label = new QLabel(letter_string, this);
        label->setGeometry(LEFT_MARGIN - SQUARE_SIDE,
                           TOP_MARGIN + i * SQUARE_SIDE,
                           SQUARE_SIDE, SQUARE_SIDE);
    }
}

/* Alustaa peliruudukon luomalla numeroruudukon (6x6) ja piirrettävän ruudunkon,
 * johon numeroita vastaavat hedelmät/värit haetaan update_screen -funktiosta.
 * Tyhjentää mahdolliset ruudukot ennen uuden alustamista.
 * Luo hedelmistä ja painonapeista omia vektoreitaan, jotka sitten lisätään
 * ruudukon rakenteeseen (vektori vektoreista).
*/
void MainWindow::init_grids()
{
    grid_.clear();
    fruitGrid_.clear();
    std::vector<QPushButton*> buttonVec = {};
    vector<Fruit_kind> fruitVec = {};
    for (int i = 0; i < GRID_COL_SIZE_+1; i++) {
        for (int j = 0; j < GRID_ROW_SIZE_+1; j++) {

            // Satunnaisgeneroitu luku rndm määrittää mikä hedelmä ruudukon kohtaan tulee
            // käyttämällä header-tiedostossa luotua enum-rakennetta.
            int rndm = distr_(randomEng_);
            Fruit_kind fruit = static_cast<Fruit_kind>(rndm);
            fruitVec.push_back(fruit);
        }
        grid_.push_back(buttonVec);
        buttonVec.clear();

        fruitGrid_.push_back(fruitVec);
        fruitVec.clear();
    }
    // Jos alustetusta ruudukosta löytyy matcheja, alustetaan pelilauta uudestaan.
    // Kun sopiva ruudukko on löytynyt, päivitetään näkymä.
    if (check_matches() == true) {
        init_grids();
    }
    update_screen();
}

// Etsitään ruudukossa olevat ruuduille (hedelmille) oikeanlaiset värit
// käyttämällä headerissa määriteltyä numeroitua enum-rakennetta.
// Parametri: rndm = satunnaisgeneroitu numero, joka määrittelee "hedelmän" ja sen värin
QColor MainWindow::paint_button(int rndm)
{
    QColor colr(Qt::black);
    switch(rndm) {
    case PLUM: colr = QColor(Qt::magenta); break;
    case STRAWBERRY: colr = QColor(Qt::red); break;
    case APPLE: colr = QColor(Qt::green); break;
    case LEMON: colr = QColor(Qt::yellow); break;
    case BLUEBERRY: colr = QColor(Qt::blue); break;
    case ORANGE: colr = QColor (255,180,51); break;
    case EMPTY: colr = QColor (225,225,225); break;
    }
    return colr;
}

// Alustetaan pistelaskuri (nollataan pisteet ja taulu).
void MainWindow::init_score()
{
    initPalette_.setColor(QPalette::Background, Qt::black);
    ui->lcdNumberScore->setPalette(initPalette_);
    ui->lcdNumberScore->setAutoFillBackground(true);
    points_ = 0;
    ui->lcdNumberScore->display(0);
}

// Alustetaan ja väritetään peli-ikkunan kello
void MainWindow::init_timer()
{
    timer_ = new QTimer;
    timer_->setInterval(1000);
    ui->lcdNumberMins->setPalette(initPalette_);
    ui->lcdNumberMins->setAutoFillBackground(true);

    ui->lcdNumberSecs->setPalette(initPalette_);
    ui->lcdNumberSecs->setAutoFillBackground(true);
}

// Sekuntikellon toiminnallisuus, johon timer yhdistetään ikkunan auetessa
// Kun sekunnit tulevat täyteen, lisätään minuutti
void MainWindow::on_timeout()
{
    int mins = ui->lcdNumberMins->intValue();
    int secs = ui->lcdNumberSecs->intValue();

    if (secs == 59) {
        update_timer(mins+1, secs = 0);
    } else {
        update_timer(mins, secs+1);
    }
}

// Päivitetään ruudulla olevan kellon minuutit ja sekunnit
void MainWindow::update_timer(int mins, int secs)
{
    ui->lcdNumberMins->display(mins);
    ui->lcdNumberSecs->display(secs);
}

// Päivittää kaikki pelilaudalla olevat ruudut oikeanvärisiksi.
// Tyhjät ruudut väritetään backgroundColor -värillä.
void MainWindow::update_screen()
{
    for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
        for (int row = 0; row < GRID_ROW_SIZE_+1; row++) {
            Fruit_kind fruitInGrid = fruitGrid_.at(col).at(row);
            QPushButton* fruitButton = new QPushButton;
            connect(fruitButton, &QPushButton::clicked, this, &MainWindow::fruitButton_clicked);

            QPalette pal = fruitButton->palette();
            pal.setColor(QPalette::Button, paint_button(fruitInGrid));
            fruitButton->setPalette(pal);

            fruitButton->setGeometry(col*SQUARE_SIDE, row*SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE);
            scene_->addWidget(fruitButton);
        }
    }
}

// Ruutujen liikutteluun käytettävä funktio.
// Hakee pelaajan klikkaamien ruutujen koordinaatit ja vaihtaa niiden paikkaa keskenään,
// mikäli ne ovat vierekkäin (pystysuunnassa tai vaakasuunnassa).
void MainWindow::swap_blocks()
{
    int startX = startPoint_.at(0); int startY = startPoint_.at(1);
    int endX = endPoint_.at(0); int endY = endPoint_.at(1);

    // Ruutuja ei siirretä mikäli, ne saadut koordinaatit ovat ruudukon ulkopuolella.
    if (startX > GRID_COL_SIZE_ || startX < 0 || startY > GRID_ROW_SIZE_ || startY < 0) {
        return;
    } else if (endX > GRID_COL_SIZE_ || endX < 0 || endY > GRID_ROW_SIZE_ || endX < 0) {
        return;
    }

    // Ruutuja ei myöskään siirretä, jos niiden etäisyys toisistaan ei ole 1, jolloin ruutujen on
    // oltava vierekkäin, jotta siirto on mahdollinen.
    int distanceX = abs(startX-endX);
    int distanceY = abs(startY-endY);
    if (distanceX+distanceY != 1) {
        return;
    }

    // Vaihdetaan hedelmien paikat hyödyntämällä apumuuttujaa.
    Fruit_kind temp = EMPTY;
    temp = fruitGrid_.at(startX).at(startY);
    fruitGrid_.at(startX).at(startY) = fruitGrid_.at(endX).at(endY);
    fruitGrid_.at(endX).at(endY) = temp;

}
// Värisuorien eli matchien tarkistukseen käytettävä funktio. Kutsutaan jokaisen pelaajan siirron jälkeen sekä pelin alustuksessa.
// Palauttaa totuusarvon true, jos yksikin värisuora löytyy.
bool MainWindow::check_matches()
{
    // Pystysuuntaiset matchit
    Fruit_kind currentFruit = EMPTY;
    bool matchFound = false;
    // Hedelmäruudukkoa aletaan käymään yksitellen läpi. Värisuoran laskuri alustetaan arvoon 1.

    for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
        int counter = 1;
        for (int row = 0; row < GRID_ROW_SIZE_-1; row++) {
            currentFruit = fruitGrid_.at(col).at(row);

            // Tyhjistä palikoista muodostuvia matcheja ei huomioida, joten niiden tarkastelusta hypätään yli
            if (currentFruit == EMPTY) {
                continue;
            }

            /* Tarkastetaan jokaisen ruudukon hedelmän kohdalla montako samanlaista löydetään seuraavista hedelmistä
             * (ruudukon reunaan asti).
             * Jos hedelmäX ja hedelmäY ovat samat, lisätään laskuriin +1.
             * Jos hedelmäX ja hedelmäZ ovat samat, lisätään laskuriin+1.
            */
            for (int nextrow = row+1; nextrow < GRID_ROW_SIZE_+1; nextrow++) {
                Fruit_kind nextFruit = fruitGrid_.at(col).at(nextrow);
                if (nextFruit == currentFruit) {
                    counter++;
                    if (nextrow == GRID_ROW_SIZE_) {
                        if (counter > 2) {
                            matchFound = true;
                            remove_matched_blocks(col, row, counter, true);
                            counter = 1;
                            break;
                        }
                    }
                    /*
                     * Jos hedelmäX ja hedelmäÖ ovat eri (ja laskurissa on vähintään 3:n suora),
                     * poistetaan samaa hedelmää sisältävät ruudut. Laskuri palautetaan alkuarvoon yksi
                     * ja siirrytään seuraavaan hedelmään (--> break).
                    */
                } else {
                    if (counter > 2) {
                        matchFound = true;
                        remove_matched_blocks(col, row, counter, true);
                    }
                    counter = 1;
                    break;
                }
            }
        }
    }

    // Vaakasuuntaiset matchit: sama kuin pystysuuntaisten matchien tarkistus, mutta liikutaan
    // samalla rivillä sarakkeelta toiselle.
    for (int col = 0; col < GRID_COL_SIZE_-1; col++) {
        int counter = 1;
        for (int row = 0; row < GRID_ROW_SIZE_+1; row++) {
            currentFruit = fruitGrid_.at(col).at(row);
            if (currentFruit == EMPTY) {
                continue;
            }
              for (int nextcol = col+1; nextcol < GRID_COL_SIZE_+1; nextcol++) {
                Fruit_kind nextfruit = fruitGrid_.at(nextcol).at(row);
                if (nextfruit == currentFruit) {
                    counter++;
                    if (nextcol == GRID_COL_SIZE_) {
                        if (counter > 2) {
                            matchFound = true;
                            remove_matched_blocks(col, row, counter, false);
                            counter = 1;
                            break;
                        }
                    }
                } else {
                    if (counter > 2) {
                        matchFound = true;
                        remove_matched_blocks(col, row, counter, false);
                    }
                    counter = 1;
                    break;
                }
            }
        }
    }

    /* Matchin löytyessä päivitetään pistetaulukko ja peliruutu vastaamaan poistettuja suoria.
     * Tämän jälkeen kutsutaan drop_blocks -funktiota (viiveellä).
     * Paluuarvo true/false kertoo kutsuvalle funktiolle, onko siirron/alustuksen johdosta
     * syntynyt matcheja vai ei.
    */
    if (matchFound) {
        update_screen();
        ui->lcdNumberScore->display(points_);
        QTimer::singleShot(delay_, this, SLOT(drop_blocks()));
        return true;
    }
    return false;
}

// Poistaa ruudukossa olevia hedelmäsuoria niin monta kuin counter-laskurissa määrätään.
void MainWindow::remove_matched_blocks(int col, int row, int counter, bool verticalmatch)
{
    if (verticalmatch == true) {
        for (int j = 0; j < counter; j++) {
            fruitGrid_.at(col).at(row+j) = EMPTY;
            points_ += 1;
        }
    } else {
        for (int j = 0; j < counter; j++) {
            fruitGrid_.at(col+j).at(row) = EMPTY;
            points_ += 1;
        }
    }

    // Isommista poistoista pelaajalle annetaan lisäpisteitä
    if (counter == 4) {
        points_ += 1;
    } else if (counter == 5) {
        points_ += 3;
    }
}


void MainWindow::drop_blocks()
{
    // Pudottaa hedelmiä niin kauas kunnes ruutujen alta ei löydy enää tyhjiä
    // Pidetään kirjaa tyhjistä ruuduista apumuuttujilla emptiesBelow (onko minkään hedelmän alla tyhjää kohtaa)
    // ja emptyFirstRow (onko ylimmällä rivillä tyhjää).
        Fruit_kind currentFruit = EMPTY;
        Fruit_kind belowFruit = EMPTY;
        bool emptiesBelow = false;
        bool emptyFirstRow = false;
        for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
            for (int row = GRID_ROW_SIZE_-1; row >= 0; row--) {
                if (fruitGrid_.at(col).at(0) == EMPTY) {
                    emptyFirstRow = true;
                }
                currentFruit = fruitGrid_.at(col).at(row);
                belowFruit = fruitGrid_.at(col).at(row+1);

                // Jos alla on tyhjä ruutu ja tämänhetkisessä tarkastelussa oleva ruutu EI ole tyhjä,
                // pudotetaan tämänhetkinen hedelmä alla olevaan tyhjään ruutuun ja jätetään nykyinen ruutu tyhjäksi.
                if (belowFruit == EMPTY && currentFruit != EMPTY) {
                    emptiesBelow = true;
                    fruitGrid_.at(col).at(row+1) = currentFruit;
                    fruitGrid_.at(col).at(row) = EMPTY;
                }
            }
        }

        // Funktio kutsuu itseään, jos jonkun ruudun alta löytyy vielä tyhjiä ruutuja.
        // Jos kaikki ruudut ovat pudonneet alas ja yläriviin on muodostunut tyhjiä kohtia,
        // täytetään tyhjät kohdat funktiolla refill_blocks ja pudotetaan ne jälleen alas.
        if (emptiesBelow == true) {
            drop_blocks();
        } else {
            if (refill_ == true && emptyFirstRow == true) {
                refill_blocks();
                drop_blocks();
            }
        }

        // Tarkastellaan onko pudotusten johdosta syntynyt matcheja ja päivitetään ruudun näkymää.
    check_matches();
    update_screen();
    return;
}

// Täytetään ruudukon ylin rivi uusilla, arvotuilla hedelmillä (mikäli ruutu on tyhjä).
void MainWindow::refill_blocks()
{
    for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
        if (fruitGrid_.at(col).at(0) != EMPTY) {
            continue;
        }
        Fruit_kind newfruit = static_cast<Fruit_kind>(distr_(randomEng_));
        fruitGrid_.at(col).at(0) = newfruit;
    }
}

// Lukitaan/avataan syöteruudut ja liikkumisnappi.
// Käytetään pelin resetoinnissa ja pelin loppuessa
void MainWindow::lock_buttons(bool lockup)
{
    if (lockup == true) {
        ui->startPointLine->setDisabled(true);
        ui->endPointLine->setDisabled(true);
        ui->moveButton->setDisabled(true);
    } else {
        ui->startPointLine->setDisabled(false);
        ui->endPointLine->setDisabled(false);
        ui->moveButton->setDisabled(false);
    }
}

/* Esittää pelaajan valitsemien ruutujen koordinaatit peli-ikkunassa.
 * Käyttää private apumuuttujaa turn_ määrittelemään kumpi ruutu on kyseessä
 * (ensimmäinen vai jälkimmäinen klikkaus). Kerrottu lisää funktion
 * fruitButton_clicked kommentoinnissa.
*/
void MainWindow::display_move_coordinates(int x, int y)
{
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString currentColumn = QString(alphabet.at(x));
    QString currentRow = QString(alphabet.at(y));
    if (turn_ % 2 != 0) {
        ui->startPointLine->setText(currentColumn+":"+currentRow);
    } else {
        ui->endPointLine->setText(currentColumn+":"+currentRow);
    }
}

// Luodaan ikkuna, jossa kerrotaan pelin päättymisestä/voitosta ja pelaajan lopulliset pisteet.
// Parametri message vaihtelee sen mukaan, onko pelaaja voittanut vai keskeyttänyt pelinsä.
// Pelin päätyttyä pelaajalle näytetään hänen ansaitsemansa pisteet.
void MainWindow::send_message(QString message)
{
    QString score = QString::number(points_);
    msg_.setText(message);
    msg_.setInformativeText("Final score: " + score);
    msg_.setStandardButtons(QMessageBox::Ok);
    msg_.setIcon(QMessageBox::Information);
    msg_.exec();
}

// Tarkastetaan onko pelaaja voittanut pelin (kaikki ruudut tyhjinä eli pelaaja ei ole valinnut jatkuvaa pelitilaa).
bool MainWindow::check_win()
{
    for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
        for (int row = 0; row < GRID_ROW_SIZE_+1; row++) {
            if (fruitGrid_.at(col).at(GRID_ROW_SIZE_) != EMPTY) {
                return false;
            }
        }
    }
    return true;
}

// Liikkumisnappulan painalluksesta tapahtuvat toiminnot.
void MainWindow::on_moveButton_clicked()
{
    // Kun peli on alkamaton (pisteet nollissa), käynnistetään kello
    // ja vaihdetaan reset-napin teksti, jolloin käyttäjä voi lopettaa pelin.
    if (points_ == 0) {
        timer_->start();
        ui->resetButton->setText("End game");
    }
    lock_buttons(true);
    // Vaihdetaan valittujen hedelmien paikat,
    swap_blocks();
    // päivitetään ruutu vaihdon jälkeen
    update_screen();
    // ja tarkistetaan syntyykö vaihdosta matcheja.
    check_matches();
    // Siirron jälkeen pudotetaan palikoita tarvittaessa.
    QTimer::singleShot(delay_, this, SLOT(drop_blocks()));

    /* Jokaisen siirron päätteeksi tarkistetaan, onko ruudukko tyhjä eli peli voitettu.
     * Jos on, ilmoitetaan voitosta pelaajalle erillisessä ikkunassa
     * ja lukitaan liikkumiskomennot. Muutetaan reset-napin teksti, josta pelin
     * voi käynnistää uudestaan.
    */
    if (refill_ == false) {
        if (check_win() == true) {
            send_message("A winner is you!");
            timer_->stop();
            ui->resetButton->setText("Play again?");
            lock_buttons(true);
            points_ = 0;
        }
    }
    // Unohdetaan siirrossa valitut ruudut ja tyhjennetään
    // koordinaattikentät jokaisen siirron päätteeksi
    // vahinkosiirtojen välttämiseksi.
    startPoint_ = {-1, -1};
    endPoint_ = {-1, -1};
    ui->startPointLine->clear();
    ui->endPointLine->clear();
    lock_buttons(false);
    return;
}

// Funktio, joka tarkistaa muutoksen viiveasetuksen checkboxissa.
// Oletuksena yhden sekunnin viive palikoiden pudotuksessa.
int MainWindow::on_delayCheck_stateChanged(int arg1)
{
    if (arg1 == false) {
        return delay_ = 0;
    }
    return delay_ = 1000;
}

// Funktio, joka tarkistaa muutoksen ruudukon täydennyksen checkboxissa.
// Oletuksena pois päältä (ei täydennystä).
void MainWindow::on_refillCheck_stateChanged()
{
    if (ui->refillCheck->isChecked()) {
        refill_ = true;
        return;
    }
    refill_ = false;
}

// Lopettaa alkaneen (keskeytetyn) pelin ja lukitsee ruutujen siirtelyn, jolloin
// pelaajan on aloitettava uusi peli. Jos pisteitä ei vielä ole, nollaa ja alutaa pelilaudan, pisteet ja kellon.
void MainWindow::on_resetButton_clicked()
{
    if (points_ > 0) {
        timer_->stop();
        lock_buttons(true);
        send_message("Game over!");
        points_ = 0;
        ui->resetButton->setText("Play again?");
        return;
    }
    timer_->stop();
    update_timer(0,0);
    lock_buttons(false);
    ui->resetButton->setText("Reset game");
    init_game();
}

/* Apufunktio, jota kutsutaan klikkaamalla jotakin peliruudukon ruuduista.
 * Tallettaa hiiren sijainnin ikkunassa klikkaamisen hetkellä ja erottelee
 * ne X/Y-koordinaateiksi, joista pystytään laskemaan peliruudun mitoilla
 * minkä sarakkeen ja rivin kohdalla klikkaus tapahtui.
*/
void MainWindow::fruitButton_clicked()
{
    QPoint pos = QCursor::pos();
    int posX = pos.x();
    int posY = pos.y();
    int columnPos = ((posX-LEFT_MARGIN)/SQUARE_SIDE)-1;
    int rowPos = ((posY-TOP_MARGIN-(SQUARE_SIDE/2))/SQUARE_SIDE)-1;

    // Laskun epätarkkuuden vuoksi hylätään klikkaukset, joissa
    // sarakkeet ja rivit ruudukon ulkopuolella
    if (columnPos > GRID_COL_SIZE_ || columnPos < 0) {
        return;
    } else if (rowPos > GRID_ROW_SIZE_ || rowPos < 0) {
        return;
    }

    // Käytetään apumuuttuja turn_, jolla joka toisella klikkauksella valitaan ensimmäinen ruutu
    // ja joka toisella toinen liikuteltava ruutu. Jokaisen onnistuneen klikkauksen jälkeen
    // turn kasvaa yhdellä.
    if (turn_ % 2 != 0) {
        startPoint_[0] = columnPos;
        startPoint_[1] = rowPos;
        display_move_coordinates(columnPos, rowPos);
    } else {
        endPoint_[0] = columnPos;
        endPoint_[1] = rowPos;
        display_move_coordinates(columnPos, rowPos);
    }
    turn_ += 1;
    return;
}

// Muuttaa pelilaudan kokoa haluttuun X kertaa Y kokoon.
void MainWindow::on_gridSettingsButton_clicked()
{
    // Ruudukon koon hakeminen Line Edit-laatikoista ja muuttaminen luvuiksi
    QString gridX = ui->gridSizeXLine->text();
    QString gridY = ui->gridSizeYLine->text();
    if (gridX != "" || gridY != "") {
        qint32 gridXint = gridX.toInt();
        qint32 gridYint = gridY.toInt();

        // Ruudukossa toisen dimension on oltava vähintään kolme, jotta peliä pystyy pelaamaan.
        if (gridXint < 2 || gridYint < 2) {
            return;
        } else {
            GRID_COL_SIZE_ = gridXint-1;
            GRID_ROW_SIZE_ = gridYint-1;

            // Poistetaan vanha ruudukon peli-ikkuna ja luodaan uusi syötyistä mitoista.
            // Ruutujen ja ruudukon ikkunan välille jätetty "kehykseksi" 2 pikseliä.
            // Peli nollataan ja lauta alustetaan uudestaan.
            delete scene_;
            scene_ = new QGraphicsScene(this);
            ui->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                          (SQUARE_SIDE*gridXint)+2, (SQUARE_SIDE*gridYint)+2);
            ui->graphicsView->setScene(scene_);
            scene_->setSceneRect(0, 0, SQUARE_SIDE*gridXint, SQUARE_SIDE*gridYint);
            points_ = 0;
            ui->gridSizeXLine->clear();
            ui->gridSizeYLine->clear();
            on_resetButton_clicked();
            return;
        }
    }
    // Jos tekstikentät jätetään tyhjäksi ja nappia on painettu, alustetaan pelilauta oletuskoolla (6x6)
    GRID_COL_SIZE_ = COLUMNS-1;
    GRID_ROW_SIZE_ = ROWS-1;
    ui->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);
    on_resetButton_clicked();
}

// Apunappi ruutujen siirtelyyn. Pelaajan klikatessa "?" nappia, aukeaa ruutuun ikkuna, jossa lukee pikaisia
// ohjeita pelin toiminnallisuuteen liittyen.
void MainWindow::on_moveHelpButton_clicked()
{
    msg_.setText("Ohjeet pelaamiseen");
    msg_.setInformativeText("- Yhdistä kolme tai useampi samanlaista hedelmää pysty- tai vaakasuunnassa! \n"
                            "- Vaihda kahden ruudun paikkaa keskenään klikkaamalla kahta vierekkäistä ruutua,"
                            "jolloin valittujen ruutujen koordinaatit näkyvät kentissä Start ja End \n"
                            "- Onnistunut suora poistaa hedelmät pelistä \n"
                            "- Värillisen ruudun siirtäminen tyhjään ruutuun on mahdollista ja korkealta \n"
                            "tehtynä aiheuttaa sen putoamisen.");
    msg_.addButton(QMessageBox::Ok);
    msg_.setIcon(QMessageBox::Information);
    msg_.exec();
}

// Apunappi asetuksille. Avaa ruutuun ikkunan, jossa on lyhyet selitykset asetusten toiminnasta ja käyttöönotosta.
void MainWindow::on_settingsHelpButton_clicked()
{
    msg_.setText("Ohjeet asetuksille");
    msg_.setInformativeText("- Reflex mode: antaa sekunnin aikaviiveen ennen hedelmien tippumista (oletusarvo: päällä) \n"
                            "- Infinite mode: jatkaa hedelmien lisäämistä ruudukkoon, kun ylälaitaan syntyy "
                            "tyhjiä kohtia \n"
                            "\n"
                            "Asetukset astuvat voimaan heti \n"
                            "(ei vaadi nollausta)");

    msg_.setIcon(QMessageBox::Information);
    msg_.exec();
}

// Apunappi ruudukon koon muuttamiselle. Avaa ruutuun ikkunan, jossa on ohjeet ruudukon koon säätelystä.
void MainWindow::on_gridHelpButton_clicked()
{
    msg_.setText("Ohjeet ruudun muokkaamiseen");
    msg_.setInformativeText("- Muokkaa peliruudukko eri kokoon syöttämällä tekstikenttiin haluamasi "
                            "sarakkeiden ja rivien määrä \n"
                            "- Toisen määristä on oltava vähintään 3, jotta peli pysyy toiminnallisena \n"
                            "- Voit palauttaa vakioruudukon (6x6) jättämällä kentät tyhjiksi. \n"
                            "- Suositeltu koko alle 10x10"
                            "\n"
                            "Ota ruudukko käyttöön painamalla 'Apply and reset' \n"
                            "(vaatii nykyisen ruudukon nollaamisen)");
    msg_.setIcon(QMessageBox::Information);
    msg_.exec();
}
