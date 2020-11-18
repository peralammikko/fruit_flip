/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *verticalButton;
    QPushButton *horizontalButton;
    QPushButton *moveButton;
    QGroupBox *verticalGroupBox;
    QRadioButton *upRadioButton;
    QRadioButton *downRadioButton;
    QGroupBox *horizontalGroupBox;
    QRadioButton *leftRadioButton;
    QRadioButton *rightRadioButton;
    QGraphicsView *graphicsView;
    QLabel *label;
    QLabel *label_2;
    QLabel *statusLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(701, 589);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalButton = new QPushButton(centralWidget);
        verticalButton->setObjectName(QString::fromUtf8("verticalButton"));
        verticalButton->setGeometry(QRect(130, 190, 83, 28));
        horizontalButton = new QPushButton(centralWidget);
        horizontalButton->setObjectName(QString::fromUtf8("horizontalButton"));
        horizontalButton->setGeometry(QRect(280, 190, 83, 28));
        moveButton = new QPushButton(centralWidget);
        moveButton->setObjectName(QString::fromUtf8("moveButton"));
        moveButton->setGeometry(QRect(430, 190, 83, 28));
        verticalGroupBox = new QGroupBox(centralWidget);
        verticalGroupBox->setObjectName(QString::fromUtf8("verticalGroupBox"));
        verticalGroupBox->setGeometry(QRect(130, 40, 120, 91));
        upRadioButton = new QRadioButton(verticalGroupBox);
        upRadioButton->setObjectName(QString::fromUtf8("upRadioButton"));
        upRadioButton->setGeometry(QRect(0, 20, 105, 26));
        downRadioButton = new QRadioButton(verticalGroupBox);
        downRadioButton->setObjectName(QString::fromUtf8("downRadioButton"));
        downRadioButton->setGeometry(QRect(0, 60, 105, 26));
        horizontalGroupBox = new QGroupBox(centralWidget);
        horizontalGroupBox->setObjectName(QString::fromUtf8("horizontalGroupBox"));
        horizontalGroupBox->setGeometry(QRect(280, 40, 120, 91));
        leftRadioButton = new QRadioButton(horizontalGroupBox);
        leftRadioButton->setObjectName(QString::fromUtf8("leftRadioButton"));
        leftRadioButton->setGeometry(QRect(0, 20, 105, 26));
        rightRadioButton = new QRadioButton(horizontalGroupBox);
        rightRadioButton->setObjectName(QString::fromUtf8("rightRadioButton"));
        rightRadioButton->setGeometry(QRect(0, 60, 105, 26));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(80, 270, 401, 192));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(130, 10, 151, 20));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(130, 160, 221, 20));
        statusLabel = new QLabel(centralWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(550, 190, 91, 20));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 701, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        verticalButton->setText(QCoreApplication::translate("MainWindow", "Up/Down", nullptr));
        horizontalButton->setText(QCoreApplication::translate("MainWindow", "Left/Right", nullptr));
        moveButton->setText(QCoreApplication::translate("MainWindow", "Move", nullptr));
        verticalGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Vertical", nullptr));
        upRadioButton->setText(QCoreApplication::translate("MainWindow", "Up", nullptr));
        downRadioButton->setText(QCoreApplication::translate("MainWindow", "Down", nullptr));
        horizontalGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Horizontal", nullptr));
        leftRadioButton->setText(QCoreApplication::translate("MainWindow", "Left", nullptr));
        rightRadioButton->setText(QCoreApplication::translate("MainWindow", "Right", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Select direction:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Another way to select direction:", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
