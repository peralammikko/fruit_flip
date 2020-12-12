 #ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <vector>
#include <deque>
#include <random>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>

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

    std::string on_startPoint_editingFinished();

    std::string on_endPoint_editingFinished();

    void on_moveButton_clicked();

    void drop_blocks();

    int on_delayCheck_stateChanged(int arg1);

    void on_resetButton_clicked();

    void on_gridSettingsButton_clicked();

    void on_refillCheck_stateChanged();

private:

    Ui::MainWindow *ui;

    // Scene for the game grid
    QGraphicsScene* scene_;
    QGraphicsRectItem* rect_;

    // Margins for the drawing area (the graphicsView object)
    // You can change the values as you wish
    const int TOP_MARGIN = 150;
    const int LEFT_MARGIN = 100;

    // Size of a square containing a fruit
    const int SQUARE_SIDE = 60;
    // Number of vertical cells (places for fruits)
    const int ROWS = 6; // give your own value here
    // Number of horizontal cells (places for fruits)
    const int COLUMNS = 6; // give your own value here

    int GRID_ROW_SIZE_ = ROWS-1;
    int GRID_COL_SIZE_ = COLUMNS-1;

    // Constants describing scene coordinates
    const int BORDER_UP = 0;
    const int BORDER_DOWN = SQUARE_SIDE * (GRID_ROW_SIZE_+1);
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = SQUARE_SIDE * (GRID_COL_SIZE_+1);

    // Constants for different fruits and the number of them
    // Add/remove fruits as you wish, or you can remove the whole enum type
    // Actually only the value NUMBER_OF_FRUITS is needed in the
    // template code
    enum Fruit_kind {PLUM,
                     STRAWBERRY,
                     APPLE,
                     LEMON,
                     BLUEBERRY,
                     ORANGE,
                     EMPTY,
                     NUMBER_OF_FRUITS};

    // For randomly selecting fruits for the grid
    std::default_random_engine randomEng_;
    std::uniform_int_distribution<int> distr_;

    std::vector<std::vector<Fruit_kind>> numbergrid_;
    std::vector<std::vector<QGraphicsRectItem*>> grid_;

    // Writes the titles for the grid rows and columns
    void init_titles();

    // Draws a single fruit near the right bottom corner of the grid
    // At the moment, this function is not called in the template,
    // but try to do so, if you want to use real fruits instead of rectangles.
    void draw_fruit();

    void init_grids();

    void swap_blocks();

    bool check_matches();

    void update_screen();

    void refill_blocks();

    QBrush paint_block(int rndm);

    QPalette palette_;

    QTimer* timer_;

    QMessageBox msg_;

    void checkSettings();

    void init_score();

    void init_timer();

    void lock_buttons(bool lockup);

    int delay_ = 1000;

    int rand_;

    bool refill_ = false;

    int points_ = 0;

    void update_timer();

};
#endif // MAINWINDOW_HH
