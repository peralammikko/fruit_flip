#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_fileLineEdit_editingFinished();

    void on_findPushButton_clicked();

    void on_keyLineEdit_editingFinished();

private:
    Ui::MainWindow *ui;
    QString fileName_ = "";
    QString findText_ = "";

    void printText(QString &output);
};
#endif // MAINWINDOW_HH
