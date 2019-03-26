#include "mainwindow.h"

#include "qt/sadqtrenderer.h"

#include "../game/game.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_game(NULL)
{
    ui.setupUi(this);

    sad::qt::Renderer* a = new sad::qt::Renderer();
    sad::qt::Renderer* b = new sad::qt::Renderer();
    delete a;
    delete b;

    this->setFixedSize(this->geometry().width(), this->geometry().height());

    connect(ui.btnStartGame, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(ui.btnQuitGame, SIGNAL(clicked()), this, SLOT(quitGame()));

    connect(ui.btnIncrementHealth, SIGNAL(clicked()), this, SLOT(incrementPlayersHealth()));
    connect(ui.btnIncrementScore, SIGNAL(clicked()), this, SLOT(incrementScore()));
    connect(ui.btnKillAllEnemies, SIGNAL(clicked()), this, SLOT(killEnemies()));
}

MainWindow::~MainWindow()
{
    if (m_game != NULL)
    {
        m_game->quit();
    }
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
    delete m_game;
    m_game = NULL;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainWindow::incrementPlayersHealth()
{
    if (m_game)
    {
        m_game->increasePlayerHealth(1);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainWindow::incrementScore()
{
    if (m_game)
    {
        m_game->increasePlayerScore(100);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainWindow::killEnemies()
{
    if (m_game)
    {
        m_game->killEnemies();
    }
}
