#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QDebug>
#include <QPixmap>
#include <vector>

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
    delete timer_;
    grid_.clear();
    numbergrid_.clear();
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
*/
void MainWindow::init_grids()
{
    qDebug() << "init grid begin";
    grid_.clear();
    numbergrid_.clear();
    std::vector<QPushButton*> buttonVec = {};
    vector<Fruit_kind> fruitVec = {};
    for (int i = 0; i < GRID_COL_SIZE_+1; i++) {
        for (int j = 0; j < GRID_ROW_SIZE_+1; j++) {
            QPushButton* newButton = new QPushButton;
            buttonVec.push_back(newButton);

            int rndm = distr_(randomEng_);
            Fruit_kind fruit = static_cast<Fruit_kind>(rndm);
            fruitVec.push_back(fruit);
        }
        grid_.push_back(buttonVec);
        buttonVec.clear();

        numbergrid_.push_back(fruitVec);
        fruitVec.clear();
    }
    qDebug() << "init grid end";

    // Jos alustetulta pelilaudalta löytyy matcheja, alustetaan pelilauta uudestaan
    if (check_matches() == true) {
        init_grids();
    }

    update_screen();
}

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

// Etsitään laudalla olevat ruuduille (hedelmille) oikeanlaiset värit
// käyttämällä headerissa määriteltyä numeroitua enum-rakennetta
QBrush MainWindow::paint_block(int numbr)
{
    QBrush brush(Qt::black);
    switch(numbr) {
    case PLUM: brush.setColor(Qt::magenta); break;
    case STRAWBERRY: brush.setColor(Qt::red); break;
    case APPLE: brush.setColor(Qt::green); break;
    case LEMON: brush.setColor(Qt::yellow); break;
    case BLUEBERRY: brush.setColor(Qt::blue); break;
    case ORANGE: brush.setColor(QColor (255,155,51)); break;
    case EMPTY: brush.setColor(QColor (225,225,225)); break;
    }
    return brush;
}

// Alustetaan pistelaskuri (nollataan pisteet ja taulu).
void MainWindow::init_score()
{
    palette_.setColor(QPalette::Background, Qt::black);
    ui->lcdNumberScore->setPalette(palette_);
    ui->lcdNumberScore->setAutoFillBackground(true);
    points_ = 0;
    ui->lcdNumberScore->display(0);
}

// Alustetaan peli-ikkunan kello
void MainWindow::init_timer()
{
    timer_ = new QTimer;
    timer_->setInterval(1000);
    palette_.setColor(QPalette::Background, Qt::black);
    ui->lcdNumberMins->setPalette(palette_);
    ui->lcdNumberMins->setAutoFillBackground(true);

    ui->lcdNumberSecs->setPalette(palette_);
    ui->lcdNumberSecs->setAutoFillBackground(true);
}

// Sekuntikellon toiminnallisuus, johon timer yhdistetään ikkunan auetessa
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
    qDebug() << "Updating begin";
    scene_->clear();
    for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
        qDebug() << "col "<< col << " updated";
        for (int row = 0; row < GRID_ROW_SIZE_+1; row++) {
            Fruit_kind fruitInGrid = numbergrid_.at(col).at(row);
            QPushButton* fruitButton = new QPushButton;
            connect(fruitButton, &QPushButton::clicked, this, &MainWindow::fruitButton_clicked);

            QPalette pal = fruitButton->palette();
            pal.setColor(QPalette::Button, paint_button(fruitInGrid));
            fruitButton->setPalette(pal);

            fruitButton->setGeometry(col*SQUARE_SIDE, row*SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE);
            scene_->addWidget(fruitButton);

            /*
            QPen blackPen(Qt::black);
            QColor backGroundColor(225, 225, 225);

            if (fruitInGrid == EMPTY) {

                rect_ = scene_->addRect(i*SQUARE_SIDE, j*SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE, blackPen, backGroundColor);

            } else {


                QBrush colorBrush = paint_block(fruitInGrid);
                rect_ = scene_->addRect(i*SQUARE_SIDE,j*SQUARE_SIDE,SQUARE_SIDE,SQUARE_SIDE, blackPen, colorBrush);

            }
            */
        }
    }
    qDebug() << "Screen updated";
}


void MainWindow::swap_blocks()
{
    qDebug() << "swapping begin";
    /*
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    string start = on_startPoint_editingFinished();
    string end = on_endPoint_editingFinished();
    if (start.size() > 2 || end.size() > 2 || start == "" || end == "") {
        return;
    }
    int startX = alphabet.find(start.at(0)); int startY = alphabet.find(start.at(1));
    int endX = alphabet.find(end.at(0)); int endY = alphabet.find(end.at(1));
    */
    int startX = startPoint_.at(0); int startY = startPoint_.at(1);
    int endX = endPoint_.at(0); int endY = endPoint_.at(1);

    if (startX > GRID_COL_SIZE_ || startX < 0 || startY > GRID_ROW_SIZE_ || startY < 0) {
        return;
    } else if (endX > GRID_COL_SIZE_ || endX < 0 || endY > GRID_ROW_SIZE_ || endX < 0) {
        return;
    }

    int distanceX = abs(startX-endX);
    int distanceY = abs(startY-endY);
    if (distanceX > 1 && distanceY > 1) {
        return;
    }

    Fruit_kind temp;
    temp = numbergrid_.at(startX).at(startY);
    numbergrid_.at(startX).at(startY) = numbergrid_.at(endX).at(endY);
    numbergrid_.at(endX).at(endY) = temp;

    qDebug() << "Blocks swapped";

}

bool MainWindow::check_matches()
{
    qDebug() << "matching begin";
    // Pystysuuntaiset matchit
    Fruit_kind currentnum = EMPTY;
    bool matchFound = false;

    for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
        int counter = 1;
        for (int row = 0; row < GRID_ROW_SIZE_-1; row++) {
            currentnum = numbergrid_.at(col).at(row);

            // Tyhjistä palikoista muodostuvia matcheja ei huomioida, joten niiden tarkastelusta hypätään yli
            if (currentnum == EMPTY) {
                continue;
            }

            /* Tarkastetaan jokaisen ruudunkon hedelmän kohdalla montako samanlaista löydetään seuraavista hedelmistä
             * (ruudukon reunaan asti).
             * Jos hedelmäX ja hedelmäY ovat samat, lisätään laskuriin +1.
             * Jos hedelmäX ja hedelmäZ ovat samat, lisätään laskuriin+1.
            */
            for (int nextrow = row+1; nextrow < GRID_ROW_SIZE_+1; nextrow++) {
                Fruit_kind nextfruit = numbergrid_.at(col).at(nextrow);
                if (nextfruit == currentnum) {
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
                     * ja siirrytään seuraavaan hedelmään.
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

    // Vaakasuuntaiset matchit
    for (int col = 0; col < GRID_COL_SIZE_-1; col++) {
        int counter = 1;
        for (int row = 0; row < GRID_ROW_SIZE_+1; row++) {
            currentnum = numbergrid_.at(col).at(row);
            if (currentnum == EMPTY) {
                continue;
            }
              for (int nextcol = col+1; nextcol < GRID_COL_SIZE_+1; nextcol++) {
                Fruit_kind nextfruit = numbergrid_.at(nextcol).at(row);
                if (nextfruit == currentnum) {
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
        qDebug() << "match found, rerolling";
        ui->lcdNumberScore->display(points_);
        update_screen();
        QTimer::singleShot(delay_, this, SLOT(drop_blocks()));
        return true;
    }
    qDebug() << "matching end";
    return false;
}

// Poistaa ruudukossa olevia hedelmäsuoria niin monta kuin counter-laskurissa määrätään.
void MainWindow::remove_matched_blocks(int col, int row, int counter, bool verticalmatch)
{
    if (verticalmatch == true) {
        for (int j = 0; j < counter; j++) {
            numbergrid_.at(col).at(row+j) = EMPTY;
            points_ += 1;
            qDebug() << "block removed at " << col << ": " << row+j;
        }
    } else {
        for (int j = 0; j < counter; j++) {
            numbergrid_.at(col+j).at(row) = EMPTY;
            points_ += 1;
            qDebug() << "block removed at " << col+j << ": " << row;
        }
    }
}


void MainWindow::drop_blocks()
{
    qDebug() << "drop begin";
    // Pudottaa hedelmiä niin kauas kunnes ruutujen alta ei löydy enää tyhjiä
    while (true) {
        Fruit_kind currentFruit = EMPTY;
        Fruit_kind belowFruit = EMPTY;
        bool emptyBelow = false;
        for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
            for (int row = GRID_ROW_SIZE_-1; row >= 0; row--) {
                currentFruit = numbergrid_.at(col).at(row);
                belowFruit = numbergrid_.at(col).at(row+1);

                // Jos alla on tyhjä ruutu ja tämänhetkisessä tarkastelussa oleva ruutu EI ole tyhjä,
                // pudotetaan tämänhetkinen hedelmä alla olevaan tyhjään ruutuun ja jätetään nykyinen ruutu tyhjäksi.
                if (belowFruit == EMPTY && currentFruit != EMPTY) {
                    emptyBelow = true;
                    numbergrid_.at(col).at(row+1) = currentFruit;
                    numbergrid_.at(col).at(row) = EMPTY;
                }
            }
        }

        // Kun palikoiden alta ei enää löydy tyhjiä ruutuja, tarkistetaan onko pudonneiden
        // palikoiden johdosta syntynyt uusia matcheja. Tiputetaan myös rivi uusia hedelmiä
        // ruudun ylälaitaan mikäli käyttäjä on valinnut jatkuvan pelimuodon.
        if (emptyBelow == false) {
            if (refill_ == true) {
                refill_blocks();
            }
            break;
        }
    }
    check_matches();
    qDebug() << "dropping end";
    update_screen();
    return;
}

// Täytetään ruudukon ylin rivi uusilla, arvotuilla hedelmillä (mikäli ruutu on tyhjä).
void MainWindow::refill_blocks()
{
    for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
        if (numbergrid_.at(col).at(0) != EMPTY) {
            continue;
        }
        Fruit_kind newfruit = static_cast<Fruit_kind>(distr_(randomEng_));
        numbergrid_.at(col).at(0) = newfruit;
    }
    // Päivitetään ruutu ja kutsutaan drop_blocks -funktiota pudottamaan luodut hedelmät (viiveellä).
    update_screen();
    QTimer::singleShot(delay_, this, SLOT(drop_blocks()));
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

// Luodaan ikkuna, jossa kerrotaan pelin päättymisestä/voitosta (message) ja pelaajan lopulliset pisteet.
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
            if (numbergrid_.at(col).at(GRID_ROW_SIZE_) != EMPTY) {
                return false;
            }
        }
    }
    return true;
}

// Haetaan valitun lähtöruudun koordinaatit (2 kirjainta)
string MainWindow::on_startPoint_editingFinished()
{
    string startPoint = ui->startPointLine->text().toStdString();
    return startPoint;
}

// Haetaan valitun pääteruudun koordinaatit
string MainWindow::on_endPoint_editingFinished()
{
    string endPoint = ui->endPointLine->text().toStdString();
    return endPoint;
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
    if (check_win() == true) {
        send_message("A winner is you!");
        timer_->stop();
        ui->resetButton->setText("Play again?");
        lock_buttons(true);
        points_ = 0;
    }
    startPoint_ = {-1, -1};
    endPoint_ = {-1, -1};
    ui->startPointLine->clear();
    ui->endPointLine->clear();
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

// Lopettaa alkaneen (keskeytetyn) pelin tai nollaa pelilaudan, pisteet ja kellon.
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

// Muuttaa pelilaudan kokoa haluttuun X kertaa Y kokoon.
void MainWindow::on_gridSettingsButton_clicked()
{
    QString gridX = ui->gridSizeXLine->text();
    QString gridY = ui->gridSizeYLine->text();
    if (gridX != "" || gridY != "") {
        qint32 gridXint = gridX.toInt();
        qint32 gridYint = gridY.toInt();
        if (gridXint > 2 || gridYint > 2) {
            GRID_COL_SIZE_ = gridXint-1;
            GRID_ROW_SIZE_ = gridYint-1;
            delete scene_;
            scene_ = new QGraphicsScene(this);
            ui->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                          (SQUARE_SIDE*gridXint)+10, (SQUARE_SIDE*gridYint)+10);
            ui->graphicsView->setScene(scene_);
            scene_->setSceneRect(0, 0, SQUARE_SIDE*gridXint+2, SQUARE_SIDE*gridYint+2);
            points_ = 0;
            on_resetButton_clicked();
            return;
        }
    }
    GRID_COL_SIZE_ = COLUMNS-1;
    GRID_ROW_SIZE_ = ROWS-1;
    on_resetButton_clicked();
}


void MainWindow::on_moveHelpButton_clicked()
{
    msg_.setText("Ohjeet pelaamiseen");
    msg_.setInformativeText("- Yhdistä kolme tai useampi samanlaista hedelmää pysty- tai vaakasuunnassa! \n"
                            "- Vaihda kahden ruudun paikkaa keskenään syöttämällä Start- ja End-ruutuihin koordinaatit haluamillesi "
                            "ruuduille muodossa xy \n"
                            "- Onnistunut suora poistaa hedelmät pelistä \n");
    msg_.addButton(QMessageBox::Ok);
    msg_.setIcon(QMessageBox::Information);
    msg_.exec();
}

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

void MainWindow::fruitButton_clicked()
{
    QPoint pos = QCursor::pos();
    int posX = pos.x();
    int posY = pos.y();
    int columnPos = ((posX-LEFT_MARGIN)/SQUARE_SIDE)-1;
    int rowPos = ((posY-TOP_MARGIN-(SQUARE_SIDE/2))/SQUARE_SIDE)-1;
    if (columnPos > GRID_COL_SIZE_ || columnPos < 0) {
        return;
    } else if (rowPos > GRID_ROW_SIZE_ || rowPos < 0) {
        return;
    }

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
