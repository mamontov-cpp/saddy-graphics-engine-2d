#include "game.h"
#include <renderer.h>
#include <orthocamera.h>
#include <extra/background.h>
#include <extra/geometry2d.h>

#include "statelabel.h"
#include "bonus.h"
#include "enemy.h"
#include "shootingenemy.h"
#include "supershootingenemy.h"
#include "startscreenrain.h"
#include "enemyspawn.h"
#include "player.h"

const hst::string GameState::START = "start";
const hst::string GameState::PLAYING = "playing";


Game::Game()
{
	m_highscore = 0;
	m_ispaused = false;

	// Create a new machine
	m_machine = new fsm::Machine();
	m_machine->addCallbacks(sad::Renderer::ref()->controls());

	// Create a new idle state
	fsm::State * idleState = new fsm::State();
	idleState->addKeyDownCallback('F',       this, &Game::tryToggleFullscreen);
	idleState->addKeyDownCallback(KEY_ESC,   this, &Game::quit);
	idleState->addKeyDownCallback(KEY_ENTER, this, &Game::startPlaying);
	
	idleState->addEnterCallback(this, &Game::enterStartingScreen);
	idleState->addLeaveCallback(this, &Game::leaveStartingScreen);

	// Create a new playing state
	fsm::State * playState = new fsm::State();
	playState->addKeyDownCallback('F', this, &Game::tryToggleFullscreen);
	playState->addKeyDownCallback('P', this, &Game::togglePaused);
	playState->addKeyDownCallback(KEY_ESC, this, &Game::quit);
	playState->addKeyDownCallback(KEY_ENTER,  this, &Game::player, &Player::startShooting);

	playState->addKeyDownCallback(KEY_LEFT,  this, &Game::player, &Player::tryStartMovingLeft);
	playState->addKeyDownCallback(KEY_RIGHT, this, &Game::player, &Player::tryStartMovingRight);
	playState->addKeyDownCallback(KEY_UP,    this, &Game::player, &Player::tryStartMovingUp);
	playState->addKeyDownCallback(KEY_DOWN,  this, &Game::player, &Player::tryStartMovingDown);
	playState->addKeyUpCallback(KEY_LEFT,  this, &Game::player, &Player::tryStopMovingHorizontally);
	playState->addKeyUpCallback(KEY_RIGHT, this, &Game::player, &Player::tryStopMovingHorizontally);
	playState->addKeyUpCallback(KEY_UP,    this, &Game::player, &Player::tryStopMovingVertically);
	playState->addKeyUpCallback(KEY_DOWN,  this, &Game::player, &Player::tryStopMovingVertically);
	playState->addKeyUpCallback(KEY_ENTER,  this, &Game::player, &Player::stopShooting);


	playState->addEventCallback(fsm::Names::MOUSEMOVE, this, &Game::player, &Player::tryLookAt);
	playState->addEventCallback(fsm::Names::MOUSEDOWN, this, &Game::player, &Player::startShooting);
	playState->addEventCallback(fsm::Names::MOUSEUP, this, &Game::player, &Player::stopShooting);
	
	playState->addEnterCallback(this, &Game::enterPlayingScreen);
	playState->addLeaveCallback(this, &Game::leavePlayingScreen);


	SL_MESSAGE("Input handlers bound succesfully");

	// Add states - starting screen and playing screen
	m_machine->addState(GameState::START, idleState);
	m_machine->addState(GameState::PLAYING, playState);
	SL_MESSAGE("States bound successfully");

}


void Game::startPlaying()
{
	m_machine->pushState(GameState::PLAYING);
}
Game::~Game()
{
	delete m_machine;
}

bool Game::isPaused() const
{
	return m_ispaused;
}

bool Game::isPlaying() const
{
	return !isPaused();
}

void Game::togglePaused()
{
	m_ispaused = !m_ispaused;
}

int Game::highscore() const
{
	return m_highscore;
}

void Game::trySetHighscore(int score)
{
	if (score > m_highscore)
	{
		m_highscore = score;
	}
}

void Game::tryToggleFullscreen()
{
	if (isPlaying())
		sad::Renderer::ref()->toggleFullscreen();
}

void Game::quit()
{
	sad::Renderer::ref()->quit();
}

void Game::run()
{
	// Init new empty scene with orthographic projection
	sad::Scene * sc= new sad::Scene();
	sc->setCamera(new OrthoCamera());
	sad::Renderer::ref()->setCurrentScene(sc);
	sad::Renderer::ref()->setWindowTitle("sad::Game");

	
	// Load textures to videocard memory and build mipmaps
	sad::Renderer::ref()->textures()->buildAll();
	SL_MESSAGE("Mips built successfully");	
	
	// Set window size to be fixed
	sad::Renderer::ref()->toggleFixedOn();

	m_machine->pushState(GameState::START);
	// Run an engine, starting a main loop
	SL_MESSAGE("Will start now");	

	sad::Renderer::ref()->run();
}

Player * Game::player()
{
	return m_player;
}

void Game::increasePlayerScore(int delta)
{
	this->trySetHighscore(this->player()->increaseScore(delta));
}

void Game::increasePlayerHealth(int by)
{
	this->player()->incrementHP(by);
}

void Game::decreasePlayerHealth(int by)
{
	this->player()->decrementHP(by);
}


sad::Scene * Game::scene()
{
	return sad::Renderer::ref()->getCurrentScene();
}

void Game::leaveStartingScreen()
{
	this->scene()->clear();
}

void Game::leavePlayingScreen()
{
	this->scene()->clear();
	m_player = NULL;
}


void Game::enterStartingScreen()
{
	m_ispaused = false;
	sad::Scene * sc = this->scene();
	// Fill screne with background, label and rain of element (the last object does that).
	sc->add(new sad::Background("title"));
	sc->add(new StateLabel(this));
	sad::Input::ref()->addPostRenderTask( 
		new TimePeriodicalTask( 
			new StartScreenRain(this) 
		) 
	);
}


void Game::enterPlayingScreen()
{
	m_ispaused = false;
	sad::Scene * sc = this->scene();
	// We add background, emitter and new player's alter-ego at 320,240 - center of screen
	sc->add(new sad::Background("background"));
	sc->add(new StateLabel(this));
	sad::Input::ref()->addPostRenderTask( 
		new TimePeriodicalTask( 
			new EnemySpawn(this) 
		) 
	);
	Player * p  = new  Player();
	p->setPosition(p2d::Point(320.0,240.0));
	addObject(p);
	m_player = p;	
}

void Game::removeObject(GameObject *o)
{
	m_world->removeBody(o->body());
	sad::Renderer::ref()->getCurrentScene()->remove(o);
	// If player is dead, no reason to continue playing, 
	// return to start screen
	if (o->metaData()->name() == "Player")
	{
		m_machine->pushState(GameState::START);
	}
	
}

const hst::string & Game::state()
{
	return m_machine->currentStateName();
}

GameObject *  Game::produce(Objects type)
{
	GameObject * result = NULL;
	switch(type)
	{
		O_BONUS: result = new Bonus(); break;
        O_ENEMY: result = new Enemy(); break;
        O_SHOOTINGENEMY: result = new ShootingEnemy(); break;
		O_SUPERSHOOTINGENEMY: result = new SuperShootingEnemy(); break;
	}
	if (result)
	{
		this->addObject(result);
	}
	return result;
}

void Game::addObject(GameObject * o)
{
	o->setGame(this);
	sad::Renderer::ref()->getCurrentScene()->add(o);
	m_world->addBody(o->body());
}
