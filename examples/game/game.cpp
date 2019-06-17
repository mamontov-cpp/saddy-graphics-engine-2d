#include "game.h"

#include "bonus.h"
#include "enemy.h"
#include "shootingenemy.h"
#include "supershootingenemy.h"
#include "startscreenrain.h"
#include "enemyspawn.h"
#include "player.h"
#include "playerbullet.h"
#include "enemybullet.h"

#include <renderer.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <orthographiccamera.h>
#include <sprite2d.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <geometry2d.h>
#include <formattedlabel.h>
#include <pipeline/pipeline.h>
#include <input/controls.h>
#include <keymouseconditions.h>
#include <objectdependentfpsinterpolation.h>
#include <mousecursor.h>
#include <shader.h>
#include <shaderednode.h>
#include <glcontext.h>

const sad::String GameState::START = "start";
const sad::String GameState::PLAYING = "playing";


Game::Game(sad::Renderer* renderer) : m_renderer(renderer)
{
    m_highscore = 0;
    m_ispaused = false;

    // Create a new machine
    m_machine = new sad::hfsm::Machine();
    sad::hfsm::Machine * m = m_machine;

    sad::input::Controls * controls = m_renderer->controls();
    controls->add(*sad::input::ET_KeyPress & sad::F, this, &Game::tryToggleFullscreen);  
    controls->add(*sad::input::ET_KeyPress & sad::Esc, this, &sad::p2d::app::App::quit);

    // Create a new idle state
    sad::hfsm::State * idleState = new sad::hfsm::State();

    controls->add(
        (*sad::input::ET_KeyPress) & sad::Enter & (m * GameState::START), 
        this, &Game::startPlaying
    );

    idleState->addEnterHandler(this, &Game::enterStartingScreen);
    idleState->addLeaveHandler(this, &Game::leaveStartingScreen);

    // Create a new playing state
    sad::hfsm::State * playState = new sad::hfsm::State();
    controls->add(
        *sad::input::ET_KeyPress & sad::P & (m * GameState::PLAYING), 
        this,  &Game::togglePaused
    );

    // Controls, that respond for player shooting
    controls->add(
        *sad::input::ET_KeyPress & sad::Enter & (m * GameState::PLAYING), 
        this, &Game::player, &Player::startShooting
    );
    controls->add(
        *sad::input::ET_KeyPress & sad::Space & (m * GameState::PLAYING), 
        this, &Game::player, &Player::startShooting
    );
    controls->add(
        *sad::input::ET_MousePress & (m * GameState::PLAYING), 
        this, &Game::player, &Player::startShooting
    );
    controls->add(
        *sad::input::ET_KeyRelease & sad::Enter & (m * GameState::PLAYING), 
        this, &Game::player, &Player::stopShooting
    );
    controls->add(
        *sad::input::ET_KeyRelease & sad::Space & (m * GameState::PLAYING), 
        this, &Game::player, &Player::stopShooting
    );
    controls->add(
        *sad::input::ET_MouseRelease & (m * GameState::PLAYING), 
        this, &Game::player, &Player::stopShooting
    );

    // Controls, that respond for player movement
    controls->add(
        *sad::input::ET_KeyPress & sad::KeyLeft & (m * GameState::PLAYING), 
        this, &Game::player, &Player::tryStartMovingLeft
    );
    controls->add(
        *sad::input::ET_KeyPress & sad::KeyRight & (m * GameState::PLAYING), 
        this, &Game::player, &Player::tryStartMovingRight
    );
    controls->add(
        *sad::input::ET_KeyPress & sad::KeyUp & (m * GameState::PLAYING), 
        this, &Game::player, &Player::tryStartMovingUp
    );
    controls->add(
        *sad::input::ET_KeyPress & sad::KeyDown & (m * GameState::PLAYING), 
        this, &Game::player, &Player::tryStartMovingDown
    );
    controls->add(
        *sad::input::ET_KeyRelease & sad::KeyLeft & (m * GameState::PLAYING), 
        this, &Game::player, &Player::tryStopMovingHorizontally
    );
    controls->add(
        *sad::input::ET_KeyRelease & sad::KeyRight & (m * GameState::PLAYING), 
        this, &Game::player, &Player::tryStopMovingHorizontally
    );
    controls->add(
        *sad::input::ET_KeyRelease & sad::KeyUp & (m * GameState::PLAYING), 
        this, &Game::player, &Player::tryStopMovingVertically
    );
    controls->add(
        *sad::input::ET_KeyRelease & sad::KeyDown & (m * GameState::PLAYING), 
        this, &Game::player, &Player::tryStopMovingVertically
    );

    // Mouse movement
    controls->add(
        *sad::input::ET_MouseMove & (m * GameState::PLAYING), 
        this, &Game::player, &Player::tryLookAt
    );

    playState->addEnterHandler(this, &Game::enterPlayingScreen);
    playState->addLeaveHandler(this, &Game::leavePlayingScreen);


    SL_LOCAL_MESSAGE("Input handlers bound succesfully", *m_renderer);

    // Add states - starting screen and playing screen
    m_machine->addState(GameState::START, idleState);
    m_machine->addState(GameState::PLAYING, playState);
    SL_LOCAL_MESSAGE("States bound successfully", *m_renderer);

    m_player = NULL;
    
    this->App::initApp(0, m_renderer);
    m_spawntask =  new sad::PeriodicalEventPollProcess(NULL);
    m_renderer->pipeline()->append(m_spawntask);

    m_walls = NULL;
    m_registered_supershooting_enemies_count = 0;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Game::startPlaying()
{
    m_machine->enterState(GameState::PLAYING);
}

sad::Renderer* Game::renderer() const
{
    return m_renderer;
}

Game::~Game()
{
    delete m_walls;
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
    if (isPaused())
    {
        // Disable stepping the world
        this->m_steptask->disable();
    }
    else
    {
        // Enable stepping the world
        this->m_steptask->enable();
    }
}

int Game::highscore() const
{
    return m_highscore;
}

bool Game::isInPlayingState()
{
	return m_machine->isInState(GameState::PLAYING);
}

void Game::trySetHighscore(int score)
{
    if (score > m_highscore)
    {
        m_highscore = score;
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Game::tryToggleFullscreen()
{
    if (isPlaying())
        m_renderer->toggleFullscreen();
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool Game::trySetup()
{
    // Init logs with target streams to file  and console
    sad::log::FileTarget * filetarget = new sad::log::FileTarget();
    filetarget->open("log.txt");
    
    // A format for console is described as {date}: [file and line] [subsystem] message
    sad::log::ConsoleTarget * consoletarget = new sad::log::ConsoleTarget(
        "{0}: [{1}] {3}{2}{4}", 0, true
    );
    m_renderer->log()->addTarget(filetarget).addTarget(consoletarget);
    
    // Output executable path
    SL_LOCAL_DEBUG   ("Executable path is ", *m_renderer);
    SL_LOCAL_DEBUG   (sad::Renderer::ref()->executablePath(), *m_renderer);

    // Test console log images
    SL_LOCAL_FATAL   ("This is sad::Game", *m_renderer);
    SL_LOCAL_CRITICAL("It\'s a game about a very sad", *m_renderer);
    SL_LOCAL_WARNING ("smiley face", *m_renderer);
    SL_LOCAL_MESSAGE ("which wants to survive", *m_renderer);
    SL_LOCAL_DEBUG   ("against other smiley faces", *m_renderer);
    SL_LOCAL_USER    ("it\'s sure will be hard for him...", *m_renderer, "END");

    // Inits a renderer as non-fullscreen 640x480 window

    sad::ObjectDependentFPSInterpolation * fps = new sad::ObjectDependentFPSInterpolation();
    fps->setRenderer(m_renderer);
    m_renderer->setFPSInterpolation(fps);

    m_renderer->init(sad::Settings(640,480,false));

    SL_LOCAL_MESSAGE("Renderer successfully initialized!", *m_renderer);	
    // Inits generator for spawns and random raings
    srand(static_cast<unsigned int>(time(NULL)));

    //Loading resources
    bool result = true; 
    sad::Vector<sad::resource::Error *> errors = m_renderer->loadResources("examples/game/resources.json");
    sad::String errortext;
    if (errors.size() != 0)
    {
        result = false;
        SL_LOCAL_FATAL(sad::resource::format(errors), *m_renderer);
    } 
    sad::util::free(errors);

    if (!result)
    {
        return false;
    }

    SL_LOCAL_MESSAGE("Resources successfully loaded", *m_renderer);

    // Set cursor
    sad::Sprite2D* a = new sad::Sprite2D(
        m_renderer->texture("objects"),
        sad::Rect2D(sad::Point2D(441,32),sad::Point2D(457,48)),
        sad::Rect2D(sad::Point2D(-8, -8), sad::Point2D(8, 8))
    );
    m_renderer->cursor()->setImage(a);

    return result;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Game::initialize()
{
    m_renderer->setWindowTitle("sad::Game");
    
    // Set window size to be fixed
    m_renderer->makeFixedSize();

    m_machine->enterState(GameState::START);
    // Run an engine, starting a main loop
    SL_LOCAL_MESSAGE("Will start now", *m_renderer);    
}

void Game::run()
{
    m_renderer->run();
}

// ReSharper disable once CppMemberFunctionMayBeConst
Player* Game::player()
{
    return m_player;
}

void Game::increasePlayerScore(int delta)
{
    this->trySetHighscore(this->player()->increaseScore(delta));
}

void Game::increasePlayerHealth(int by)
{
    if (m_machine->isInState(GameState::PLAYING)) 
    {
        this->player()->incrementHP(by);
    }
}

void Game::decreasePlayerHealth(int by)
{
    if (m_machine->isInState(GameState::PLAYING)) 
    {
        this->player()->decrementHP(by);
    }
}

void Game::killEnemies()
{
    if (m_machine->isInState(GameState::PLAYING)) 
    {
        const sad::Vector<sad::SceneNode*>& objects = this->scene()->objects();
        
        // Filter enemies
        sad::Vector<sad::SceneNode*> enemies;
        sad::Hash<sad::String, bool>  sets;
        sets.insert("Enemy", true);
        sets.insert("EnemyBullet", true);
        sets.insert("ShootingEnemy", true);
        sets.insert("SuperShootingEnemy", true);
        for(size_t i = 0; i < objects.size(); i++)
        {
            if (sets.contains(objects[i]->className()))
            {
                enemies << objects[i];
            }
        }

        // Remove all of theme at once
        for(size_t i = 0; i < enemies.size(); i++)
        {
            this->removeObject(static_cast<sad::p2d::app::Object*>(enemies[i]));
        }
    }
}

void Game::leaveStartingScreen()
{
    this->scene()->clear();
    
    if (m_world)
    {
        m_world->delRef();
    }
    createWorld();
    m_steptask->setWorld(m_world);
    m_registered_supershooting_enemies_count = 0;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void Game::leavePlayingScreen()
{
    // We don't leave screen here, because it can be called from p2d::World::step()
    // so we can break execution chain, if we kill world here
}


void Game::enterStartingScreen()
{
    // See Game::moveToPlayingScreen. We should perform change from here,
    // otherwise, execution chain will break
    SL_LOCAL_SCOPE("Game::enterStartingScreen()", *m_renderer);
    m_renderer->pipeline()->appendTask(this, &Game::moveToStartingScreen);
} 


void Game::enterPlayingScreen()
{
    m_ispaused = false;
    sad::Scene * sc = this->scene();

    sad::Texture * tex = m_renderer->texture("background");
    sad::Sprite2D * background = new sad::Sprite2D(
        tex, 
        sad::Rect2D(0, 0, 512, 512),
        sad::Rect2D(0, 0, 640, 480)
    );

    if (m_renderer->context()->isOpenGL3compatible())
    {
        sad::Shader* shader = new sad::Shader();
        shader->setRenderer(m_renderer);
        shader->loadVertexProgramFromFile("vertex.gl");
        shader->loadFragmentProgramFromFile("fragment.gl");
        sad::ShaderedNode* local_node = new sad::ShaderedNode();
        local_node->setNode(background);
        local_node->setShader(shader);
        sc->add(local_node);
    }
    else
    {
        sc->add(background);
    }
    sad::FormattedLabel * label = new sad::FormattedLabel();
    label->setFont("font");
    label->setTreeName(m_renderer, "");
    label->setPoint(0, 480);
    label->setUpdateInterval(200);
    label->setSize(23);

    label->setFormatString("Health: {0} Score: {1} Highscore: {2}\nFPS: {3}");
    label->arg(this, &Game::player, &GameObject::hitPoints);
    label->arg(this, &Game::player, &Player::score);
    label->arg(this, &Game::highscore);
    label->argFPS(m_renderer);

    sc->add(label);
    m_spawntask->setEvent(new EnemySpawn(this) );

    // Handlers also register types in world, so they MUST BE added before
    // any object ia added to scene

    m_world->addHandler("sad::p2d::Wall", "GameObject", this, &Game::onWallCollision);
    m_world->addHandler(this, &Game::onBonusCollision);
    m_world->addHandler(this, &Game::onPlayerBulletEnemy);
    m_world->addHandler(this, &Game::onPlayerBulletSEnemy);
    m_world->addHandler(this, &Game::onPlayerBulletSuperEnemy);
    m_world->addHandler(this, &Game::onPlayerEnemyBullet);
    m_world->addHandler(this, &Game::onPlayerEnemy);
    m_world->addHandler(this, &Game::onPlayerShootingEnemy);
    m_world->addHandler(this, &Game::onPlayerSuperShootingEnemy);

    // We add background, emitter and new player's alter-ego at 320,240 - center of screen
    Player* p  = new  Player();
    p->setPosition(sad::p2d::Point(320.0,240.0));
    addObject(p, "GameObject");

    m_player = p;

    createWalls();
}

void Game::removeObject(sad::p2d::app::Object *o)
{
    // If player is dead, no reason to continue playing, 
    // return to start screen
    if (o->metaData()->name() == "Player")
    {
        m_machine->enterState(GameState::START);
    }
    if (o->metaData()->name() == "SuperShootingEnemy")
    {
        --m_registered_supershooting_enemies_count;
    }
    this->sad::p2d::app::App::removeObject(o);
}


// ReSharper disable once CppMemberFunctionMayBeConst
const sad::String & Game::state()
{
    return m_machine->currentState();
}

GameObject *  Game::produce(Objects type)
{
    GameObject * result = NULL;
    switch(type)
    {
        case O_BONUS: result = new Bonus(); break;
        case O_ENEMY: result = new Enemy(); break;
        case O_SHOOTINGENEMY: result = new ShootingEnemy(); break;
        case O_SUPERSHOOTINGENEMY: 
        {
            if (m_registered_supershooting_enemies_count < 3)
            {
                result = new SuperShootingEnemy(); 
                ++m_registered_supershooting_enemies_count;
            }
            break;
        }
    }
    if (result)
    {
        // At the moment we need a common game object group to scan walls for collisions only with enemy object
        this->addObject(result, "GameObject");
    }
    return result;
}




void Game::createWalls()
{
    if (m_walls)
    {
        m_walls->delRef();
    }
    m_walls = new sad::p2d::Walls(14);
    m_walls->addRef();
    const sad::Vector<sad::p2d::Body *> & bodies = m_walls->bodies();
    for(size_t i = 0; i < bodies.count(); i++)
    {
        m_world->addBody(bodies[i]);
    }
}


void Game::onWallCollision(const sad::p2d::CollisionEvent<sad::p2d::Wall, GameObject> & ev)
{
    if (ev.object2().metaData()->name() == "Player")
    {
        ev.object1().tryTeleport(ev.object2().body());
    }
    else
    {
        removeObject(&(ev.object2()));
    }
}


// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void Game::onBonusCollision(const sad::p2d::CollisionEvent<Player, Bonus> & ev)
{
    ev.object2().decrementHP(1);
    ev.object1().incrementHP(1);
    ev.object1().increaseScore(50);
}

void Game::onPlayerBulletEnemy(const sad::p2d::CollisionEvent<PlayerBullet, Enemy> & ev)
{
    ev.object2().decrementHP(1);
    ev.object1().decrementHP(1);
    this->player()->increaseScore(100);
}

void Game::onPlayerBulletSEnemy(const sad::p2d::CollisionEvent<PlayerBullet, ShootingEnemy> & ev)
{
    ev.object2().decrementHP(1);
    ev.object1().decrementHP(1);
    this->player()->increaseScore(200);
}

void Game::onPlayerBulletSuperEnemy(const sad::p2d::CollisionEvent<PlayerBullet, SuperShootingEnemy> & ev)
{
    ev.object2().decrementHP(1);
    ev.object1().decrementHP(1);
    this->player()->increaseScore(400);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void Game::onPlayerEnemyBullet(const sad::p2d::CollisionEvent<Player, EnemyBullet> & ev)
{
    ev.object2().decrementHP(ev.object2().hitPoints());
    ev.object1().decrementHP(1);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void Game::onPlayerEnemy(const sad::p2d::CollisionEvent<Player, Enemy> & ev)
{
    ev.object2().decrementHP(ev.object2().hitPoints());
    ev.object1().decrementHP(1);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void Game::onPlayerShootingEnemy(const sad::p2d::CollisionEvent<Player, ShootingEnemy> & ev)
{
    ev.object2().decrementHP(ev.object2().hitPoints());
    ev.object1().decrementHP(1);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void Game::onPlayerSuperShootingEnemy(const sad::p2d::CollisionEvent<Player, SuperShootingEnemy> & ev)
{
    ev.object2().decrementHP(ev.object2().hitPoints());
    ev.object1().decrementHP(1);
}


void Game::moveToStartingScreen()
{
    SL_LOCAL_SCOPE("Game::moveToStartingScreen()", *m_renderer);
    m_renderer->setScene(new sad::Scene());
    if (m_world)
    {
        m_world->delRef();
    }
    createWorld();
    m_steptask->setWorld(m_world);
    
    m_player = NULL;
    m_ispaused = false;
    sad::Scene * sc = this->scene();

    // Fill screne with background, label and rain of element (the last object does that).
    sad::Texture * tex = m_renderer->texture("title");
    sad::Sprite2D * background = new sad::Sprite2D(
        tex, 
        sad::Rect2D(0, 0, 640, 480),
        sad::Rect2D(0, 0, 640, 480)
    );
    sc->add(background);
    
    sad::FormattedLabel  * label = new sad::FormattedLabel();
    label->setFont("font");
    label->setTreeName(m_renderer, "");
    label->setSize(23);
    label->setColor(sad::AColor(255, 255, 255));
    label->setPoint(260, 240);
    label->setUpdateInterval(1000);
    // Here we build a format string, where {0} will be replaced with call of this->highscore()
    label->setFormatString("Highscore: {0}");
    label->arg(this, &Game::highscore);
    sc->add(label);
    

    m_spawntask->setEvent(new StartScreenRain(this) );
    // Handlers also register types in world, so they MUST BE added before
    // any object ia added to scene
    m_world->addHandler("sad::p2d::Wall", "GameObject", this, &Game::onWallCollision);
    this->createWalls();
    
    m_registered_supershooting_enemies_count = 0;
}

