#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QMutex>
#include "ui_mainwindow.h"

class Game;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    /*! Starts game in window
     */
    void startGame();
    /*! Quits game in window
     */
    void quitGame();
    /*! If game is being played, increments players HP 
     */
    void incrementPlayersHealth();
    /*! If game is being played, decrements players HP
    */
    void incrementScore();
    /*! If game is being played, kills all enemies
     */
    void killEnemies();
    /*! Returns game, stored in window
     *  \return game in window
     */
    inline Game* game() const { return m_game; }
    /*! Frees a game, destroying a game
     */
    static void freeGame(Game* game);
private:
    /*! An inner UI
     */
    Ui::MainWindow ui;
    /*! A game, that should be displayed in widget
     */
    Game* m_game;
    /*! A call protection mutex
     */
    QMutex m_call_protector;
};

#endif // MAINWINDOW_H
