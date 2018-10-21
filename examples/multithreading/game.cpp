#include "game.h"

#include "threads/gamethread.h"

#include "nodes/background.h"
#include "nodes/inventorynode.h"
#include "nodes/inventorypopup.h"

#include "bots/jsbot.h"
#include "bots/randombot.h"
#include "bots/divingfloaterbot.h"
#include "bots/followplayerfloater.h"
#include "bots/directionbot.h"
#include "bots/platformpatrolbot.h"

#include <input/controls.h>

#include <pipeline/pipeline.h>
#include <pipeline/pipelinedelayedtask.h>

#include <keymouseconditions.h>

#include <sprite2d.h>
#include <label.h>

#include <db/dbdatabase.h>
#include <db/dbpopulatescenesfromdatabase.h>

#include <animations/animationsinstance.h>
#include <animations/animationssimplemovement.h>
#include <animations/animationsblinking.h>
#include <animations/animationsparallel.h>
#include <animations/animationsrotate.h>
#include <animations/animationsgroup.h>

#include <slurpjson.h>
#include <spitjson.h>

#include <functional>

#include <dukpp-03-irrklang/dukpp-03-irrklang.h>

#include <p2d/force.h>
#include <p2d/infiniteline.h>

#include "weapons/laser.h"


// A precision error for designer's editor when designing level
#define DESIGNER_PRECISION_ERROR  (2.0)

const sad::Point2D Game::GravityForceValue(0.0, -300.0); // -300 is arbitrarily defined, to make player fall slowly

const int Game::BasicEnemyLivesCount = 3; //< Amount of lives for enemy

const int Game::BasicPlayerLivesCount = 1; // Amount of lives for player

DECLARE_COMMON_TYPE(Game);

// ==================================== PUBLIC METHODS ====================================

Game::Game()  : m_is_quitting(false),  // NOLINT(cppcoreguidelines-pro-type-member-init)
m_main_menu_state(Game::GMMS_PLAY),
m_highscore(0),
m_loaded_options_database{false, false},
m_loaded_lose_screen_database{false, false},
m_loaded_win_screen_database{false, false},
m_loaded_game_screen(false),
m_theme_playing(NULL),
m_inventory_node(NULL),
m_inventory_popup(NULL),
m_eval_context(NULL),
m_physics_world(NULL),
m_is_rendering_world_bodies(false),
m_max_level_x(0.0),
m_hit_animation_for_enemies(NULL),
m_hit_animation_for_players(NULL)// NOLINT
{
    m_main_thread = new threads::GameThread();
    m_inventory_thread = new threads::GameThread();

    this->initContext();

    m_player = new game::Player();
    m_player->setGame(this);
    m_player->setLives(Game::BasicPlayerLivesCount);

    m_main_menu_states_to_labels.insert(Game::GMMS_PLAY   , "Play");
    m_main_menu_states_to_labels.insert(Game::GMMS_OPTIONS, "Options");
    m_main_menu_states_to_labels.insert(Game::GMMS_EXIT   , "Exit");


    m_transition_process = new SceneTransitionProcess(this);

    m_loaded_options_database[0] = false;
    m_loaded_options_database[1] = false;

    m_options_screen.init(this, m_main_thread->renderer(), m_inventory_thread->renderer());
    
    m_step_task = new sad::p2d::WorldStepTask(NULL, m_main_thread->renderer());
    
    m_bounce_solver = new sad::p2d::BounceSolver();
    m_bounce_solver->toggleIgnoreContactPoints(true);
    m_bounce_solver->toggleInelasticCollisions(true);
    m_bounce_solver->setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
    m_bounce_solver->enableDebug();

    m_bot_registry.insert("random_60_500", new bots::RandomBot(50, 600));

    m_player->setActorOptions(m_actor_options["player"]);

    m_hit_animation_for_enemies = new sad::animations::Blinking();
    m_hit_animation_for_enemies->addRef();
    m_hit_animation_for_enemies->setTime(500);
    m_hit_animation_for_enemies->setLooped(false);
    m_hit_animation_for_enemies->setFrequency(5);

    m_hit_animation_for_players = new sad::animations::Blinking();
    m_hit_animation_for_players->addRef();
    m_hit_animation_for_players->setTime(2000);
    m_hit_animation_for_players->setLooped(false);
    m_hit_animation_for_players->setFrequency(20);
    m_player->setHurtAnimation(m_hit_animation_for_players);
}

Game::~Game()  // NOLINT
{
    m_hit_animation_for_players->delRef();
    m_hit_animation_for_enemies->delRef();

    delete m_bounce_solver;

    delete m_transition_process;
    delete m_main_thread;
    delete m_inventory_thread;

    delete m_player;
    delete m_eval_context;
    for (sad::Hash<sad::String, sad::String*>::iterator it = m_item_names_to_scripts.begin(); it != m_item_names_to_scripts.end(); ++it)
    {
        delete it.value();
    }

    for (sad::Hash<sad::String, game::ActorOptions*>::iterator ao = m_actor_options.begin(); ao != m_actor_options.end(); ++ao)
    {
        ao.value()->delRef();
    }
    this->clearProjectiles(false);
    this->destroyWorld();
    delete m_step_task;
}

/*! A padding, that will be used in main menu between label and player choice
 */
const double padding_between_label_and_player_choice = 15;

void Game::runMainGameThread()
{
    sad::Renderer& renderer= *(m_main_thread->renderer());
    m_options.load(&renderer);
    m_conditions.apply(m_options);

    sad::Maybe<picojson::value> maybe_json = sad::slurpJson("highscore.json", &renderer);
    if (maybe_json.exists())
    {
        picojson::value val = maybe_json.value();
        if (val.is<double>())
        {
            setHighscore(val.get<double>());
        }
    }
    // We wait for inventory thread later to get initialization result,
    // so initialize flag now
    m_inventory_thread->needsToBeNotifiedFromLater();
    SL_LOCAL_DEBUG("Initializing main thread\n", renderer);
    // Attempt to load resouces
    m_main_thread->tryInitialize(
        "main_thread.txt",
        "examples/multithreading/config_main_thread.json",
        "Platformer (MultiWindow)"
    );
    SL_LOCAL_DEBUG("Waiting for inventory thread\n", renderer);
    // Wait for inventory to load it's resources
    m_inventory_thread->waitForNotify();
    SL_LOCAL_DEBUG("Checking status\n", renderer);
    if (m_main_thread->hasErrors() || m_inventory_thread->hasErrors())
    {
        // Format errors
        sad::String error;
        if (m_main_thread->hasErrors())
        {
            error += "Main thread errors: ";
            error += m_main_thread->errors();
            error +=  "\n";
        }
        if (m_inventory_thread->hasErrors())
        {
            error += "Inventory thread errors: ";
            error += m_inventory_thread->errors();
            error +=  "\n";
        }
        // Output errors
        m_main_thread->renderer()->error(error, "Initilaization error");

        // Kill inventory thread
        m_inventory_thread->sendKillSignalFrom(m_main_thread);
        // Exit here
        return;
    }
    else
    {
        m_inventory_thread->sendResumeSignalFrom(m_main_thread);
    }

    sad::db::Database* db = new sad::db::Database();
    db->setRenderer(&renderer);
    bool result = db->loadFromFile("examples/multithreading/titlescreen.json", &renderer);
    if (result)
    {
        renderer.addDatabase("titlescreen", db);
    }
    else
    {
        SL_LOCAL_FATAL("Unable to load title screen", renderer);
    }

    sad::db::populateScenesFromDatabase(&renderer, db);

    sad::Scene* scene = renderer.scenes()[0];
    // Use introspection to dump or debug some objects
    SL_LOCAL_DEBUG(fmt::Format("Scene has {0} objects") << scene->objectCount(), renderer);
    const sad::Vector<sad::SceneNode*>& objects = scene->objects();
    for(size_t i = 0; i < scene->objectCount(); i++)
    {
        sad::SceneNode* node = objects[i];
        SL_LOCAL_DEBUG(fmt::Format("Object {0}: {1}") << i << node->metaData()->name(), renderer);
    }

    tryStartStartingState();

    setControlsForMainThread(&renderer, db);
    setControlsForInventoryThread(m_inventory_thread->renderer());

    renderer.controls()->addLambda(
        *sad::input::ET_KeyPress & sad::Esc,
        [this]() -> void {
            this->quitGame();
        }
    );

    SL_LOCAL_DEBUG("Starting\n", renderer);
    renderer.controls()->addLambda(
        *sad::input::ET_Quit,
        [this]() -> void {
            if (!m_is_quitting) {
                m_is_quitting = true;
                m_inventory_thread->sendKillSignalFrom(m_main_thread);
                m_transition_process->unloadTexturesForMainThread();
                sad::spitJson("highscore.json", picojson::value(static_cast<double>(m_highscore)), m_main_thread->renderer());
                m_options.save(m_main_thread->renderer());
                sad::irrklang::Engine::eref()->stopAllSounds();
            }
        }
    );
    m_main_thread->markAsRendererStarted();
    renderer.run();
}

void Game::runInventoryThread()
{
    sad::Renderer& renderer= *(m_inventory_thread->renderer());

    // We wait for main thread later to get initialization result,
    // so initialize flag now
    m_main_thread->needsToBeNotifiedFromLater();
    // Attempt to load resouces
    SL_LOCAL_DEBUG("Initializing inventory thread\n", renderer);
    m_inventory_thread->tryInitialize(
        "inventory_thread.txt",
        "examples/multithreading/config_inventory_thread.json",
        "Inventory"
    );
    m_inventory_thread->notify();
    SL_LOCAL_DEBUG("Waiting for main thread\n", renderer);
    // Wait for signal (kill or resume from main thread)
    m_main_thread->waitForNotify();
    if (m_inventory_thread->isKilled())
    {
        SL_LOCAL_DEBUG("Killed\n", renderer);
        return;
    }

    this->initStartScreenForInventoryThread();

    SL_LOCAL_DEBUG("Starting new renderer\n", renderer);
    m_inventory_thread->markAsRendererStarted();
    // Kill other window, if closed
    renderer.controls()->addLambda(
        *sad::input::ET_Quit,
        [this]() -> void {
            if (!m_is_quitting) {
                m_is_quitting = true;
                m_main_thread->sendKillSignalFrom(m_inventory_thread);
                m_transition_process->unloadTexturesForInventoryThread();
                sad::irrklang::Engine::eref()->stopAllSounds();
                sad::spitJson("highscore.json", picojson::value(static_cast<double>(m_highscore)), m_main_thread->renderer());
                m_options.save(m_main_thread->renderer());
            }
        }
    );

    renderer.pipeline()->appendProcess([=]() {
        this->m_task_lock.acquire();
        if (this->m_inventory_popup)
        {
            if (!(this->m_player->inventory()->isStartedDraggingItem()))
            {
                if (!(this->m_paused_state_machine.isInState("paused")))
                {
                    if (this->m_inventory_popup)
                    {
                        this->m_inventory_popup->render();
                    }
                }
            }
        }
        this->m_task_lock.release();
    });

    renderer.run();
}

void Game::putPlayerPickAccordingToMenuState(Game::MainMenuState state)
{
    sad::Renderer* renderer = m_main_thread->renderer();
    sad::db::Database* db = renderer->database("titlescreen");
    sad::Sprite2D* choice_pointer = db->objectByName<sad::Sprite2D>("PlayerPick");
    sad::Label* new_game_label = db->objectByName<sad::Label>(this->m_main_menu_states_to_labels[state]);
    double x = new_game_label->area().p0().x() - padding_between_label_and_player_choice - (choice_pointer->area().width() / 2.0);
    double y = (new_game_label->area().p0().y() - new_game_label->area().height() / 2.0);
    choice_pointer->setMiddle(sad::Point2D(x, y));
}

void Game::setControlsForMainThread(sad::Renderer* renderer, sad::db::Database*)
{
    // Set pointer for the main menu options
    this->m_main_menu_state = GMMS_PLAY;
    this->putPlayerPickAccordingToMenuState(this->m_main_menu_state);

    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[game::Conditions::CS_START_SCREEN]
        & ((&m_state_machine) * sad::String("starting_screen"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() -> void {
        if (this->m_main_menu_state == Game::GMMS_PLAY)
        {
            this->m_main_menu_state = Game::GMMS_EXIT;
        }
        else
        {
            this->m_main_menu_state = static_cast<Game::MainMenuState>(static_cast<int>(this->m_main_menu_state) - 1);
        }
        this->playSound("misc_menu");
        this->putPlayerPickAccordingToMenuState(this->m_main_menu_state);
    });

    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[game::Conditions::CS_START_SCREEN]
        & ((&m_state_machine) * sad::String("starting_screen"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() -> void {
        if (this->m_main_menu_state == Game::GMMS_EXIT)
        {
            this->m_main_menu_state = Game::GMMS_PLAY;
        }
        else
        {
            this->m_main_menu_state = static_cast<Game::MainMenuState>(static_cast<int>(this->m_main_menu_state) + 1);  // NOLINT(misc-misplaced-widening-cast)
        }
        this->playSound("misc_menu");
        this->putPlayerPickAccordingToMenuState(this->m_main_menu_state);
    });

    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.JumpActionConditions[game::Conditions::CS_START_SCREEN]
        & ((&m_state_machine) * sad::String("starting_screen"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() -> void {
        this->playSound("misc_menu_2");
        switch (this->m_main_menu_state)
        {
            case Game::GMMS_PLAY: this->changeSceneToPlayingScreen(); break;
            case Game::GMMS_OPTIONS: this->changeSceneToOptions(); break;
            case Game::GMMS_EXIT: this->quitGame(); break;
        }
    });

    std::function<void()> empty_callback = [](){};

    // An options
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.LeftKeyConditions[game::Conditions::CS_OPTIONS_SCREEN]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToPreviousItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.RightKeyConditions[game::Conditions::CS_OPTIONS_SCREEN]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToNextItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[game::Conditions::CS_OPTIONS_SCREEN]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToPreviousItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[game::Conditions::CS_OPTIONS_SCREEN]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToNextItem(); }
    );
    renderer->controls()->add(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.JumpActionConditions[game::Conditions::CS_OPTIONS_SCREEN]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        &(this->optionsScreen()),
        &OptionsScreen::tryStartEditing
    );
    renderer->controls()->add(
        *sad::input::ET_KeyPress
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        &(this->optionsScreen()),
        &OptionsScreen::tryHandleEditing
    );

    // A playing game screen
    renderer->controls()->add(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.LeftKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStartGoingLeft
    );
    renderer->controls()->add(
        *sad::input::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.LeftKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStopGoingLeft
    );
    renderer->controls()->add(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.RightKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStartGoingRight
    );
    renderer->controls()->add(
        *sad::input::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.RightKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStopGoingRight
    );
    renderer->controls()->add(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStartGoingUp
    );
    renderer->controls()->add(
        *sad::input::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStopGoingUp
    );
    renderer->controls()->add(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStartGoingDown
    );
    renderer->controls()->add(
        *sad::input::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStopGoingDown
    );

    std::function<void()> perform_action = [this] {
        // this->m_player->setFloaterState(!(this->m_player->isFloater()));
        sad::MaybePoint3D pnt = this->rendererForMainThread()->cursorPosition();
        //bool is_left = this->m_player->isLastMovedLeft();
        double angle = 0;
        if (pnt.exists())
        {
            sad::Point2D middle = this->m_player->middle();
            double dy = pnt.value().y() - middle.y();
            double dx = pnt.value().x() - middle.x();
            if (!sad::is_fuzzy_zero(dx))
            {
                angle = atan2(dy, dx);
            }
        }
        //this->playSound("swing");
        //this->addProjectile(new weapons::Swing(this, this->m_player->actor(), "icons_list/S_Sword01ng", 2, 5000, is_left, true));
        this->playSound("shooting_1");
        this->spawnBullet("bullets/green/x_huge", this->m_player->actor(), 400, angle, false, true);
        //this->addProjectile(new weapons::Laser(this, this->m_player->actor(), "bullets/green/x_huge", angle, 10, 600, 500, true));
    };
    renderer->controls()->addLambda(
        *sad::input::ET_MousePress
        & sad::MouseLeft
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        perform_action
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.JumpActionConditions[game::Conditions::CS_PLAYGAME_PLAYING]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        perform_action
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.PauseCondition
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & sad::Tab
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this] { this->m_is_rendering_world_bodies = !this->m_is_rendering_world_bodies;  }
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & sad::Backspace
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this] { this->changeSceneToStartingScreen();  }
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & sad::End
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this] {
        sad::Vector<sad::p2d::Body*> bodies = this->m_physics_world->allBodiesInGroup("enemies");
        for(size_t i = 0; i < bodies.size(); i++)
        {
            this->killActorByBody(bodies[i]);
        }
    });

    // A paused game screen
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[game::Conditions::CS_PLAYGAME_PAUSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[game::Conditions::CS_PLAYGAME_PAUSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.JumpActionConditions[game::Conditions::CS_PLAYGAME_PAUSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        empty_callback
    );
    
    // Processing of physics events
    renderer->pipeline()->appendProcess([=]() {
        if (this->m_state_machine.isInState("playing"))
        {
            if (this->m_paused_state_machine.isInState("playing"))
            {
                this->m_task_lock.acquire();

                this->m_moving_platform_registry.movePlatforms(m_step_task->stepTick());
                this->updateProjectiles();
                this->m_player->testResting();
                this->m_actors.testResting();
                this->m_actors.process(this, this->m_eval_context);
                this->m_step_task->enable();
                this->m_step_task->process();
                this->tryRenderDebugShapes();
                this->m_triggers.tryRun(this->m_player, this->m_eval_context);

                this->m_task_lock.release();
            }
        }
    });
    
}

void Game::setControlsForInventoryThread(sad::Renderer* renderer)
{
    std::function<void()> empty_callback = []() {};

    // An options
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.LeftKeyConditions[game::Conditions::CS_OPTIONS_SCREEN]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToPreviousItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.RightKeyConditions[game::Conditions::CS_OPTIONS_SCREEN]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToNextItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.UpKeyConditions[game::Conditions::CS_OPTIONS_SCREEN]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToPreviousItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.DownKeyConditions[game::Conditions::CS_OPTIONS_SCREEN]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToNextItem(); }
    );
    renderer->controls()->add(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.JumpActionConditions[game::Conditions::CS_OPTIONS_SCREEN]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        &(this->optionsScreen()),
        &OptionsScreen::tryStartEditing
    );
    renderer->controls()->add(
        *sad::input::ET_KeyPress
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        &(this->optionsScreen()),
        &OptionsScreen::tryHandleEditing
    );

    // A playing game screen
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.LeftKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.RightKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.UpKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.DownKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.JumpActionConditions[game::Conditions::CS_PLAYGAME_PLAYING]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() {
            game::Item* item = this->makeItem("icons_list/Ac_Medal01ng", "A medal", "[DATA EXPUNGED]");
            if (!this->m_player->inventory()->addItem(item)) {
                delete item;
            }
    });
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.PauseCondition
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        empty_callback
    );

    // A paused game screen
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.UpKeyConditions[game::Conditions::CS_PLAYGAME_PAUSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.DownKeyConditions[game::Conditions::CS_PLAYGAME_PAUSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.JumpActionConditions[game::Conditions::CS_PLAYGAME_PAUSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        empty_callback
    );

    std::function<void(const sad::input::MousePressEvent& e)> left_button_click = [=](const sad::input::MousePressEvent& e) {
        sad::Maybe<sad::Triplet<int, int, game::Item*> > result_data = this->m_player->inventory()->getItemWhichIsUnderCursor(e.pos2D());
        if (result_data.exists())
        {
            game::Item* item  = result_data.value().p3();
            this->playSound("apply_item");
            item->applyActiveEffect();
            if (item->shouldDeleteWhenApplied())
            {
                delete this->m_player->inventory()->takeItem(result_data.value().p1(), result_data.value().p2());
                if (this->m_inventory_popup)
                {
                    this->m_inventory_popup->setVisible(false);
                }
            }
        }
    };
    renderer->controls()->addLambda(
        *sad::input::ET_MousePress
        & sad::MouseLeft
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        left_button_click
    );

    std::function<void(const sad::input::MousePressEvent& e)> right_button_click = [=](const sad::input::MousePressEvent& e) {
        this->m_player->inventory()->tryStartDraggingItem(e.pos2D());
    };
    renderer->controls()->addLambda(
        *sad::input::ET_MousePress
        & sad::MouseRight
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        right_button_click
    );

    std::function<void(const sad::input::MouseReleaseEvent& e)> right_button_release = [=](const sad::input::MouseReleaseEvent& e) {
        this->m_player->inventory()->tryReleaseDraggedItem(e.pos2D());
    };
    renderer->controls()->addLambda(
        *sad::input::ET_MouseRelease
        & sad::MouseRight
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        right_button_release
    );

    std::function<void(const sad::input::MouseMoveEvent& e)> move_callback = [=](const sad::input::MouseMoveEvent& e) {
        this->tryShowInventoryPopup(e.pos2D());
        this->m_player->inventory()->tryMoveDraggedItem(e.pos2D());
    };
    renderer->controls()->addLambda(
        *sad::input::ET_MouseMove
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        move_callback
    );
}



void Game::quitGame()
{
    if (!m_is_quitting) {
        m_is_quitting = true;
        m_inventory_thread->sendKillSignalFrom(m_main_thread);
        m_main_thread->sendKillSignalFrom(m_inventory_thread);
        sad::irrklang::Engine::eref()->stopAllSounds();
        // Save data to JSON
        sad::spitJson("highscore.json", picojson::value(static_cast<double>(m_highscore)), m_main_thread->renderer());
        m_options.save(m_main_thread->renderer());
    }
}

int Game::highscore() const
{
    return m_highscore;
}

void Game::setHighscore(int highscore)
{
    m_highscore = highscore;
}

void Game::tryStartStartingState()
{
    this->initStartScreenForMainThread();

    m_state_machine.enterState("starting_screen");
    m_paused_state_machine.enterState("playing");

    playTheme("main_theme");
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Game::initStartScreenForMainThread()
{
    // Play animations
    sad::Renderer* renderer = m_main_thread->renderer();
    sad::db::Database* db  = renderer->database("titlescreen");
    sad::animations::Instance* player_walk = db->objectByName<sad::animations::Instance>("player_walk");
    sad::animations::Instance* player_walk2 = db->objectByName<sad::animations::Instance>("player_walk2");
    renderer->animations()->add(player_walk);
    renderer->animations()->add(player_walk2);

    sad::Scene* scene = renderer->scenes()[0];
    nodes::Background* background = new nodes::Background(true);
    scene->addNode(background);
    scene->setLayer(background, 0);

    sad::Label* highscore = db->objectByName<sad::Label>("Highscore");
    highscore->setString(sad::String("HIGHSCORE   IS   ") + sad::String::number(m_highscore));
    const double middle = 400; // A middle position data
    double width = highscore->area().width();
    // A hardcoded data from titlescreen database
    highscore->setArea(sad::Rect2D(middle - width / 2.0, 585, middle + width / 2.0, 549));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Game::initStartScreenForInventoryThread()
{
    sad::Renderer& renderer= *(m_inventory_thread->renderer());

    sad::Scene* scene = renderer.scenes()[0];
    nodes::Background* background = new nodes::Background(false);
    scene->addNode(background);
}

void Game::playTheme(const sad::String& theme)
{
    sad::irrklang::Sound* theme_data = m_inventory_thread->renderer()->tree("")->get<sad::irrklang::Sound>(theme);
    m_theme_playing = m_theme.play2D(theme_data, m_options.MusicVolume);
}

void Game::setThemeVolume(double volume)
{
    m_options.MusicVolume = volume;
    m_theme_playing->setVolume(volume);
}

void Game::enterPlayingState()
{
    m_paused_state_machine.enterState("playing");
}

void Game::enterTransitioningState()
{
    m_paused_state_machine.enterState("transitioning");
}

void Game::enterPausedState()
{
    m_paused_state_machine.enterState("paused");
}

void Game::triggerWinGame()
{
    // TODO: Copy score to highscore

    this->m_player->toggleIsDead(true);
    sad::Sprite2D* local_sprite = this->m_player->actor()->sprite();
    this->playSound("win");
    this->m_physics_world->removeBody(this->m_player->actor()->body());
    this->rendererForMainThread()->animations()->stopProcessesRelatedToObject(local_sprite);

    sad::Point2D middle = local_sprite->middle();

    sad::animations::SimpleMovement* movement = new sad::animations::SimpleMovement();
    movement->setStartingPoint(middle);
    movement->setEndingPoint(sad::Point2D(middle.x() + this->rendererForMainThread()->settings().width(), middle.y()));
    movement->setTime(800);
    movement->setLooped(false);


    sad::animations::Instance* instance = new sad::animations::Instance();
    instance->setObject(local_sprite);
    instance->setAnimation(movement);
    instance->end([=] { local_sprite->scene()->removeNode(local_sprite); this->changeSceneToWinScreen(); });

    this->rendererForMainThread()->animations()->add(instance);
}

void Game::changeScene(const SceneTransitionOptions& opts) const
{
    m_transition_process->start(opts);
}

void Game::changeSceneToStartingScreen()
{
    m_footsteps.stop();
    m_triggers.clear();
    m_actors.clear();
    this->clearProjectiles();

    m_is_rendering_world_bodies = false;
    this->destroyWorld();
    SceneTransitionOptions options;

    m_inventory_popup = NULL;
    

    sad::Renderer* main_renderer = m_main_thread->renderer();
    sad::Renderer* inventory_renderer = m_inventory_thread->renderer();

    Game::MainMenuState  state = m_main_menu_state;
    options.mainThread().LoadFunction = [main_renderer]() { main_renderer->database("titlescreen")->restoreSnapshot(); };
    options.inventoryThread().LoadFunction = [this]() { this->m_player->reset(); };
    options.mainThread().OnLoadedFunction = [=]()  {
        sad::db::populateScenesFromDatabase(main_renderer, main_renderer->database("titlescreen"));
        this->initStartScreenForMainThread();
        this->putPlayerPickAccordingToMenuState(state);
    };

    options.inventoryThread().OnLoadedFunction = [this, inventory_renderer]() {
        inventory_renderer->clearScenes();
        inventory_renderer->addScene(new sad::Scene());
        this->initStartScreenForInventoryThread();
    };

    options.mainThread().OnFinishedFunction = [this]() {   this->playTheme("main_theme"); this->enterStartScreenState(); this->enterPlayingState(); };
    options.inventoryThread().OnFinishedFunction = [this]() { this->enterStartScreenState();  this->enterPlayingState(); };

    this->enterTransitioningState();
    this->waitForPipelineTasks();
    printf("Starting to change screen to starting\n");
    changeScene(options);
}

void Game::changeSceneToLoseScreen()
{
    m_footsteps.stop();
    m_triggers.clear();
    m_actors.clear();
    this->clearProjectiles();

    m_is_rendering_world_bodies = false;
    this->destroyWorld();
    SceneTransitionOptions options;

    m_inventory_popup = NULL;


    sad::Renderer* main_renderer = m_main_thread->renderer();
    sad::Renderer* inventory_renderer = m_inventory_thread->renderer();

    options.mainThread().LoadFunction = [this]() { this->tryLoadLoseScreen(false); };
    options.inventoryThread().LoadFunction = [this]() { this->m_player->reset(); this->tryLoadLoseScreen(true); };
    options.mainThread().OnLoadedFunction = [=]() {
        sad::db::populateScenesFromDatabase(main_renderer, main_renderer->database("lose_screen"));
    };

    options.inventoryThread().OnLoadedFunction = [inventory_renderer]() {
        sad::db::populateScenesFromDatabase(inventory_renderer, inventory_renderer->database("lose_screen"));
    };

    options.mainThread().OnFinishedFunction = [this]() {
        this->playSound("lose_screen"); this->m_state_machine.enterState("lose_screen"); this->enterPlayingState();
        this->rendererForMainThread()->pipeline()->append(new sad::pipeline::DelayedTask([=] { this->changeSceneToStartingScreen();  }, 1500));
    };
    options.inventoryThread().OnFinishedFunction = [this]() {  this->enterPlayingState(); };

    this->enterTransitioningState();
    this->waitForPipelineTasks();
    printf("Starting to change screen to losing\n");
    changeScene(options);
}

void Game::changeSceneToWinScreen()
{
    m_footsteps.stop();
    m_triggers.clear();
    m_actors.clear();
    this->clearProjectiles();

    m_is_rendering_world_bodies = false;
    this->destroyWorld();
    SceneTransitionOptions options;

    m_inventory_popup = NULL;


    sad::Renderer* main_renderer = m_main_thread->renderer();
    sad::Renderer* inventory_renderer = m_inventory_thread->renderer();

    options.mainThread().LoadFunction = [this]() { this->tryLoadWinScreen(false); };
    options.inventoryThread().LoadFunction = [this]() { this->m_player->reset(); this->tryLoadWinScreen(true); };
    options.mainThread().OnLoadedFunction = [=]() {
        sad::db::populateScenesFromDatabase(main_renderer, main_renderer->database("win_screen"));
    };

    options.inventoryThread().OnLoadedFunction = [inventory_renderer]() {
        sad::db::populateScenesFromDatabase(inventory_renderer, inventory_renderer->database("win_screen"));
    };

    options.mainThread().OnFinishedFunction = [this]() {
        this->playSound("win_screen"); this->m_state_machine.enterState("win_screen"); this->enterPlayingState();
        this->rendererForMainThread()->pipeline()->append(new sad::pipeline::DelayedTask([=] { this->changeSceneToStartingScreen();  }, 2100));
        sad::animations::Group* group = this->rendererForMainThread()->database("win_screen")->objectByName<sad::animations::Group>("Group1");
        this->rendererForMainThread()->animations()->add(group);
    };
    options.inventoryThread().OnFinishedFunction = [this]() {
        sad::animations::Group* group = this->rendererForInventoryThread()->database("win_screen")->objectByName<sad::animations::Group>("Group1");
        this->rendererForInventoryThread()->animations()->add(group);

        this->enterPlayingState();
    };

    this->enterTransitioningState();
    this->waitForPipelineTasks();
    printf("Starting to change screen to winning\n");
    changeScene(options);
}

void Game::changeSceneToPlayingScreen()
{
    this->destroyWorld();
    m_footsteps.stop();
    m_actors.clear();
    m_triggers.clear();
    m_moving_platform_registry.clear();
    this->clearProjectiles();

    SceneTransitionOptions options;

    m_inventory_popup = NULL;
    m_is_rendering_world_bodies = false;
    

    sad::Renderer* main_renderer = m_main_thread->renderer();
    sad::Renderer* inventory_renderer = m_inventory_thread->renderer();

    m_player->reset();
    m_player->inventory()->addItem(this->makeItem("icons_list/Ac_Medal01ng", "A medal", "Data "));
    m_player->inventory()->addItem(this->makeItem("icons_list/Ac_Medal02ng", "An item", "Data 2", true));
    m_player->inventory()->addItem(this->makeItem("icons_list/Ac_Medal03ng", "A stuff", "Data 3"));

    options.mainThread().LoadFunction = [this]() {  this->tryLoadGameScreen(); };

    options.mainThread().OnLoadedFunction = [=]() {
        sad::db::Database* db = main_renderer->database("gamescreen");
        this->m_moving_platform_registry.setDatabase(db);
        sad::db::populateScenesFromDatabase(main_renderer, db);
        this->initGamePhysics();
        // When loaded we should evaluate initialization script
        this->evaluateInitializationScript();
    };

    options.inventoryThread().OnLoadedFunction = [=]() {
        sad::Scene* scene = new sad::Scene();
        scene->setRenderer(inventory_renderer);
        this->m_inventory_node = new nodes::InventoryNode(m_player->inventory());
        this->m_inventory_popup = this->m_inventory_node->popup();
        scene->addNode(this->m_inventory_node);
        inventory_renderer->clearScenes();
        inventory_renderer->addScene(scene);
        this->playTheme("title_theme");
    };

    options.mainThread().OnFinishedFunction = [this]() {  this->enterPlayingScreenState(); this->enterPlayingState(); };
    options.inventoryThread().OnFinishedFunction = [this, inventory_renderer]() {
        this->enterPlayingScreenState();
        this->enterPlayingState();
        sad::MaybePoint3D mbp3d = inventory_renderer->cursorPosition();
        if (mbp3d.exists())
        {
            sad::Point2D pnt(mbp3d.value().x(), mbp3d.value().y());
            this->tryShowInventoryPopup(pnt);
        }
    };

    this->enterTransitioningState();
    this->waitForPipelineTasks();

    changeScene(options);
}

void Game::changeSceneToOptions()
{
    m_footsteps.stop();
    m_triggers.clear();
    m_actors.clear();
    this->clearProjectiles();


    m_is_rendering_world_bodies = false;
    this->destroyWorld();
    this->m_player->reset();


    m_inventory_popup = NULL;
    

    SceneTransitionOptions options;
    sad::Renderer* main_renderer = m_main_thread->renderer();
    sad::Renderer* inventory_renderer = m_inventory_thread->renderer();

    options.mainThread().LoadFunction = [this]() {  this->tryLoadOptionsScreen(false); };
    options.inventoryThread().LoadFunction = [this]() { this->m_player->reset(); this->tryLoadOptionsScreen(true); };

    options.mainThread().OnLoadedFunction = [=]()  {
        sad::db::populateScenesFromDatabase(main_renderer, main_renderer->database("optionsscreen"));
        this->optionsScreen().initForMainRenderer();
    };

    options.inventoryThread().OnLoadedFunction = [=]() {
        sad::db::populateScenesFromDatabase(inventory_renderer, inventory_renderer->database("optionsscreen"));
        this->optionsScreen().initForInventoryRenderer();
    };

    options.mainThread().OnFinishedFunction = [this]() {  this->enterOptionsState(); this->enterPlayingState(); };
    options.inventoryThread().OnFinishedFunction = [this]() { this->enterOptionsState();  this->enterPlayingState(); };

    this->enterTransitioningState();
    this->waitForPipelineTasks();

    changeScene(options);
}

void Game::enterStartScreenState()
{
    m_state_machine.enterState("starting_screen");
}

void Game::enterOptionsState()
{
     m_state_machine.enterState("options");
}

void Game::enterPlayingScreenState()
{
    m_state_machine.enterState("playing");
}

void Game::tryLoadOptionsScreen(bool is_inventory_thread)
{
    this->tryLoadIdenticalScreenDatabase(m_loaded_options_database, "optionsscreen", "examples/multithreading/optionsscreen.json", is_inventory_thread);
}

void Game::tryLoadLoseScreen(bool is_inventory_thread)
{
    this->tryLoadIdenticalScreenDatabase(m_loaded_lose_screen_database, "lose_screen", "examples/multithreading/lose_screen.json", is_inventory_thread);
}

void Game::tryLoadWinScreen(bool is_inventory_thread)
{
    this->tryLoadIdenticalScreenDatabase(m_loaded_win_screen_database, "win_screen", "examples/multithreading/win_screen.json", is_inventory_thread);
}

void Game::tryLoadGameScreen()
{
    sad::Renderer* renderer = m_main_thread->renderer();
    if (m_loaded_game_screen)
    {
        renderer->database("gamescreen")->restoreSnapshot();
    }
    else
    {
        sad::db::Database* database = new sad::db::Database();
        database->setRenderer(renderer);
        database->tryLoadFrom("examples/multithreading/game_screen.json");
        database->saveSnapshot();
        renderer->addDatabase("gamescreen", database);
        m_loaded_game_screen = true;
    }
}

OptionsScreen& Game::optionsScreen()
{
    return m_options_screen;
}

void Game::playSound(const sad::String& sound_name) const
{
    sad::irrklang::Sound* theme_data = m_inventory_thread->renderer()->tree("")->get<sad::irrklang::Sound>(sound_name);
    theme_data->play2D(m_options.SoundVolume, false);
}

void Game::playWalkingSound()
{
    sad::irrklang::Sound* theme_data = m_inventory_thread->renderer()->tree("")->get<sad::irrklang::Sound>("footstep");
    m_footsteps.play2D(theme_data, m_options.MusicVolume);
}

void Game::stopWalkingSound()
{
    m_footsteps.stop();
}

game::Options* Game::options()
{
    return &m_options;
}

game::Conditions* Game::conditions()
{
    return &m_conditions;
}

sad::Renderer* Game::rendererForMainThread() const
{
    return m_main_thread->renderer();
}

sad::Renderer* Game::rendererForInventoryThread() const
{
    return m_inventory_thread->renderer();
}

void Game::tryShowInventoryPopup(const sad::Point2D& p) const
{
    sad::Maybe<sad::Triplet<int, int, game::Item*> > result_data = m_player->inventory()->getItemWhichIsUnderCursor(p);
    bool handled = false;
    if (!(m_player->inventory()->isStartedDraggingItem()))
    { 
        if (m_player->inventory()->isInBasketArea(p))
        {
            if (m_inventory_popup)
            {
                m_inventory_popup->setDescription("Basket", "Drag here item, which you want to be removed.\nYou can drag items by pressing right mouse buttons on item, which you want to drag and dragging them here\nUse left mouse button to apply item\'s effect, if any.");
                m_inventory_popup->showAt(p);
                m_inventory_popup->setVisible(true);
                handled = true;
            }
        }
        if (result_data.exists())
        {
            game::Item* item = result_data.value().p3();
            if (m_inventory_popup)
            {
                m_inventory_popup->setDescription(item->title(), item->description());
                m_inventory_popup->showAt(p);
                m_inventory_popup->setVisible(true);
                handled = true;
            }
        }
    }

    if (!handled)
    {
        if (m_inventory_popup)
        {
            m_inventory_popup->setVisible(false);
        }
    }
}

bool Game::evalScript(const sad::String& s) const
{
    sad::String result;
    if (!m_eval_context->eval(s, true, &result))
    {
        SL_LOCAL_WARNING(sad::String("Warning: error, when evaluating script:") + s + ": \n" + result + "\n\n", (*m_main_thread->renderer()));
        return false;
    }
    return true;
}

void Game::evaluateInitializationScript() const
{
    sad::Maybe<sad::String> maybe_script = sad::slurp("examples/multithreading/init.js", m_main_thread->renderer());
    if (maybe_script.exists())
    {
        // ReSharper disable CppExpressionWithoutSideEffects
        this->evalScript(maybe_script.value());
    }
}

game::Item* Game::makeItem(const sad::String& icon, const sad::String& title, const sad::String& description, bool delete_after_apply)
{
    game::Item* item = new game::Item(icon, title, description, delete_after_apply);
    item->setGame(this);
    item->setEvaluatedScript(this->tryGetScriptForItem(title));
    return item;
}

void Game::enableGravity(sad::p2d::Body* b)
{
    Game::setGravityForBody(b, Game::GravityForceValue);
}

void Game::waitForPipelineTasks()
{
    m_task_lock.waitForTasks();
}

sad::p2d::BounceSolver* Game::bounceSolver() const
{
    return m_bounce_solver;
}

sad::p2d::World* Game::physicsWorld() const
{
    return m_physics_world;
}

void Game::killActorByBody(sad::p2d::Body* body)
{
    sad::db::Database* db = this->rendererForMainThread()->database("gamescreen");
    sad::Scene* main_scene = db->objectByName<sad::Scene>("main");

    game::Actor* actor = static_cast<game::Actor*>(body->userObject());
    this->killProjectilesRelatedToActor(actor);
    this->rendererForMainThread()->animations()->stopProcessesRelatedToObject(actor->sprite());
    m_physics_world->removeBody(body);
    main_scene->removeNode(actor->sprite());
    m_actors.remove(actor);
}

void Game::killActorWithoutSprite(game::Actor* actor)
{
    this->killProjectilesRelatedToActor(actor);
    this->rendererForMainThread()->animations()->stopProcessesRelatedToObject(actor->sprite());
    m_physics_world->removeBody(actor->body());
    m_actors.remove(actor);
}

game::Player* Game::player() const
{
    return m_player;
}

void Game::killProjectilesRelatedToActor(game::Actor* actor)
{
    for(size_t i  = 0; i < m_projectiles.size(); i++)
    {
        if (m_projectiles[i]->actor() == actor)
        {
            m_projectiles[i]->kill();
            m_projectiles[i]->delRef();
            m_projectiles.removeAt(i);
            --i;
        }
    }
}

void Game::killProjectile(weapons::Projectile* projectile)
{
    for (size_t i = 0; i < m_projectiles.size(); i++)
    {
        if (m_projectiles[i] == projectile)
        {
            m_projectiles[i]->kill();
            m_projectiles[i]->delRef();
            m_projectiles.removeAt(i);
            --i;
        }
    }
}

void Game::addProjectile(weapons::Projectile* projectile)
{
    if (std::find(m_projectiles.begin(), m_projectiles.end(), projectile) == m_projectiles.end())
    {
        projectile->addRef();
        m_projectiles << projectile;
    }
}

sad::Point2D Game::pointOnActorForBullet(game::Actor* actor, double angle)  const
{
    while (angle < 0)
    {
        angle += 2 * M_PI;
    }

    while (angle > 2 * M_PI)
    {
        angle -= 2 * M_PI;
    }

    sad::Point2D middle = actor->middle();
    sad::p2d::InfiniteLine line = sad::p2d::InfiniteLine::appliedVector(middle, sad::p2d::Vector(cos(angle), sin(angle)));
    double rect_angle = 0;
    if (!sad::is_fuzzy_zero(actor->area().width()))
    {
        rect_angle = atan2(actor->area().height(), actor->area().width());
    }
    sad::p2d::MaybePoint pivot_point;
    if ((angle <= rect_angle) || (angle >= (2 * M_PI - rect_angle)))
    {
        pivot_point = line.intersection(sad::p2d::Cutter2D(actor->area()[1], actor->area()[2]));
    }
    else
    {
        if (angle <= (M_PI - rect_angle))
        {
            pivot_point = line.intersection(sad::p2d::Cutter2D(actor->area()[2], actor->area()[3]));
        }
        else
        {
            if (angle < M_PI + rect_angle)
            {
                pivot_point = line.intersection(sad::p2d::Cutter2D(actor->area()[0], actor->area()[3]));
            }
            else
            {
                pivot_point = line.intersection(sad::p2d::Cutter2D(actor->area()[0], actor->area()[1]));
            }
        }
    }
    sad::Point2D result_middle;
    if (!pivot_point.exists())
    {
        result_middle = middle;
    }
    else
    {
        result_middle = pivot_point.value();
    }
    return result_middle;
}

weapons::Bullet* Game::spawnBullet(const sad::String& icon_name, game::Actor* actor, double speed, double angle, bool apply_gravity, bool is_player) const
{
    while(angle < 0)
    {
        angle += 2* M_PI;
    }

    while (angle > 2 * M_PI)
    {
        angle -= 2 * M_PI;
    }

    sad::Renderer* r = this->rendererForMainThread();
    sad::Sprite2D::Options* opts = r->tree()->get<sad::Sprite2D::Options>(icon_name);
    if (opts)
    {
        sad::db::Database* db = r->database("gamescreen");
        sad::Scene* main_scene = db->objectByName<sad::Scene>("main");

        sad::Point2D result_middle = this->pointOnActorForBullet(actor, angle);
        
        double halfwidth = opts->Rectangle.width() / 2.0, halfheight = opts->Rectangle.height() / 2.0;
        result_middle.setX(result_middle.x() + halfwidth * cos(angle));
        result_middle.setY(result_middle.y() + halfheight * sin(angle));
        sad::Rect2D area(result_middle.x() - halfwidth, result_middle.y() - halfheight, result_middle.x() + halfwidth, result_middle.y() + halfheight);
        sad::Sprite2D* sprite = new sad::Sprite2D();
        sprite->setScene(main_scene);
        sprite->setTreeName(r, "");
        sprite->set(icon_name);
        sprite->setArea(area);
        main_scene->addNode(sprite);

        sad::p2d::Rectangle* rectangle = new sad::p2d::Rectangle();
        rectangle->setRect(sprite->renderableArea());

        weapons::Bullet* bullet = new weapons::Bullet();
        bullet->setSprite(sprite);

        sad::p2d::Body* body = new sad::p2d::Body();
        body->setCurrentAngularVelocity(0);
        body->setCurrentTangentialVelocity(sad::p2d::Vector(speed * cos(angle), speed * sin(angle)));
        body->attachObject(bullet);
        body->setShape(rectangle);
        body->initPosition(sprite->middle());
        if (apply_gravity)
        {
            body->addForce(new sad::p2d::TangentialForce(Game::GravityForceValue));
        }

        if (is_player)
        {
            this->physicsWorld()->addBodyToGroup("player_bullets", body);
        }
        else
        {
            this->physicsWorld()->addBodyToGroup("enemy_bullets", body);
        }
        return bullet;
    }
    return NULL;
}

void Game::tryDecayBullet(sad::p2d::Body* bullet, const sad::String& sound)
{
    sad::Object* a = bullet->userObject();
    // This conditions only holds only in case of bullets,
    if (a->metaData()->name() == weapons::Bullet::globalMetaData()->name())
    {
        weapons::Bullet* bullet_object = static_cast<weapons::Bullet*>(a);
        sad::Sprite2D* local_sprite = bullet_object->sprite();
        sad::Scene* scene = local_sprite->scene();
        sad::Point2D middle = local_sprite->middle();

        sad::String options_name = local_sprite->optionsName();
        sad::Sprite2D::Options* opts = scene->renderer()->tree()->get<sad::Sprite2D::Options>(options_name);

        this->rendererForMainThread()->pipeline()->appendTask([=]() {
            local_sprite->scene()->removeNode(local_sprite);
            this->m_physics_world->removeBody(bullet);
        });
        

        double dist  = 10;
        sad::Point2D vectors[4] = {
            sad::Point2D(-dist, -dist),
            sad::Point2D(dist, -dist),
            sad::Point2D(-dist, dist),
            sad::Point2D(dist, dist)
        };
        if (sound.length())
        {
            this->playSound(sound);
        }
        for (auto vector : vectors)
        {
            sad::Sprite2D* anim_sprite = new sad::Sprite2D();
            anim_sprite->setScene(scene);
            anim_sprite->setTreeName(scene->renderer(), "");
            anim_sprite->set(options_name);

            sad::Rect2D rect(
                middle.x() - opts->Rectangle.width() / 4.0, -middle.x() - opts->Rectangle.height() / 4.0,
                middle.x() + opts->Rectangle.width() / 4.0, -middle.x() + opts->Rectangle.height() / 4.0
            );
            anim_sprite->setArea(rect);
            scene->addNode(anim_sprite);

            sad::animations::SimpleMovement* movement = new sad::animations::SimpleMovement();
            movement->setTime(300);
            movement->setStartingPoint(middle);
            movement->setEndingPoint(middle + vector);

            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setObject(anim_sprite);
            instance->setAnimation(movement);
            instance->end([=] { anim_sprite->scene()->removeNode(anim_sprite); });

            scene->renderer()->animations()->add(instance);
        }
    }
}

sad::animations::Instance* Game::spawnDeathAnimationForActorsSprite(sad::Sprite2D* sprite)
{
    sad::Point2D middle = sprite->middle();

    sad::animations::SimpleMovement* movement = new sad::animations::SimpleMovement();
    movement->setStartingPoint(middle);
    movement->setEndingPoint(sad::Point2D(middle.x(), -(sprite->area().height())));
    movement->setTime(1000);
    movement->setLooped(false);

    sad::animations::Rotate* rotate = new sad::animations::Rotate();
    rotate->setMinAngle(0);
    rotate->setMaxAngle(4 * M_PI);
    rotate->setTime(1000);
    rotate->setLooped(false);

    sad::animations::Parallel* parallel = new sad::animations::Parallel();
    parallel->add(movement);
    parallel->add(rotate);
    parallel->setTime(1000);
    parallel->setLooped(false);

    sad::animations::Instance* instance= new sad::animations::Instance();
    instance->setObject(sprite);
    instance->setAnimation(parallel);
    instance->end([=]{ sprite->scene()->removeNode(sprite); });

    this->rendererForMainThread()->animations()->add(instance);

    return instance;
}

void Game::disableGravity(sad::p2d::Body* b)
{
    Game::setGravityForBody(b, sad::p2d::Vector(0.0, 0.0));
}

void Game::setGravityForBody(sad::p2d::Body* b, const sad::p2d::Vector& v)
{
    if (!b)
    {
        return;
    }
    const sad::Vector<sad::p2d::Force<sad::p2d::Vector>* >& forces = b->tangentialForcesList();
    if (forces.empty())
    {
        b->addForce(new sad::p2d::TangentialForce(Game::GravityForceValue)); 
    }
    else
    {
        forces[0]->setValue(v);
    }
}

// ==================================== PRIVATE METHODS ====================================

// Player cannot be copied so, disable it here to ensure context proper initialization
namespace sad
{

namespace dukpp03
{


template<>
struct GetAddressOfType<game::Player*, false, false>
{
public:
    /*! Returns address of type, stored in variant.
        \return empty maybe
     */
    static ::dukpp03::Maybe<game::Player*> getAddress(sad::db::Variant*)
    {
        return {};
    }
};

}

}


void Game::initContext()
{
    // Initialize context
    m_eval_context = new sad::dukpp03::Context();
    sad::dukpp03irrklang::init(m_eval_context);
    // Bind self
    m_eval_context->registerGlobal("game", this);
    m_eval_context->registerGlobal("state_machine", &m_state_machine);
    m_eval_context->registerGlobal("paused_state_machine", &m_paused_state_machine);


    // Bind callables
    std::function<void(const sad::String&, const sad::String&)> make_platform_go_on_way = [=](const sad::String& platform, const sad::String& way) {
        this->m_moving_platform_registry.add(platform, way);
    };
    std::function<void(double, const sad::dukpp03::CompiledFunction&)> add_trigger = [=](double x, const sad::dukpp03::CompiledFunction& f) {
        this->m_triggers.add(x, f, false);
    };
    std::function<void(double, const sad::dukpp03::CompiledFunction&)> add_trigger_once = [=](double x, const sad::dukpp03::CompiledFunction& f) {
        this->m_triggers.add(x, f, true);
    };
    std::function<void(const sad::String&)> print = [=](const sad::String& message) {
        printf("%s\n", message.c_str());
        SL_LOCAL_DEBUG(message, *(m_main_thread->renderer()));
    };

    std::function<void(const sad::String&,
                       const sad::Point2D&,
                       const sad::dukpp03::CompiledFunction&,
                       const sad::Hash<sad::String, sad::db::Variant>&)
    > spawn_enemy_walker =  [=](const sad::String& optname,
            const sad::Point2D& middle,
            const sad::dukpp03::CompiledFunction& function,
            const sad::Hash<sad::String, sad::db::Variant>& state
    ) {
        game::Actor* actor = this->makeEnemy(optname, middle);
        if (actor)
        {
            this->m_actors.add(actor, new bots::JSBot(function, state, this->m_eval_context));
        }
    };
    std::function<void(const sad::String&,
        const sad::Point2D&,
        const sad::String& botname)
    > spawn_enemy_walker2 = [=](const sad::String& optname,
        const sad::Point2D& middle,
        const sad::String& botname
    ) {
        if (this->m_bot_registry.contains(botname))
        { 
            game::Actor* actor = this->makeEnemy(optname, middle);
            if (actor)
            {
                this->m_actors.add(actor, this->m_bot_registry.get(botname)->clone());
            }
        }
    };
    std::function<void(const sad::String&,
        const sad::Point2D&,
        double from,
        double to,
        double dive_height
    )
    > spawn_animated_floater = [=](const sad::String& optname,
        const sad::Point2D& middle,
        double from,
        double to,
        double dive_height
        ) {
        game::Actor* actor = this->makeEnemy(optname, middle);
        if (actor)
        {
            this->m_actors.add(actor, new bots::DivingFloaterBot(from, to, middle.y(), middle.y() - dive_height));

            sad::animations::OptionList* list = new sad::animations::OptionList();
            list->setList(this->m_actor_options[optname]->FloaterFlyAnimationOptions);
            list->setTime(200);
            list->setLooped(true);

            sad::animations::Instance* i = new sad::animations::Instance();
            i->setAnimation(list);
            i->setObject(actor->sprite());

            this->rendererForMainThread()->animations()->add(i);
        }
    };
    std::function<void(const sad::String&,
        const sad::Point2D&
    )
    > spawn_follow_player_floater = [=](const sad::String& optname,
        const sad::Point2D& middle
    ) {
        game::Actor* actor = this->makeEnemy(optname, middle);
        if (actor)
        {
            this->m_actors.add(actor, new bots::FollowPlayerFloater());
        }
    };
    std::function<void(const sad::String&,
        const sad::Point2D&,
        int,
        int
    )
    > spawn_enemy_in_direction = [=](const sad::String& optname,
        const sad::Point2D& middle,
        int h, int v
    ) {
        game::Actor* actor = this->makeEnemy(optname, middle);
        if (actor)
        {
            this->m_actors.add(actor, new bots::DirectionBot(h, v));
        }
    };
    std::function<void(const sad::String&,
        const sad::Point2D&
    )
    > spawn_platform_patrol = [=](const sad::String& optname,
        const sad::Point2D& middle
    ) {
        game::Actor* actor = this->makeEnemy(optname, middle);
        if (actor)
        {
            this->m_actors.add(actor, new bots::PlatformPatrolBot());
        }
    };
    sad::dukpp03::MultiMethod* spawn_enemy_walker_at = new sad::dukpp03::MultiMethod();
    spawn_enemy_walker_at->add(sad::dukpp03::make_lambda::from(spawn_enemy_walker));
    spawn_enemy_walker_at->add(sad::dukpp03::make_lambda::from(spawn_enemy_walker2));
    m_eval_context->registerCallable("spawnEnemyWalkerAt", spawn_enemy_walker_at);
    m_eval_context->registerCallable("spawnAnimatedFloater", sad::dukpp03::make_lambda::from(spawn_animated_floater));
    m_eval_context->registerCallable("spawnFollowPlayerFloater", sad::dukpp03::make_lambda::from(spawn_follow_player_floater));
    m_eval_context->registerCallable("spawnEnemyInDirection", sad::dukpp03::make_lambda::from(spawn_enemy_in_direction));
    m_eval_context->registerCallable("spawnPlatformPatrol", sad::dukpp03::make_lambda::from(spawn_platform_patrol));
    m_eval_context->registerCallable("makePlatformGoOnWay", sad::dukpp03::make_lambda::from(make_platform_go_on_way));
    m_eval_context->registerCallable("addTrigger", sad::dukpp03::make_lambda::from(add_trigger));
    m_eval_context->registerCallable("addTriggerOnce", sad::dukpp03::make_lambda::from(add_trigger_once));
    m_eval_context->registerCallable("gamePrint", sad::dukpp03::make_lambda::from(print));

    std::function<void()> trigger_win_game = [=] {
        this->rendererForMainThread()->pipeline()->appendTask([=] { this->triggerWinGame(); });
    };
    m_eval_context->registerCallable("triggerWinGame", sad::dukpp03::make_lambda::from(trigger_win_game));

    game::exposeActorOptions(m_eval_context);

    std::function<bool(const sad::String&, const game::ActorOptions&)> add_actor_options = [=](const sad::String& name, const game::ActorOptions& opts) {
        if (m_actor_options.contains(name)) {
            return false;
        }
        game::ActorOptions* nopts = new game::ActorOptions(opts);
        nopts->addRef();
        m_actor_options.insert(name, nopts);
        return true;
    };
    m_eval_context->registerCallable("addActorOptions", sad::dukpp03::make_lambda::from(add_actor_options));

    // Expose player to script
    sad::dukpp03::ClassBinding* player_binding = new sad::dukpp03::ClassBinding();
    player_binding->addMethod("reset", sad::dukpp03::bind_method::from(&game::Player::reset));
    player_binding->addMethod("middle", sad::dukpp03::bind_method::from(&game::Player::middle));
    player_binding->addMethod("area", sad::dukpp03::bind_method::from(&game::Player::area));
    player_binding->addMethod("isFloater", sad::dukpp03::bind_method::from(&game::Player::isFloater));
    player_binding->addMethod("setFloaterState", sad::dukpp03::bind_method::from(&game::Player::setFloaterState));

    player_binding->addMethod("tryStartGoingUp", sad::dukpp03::bind_method::from(&game::Player::tryStartGoingUp));
    player_binding->addMethod("tryStartGoingDown", sad::dukpp03::bind_method::from(&game::Player::tryStartGoingDown));
    player_binding->addMethod("tryStartGoingLeft", sad::dukpp03::bind_method::from(&game::Player::tryStartGoingLeft));
    player_binding->addMethod("tryStartGoingRight", sad::dukpp03::bind_method::from(&game::Player::tryStartGoingRight));

    player_binding->addMethod("tryStopGoingUp", sad::dukpp03::bind_method::from(&game::Player::tryStopGoingUp));
    player_binding->addMethod("tryStopGoingDown", sad::dukpp03::bind_method::from(&game::Player::tryStopGoingDown));
    player_binding->addMethod("tryStopGoingLeft", sad::dukpp03::bind_method::from(&game::Player::tryStopGoingLeft));
    player_binding->addMethod("tryStopGoingRight", sad::dukpp03::bind_method::from(&game::Player::tryStopGoingRight));
    player_binding->addMethod("setLives", sad::dukpp03::bind_method::from(&game::Player::setLives));
    player_binding->addMethod("incrementLives", sad::dukpp03::bind_method::from(&game::Player::incrementLives));
    player_binding->addMethod("decrementLives", sad::dukpp03::bind_method::from(&game::Player::decrementLives));
    player_binding->addMethod("lives", sad::dukpp03::bind_method::from(&game::Player::lives));
    player_binding->addMethod("toggleInvincibility", sad::dukpp03::bind_method::from(&game::Player::toggleInvincibility));
    player_binding->addMethod("isInvincible", sad::dukpp03::bind_method::from(&game::Player::isInvincible));

    player_binding->addMethod("testResting", sad::dukpp03::bind_method::from(&game::Player::testResting));
    player_binding->addMethod("enableGravity", sad::dukpp03::bind_method::from(&game::Player::enableGravity));

    m_eval_context->addClassBinding("game::Player", player_binding);
    std::function<game::Player*()> player = [=]() { return this->m_player; };
    m_eval_context->registerCallable("player", sad::dukpp03::make_lambda::from(player));

    sad::dukpp03::ClassBinding* actor_binding = new sad::dukpp03::ClassBinding();

    actor_binding->addMethod("isResting", sad::dukpp03::bind_method::from(&game::Actor::isResting));
    actor_binding->addMethod("area", sad::dukpp03::bind_method::from(&game::Actor::area));
    actor_binding->addMethod("middle", sad::dukpp03::bind_method::from(&game::Actor::middle));

    actor_binding->addMethod("lives", sad::dukpp03::bind_method::from(&game::Actor::lives));
    actor_binding->addMethod("setLives", sad::dukpp03::bind_method::from(&game::Actor::setLives));
    actor_binding->addMethod("incrementLives", sad::dukpp03::bind_method::from(&game::Actor::incrementLives));
    actor_binding->addMethod("decrementLives", sad::dukpp03::bind_method::from(&game::Actor::decrementLives));
    actor_binding->addMethod("toggleInvincibility", sad::dukpp03::bind_method::from(&game::Actor::toggleInvincibility));
    actor_binding->addMethod("isInvincible", sad::dukpp03::bind_method::from(&game::Actor::isInvincible));

    actor_binding->addMethod("isFloater", sad::dukpp03::bind_method::from(&game::Actor::isFloater));
    actor_binding->addMethod("setFloaterState", sad::dukpp03::bind_method::from(&game::Actor::setFloaterState));

    actor_binding->addMethod("setVelocity", sad::dukpp03::bind_method::from(&game::Actor::setVelocity));

    actor_binding->addMethod("tryStartGoingUp", sad::dukpp03::bind_method::from(&game::Actor::tryStartGoingUp));
    actor_binding->addMethod("tryStartGoingDown", sad::dukpp03::bind_method::from(&game::Actor::tryStartGoingDown));
    actor_binding->addMethod("tryStartGoingLeft", sad::dukpp03::bind_method::from(&game::Actor::tryStartGoingLeft));
    actor_binding->addMethod("tryStartGoingRight", sad::dukpp03::bind_method::from(&game::Actor::tryStartGoingRight));

    actor_binding->addMethod("tryStopGoingUp", sad::dukpp03::bind_method::from(&game::Actor::tryStopGoingUp));
    actor_binding->addMethod("tryStopGoingDown", sad::dukpp03::bind_method::from(&game::Actor::tryStopGoingDown));
    actor_binding->addMethod("tryStopGoingLeft", sad::dukpp03::bind_method::from(&game::Actor::tryStopGoingLeft));
    actor_binding->addMethod("tryStopGoingRight", sad::dukpp03::bind_method::from(&game::Actor::tryStopGoingRight));

    m_eval_context->addClassBinding("game::Actor", actor_binding);


    // Fetch and run game initialization script
    sad::Maybe<sad::String> maybe_script = sad::slurp("examples/multithreading/game_init.js", m_main_thread->renderer());
    if (maybe_script.exists())
    {
        this->evalScript(maybe_script.value());
    }
    else
    {
        SL_LOCAL_FATAL("Unable to run game initialization script", *(m_main_thread->renderer()));
    }
}


void Game::clearProjectiles(bool kill)
{
    for(size_t i = 0; i < m_projectiles.size(); i++)
    {
        if (kill)
        { 
            m_projectiles[i]->kill();
        }
        m_projectiles[i]->delRef();
    }
    m_projectiles.clear();
}

sad::String* Game::tryGetScriptForItem(const sad::String& title)
{
    if (m_item_names_to_scripts.contains(title))
    {
        return m_item_names_to_scripts[title];
    }

    sad::String escaped_title = title;
    sad::String escaped_characters = "\\/`~!@#$%^&*-+=\"\'[]{}<>,.?";  
    for(size_t i = 0; i < escaped_characters.length(); i++)
    {
        escaped_title.replaceAllOccurences(sad::String(&(escaped_characters[i]), 1), "_");
    }
    sad::Renderer* renderer = m_main_thread->renderer();
    sad::Maybe<sad::String> maybe_js = sad::slurp(sad::String("examples/multithreading/") + escaped_title + ".js", renderer);
    sad::String* result = new sad::String();
    if (maybe_js.exists())
    {
        *result = maybe_js.value();
    }
    m_item_names_to_scripts.insert(title, result);
    return result;
}

void Game::destroyWorld()
{
    if (m_physics_world)
    {
        m_step_task->setWorld(NULL);
        m_physics_world->delRef();
        m_physics_world = NULL;
    }
}

void Game::initGamePhysics()
{
    m_physics_world = new sad::p2d::World();
    m_physics_world->setDetector(new sad::p2d::MultisamplingCollisionDetector(2.0));
    m_physics_world->addRef();
    m_physics_world->addGroup("player");
    m_physics_world->addGroup("enemies");
    m_physics_world->addGroup("platforms");
    m_physics_world->addGroup("walls");
    m_physics_world->addGroup("player_bullets");
    m_physics_world->addGroup("enemy_bullets");

    m_max_level_x = 0;

    sad::Renderer* renderer = m_main_thread->renderer();
    sad::db::Database* db = renderer->database("gamescreen");
    sad::Sprite2D* sprite = db->objectByName<sad::Sprite2D>("Player");
    if (sprite)
    { 
        m_player->setSprite(sprite);
        sad::p2d::Body* body = new sad::p2d::Body();
        body->setCurrentAngularVelocity(0);
        body->setCurrentTangentialVelocity(sad::p2d::Vector(0,0));
        body->setUserObject(sprite);
        sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
        rect->setRect(sprite->area());
        body->setShape(rect);
        body->attachObject(sprite);
        body->initPosition(sprite->middle());

        m_physics_world->addBodyToGroup("player", body);
        m_player->setBody(body);
        m_player->enableGravity();
        m_player->init(true);
    }
    sad::Scene* main_scene = db->objectByName<sad::Scene>("main");
    if (main_scene)
    {
        const sad::Vector<sad::SceneNode*>&  nodes = main_scene->objects();
        // Filter grouped and ungrouped platforms
        sad::Vector<sad::Sprite2D*> platform_sprites;
        sad::Vector<sad::Sprite2D*> ungrouped_platform_sprites;
        for (size_t i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->isInstanceOf("sad::Sprite2D"))
            {
                sad::String name = nodes[i]->objectName();
                name.toUpper();
                if ((name.getOccurence("FLOOR") != -1)  || (name.getOccurence("PLATFORM") != -1))
                {
                    sad::Sprite2D* node = dynamic_cast<sad::Sprite2D*>(nodes[i]);
                    if (node) 
                    {
                        if (name.getOccurence("MOVING") != -1)
                        {
                            ungrouped_platform_sprites << node;
                        }
                        else
                        {
                            platform_sprites << node;
                        }
                        m_max_level_x = std::max(m_max_level_x, node->area()[2].x());
                    }
                }
            }
        }
        // Add unfiltered platforms to group
        for (size_t i = 0; i < ungrouped_platform_sprites.size(); i++)
        {
            sad::p2d::Body* body = new sad::p2d::Body();
            body->setCurrentAngularVelocity(0);
            body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
            body->attachObject(ungrouped_platform_sprites[i]);
            sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
            sad::Rect2D rct = ungrouped_platform_sprites[i]->area();
            // Slight increase of area, due to paddings
            rct = sad::Rect2D(rct[0].x(), rct[0].y(), rct[2].x() + 1, rct[2].y());
            rect->setRect(rct);
            body->setShape(rect);
            body->initPosition(ungrouped_platform_sprites[i]->middle());
            m_moving_platform_registry.addPlatform(ungrouped_platform_sprites[i]->objectName(), body);

            m_physics_world->addBodyToGroup("platforms", body);
        }
        // Sort sprites according from left to right
        std::sort(platform_sprites.begin(), platform_sprites.end(), [](sad::Sprite2D* a, sad::Sprite2D* b) {
            return a->area()[0].x() < b->area()[0].x();
        });
        // Group some platforms horizontally
        for(size_t i = 0; i < platform_sprites.size(); i++)
        {
            sad::Vector<sad::Sprite2D*> sprites_in_group;
            sprites_in_group << platform_sprites[i];
            sad::Rect2D common_rectangle = platform_sprites[i]->area();
            for (size_t j = i + 1; j < platform_sprites.size(); j++)
            {
                sad::Rect2D candidate_rectangle = platform_sprites[j]->area();
                double common_ymin = std::min(common_rectangle.p0().y(), common_rectangle.p2().y());
                double common_ymax = std::max(common_rectangle.p0().y(), common_rectangle.p2().y());

                double candidate_ymin = std::min(candidate_rectangle.p0().y(), candidate_rectangle.p2().y());
                double candidate_ymax = std::max(candidate_rectangle.p0().y(), candidate_rectangle.p2().y());

                double common_xmin = std::min(common_rectangle.p0().x(), common_rectangle.p2().x());
                double common_xmax = std::max(common_rectangle.p0().x(), common_rectangle.p2().x());

                double candidate_xmin = std::min(candidate_rectangle.p0().x(), candidate_rectangle.p2().x());
                double candidate_xmax = std::max(candidate_rectangle.p0().x(), candidate_rectangle.p2().x());

                if (sad::is_fuzzy_equal(common_ymin, candidate_ymin, DESIGNER_PRECISION_ERROR) 
                    && sad::is_fuzzy_equal(common_ymax, candidate_ymax, DESIGNER_PRECISION_ERROR)
                    && sad::is_fuzzy_equal(common_xmax, candidate_xmin, DESIGNER_PRECISION_ERROR))
                {
                    sprites_in_group << platform_sprites[j];
                    common_rectangle = sad::Rect2D(common_xmin, common_ymin, candidate_xmax, common_ymax);
                }
            }
            if (sprites_in_group.size() > 1)
            {
                sad::p2d::Body* body = new sad::p2d::Body();
                body->setCurrentAngularVelocity(0);
                body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
                sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
                // Slight increase of area, due to paddings
                common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y(), common_rectangle[2].x() + 1, common_rectangle[2].y());
                // Small fix for tiny platforms, to ensure their bounding box will be some times smaller than one
                if (common_rectangle.height() < 40) {
                    common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y()  + 3, common_rectangle[2].x() - 1, common_rectangle[2].y());
                }
                rect->setRect(common_rectangle);
                body->setShape(rect);
                body->attachObjects(sprites_in_group);
                body->initPosition((common_rectangle[0] + common_rectangle[2]) / 2.0);

                m_physics_world->addBodyToGroup("platforms", body);
                for(size_t k = 0 ; k < sprites_in_group.size(); k++)
                {
                    platform_sprites.removeFirst(sprites_in_group[k]);
                }
                // Decrement, so we can iterate through other platforms
                --i;
            }
        }
        // We don't group platforms vertically except it's columns, because it could be solved by bounce solver easily
        std::sort(platform_sprites.begin(), platform_sprites.end(), [](sad::Sprite2D* a, sad::Sprite2D* b) {
            return a->area()[0].y() < b->area()[0].y();
        });
        // Group some platforms vertically
        for (size_t i = 0; i < platform_sprites.size(); i++)
        {
            sad::Vector<sad::Sprite2D*> sprites_in_group;
            sprites_in_group << platform_sprites[i];
            sad::Rect2D common_rectangle = platform_sprites[i]->area();
            for (size_t j = i + 1; j < platform_sprites.size(); j++)
            {
                sad::Rect2D candidate_rectangle = platform_sprites[j]->area();
                double common_ymin = std::min(common_rectangle.p0().y(), common_rectangle.p2().y());
                double common_ymax = std::max(common_rectangle.p0().y(), common_rectangle.p2().y());

                double candidate_ymin = std::min(candidate_rectangle.p0().y(), candidate_rectangle.p2().y());
                double candidate_ymax = std::max(candidate_rectangle.p0().y(), candidate_rectangle.p2().y());

                double common_xmin = std::min(common_rectangle.p0().x(), common_rectangle.p2().x());
                double common_xmax = std::max(common_rectangle.p0().x(), common_rectangle.p2().x());

                double candidate_xmin = std::min(candidate_rectangle.p0().x(), candidate_rectangle.p2().x());
                double candidate_xmax = std::max(candidate_rectangle.p0().x(), candidate_rectangle.p2().x());

                if (sad::is_fuzzy_equal(common_xmin, candidate_xmin, DESIGNER_PRECISION_ERROR)
                    && sad::is_fuzzy_equal(common_xmax, candidate_xmax, DESIGNER_PRECISION_ERROR)
                    && sad::is_fuzzy_equal(common_ymax, candidate_ymin, DESIGNER_PRECISION_ERROR))
                {
                    sprites_in_group << platform_sprites[j];
                    common_rectangle = sad::Rect2D(common_xmin, common_ymin, common_xmax, candidate_ymax);
                }
            }
            if (sprites_in_group.size() > 1)
            {
                sad::p2d::Body* body = new sad::p2d::Body();
                body->setCurrentAngularVelocity(0);
                body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
                sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
                // Slight increase of area, due to paddings
                common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y(), common_rectangle[2].x() + 1, common_rectangle[2].y());
                rect->setRect(common_rectangle);
                body->setShape(rect);
                body->attachObjects(sprites_in_group);
                body->initPosition((common_rectangle[0] + common_rectangle[2]) / 2.0);


                m_physics_world->addBodyToGroup("platforms", body);
                for (size_t k = 0; k < sprites_in_group.size(); k++)
                {
                    platform_sprites.removeFirst(sprites_in_group[k]);
                }
                // Decrement, so we can iterate through other platforms
                --i;
            }
        }
        // Add remaining platforms to world
        for (size_t i = 0; i < platform_sprites.size(); i++)
        {
            sad::p2d::Body* body = new sad::p2d::Body();
            body->setCurrentAngularVelocity(0);
            body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
            body->attachObject(platform_sprites[i]);
            sad::Rect2D common_rectangle = platform_sprites[i]->area();
            sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
            // Slight increase of area, due to paddings
            common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y(), common_rectangle[2].x() + 1, common_rectangle[2].y());
            // Small fix for tiny platforms, to ensure their bounding box will be some times smaller than one
            if (common_rectangle.height() < 40) {
                common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y() + 3, common_rectangle[2].x() - 1, common_rectangle[2].y());
            }
            rect->setRect(common_rectangle);
            body->setShape(rect);
            body->initPosition(platform_sprites[i]->middle());

            m_physics_world->addBodyToGroup("platforms", body);
        }
    }
    m_walls.init(0, m_max_level_x, 600, 0);
    m_walls.addToWorld(m_physics_world);

    std::function<void()> player_die_callback = [=]() {
        // TODO: Copy score to highscore

        this->m_player->toggleIsDead(true);
        sad::Sprite2D* local_sprite = this->m_player->actor()->sprite();
        this->playSound("lose");
        this->m_physics_world->removeBody(this->m_player->actor()->body());
        this->rendererForMainThread()->animations()->stopProcessesRelatedToObject(local_sprite);
        sad::animations::Instance* instance =  this->spawnDeathAnimationForActorsSprite(local_sprite);
        instance->end([=]() {  this->changeSceneToLoseScreen(); });
    };

    // Handle all collision as non-resilient, enabling sliding
    std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_player_and_platforms = [=](const sad::p2d::BasicCollisionEvent & ev) {
        if (!this->m_player->isFloater() && ev.m_object_2 == this->m_walls.bottomWall())
        {
            player_die_callback();
            return;
        }
        this->m_player->onPlatformCollision(ev);
    };
    std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_enemy_and_platforms = [=](const sad::p2d::BasicCollisionEvent & ev) {
        game::Actor* a = dynamic_cast<game::Actor*>(ev.m_object_1->userObject());
        if (!a->isFloater() && ev.m_object_2 == this->m_walls.bottomWall())
        {
            sad::Sprite2D* local_sprite = a->sprite();
            this->killActorWithoutSprite(a);
            this->spawnDeathAnimationForActorsSprite(local_sprite);
            return;
        }
        if (a)
        {
            a->onPlatformCollision(ev);
        }
    };
    m_physics_world->addHandler("player", "platforms", collision_between_player_and_platforms);
    m_physics_world->addHandler("enemies", "platforms", collision_between_enemy_and_platforms);
    m_physics_world->addHandler("player", "walls", collision_between_player_and_platforms);
    m_physics_world->addHandler("enemies", "walls", collision_between_enemy_and_platforms);
    std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_walls_and_bullet = [=](const sad::p2d::BasicCollisionEvent & ev) {
        sad::Object* a = ev.m_object_2->userObject();
        // This conditions only holds only in case of bullets,
        if (a->metaData()->name() == weapons::Bullet::globalMetaData()->name())
        {
            weapons::Bullet* b = static_cast<weapons::Bullet*>(a);
            sad::Sprite2D* local_sprite = b->sprite();
            this->rendererForMainThread()->pipeline()->appendTask([=]() {
                local_sprite->scene()->removeNode(local_sprite);

                this->m_physics_world->removeBody(ev.m_object_2);
            });
        }
    };
    m_physics_world->addHandler("walls", "player_bullets", collision_between_walls_and_bullet);
    m_physics_world->addHandler("walls", "enemy_bullets", collision_between_walls_and_bullet);
    std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_platform_and_bullet = [=](const sad::p2d::BasicCollisionEvent & ev) {
        this->tryDecayBullet(ev.m_object_2, "hit");
    };

    m_physics_world->addHandler("platforms", "player_bullets", collision_between_platform_and_bullet);
    m_physics_world->addHandler("platforms", "enemy_bullets", collision_between_platform_and_bullet);



    std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_enemy_and_bullet = [=](const sad::p2d::BasicCollisionEvent & ev) {
        game::Actor* actor = static_cast<game::Actor*>(ev.m_object_1->userObject());
        weapons::Projectile* projectile = static_cast<weapons::Projectile*>(ev.m_object_2->userObject());
        if (!actor->isInvincible()) this->tryDecayBullet(ev.m_object_2, "hit_enemy");
        actor->tryDecrementLives(projectile->damage());
    };

    std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_player_and_bullet = [=](const sad::p2d::BasicCollisionEvent & ev) {
        weapons::Projectile* projectile = static_cast<weapons::Projectile*>(ev.m_object_2->userObject());
        int dmg = projectile->damage();
        if (!this->player()->isInvincible()) this->tryDecayBullet(ev.m_object_2, "hurt");
        this->player()->tryDecrementLives(dmg);
    };

    std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_player_and_enemies = [=](const sad::p2d::BasicCollisionEvent & ev) {
        if (!this->player()->isInvincible()) this->playSound("hurt");
        this->player()->tryDecrementLives(1);
    };


    m_physics_world->addHandler("enemies", "player_bullets", collision_between_enemy_and_bullet);
    m_physics_world->addHandler("player", "enemy_bullets", collision_between_player_and_bullet);
    m_physics_world->addHandler("enemies", "player", collision_between_player_and_enemies);

    m_step_task->setWorld(m_physics_world);

    // Make player die, when killed by bullet
    m_player->onDeath([=](game::Actor*) {
        player_die_callback();
    });
}



void Game::tryRenderDebugShapes() const
{
    if (!m_is_rendering_world_bodies)
    {
        return;
    }
    sad::Renderer* renderer = m_main_thread->renderer();
    sad::Vector<sad::p2d::Body*> bodies = m_physics_world->allBodies();
    if (!bodies.empty())
    {
        for (size_t i = 0; i < bodies.size(); i++)
        {
            sad::p2d::CollisionShape* shape = bodies[i]->currentShape();
            if (shape->metaIndex() == sad::p2d::Rectangle::globalMetaIndex())
            {
                renderer->render()->rectangle(dynamic_cast<sad::p2d::Rectangle*>(shape)->rect(), sad::AColor(0, 0, 255, 255));
            }
            if (shape->metaIndex() == sad::p2d::Circle::globalMetaIndex())
            {
                sad::Vector<sad::p2d::Point> list_of_points;
                shape->populatePoints(list_of_points);
                for (size_t j = 0; j < (list_of_points.size() - 1); j++)
                {
                    renderer->render()->line(list_of_points[j], list_of_points[j + 1], sad::AColor(0, 0, 255, 255));
                }
            }
            if (shape->metaIndex() == sad::p2d::Line::globalMetaIndex())
            {
                sad::p2d::Line* line = dynamic_cast<sad::p2d::Line*>(shape);
                renderer->render()->line(line->cutter().p1(), line->cutter().p2(), sad::AColor(0, 0, 255, 255));
            }
        }
    }
}

game::Actor* Game::makeEnemy(const sad::String& optname, const sad::Point2D& middle)
{
    if (m_actor_options.contains(optname))
    {
        game::ActorOptions* options = m_actor_options[optname];

        sad::Scene* scene = this->rendererForMainThread()->database("gamescreen")->objectByName<sad::Scene>("main");
        if (scene)
        {
            sad::Sprite2D* sprite = new sad::Sprite2D();
            sprite->setScene(scene);
            sprite->setTreeName(this->rendererForMainThread(), "");
            sprite->set(options->JumpingSprite);
            sprite->moveTo(middle);
            scene->add(sprite);

            game::Actor* actor = new game::Actor();
            actor->setLives(Game::BasicEnemyLivesCount);
            actor->setGame(this);
            actor->setSprite(sprite);
            actor->setOptions(options);

            sad::p2d::Body* body = new sad::p2d::Body();
            body->setCurrentAngularVelocity(0);
            body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
            sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
            rect->setRect(sprite->area());
            body->setShape(rect);

            body->attachObject(actor);
            body->initPosition(sprite->middle());

            this->m_physics_world->addBodyToGroup("enemies", body);
            actor->setBody(body);
            actor->enableGravity();
            actor->init(true);
            actor->setHurtAnimation(m_hit_animation_for_enemies);
            actor->onDeath([=](game::Actor* me) {
                sad::Sprite2D* local_sprite = me->sprite();
                this->killActorWithoutSprite(me);
                this->spawnDeathAnimationForActorsSprite(local_sprite);
            });

            return actor;
        }
    }
    return NULL;
}

void Game::updateProjectiles() const
{
    if (!m_projectiles.empty()) 
    {
        sad::Vector<weapons::Projectile*> projectiles = m_projectiles;
        for (size_t i = 0; i < projectiles.size(); i++)
        {
            projectiles[i]->update();
        }
    }
}

void Game::tryLoadIdenticalScreenDatabase(bool* loaded, const sad::String& db_name, const sad::String& file_name, bool is_inventory_thread) const
{
    int index = (is_inventory_thread) ? 1 : 0;
    sad::Renderer* renderer = (is_inventory_thread) ? (m_inventory_thread->renderer()) : (m_main_thread->renderer());
    if (loaded[index])
    {
        renderer->database(db_name)->restoreSnapshot();
    }
    else
    {
        sad::db::Database* database = new sad::db::Database();
        database->setRenderer(renderer);
        database->tryLoadFrom(file_name);
        database->saveSnapshot();
        renderer->addDatabase(db_name, database);
        loaded[index] = true;
    }
}

Game::Game(const Game&)  // NOLINT
: m_main_thread(NULL),
m_inventory_thread(NULL),
m_is_quitting(false),
m_main_menu_state(Game::GMMS_PLAY),
m_highscore(0),
m_loaded_options_database{false, false},
m_loaded_lose_screen_database{ false, false },
m_loaded_win_screen_database{false, false},
m_loaded_game_screen(false),
m_theme_playing(NULL),
m_transition_process(NULL),
m_inventory_node(NULL),
m_inventory_popup(NULL),
m_player(NULL),
m_eval_context(NULL),
m_physics_world(NULL),
m_step_task(NULL),
m_bounce_solver(NULL),
m_is_rendering_world_bodies(false),
m_max_level_x(0),
m_hit_animation_for_enemies(NULL),
m_hit_animation_for_players(NULL)
{
    throw std::logic_error("Not implemented");
}

Game& Game::operator=(const Game&)
{
    throw std::logic_error("Not implemented");
    // ReSharper disable once CppUnreachableCode
    return *this;
}
