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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *labelBlue;
    QLabel *labelRed;
    QSlider *horizontalSliderBlue;
    QLabel *labelGreen;
    QSlider *horizontalSliderRed;
    QSlider *horizontalSliderGreen;
    QSpinBox *spinBoxRed;
    QSpinBox *spinBoxGreen;
    QSpinBox *spinBoxBlue;
    QLabel *colorLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(50, 80, 231, 101));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        labelBlue = new QLabel(gridLayoutWidget);
        labelBlue->setObjectName(QString::fromUtf8("labelBlue"));

        gridLayout->addWidget(labelBlue, 3, 0, 1, 1);

        labelRed = new QLabel(gridLayoutWidget);
        labelRed->setObjectName(QString::fromUtf8("labelRed"));

        gridLayout->addWidget(labelRed, 1, 0, 1, 1);

        horizontalSliderBlue = new QSlider(gridLayoutWidget);
        horizontalSliderBlue->setObjectName(QString::fromUtf8("horizontalSliderBlue"));
        horizontalSliderBlue->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSliderBlue, 3, 1, 1, 1);

        labelGreen = new QLabel(gridLayoutWidget);
        labelGreen->setObjectName(QString::fromUtf8("labelGreen"));

        gridLayout->addWidget(labelGreen, 2, 0, 1, 1);

        horizontalSliderRed = new QSlider(gridLayoutWidget);
        horizontalSliderRed->setObjectName(QString::fromUtf8("horizontalSliderRed"));
        horizontalSliderRed->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSliderRed, 1, 1, 1, 1);

        horizontalSliderGreen = new QSlider(gridLayoutWidget);
        horizontalSliderGreen->setObjectName(QString::fromUtf8("horizontalSliderGreen"));
        horizontalSliderGreen->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSliderGreen, 2, 1, 1, 1);

        spinBoxRed = new QSpinBox(gridLayoutWidget);
        spinBoxRed->setObjectName(QString::fromUtf8("spinBoxRed"));

        gridLayout->addWidget(spinBoxRed, 1, 2, 1, 1);

        spinBoxGreen = new QSpinBox(gridLayoutWidget);
        spinBoxGreen->setObjectName(QString::fromUtf8("spinBoxGreen"));

        gridLayout->addWidget(spinBoxGreen, 2, 2, 1, 1);

        spinBoxBlue = new QSpinBox(gridLayoutWidget);
        spinBoxBlue->setObjectName(QString::fromUtf8("spinBoxBlue"));

        gridLayout->addWidget(spinBoxBlue, 3, 2, 1, 1);

        colorLabel = new QLabel(centralWidget);
        colorLabel->setObjectName(QString::fromUtf8("colorLabel"));
        colorLabel->setGeometry(QRect(300, 100, 62, 51));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(spinBoxRed, SIGNAL(valueChanged(int)), horizontalSliderRed, SLOT(setValue(int)));
        QObject::connect(spinBoxGreen, SIGNAL(valueChanged(int)), horizontalSliderGreen, SLOT(setValue(int)));
        QObject::connect(spinBoxBlue, SIGNAL(valueChanged(int)), horizontalSliderBlue, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        labelBlue->setText(QCoreApplication::translate("MainWindow", "blue:", nullptr));
        labelRed->setText(QCoreApplication::translate("MainWindow", "red:", nullptr));
        labelGreen->setText(QCoreApplication::translate("MainWindow", "green:", nullptr));
        colorLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
