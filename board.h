#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include <QPushButton>
#include <QHash>
#include <QDebug>
#include <QSignalMapper>
#include <cstdlib>
#include "include/player.h"
#include <unordered_map>

namespace Ui {
class Board;
}

class Board : public QMainWindow
{
    Q_OBJECT

public:    
    explicit Board(int x, int y, QStringList &p, QWidget *parent = 0);
    ~Board();

private slots:
    void on_pb_quit_clicked();
    void on_gb_clicked();
    void on_pb_commit_clicked();

private:
    /** GUI Stuff **/
    Ui::Board *ui;
    QPair<int,int> key;
    QHash<QPair<int,int>, QPushButton*> button2d; //Use Player::Point structure?

    /** Board Stuff **/
    const int X, Y, N, INIT_WITH; //Board Dimensions and Players. Initializes with 2 random positions
    static const QStringList bckgnd_color;
    static const QStringList clnble_color; //Stores clonable site colors for player clonable sites. (CurrentMAX of 4)

    QStringList player_names; // Names of Players.
    std::vector<std::vector<char> > board; // Boads Stores the player owned sites
    std::vector<Player> players;
    Player  *board_owner; //use unique pointer?
    QPair<int,int> bindx_clicked;
    QPair<int,int> bindx_jump_parent;
    QString b_tmp_style;

    bool owner_site_selected = false,clone_sites_enabled = true, jump_sites_enabled = false; //flags

    /** Functions **/
    inline void initPlayers(QStringList &p_n);
    inline void initBoard();
    inline void nextBoardOwner();
    inline void setButtonOwnColor(int b_idx, int b_idy, int color_id);
    inline void setClonableStyle(int b_idx, int b_idy, int color_id);
    inline void setButtonNeutralColor(int b_idx, int b_idy);
    inline void resetStyle(QPair<int,int> key, QString style);
    inline void setButtonChecked(int b_idx, int b_idy, bool val);
    inline void setButtonEnable(int b_idx, int b_idy, bool val);
    inline bool isOwner(QPair<int,int> bindx_clicked);
    inline void toDoJump(QPushButton *b);

    void loadButtons();
    void initSpawn();
    void notifyAllPlayers();
    void enableOwnerSites();
    void disableOwnerSites();
    void enableCloneableSites();
    void disableCloneableSites();
    void enableJumpSites();
    void disableJumpSites();
    void toDoCloneable(QPushButton *b);
    inline void clone_commit_clicked();
    inline void jump_commit_clicked();
    void updateBoardAfterMove();
};

#endif // BOARD_H
