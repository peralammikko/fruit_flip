#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onColorChanged();

    void on_spinBoxRed_valueChanged(int arg1);

    void on_spinBoxGreen_valueChanged(int arg1);

    void on_spinBoxBlue_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    const int RGB_VALUE_MAX = 255;
};

#endif // MAINWINDOW_HH
