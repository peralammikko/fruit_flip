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

    void on_moveHelpButton_clicked();

    void on_settingsHelpButton_clicked();

    void fruitButton_clicked();

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

    int new_border_down = SQUARE_SIDE * (GRID_ROW_SIZE_+1);
    int new_border_right = SQUARE_SIDE * (GRID_COL_SIZE_+1);

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
    std::vector<std::vector<QPushButton*>> grid_;

    //  - Setup-funktiot -

    // Alustaa pelin: luo peliruudukon, tarkistaa asetukset nollaa pisteet, kellon ja tekstikentät
    void init_game();

    // Käy läpi pelissä olevat asetukset pelin alustuksessa
    void checkSettings();

    // Writes the titles for the grid rows and columns
    void init_titles();

    // Luo ruudukon väritettyjä ruuduille (ja buttoneille)
    void init_grids();

    // Maalaa ruudulla olevat painonappulat hedelmän värin mukaisesti. Parametri rndm on
    // satunnaislukugeneraattorista saatu luku, jonka arvo muutetaan vastaamaan hedelmää
    // header-tiedoston enum-rakenteella Fruit_kind.
    QColor paint_button(int rndm);

    // Maalaaa ruudulla olevat ruudut hedelmän värin mukaisesti.
    QBrush paint_block(int rndm);

    // Alustaa pistetaulukon
    void init_score();

    // Alustaa pelin ajanoton
    void init_timer();

    // QTimerin apufunktio, jolla pidetään kirjaa kuluneista sekunneista ja minuuteista
    void on_timeout();

    // Päivittää ruudulla olevan kellon minuutit ja sekunnit
    void update_timer(int mins, int secs);

    // - Toiminnalliset funktiot -

    // Päivittää peliruudukon värit muutosten (ruutujen siirtelyn, matchailun ja pudotusten) jälkeen
    void update_screen();

    // Vaihtaaa kahden ruudun paikkoja keskenään
    void swap_blocks();

    // Käy läpi ruudukon ja tarkistaa onko pysty- tai vaakasuuntaisia värisuoria
    bool check_matches();

    /* Poistaa matchatut ruudut pelistä. Parametrit:
     * Parametrit:
     * (1) col = tarkasteltava sarake
     * (2) row = tarkasteltava rivi
     * (3) counter = värisuoran pituuden "laskuri"
     * (4) verticalmatch = onko kyseessä pystysuuntainen match (jos ei, match on vaakasuunnassa)
    */
    void remove_matched_blocks(int col, int row, int counter, bool verticalmatch);

    // Generoi peliruudukon ylälaitaan uusia hedelmiä tyhjien kenttien kohdalle.
    void refill_blocks();

    // - Apufunktiot ja muuttujat -

    // Lukitsee/avaa pelissä käytettävät napit pelaajalta. Parametri lockup voidaan päättää
    // olevan true(lukitse)/false(avaa) kutsuvan funktion mukaisesti.
    void lock_buttons(bool lockup);

    void display_move_coordinates(int x, int y);

    // Luo pelaajalle ilmoituksen (viesti-ikkunan) pelin lopussa. Parametri message on ilmoituksen "aihe"
    // eli ikkunan ilmoitettava aihe (Game over, voitit pelin, ...)
    void send_message(QString message);

    // Tarkistaa onko pelaaja voittanut pelin eli kaikki ruudukon kohdat ovat tyhjiä.
    bool check_win();

    // Pelissä käytettävä aikaviive, jota käytetään ennen ruutujen pudottelua pelin seuraamisen selkeyttämiseksi
    int delay_ = 1000;

    // Totuusarvo halutaanko pelin täydentävän ruudukon ylälaidan tyhjiä ruutuja. Jos arvo on false, funktiota
    // refill_blocks ei kutsuta.
    bool refill_ = false;

    // Pelissä kerättävät pisteet
    int points_ = 0;

    int turn_ = 1;

    std::vector<int> startPoint_ = {-1,-1};
    std::vector<int> endPoint_ = {-1,-1};

    // Kellotaulun, pistetaulun ja painonappien maalaamiseen käytettävä väripaletti. Napeille etsitään
    // sopiva väri paint_button -funktiossa
    QPalette palette_;

    // Pelissä käytettävä widget ajanotolle
    QTimer* timer_;

    // Ilmoituksia varten käytettävä ikkunawidget, joka suoritetaan funktiossa send_message.
    QMessageBox msg_;

};
#endif // MAINWINDOW_HH
