#include "mainwindow.h"

#include "qt/sadqtrenderer.h"

#include "../game/game.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)  // NOLINT(cppcoreguidelines-pro-type-member-init)
    : QMainWindow(parent), m_game(nullptr)
{
    ui.setupUi(this);

    this->setFixedSize(this->geometry().width(), this->geometry().height());

    connect(ui.btnStartGame, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(ui.btnQuitGame, SIGNAL(clicked()), this, SLOT(quitGame()));

    connect(ui.btnIncrementHealth, SIGNAL(clicked()), this, SLOT(incrementPlayersHealth()));
    connect(ui.btnIncrementScore, SIGNAL(clicked()), this, SLOT(incrementScore()));
    connect(ui.btnKillAllEnemies, SIGNAL(clicked()), this, SLOT(killEnemies()));
}

MainWindow::~MainWindow() = default;


void MainWindow::startGame()
{
    m_call_protector.lock();
    m_game = new Game(ui.glWidget->renderer());
    if (!m_game->trySetup())
    {
        QMessageBox::critical(nullptr, "Unable to start game", "Failed to launch the game");
        m_call_protector.unlock();
        quitGame();
    }
    else
    {
        m_game->initialize();
        m_game->run();
    }
    m_call_protector.unlock();
}

void MainWindow::quitGame()
{
    m_call_protector.lock();
    ui.glWidget->setRenderer(new sad::qt::Renderer());
    Game* local_game = m_game;
    m_game = nullptr;
    QTimer::singleShot(100, [=]() {
        delete local_game;
    });
    m_call_protector.unlock();
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


void MainWindow::freeGame(Game* game)
{
    if (game)
    {
        delete game;
    }
}
