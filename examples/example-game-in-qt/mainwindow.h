#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

class Game;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
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
private:
	/*! An inner UI
	 */
	Ui::MainWindow ui;
	/*! A game, that should be displayed in widget
	 */
	Game* m_game;
};

#endif // MAINWINDOW_H
