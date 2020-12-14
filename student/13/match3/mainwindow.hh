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

    // Suorittaa kaikki kahden ruudun siirtoon liittyvät toimenpiteet.
    // Kutsuu funktioita ruutujen siirtoon, matchien etsintään, ruudukon päivitykseen,
    // ja hedelmien tiputteluun.
    void on_moveButton_clicked();

    // Tiputtaa ruudukon hedelmiä alaspäin niin kauan, kunnes hedelmien alapuolelta ei löydy
    // enää tyhjiä ruutuja. Kutsutaan aina viiveellä värisuorien löydyttyä ja poistuttua ruudukosta.
    void drop_blocks();

    // Viiveasetuksen tarkistusfunktio. Parametrin arg1 ollessa tosi (>0), apumuuttuja delay_ saa arvon 1000, jolloin
    //hedelmien pudotteluun käytettävä viive on yhden sekunnin mittainen. Parametrin ollessa epätosi, viive poistetaan
    // ja hedelmät tippuvat välittömästi.
    int on_delayCheck_stateChanged(int arg1);

    // Funktio, jota kutsutaan, kun ruudukon palautusnappia (reset) painetaan. Keskeyttää nykyisen pelin, lukitsee
    // pelaajalta siirtelykontrollit ja alustaa uuden pelin.
    void on_resetButton_clicked();

    // Funktio, jota kutsutaan, kun ruudukon asetusten alustusnappia painetaan. Antaa pelaajan
    // vaihtaa peliruudukon kokoa haluamissaan mitoissa (sarakkeet * rivit).
    // Vaatii peliruudukon alustamisen (ei voi suurentaa kesken pelin ja jatkaa).
    void on_gridSettingsButton_clicked();

    // Funktio, jota kutsutaan, kun pelaaja klikkaa yhtä peliruudukossa olevaa hedelmää (nappia).
    // Tallettaa hiiren sijainnin ikkunassa klikkauksen hetkellä ja tallettaa sen X/Y-koordinaatit
    // muuttujiin startPoint_ ja endPoint_, joita käytetään funktiossa swap_blocks ruutujen liikutteluun ja
    // apufunktiossa display_move_coordinates valittujen ruutujen koordinaattien esittämiseen peli-ikkunassa.
    void fruitButton_clicked();

    // Peliruudukon täytön tarkistusfunktio. Pelaajalla on mahdollisuus muuttaa pelin sääntöjä siten, että ruudukkoon
    // ei synny tyhjiä kohtia vaan ne täytetään uusilla, satunnaisgeneroiduilla hedelmillä. Tarkastaa asetuksen arvon
    // (päällä/pois) ja muuttaa apumuuttujaa refill_ sen mukaan.
    void on_refillCheck_stateChanged();

    // Funktio, jota kutsutaan, kun pelaaja klikkaa liikkumiskontrollien vierellä olevaa "?" nappia.
    // Antaa pelaajalle lyhyet ja yksinkertaiset ohjeet pelin toiminnallisuudesta.
    void on_moveHelpButton_clicked();

    // Funktio, jota kutsutaan, kun pelaaja klikkaa asetusten vierellä olevaa "?" nappia.
    // Antaa pelaajalle lyhyet kuvaukset pelin mahdollisista asetuksista (em. viive ja täyttö).
    void on_settingsHelpButton_clicked();

    // Funktio, jota kutsutaan, kun pelaaja klikkaa ruudukon asetusten vierellä olevaa "?" nappia.
    // Antaa pelaajalle ohjeita ruudun koon muuttamisesta ja mahdollisista rajoitteista.
    void on_gridHelpButton_clicked();

private:

    Ui::MainWindow *ui;

    // Scene for the game grid
    QGraphicsScene* scene_;

    // Margins for the drawing area (the graphicsView object)
    // You can change the values as you wish
    const int TOP_MARGIN = 150;
    const int LEFT_MARGIN = 100;

    // Size of a square containing a fruit
    const int SQUARE_SIDE = 60;
    // Number of vertical cells (places for fruits)
    const int ROWS = 6;
    // Number of horizontal cells (places for fruits)
    const int COLUMNS = 6;

    // For-looppaavissa funktioissa ja ruudukon koon muuttamisessa käytettävät apumuuttujat.
    // Ruudukon oletusarvo koolle on 6x6.
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

    std::vector<std::vector<Fruit_kind>> fruitGrid_;
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

    // Alustaa pistetaulukon
    void init_score();

    // Alustaa pelin ajanoton
    void init_timer();

    // QTimerin apufunktio, jolla pidetään kirjaa kuluneista sekunneista ja minuuteista
    void on_timeout();

    // Päivittää ruudulla olevan kellon minuutit (mins) ja sekunnit (secs).
    void update_timer(int mins, int secs);

    // - Toiminnalliset funktiot -

    // Päivittää peliruudukon värit muutosten (ruutujen siirtelyn, matchailun ja pudotusten) jälkeen
    // Päivittää kaikki ruudukossa olevat hedelmät oikeilla väreillä, jotka saadaan funktiosta
    // paint_button.
    void update_screen();

    // Slot-funktio ruutujen siirtelyä varten. Klikattuaan kahta laattaa pelaaja
    // vahvistaa ruutujen vaihdon klikkaamalla nappia "Move".
    // Tarkistaa siirron laillisuuden ja toteuttaa sen jos se on sallittu.
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

    // Apufunktio, jota käytetään esittämään pelaajalle valittujen ruutujen koordinaatit
    // kahdessa tekstikentässä. Helpottaa näkemään mitkä laatat ovat valittuna.
    // Parametrit: x ja y ovat laatan koordinaatit, jotka välittyvät slotfunktiosta
    // fruitButton_clicked.
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

    // Pelissä kerättävät pisteet. Alussa ja alustettaessa pisteet  alkavat nollasta.
    int points_ = 0;

    // Klikkausvuoroja ylläpitävä apufunktio. Käytetään funktioiden fruitButton_clicked ja display_move_coordinates
    // toiminnassa määrittelemään kumpi vaihdettavista ruuduista on alkupiste ja kumpi loppupiste.
    // Vaihtelee kumpaan tekstikenttään display_move_coordinates -funktio esittä ruudun koordinaatit.
    int turn_ = 1;

    // Apumuuttujat, joihin klikattavien ruutujen koordinaatit tallennetaan (X ja Y). Oletuksena (ja jokaisen vuoron päätteeksi)
    // arvot ovat ruudukon ulkopuolella, jolloin siirtelyfunktio ei onnistu käyttämään niitä.
    std::vector<int> startPoint_ = {-1,-1};
    std::vector<int> endPoint_ = {-1,-1};

    // Kellotaulun, pistetaulun ja painonappien maalaamiseen käytettävä väripaletti. Napeille etsitään
    // sopiva väri paint_button -funktiossa
    QPalette initPalette_;

    // Pelissä käytettävä widget ajanotolle
    QTimer* timer_;

    // Ilmoituksia varten käytettävä ikkunawidget, joka suoritetaan funktiossa send_message.
    QMessageBox msg_;

};
#endif // MAINWINDOW_HH
