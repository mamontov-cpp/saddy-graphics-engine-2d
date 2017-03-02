#include "mainwindow.h"

#include "../game/game.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.btnStartGame, SIGNAL(clicked()), this, SLOT(startGame()));
	connect(ui.btnQuitGame, SIGNAL(clicked()), this, SLOT(quitGame()));
}

MainWindow::~MainWindow()
{
	delete m_game;
}


void MainWindow::startGame()
{
	m_game = new Game(ui.glWidget->renderer());
	if (!m_game->trySetup())
	{
		QMessageBox::critical(NULL, "Unable to start game", "Failed to launch the game");
		quitGame();
	}
	else
	{
		m_game->initialize();
		m_game->run();
	}
}

void MainWindow::quitGame()
{
	ui.glWidget->setRenderer(new sad::qt::Renderer());
}