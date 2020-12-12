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

    checkSettings();

    init_grids();

    init_titles();

    init_score();

    init_timer();

    connect(timer_, &QTimer::timeout, this, &MainWindow::update_timer);

    // More code perhaps needed
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

void MainWindow::draw_fruit()
{
    // Vector of fruits
    const std::vector<std::string>
            fruits = {"cherries", "strawberry", "orange", "pear", "apple",
                      "bananas", "tomato", "grapes", "eggplant"};

    // Defining where the images can be found and what kind of images they are
    const std::string PREFIX(":/");
    const std::string SUFFIX(".png");

    // Converting image (png) to a pixmap
    int i = 0; // try different values in 0 <= i < fruits.size()
    std::string filename = PREFIX + fruits.at(i) + SUFFIX;
    QPixmap image(QString::fromStdString(filename));

    // Scaling the pixmap
    image = image.scaled(SQUARE_SIDE, SQUARE_SIDE);

    // Setting the pixmap for a new label
    QLabel* label = new QLabel("test", this);
    label->setGeometry(LEFT_MARGIN + COLUMNS * SQUARE_SIDE,
                       TOP_MARGIN + ROWS * SQUARE_SIDE,
                       SQUARE_SIDE, SQUARE_SIDE);
    label->setPixmap(image);
}

void MainWindow::init_grids()
{
    grid_.clear();
    numbergrid_.clear();
    std::vector<QGraphicsRectItem*> tempvector = {};
    vector<Fruit_kind> tempvec = {};
    for (int i = 0; i < GRID_COL_SIZE_+1; i++) {
        for (int j = 0; j < GRID_ROW_SIZE_+1; j++) {
            tempvector.push_back(rect_);
            int rndm = distr_(randomEng_);
            Fruit_kind fruit = static_cast<Fruit_kind>(rndm);
            tempvec.push_back(fruit);

            QPen blackPen(Qt::black);
            QBrush colorBrush = paint_block(rndm);
            rect_ = scene_->addRect(i*SQUARE_SIDE,j*SQUARE_SIDE,SQUARE_SIDE,SQUARE_SIDE, blackPen, colorBrush);
        }
        grid_.push_back(tempvector);
        tempvector.clear();

        numbergrid_.push_back(tempvec);
        tempvec.clear();
    }
    if (check_matches() == true) {
        init_grids();
    }
}

void MainWindow::swap_blocks()
{
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    string start = on_startPoint_editingFinished();
    string end = on_endPoint_editingFinished();
    if (start.size() > 2 || end.size() > 2 || start == "" || end == "") {
        return;
    }
    int startX = alphabet.find(start.at(0)); int startY = alphabet.find(start.at(1));
    int endX = alphabet.find(end.at(0)); int endY = alphabet.find(end.at(1));
    if (startX > GRID_COL_SIZE_ || startY > GRID_ROW_SIZE_) {
        return;
    } else if (endX > GRID_COL_SIZE_ || endY > GRID_ROW_SIZE_) {
        return;
    }

    int distance = abs(startX-endX + startY-endY);
    if (distance != 1) {
        return;
    }

    Fruit_kind temp;
    temp = numbergrid_.at(startX).at(startY);
    numbergrid_.at(startX).at(startY) = numbergrid_.at(endX).at(endY);
    numbergrid_.at(endX).at(endY) = temp;

}

bool MainWindow::check_matches()
{
    // Pystysuuntaiset matchit
    Fruit_kind currentnum = EMPTY;
    bool matchFound = false;

    for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
        int counter = 1;
        for (int row = 0; row < GRID_ROW_SIZE_-1; row++) {
            currentnum = numbergrid_.at(col).at(row);
            if (currentnum == EMPTY) {
                continue;
            }
            for (int nextrow = row+1; nextrow < GRID_ROW_SIZE_+1; nextrow++) {
                Fruit_kind nextfruit = numbergrid_.at(col).at(nextrow);
                if (nextfruit == currentnum) {
                    counter++;
                } else {
                    if (counter > 2) {
                        matchFound = true;
                        for (int j = 0; j < counter; j++) {
                            numbergrid_.at(col).at(row+j) = EMPTY;
                            points_ += 1;
                        }
                    }
                    counter = 1;
                    break;
                }
                if (nextrow == GRID_ROW_SIZE_) {
                    if (counter > 2) {
                        matchFound = true;
                        for (int j = 0; j < counter; j++) {
                            numbergrid_.at(col).at(row+j) = EMPTY;
                            points_ += 1;
                        }
                        counter = 1;
                    }
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
                } else {
                    if (counter > 2) {
                        matchFound = true;
                        for (int j = 0; j < counter; j++) {
                            numbergrid_.at(col+j).at(row) = EMPTY;
                            points_ += 1;
                        }
                    }
                    counter = 1;
                    break;
                }
                if (nextcol == GRID_COL_SIZE_) {
                    if (counter > 2) {
                        matchFound = true;
                        for (int j = 0; j < counter; j++) {
                            numbergrid_.at(col+j).at(row) = EMPTY;
                            points_ += 1;
                        }
                        counter = 1;
                    }
                }
            }
        }
    }

    if (matchFound) {
        ui->lcdNumberScore->display(points_);
        update_screen();
        QTimer::singleShot(delay_, this, SLOT(drop_blocks()));
        return true;
    }
    return false;
}


void MainWindow::drop_blocks()
{
    // Pudottaa hedelmiä niin kauas kunnes ruutujen alta ei löydy enää tyhjiä
    while (true) {
        Fruit_kind currentFruit = EMPTY;
        Fruit_kind belowFruit = EMPTY;
        bool emptybelow = false;
        for (int col = 0; col < COLUMNS; col++) {
            for (int row = GRID_COL_SIZE_-1; row >= 0; row--) {
                currentFruit = numbergrid_.at(col).at(row);
                belowFruit = numbergrid_.at(col).at(row+1);
                if (belowFruit == EMPTY && currentFruit != EMPTY) {
                    emptybelow = true;
                    numbergrid_.at(col).at(row+1) = currentFruit;
                    numbergrid_.at(col).at(row) = EMPTY;
                }
            }
        }
        update_screen();
        if (emptybelow == false) {
            if (refill_ == true) {
                refill_blocks();
            }
            check_matches();
            break;
        }
    }
    return;
}

void MainWindow::update_screen()
{
    for (int i = 0; i < GRID_COL_SIZE_+1; i++) {
        for (int j = 0; j < GRID_ROW_SIZE_+1; j++) {
            Fruit_kind colornumber = numbergrid_.at(i).at(j);
            QPen blackPen(Qt::black);
            QColor backGroundColor(225, 225, 225);
            if (colornumber == EMPTY) {
                rect_ = scene_->addRect(i*SQUARE_SIDE, j*SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE, blackPen, backGroundColor);
            } else {
                QBrush colorBrush = paint_block(colornumber);
                rect_ = scene_->addRect(i*SQUARE_SIDE,j*SQUARE_SIDE,SQUARE_SIDE,SQUARE_SIDE, blackPen, colorBrush);
            }
        }
    }
}

void MainWindow::refill_blocks()
{
    for (int col = 0; col < GRID_COL_SIZE_+1; col++) {
        if (numbergrid_.at(col).at(0) != EMPTY) {
            continue;
        }
        Fruit_kind newfruit = static_cast<Fruit_kind>(distr_(randomEng_));
        numbergrid_.at(col).at(0) = newfruit;
    }
    update_screen();
    QTimer::singleShot(delay_, this, SLOT(drop_blocks()));
}

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

void MainWindow::checkSettings()
{
    on_delayCheck_stateChanged(ui->delayCheck->checkState());
}

void MainWindow::init_score()
{
    palette_.setColor(QPalette::Background, Qt::black);
    ui->lcdNumberScore->setPalette(palette_);
    ui->lcdNumberScore->setAutoFillBackground(true);
    points_ = 0;
    ui->lcdNumberScore->display(0);
}

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

void MainWindow::lock_buttons(bool lockup)
{
    if (lockup == true) {
        ui->startPoint->setDisabled(true);
        ui->endPoint->setDisabled(true);
        ui->moveButton->setDisabled(true);
    } else {
        ui->startPoint->setDisabled(false);
        ui->endPoint->setDisabled(false);
        ui->moveButton->setDisabled(false);
    }
}

void MainWindow::update_timer()
{
    int mins = ui->lcdNumberMins->intValue();
    int secs = ui->lcdNumberSecs->intValue();

    if (secs == 59) {
        ui->lcdNumberMins->display(mins+1);
        ui->lcdNumberSecs->display(0);
    } else {
        ui->lcdNumberMins->display(mins);
        ui->lcdNumberSecs->display(secs+1);
    }
}

string MainWindow::on_startPoint_editingFinished()
{
    string startPoint = ui->startPoint->text().toStdString();
    return startPoint;
}

string MainWindow::on_endPoint_editingFinished()
{
    string endPoint = ui->endPoint->text().toStdString();
    return endPoint;
}

void MainWindow::on_moveButton_clicked()
{
    if (points_ == 0) {
        timer_->start();
    }
    swap_blocks();
    update_screen();
    check_matches();
    QTimer::singleShot(delay_, this, SLOT(drop_blocks()));
    ui->resetButton->setText("End game");
    return;
}

int MainWindow::on_delayCheck_stateChanged(int arg1)
{
    if (arg1 == false) {
        return delay_ = 0;
    }
    return delay_ = 1000;

}

void MainWindow::on_resetButton_clicked()
{
    if (points_ > 0) {
        QString score = QString::number(points_);
        timer_->stop();
        lock_buttons(true);
        msg_.setText("Final score: ");
        msg_.setInformativeText(score);
        msg_.setStandardButtons(QMessageBox::Ok);
        points_ = 0;
        ui->resetButton->setText("Play again?");
        return;
    }
    ui->resetButton->setText("Reset game");
    lock_buttons(false);
    grid_.clear();
    numbergrid_.clear();
    init_grids();
    init_score();
}

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
            init_grids();
            init_titles();
            return;
        }
    }
    GRID_COL_SIZE_ = COLUMNS-1;
    GRID_ROW_SIZE_ = ROWS-1;
    init_grids();
    init_titles();
}

void MainWindow::on_refillCheck_stateChanged()
{
    if (ui->refillCheck->isChecked()) {
        refill_ = true;
        return;
    }
    refill_ = false;
}
