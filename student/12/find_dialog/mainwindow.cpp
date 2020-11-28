#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <fstream>
#include <QDebug>

using namespace std;

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


void MainWindow::on_fileLineEdit_editingFinished()
{
    fileName_ = ui->fileLineEdit->text();
}

void MainWindow::on_keyLineEdit_editingFinished()
{
    findText_ = ui->keyLineEdit->text();
}

void MainWindow::printText(QString &output)
{
    ui->textBrowser->setText(output);
}

void MainWindow::on_findPushButton_clicked()
{
    string fileString = fileName_.toStdString();
    ifstream read_file(fileString);
    QString output = "File not found";
    if (not read_file) {
        printText(output);
    } else {
        if (findText_ == "") {
            printText(output = "File found");
            return;
        }
        string row = "";
        string keyString = findText_.toStdString();
        while (getline(read_file, row)) {
            if (not ui->matchCheckBox->isChecked()) {
               transform(keyString.begin(),keyString.end(),keyString.begin(), ::toupper);
               transform(row.begin(),row.end(),row.begin(),::toupper);
            }
            if (row.find(keyString) != string::npos) {
                printText(output = "Word found");
                return;
            }
        }
        printText(output = "Word not found");
    }
}
