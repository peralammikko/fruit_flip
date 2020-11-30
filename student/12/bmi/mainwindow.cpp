#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::checkBmi(double index)
{
    QString indexResult = "";
    if (index < 18.5) {
        indexResult = "You are underweight.";
    } else if (index > 25) {
        indexResult = "You are overweight.";
    } else {
        indexResult = "Your weight is normal.";
    }
    return indexResult;
}


void MainWindow::on_weightLineEdit_editingFinished()
{
    QString wString = ui->weightLineEdit->text();
    weight_ = wString.toInt();
}

void MainWindow::on_heightLineEdit_editingFinished()
{
    QString hString = ui->heightLineEdit->text();
    height_ = hString.toDouble()*0.01;
}

void MainWindow::on_countButton_clicked()
{
    double num = weight_;
    double denom = height_*height_;
    double bmi = num/denom;
    QString bmiInfo = checkBmi(bmi);
    if (bmi > 0) {
        ui->resultLabel->setNum(bmi);
        ui->infoTextBrowser->setText(bmiInfo);
    }
}

void MainWindow::on_closeButton_clicked()
{
    close();
}
