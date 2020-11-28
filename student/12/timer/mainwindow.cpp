#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setColor(QPalette::Background, Qt::black);
    ui->lcdNumberSec->setPalette(palette);
    ui->lcdNumberSec->setAutoFillBackground(true);

    ui->lcdNumberMin->setPalette(palette);
    ui->lcdNumberMin->setAutoFillBackground(true);

    timer = new QTimer();

    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &MainWindow::on_timeout);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_timeout()
{
    int current_secs = ui->lcdNumberSec->intValue();
    int current_mins = ui->lcdNumberMin->intValue();

    if (current_secs == 59) {
        update_display(current_mins+1, current_secs = 0);
    } else {
        update_display(current_mins, current_secs+1);
    }
}

void MainWindow::on_startButton_clicked()
{
    timer->start();
}

void MainWindow::on_stopButton_clicked()
{
    timer->stop();
}

void MainWindow::on_resetButton_clicked()
{
    update_display(0, 0);
}

void MainWindow::update_display(int minutes, int seconds)
{
    ui->lcdNumberMin->display(minutes);
    ui->lcdNumberSec->display(seconds);
}
