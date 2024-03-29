#include "game.h"

#include "game/healthbar.h"
#include "game/scorebar.h"
#include "game/getpenetrationdepthforitem.h"
#include "game/snowparticles.h"
#include "game/platformblinking.h"
#include "game/staticobjectcontainer.h"
#include "game/levelstorageloader.h"
#include "game/mainlevelloader.h"

#include "threads/gamethread.h"

#include "nodes/background.h"
#include "nodes/inventorynode.h"
#include "nodes/inventorypopup.h"

#include <input/controls.h>

#include <pipeline/pipeline.h>
#include <pipeline/pipelinedelayedtask.h>

// ReSharper disable once CppUnusedIncludeDirective
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
#include <camera.h>
#include <window.h>

#include <functional>

#include <dukpp-03-irrklang/dukpp-03-irrklang.h>

#include <p2d/force.h>

#include "weapons/laser.h"

#include "bots/randombot.h"

#include "bots/shootingstrategies/shootingstrategy.h"
#include "bots/shootingstrategies/fixedanglestrategy.h"
#include "bots/shootingstrategies/playerlocatingstrategy.h"
#include "bots/shootingstrategies/randomstrategy.h"
#include "bots/shootingstrategies/turningstrategy.h"


#include "scripting/exposespawnenemy.h"

#include "initphysics.h"
#include "weapons/weapon.h"
#include "bots/nullbot.h"

#include "cameramovement.h"
#include "enemycounter.h"
#include "sounds.h"
#include "cameralockanimation.h"

const sad::Point2D Game::GravityForceValue(0.0, -300.0); // -300 is arbitrarily defined, to make player fall slowly

const int Game::BasicEnemyLivesCount = 3; // Amount of lives for enemy

const int Game::BasicPlayerLivesCount = 1; // Amount of lives for player


DECLARE_COMMON_TYPE(Game);

// ==================================== PUBLIC METHODS ====================================

Game::Game()  : m_is_quitting(false),  // NOLINT(cppcoreguidelines-pro-type-member-init)
m_main_menu_state(Game::MainMenuState::GMMS_PLAY),
m_pause_menu_state(Game::PauseMenuState::GPMS_RESUME),
m_score(0),
m_highscore(0),
m_loaded_options_database{false, false},
m_loaded_lose_screen_database{false, false},
m_loaded_win_screen_database{false, false},
m_main_level_loader(nullptr),
m_theme_playing(nullptr),
m_sounds(nullptr),
m_inventory_node(nullptr),
m_inventory_popup(nullptr),
m_eval_context(nullptr),
m_physics_world(nullptr),
m_is_rendering_world_bodies(false),
m_max_level_x(0.0),
m_hit_animation_for_enemies(nullptr),
m_hit_animation_for_players(nullptr),
m_wind_speed(0),
m_snow_particles(nullptr),
m_camera_movement(nullptr),
m_winning(false),
m_level_storage_loader(nullptr)// NOLINT
{
    m_main_thread = new threads::GameThread();
    m_inventory_thread = new threads::GameThread();

    m_sounds = new Sounds(m_inventory_thread->renderer());

    m_snow_particles = new game::SnowParticles();
    m_snow_particles->addRef();

    this->initContext();
    this->runGameInitializationScript();

    m_player = new game::Player();
    m_player->setGame(this);
    m_player->setLives(Game::BasicPlayerLivesCount);

    m_main_menu_states_to_labels.insert(static_cast<size_t>(Game::MainMenuState::GMMS_PLAY)   , "Play");
    m_main_menu_states_to_labels.insert(static_cast<size_t>(Game::MainMenuState::GMMS_OPTIONS), "Options");
    m_main_menu_states_to_labels.insert(static_cast<size_t>(Game::MainMenuState::GMMS_EXIT)   , "Exit");


    m_transition_process = new SceneTransitionProcess(this);

    m_loaded_options_database[0] = false;
    m_loaded_options_database[1] = false;

    m_options_screen.init(this, m_main_thread->renderer(), m_inventory_thread->renderer());
    
    m_step_task = new sad::p2d::WorldStepTask(nullptr, m_main_thread->renderer());
    
    m_bounce_solver = new sad::p2d::BounceSolver();
    m_bounce_solver->toggleIgnoreContactPoints(true);
    m_bounce_solver->toggleInelasticCollisions(true);
    m_bounce_solver->setInelasticCollisionType(sad::p2d::BounceSolver::InelasticCollisionType::ICT_FIRST);

    m_bounce_solver_for_bullets = new sad::p2d::BounceSolver();

    m_bot_registry.insert("random_60_500", new bots::RandomBot(50, 600));
    m_bot_registry.insert("null", new bots::NullBot());

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

    m_score_bar = new game::ScoreBar(this);

    m_camera_movement = new CameraMovement(this);
    m_camera_movement->addRef();

    m_enemy_counter = new EnemyCounter();
    m_enemy_counter->addRef();

    m_main_level_loader = new game::MainLevelLoader(m_main_thread->renderer());
}

Game::~Game()  // NOLINT
{
    m_snow_particles->delRef();
    m_camera_movement->delRef();

    delete m_score_bar;
    m_hit_animation_for_players->delRef();
    m_hit_animation_for_enemies->delRef();

    delete m_bounce_solver;
    delete m_bounce_solver_for_bullets;

    delete m_transition_process;
    delete m_main_thread;
    delete m_inventory_thread;

    delete m_eval_context;
    delete m_player;

    for (sad::Hash<sad::String, game::ActorOptions*>::iterator ao = m_actor_options.begin(); ao != m_actor_options.end(); ++ao)
    {
        ao.value()->delRef();
    }
    this->clearProjectiles(false);
    this->destroyWorld();
    delete m_step_task;

    clearItemDefinitions();
    m_enemy_counter->delRef();

    for(sad::Hash<sad::Sprite2D*, game::PlatformBlinking*>::iterator it = m_blinking_platforms.begin(); it != m_blinking_platforms.end(); ++it)
    {
        delete it.value();
    }
    delete m_main_level_loader;
}

/*! A padding, that will be used in main menu between label and player choice
 */
const double padding_between_label_and_player_choice = 15;

void Game::runMainGameThread()
{
    sad::Renderer& renderer= *(m_main_thread->renderer());
    m_options.load(&renderer);
    m_conditions.apply(m_options);

    const sad::Maybe<picojson::value> maybe_json = sad::slurpJson("highscore.json", &renderer);
    if (maybe_json.exists())
    {
        picojson::value val = maybe_json.value();
        if (val.is<double>())
        {
            setHighscore(static_cast<int>(val.get<double>()));
        }
    }
    // We wait for inventory thread later to get initialization result,
    // so initialize flag now
    m_inventory_thread->needsToBeNotifiedFromLater();
    SL_LOCAL_DEBUG("Initializing main thread", renderer);
    // Attempt to load resources
    m_main_thread->tryInitialize(
        "M",
        "main_thread.txt",
        "examples/multithreading/config_main_thread.json",
        "Platformer (MultiWindow)"
    );
    SL_LOCAL_DEBUG("Waiting for inventory thread", renderer);
    // Wait for inventory to load it's resources
    m_inventory_thread->waitForNotify();
    SL_LOCAL_DEBUG("Checking status", renderer);
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
        m_main_thread->renderer()->error(error, "Initialization error");

        // Kill inventory thread
        m_inventory_thread->sendKillSignalFrom(m_main_thread);
        // Exit here
        return;
    }
    else
    {
        m_inventory_thread->sendResumeSignalFrom(m_main_thread);
    }

    auto* db = new sad::db::Database();
    db->setRenderer(&renderer);
    const bool result = db->loadFromFile("examples/multithreading/titlescreen.json", &renderer);
    if (result)
    {
        renderer.addDatabase("titlescreen", db);
    }
    else
    {
        SL_LOCAL_FATAL("Unable to load title screen", renderer);
    }

    sad::db::populateScenesFromDatabase(&renderer, db);

    tryStartStartingState();

    setControlsForMainThread(&renderer, db);
    setControlsForInventoryThread(m_inventory_thread->renderer());

    renderer.controls()->addLambda(
        *sad::input::EventType::ET_KeyPress & sad::KeyboardKey::Esc,
        [this]() -> void {
            this->quitGame();
        }
    );

    SL_LOCAL_DEBUG("Starting", renderer);
    renderer.controls()->addLambda(
        *sad::input::EventType::ET_Quit,
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
    renderer.pipeline()->appendTask([this] {
        this->m_main_level_loader->runLoaderThread();
        this->rendererForMainThread()->window()->setRect(sad::Rect2I(0, 0, 800, 600));
    });
    m_main_thread->markAsRendererStarted();
    renderer.run();
}

void Game::runInventoryThread()
{
    sad::Renderer& renderer= *(m_inventory_thread->renderer());

    // We wait for main thread later to get initialization result,
    // so initialize flag now
    m_main_thread->needsToBeNotifiedFromLater();
    // Attempt to load resources
    SL_LOCAL_DEBUG("Initializing inventory thread", renderer);
    m_inventory_thread->tryInitialize(
        "I",
        "inventory_thread.txt",
        "examples/multithreading/config_inventory_thread.json",
        "Inventory"
    );
    m_inventory_thread->notify();
    SL_LOCAL_DEBUG("Waiting for main thread", renderer);
    // Wait for signal (kill or resume from main thread)
    m_main_thread->waitForNotify();
    if (m_inventory_thread->isKilled())
    {
        SL_LOCAL_DEBUG("Killed\n", renderer);
        return;
    }

    this->initStartScreenForInventoryThread();

    SL_LOCAL_DEBUG("Starting new renderer", renderer);
    m_inventory_thread->markAsRendererStarted();
    // Kill other window, if closed
    renderer.controls()->addLambda(
        *sad::input::EventType::ET_Quit,
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
                if (this->m_inventory_popup)
                {
                    this->m_inventory_popup->render();
                }
            }
        }
        this->m_task_lock.release();
    });
    renderer.pipeline()->appendTask([this] {
        this->rendererForInventoryThread()->window()->setRect(sad::Rect2I(800, 0, 1600, 600));
    });
    renderer.run();
}

void Game::putPlayerPickAccordingToMenuState(Game::MainMenuState state)
{
    sad::Renderer* renderer = m_main_thread->renderer();
    sad::db::Database* db = renderer->database("titlescreen");
    auto* choice_pointer = db->objectByName<sad::Sprite2D>("PlayerPick");
    auto* new_game_label = db->objectByName<sad::Label>(this->m_main_menu_states_to_labels[static_cast<size_t>(state)]);
    const double x = new_game_label->area().p0().x() - padding_between_label_and_player_choice - (choice_pointer->area().width() / 2.0);
    const double y = (new_game_label->area().p0().y() - new_game_label->area().height() / 2.0);
    choice_pointer->setMiddle(sad::Point2D(x, y));
}

void Game::removeItemFromPlayersInventoryWithWeapon(weapons::Weapon* weapon) const
{
    this->m_player->inventory()->removeItemWithWeapon(weapon);
    if (this->m_inventory_popup)
    {
        this->m_inventory_popup->setVisible(false);
    }
}

void Game::setControlsForMainThread(sad::Renderer* renderer, sad::db::Database*)
{
    // Set pointer for the main menu options
    this->m_main_menu_state = MainMenuState::GMMS_PLAY;
    this->putPlayerPickAccordingToMenuState(this->m_main_menu_state);

    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[static_cast<size_t>(game::Conditions::State::CS_START_SCREEN)]
        & ((&m_state_machine) * sad::String("starting_screen"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() -> void {
        if (this->m_main_menu_state == Game::MainMenuState::GMMS_PLAY)
        {
            this->m_main_menu_state = Game::MainMenuState::GMMS_EXIT;
        }
        else
        {
            this->m_main_menu_state = static_cast<Game::MainMenuState>(static_cast<int>(this->m_main_menu_state) - 1);
        }
        this->playSound("misc_menu");
        this->putPlayerPickAccordingToMenuState(this->m_main_menu_state);
    });

    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[static_cast<size_t>(game::Conditions::State::CS_START_SCREEN)]
        & ((&m_state_machine) * sad::String("starting_screen"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() -> void {
        if (this->m_main_menu_state == Game::MainMenuState::GMMS_EXIT)
        {
            this->m_main_menu_state = Game::MainMenuState::GMMS_PLAY;
        }
        else
        {
            this->m_main_menu_state = static_cast<Game::MainMenuState>(static_cast<int>(this->m_main_menu_state) + 1);  // NOLINT(misc-misplaced-widening-cast, bugprone-misplaced-widening-cast)
        }
        this->playSound("misc_menu");
        this->putPlayerPickAccordingToMenuState(this->m_main_menu_state);
    });

    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.JumpActionConditions[static_cast<size_t>(game::Conditions::State::CS_START_SCREEN)]
        & ((&m_state_machine) * sad::String("starting_screen"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() -> void {
        this->playSound("misc_menu_2");
        switch (this->m_main_menu_state)
        {
            case Game::MainMenuState::GMMS_PLAY: this->changeSceneToPlayingScreen(); break;
            case Game::MainMenuState::GMMS_OPTIONS: this->changeSceneToOptions(); break;
            case Game::MainMenuState::GMMS_EXIT: this->quitGame(); break;
        }
    });

    std::function<void()> empty_callback = [](){};

    // An options
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.LeftKeyConditions[static_cast<size_t>(game::Conditions::State::CS_OPTIONS_SCREEN)]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToPreviousItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.RightKeyConditions[static_cast<size_t>(game::Conditions::State::CS_OPTIONS_SCREEN)]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToNextItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[static_cast<size_t>(game::Conditions::State::CS_OPTIONS_SCREEN)]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToPreviousItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[static_cast<size_t>(game::Conditions::State::CS_OPTIONS_SCREEN)]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToNextItem(); }
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.JumpActionConditions[static_cast<size_t>(game::Conditions::State::CS_OPTIONS_SCREEN)]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        &(this->optionsScreen()),
        &OptionsScreen::tryStartEditing
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        &(this->optionsScreen()),
        &OptionsScreen::tryHandleEditing
    );

    // A playing game screen
    const std::function<void()> stop_going_into_any_direction = [=]() {
        // Stop going into any direction
        this->player()->tryStopGoingLeft();
        this->player()->tryStopGoingRight();
    };
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_Deactivate
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        stop_going_into_any_direction
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.LeftKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING_PRESSED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStartGoingLeft
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.LeftKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStopGoingLeft
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.RightKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING_PRESSED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStartGoingRight
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.RightKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStopGoingRight
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING_PRESSED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStartGoingUp
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStopGoingUp
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING_PRESSED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStartGoingDown
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this->m_player,
        &game::Player::tryStopGoingDown
    );

    std::function<void(const sad::Point2D& p)> set_lookup_angle = [this](const sad::Point2D& pnt) {
        double angle = 0;
        const sad::Point2D middle = this->m_player->middle();
        const double dy = pnt.y() - middle.y();
        const double dx = pnt.x() - middle.x();
        if (!sad::is_fuzzy_zero(dx))
        {
            angle = atan2(dy, dx);
        }
        this->m_player->actor()->setLookupAngle(angle);
    };

    const std::function<void(const sad::input::MouseMoveEvent&)> mouse_move_event = [=](const sad::input::MouseMoveEvent& ev) {
        set_lookup_angle(ev.pos2D());
    };

    const std::function<void()> perform_action = [this, set_lookup_angle]() {
        // this->m_player->setFloaterState(!(this->m_player->isFloater()));
        const sad::MaybePoint3D pnt = this->rendererForMainThread()->cursorPosition();
        if (pnt.exists())
        {
           set_lookup_angle(pnt.value());
        }
        m_player->tryShoot();
    };

    // This handler is reserved for future purposes
    const std::function<void()> right_click_handler = [=] {};

    renderer->controls()->addLambda(
        *sad::input::EventType::ET_MouseMove
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        mouse_move_event
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_MousePress
        & sad::MouseButton::MouseLeft
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        perform_action
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_MousePress
        & sad::MouseButton::MouseRight
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        right_click_handler
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.JumpActionConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        perform_action
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.PauseCondition
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this, &Game::tryEnterPause
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & sad::KeyboardKey::Tab
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this] { this->m_is_rendering_world_bodies = !this->m_is_rendering_world_bodies;  }
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & sad::KeyboardKey::Backspace
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->changeSceneToStartingScreen();  }
    );
    // Kill all bodies, simple cheat code :)
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & sad::KeyboardKey::End
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() {
        sad::Vector<sad::p2d::Body*> bodies = this->m_physics_world->allBodiesInGroup("enemies");
        for(size_t i = 0; i < bodies.size(); i++)
        {
            this->killActorByBody(bodies[i]);
        }
    });

    // A paused game screen
    const std::function<void()> pause_menu_next_option = [=]() {
        this->m_pause_menu_state = (this->m_pause_menu_state == Game::PauseMenuState::GPMS_RESUME) ? Game::PauseMenuState::GPMS_EXIT : Game::PauseMenuState::GPMS_RESUME;
        this->showCurrentPauseMenuOption();
    };
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        pause_menu_next_option
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        pause_menu_next_option
    );
    // On pressed state
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.JumpActionConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        [=]() {
        if (this->m_pause_menu_state == Game::PauseMenuState::GPMS_RESUME)
        {
            this->tryExitPause([=]() {
                this->playSound("misc_menu_2");
            });
        }
        if (this->m_pause_menu_state == Game::PauseMenuState::GPMS_EXIT)
        {
            this->tryExitPause([=]() {
                this->changeSceneToStartingScreen();
                this->playSound("misc_menu_2");
            });
        }
    });
    void (Game::*try_exit_pause)() = &Game::tryExitPause;
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.PauseConditionWhenPaused
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        this, try_exit_pause
    );
    // A paused key release handlers
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.LeftKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        [=]() { this->player()->tryStopGoingLeft(); }
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.RightKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        [=]() { this->player()->tryStopGoingRight(); }
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        [=]() { this->player()->tryStopGoingUp(); }
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        [=]() { this->player()->tryStopGoingDown(); }
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
                this->m_actors.process(this);
                this->m_camera_movement->process();
                this->m_snow_particles->process();
                this->m_step_task->enable();
                this->m_step_task->process();
                if (m_level_storage_loader)
                {
                    m_level_storage_loader->tryLoadRelevantRoom(this->m_player->area());
                }
                this->tryRenderDebugShapes();
                this->m_triggers.tryRun(this->m_player, this->m_eval_context);
                this->m_unanimated_coins.animateNearestCoins(this->m_player->middle());
                this->m_delayed_tasks.tryExecute();
                this->m_score_bar->update();

                this->m_task_lock.release();
            }
        }
    });
}

void Game::setControlsForInventoryThread(sad::Renderer* renderer)
{
	const std::function<void()> empty_callback = []() {};

    // An options
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.LeftKeyConditions[static_cast<size_t>(game::Conditions::State::CS_OPTIONS_SCREEN)]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToPreviousItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.RightKeyConditions[static_cast<size_t>(game::Conditions::State::CS_OPTIONS_SCREEN)]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToNextItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.UpKeyConditions[static_cast<size_t>(game::Conditions::State::CS_OPTIONS_SCREEN)]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToPreviousItem(); }
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.DownKeyConditions[static_cast<size_t>(game::Conditions::State::CS_OPTIONS_SCREEN)]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() { this->optionsScreen().moveToNextItem(); }
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.JumpActionConditions[static_cast<size_t>(game::Conditions::State::CS_OPTIONS_SCREEN)]
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        &(this->optionsScreen()),
        &OptionsScreen::tryStartEditing
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & ((&m_state_machine) * sad::String("options"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        &(this->optionsScreen()),
        &OptionsScreen::tryHandleEditing
    );

    // A playing game screen
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.LeftKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.RightKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.UpKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.DownKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PLAYING)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        empty_callback
    );
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.PauseCondition
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        this, &Game::tryEnterPause
    );

    // A paused game screen
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.UpKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.DownKeyConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        empty_callback
    );
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.JumpActionConditions[static_cast<size_t>(game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED)]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        empty_callback
    );
    void (Game::*try_exit_pause)() = &Game::tryExitPause;
    renderer->controls()->add(
        *sad::input::EventType::ET_KeyPress
        & m_conditions.ConditionsForInventoryRenderer.PauseConditionWhenPaused
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("paused")),
        this, try_exit_pause
    );

    const std::function<void(const sad::input::MousePressEvent& e)> left_button_click = [=](const sad::input::MousePressEvent& e) {
        sad::Maybe<sad::Triplet<int, int, game::Item*> > result_data = this->m_player->inventory()->getItemWhichIsUnderCursor(e.pos2D());
        if (result_data.exists())
        {
            game::Item* item  = result_data.value().p3();
            this->playSound("apply_item");
            item->applyActiveEffect(this->m_player->inventory()->owner());
            if (item->shouldDeleteWhenApplied())
            {
                this->rendererForMainThread()->pipeline()->appendTask([=]() {
                    this->m_player->inventory()->takeItem(result_data.value().p1(), result_data.value().p2())->delRef();
                    if (this->m_inventory_popup)
                    {
                        this->m_inventory_popup->setVisible(false);
                    }
                });
            }
        }
    };
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_MousePress
        & sad::MouseButton::MouseLeft
        & ((&m_state_machine) * sad::String("playing")),
        left_button_click
    );

   const std::function<void(const sad::input::MousePressEvent& e)> right_button_click = [=](const sad::input::MousePressEvent& e) {
        this->m_player->inventory()->tryStartDraggingItem(e.pos2D());
    };
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_MousePress
        & sad::MouseButton::MouseRight
        & ((&m_state_machine) * sad::String("playing")),
        right_button_click
    );

    const std::function<void(const sad::input::MouseReleaseEvent& e)> right_button_release = [=](const sad::input::MouseReleaseEvent& e) {
        this->m_player->inventory()->tryReleaseDraggedItem(e.pos2D());
    };
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_MouseRelease
        & sad::MouseButton::MouseRight
        & ((&m_state_machine) * sad::String("playing")),
        right_button_release
    );

    const std::function<void(const sad::input::MouseMoveEvent& e)> move_callback = [=](const sad::input::MouseMoveEvent& e) {
        this->tryShowInventoryPopup(e.pos2D());
        this->m_player->inventory()->tryMoveDraggedItem(e.pos2D());
    };
    renderer->controls()->addLambda(
        *sad::input::EventType::ET_MouseMove
        & ((&m_state_machine) * sad::String("playing")),
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

int Game::score() const
{
    return m_score;
}

void Game::setScore(int score)
{
    m_score = score;
    setHighscore(std::max(m_score, m_highscore));
}

void Game::incrementScore(int delta_score)
{
    m_score += delta_score;
    setHighscore(std::max(m_score, m_highscore));
}

void Game::decrementScore(int delta_score)
{
    m_score -= delta_score;
    setHighscore(std::max(m_score, m_highscore));
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
    auto* player_walk = db->objectByName<sad::animations::Instance>("player_walk");
    auto* player_walk2 = db->objectByName<sad::animations::Instance>("player_walk2");
    renderer->animations()->add(player_walk);
    renderer->animations()->add(player_walk2);

    sad::Scene* scene = renderer->scenes()[0];
    auto* background = new nodes::Background(true);
    scene->addNode(background);
    scene->setLayer(background, 0);

    auto* highscore = db->objectByName<sad::Label>("Highscore");
    highscore->setString(sad::String("HIGHSCORE   IS   ") + sad::String::number(m_highscore));
    const double middle = 400; // A middle position data
    const double width = highscore->area().width();
    // A hardcoded data from titlescreen database
    highscore->setArea(sad::Rect2D(middle - width / 2.0, 585, middle + width / 2.0, 549));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Game::initStartScreenForInventoryThread()
{
    sad::Renderer& renderer= *(m_inventory_thread->renderer());

    sad::Scene* scene = renderer.scenes()[0];
    auto* background = new nodes::Background(false);
    scene->addNode(background);
}

void Game::playTheme(const sad::String& theme)
{
    auto* theme_data = m_inventory_thread->renderer()->tree("")->get<sad::irrklang::Sound>(theme);
    m_theme_playing = m_theme.play2D(theme_data, m_options.MusicVolume);
}

void Game::setThemeVolume(double volume)
{
    m_options.MusicVolume = volume;
    m_theme_playing->setVolume(static_cast<float>(volume));
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
    m_winning = true;
    // We append task here to avoid dangers of dying inside physics loop
    this->rendererForMainThread()->pipeline()->appendTask([=] {
        this->m_player->toggleIsDead(true);
        sad::Sprite2D* local_sprite = this->m_player->actor()->sprite();
        this->playSound("win");
        if (this->m_physics_world)
        { 
            this->m_physics_world->removeBody(this->m_player->actor()->body());
        }
        this->rendererForMainThread()->animations()->stopProcessesRelatedToObject(local_sprite);

        const sad::Point2D middle = local_sprite->middle();

        auto* movement = new sad::animations::SimpleMovement();
        movement->setStartingPoint(middle);
        movement->setEndingPoint(sad::Point2D(middle.x() + this->rendererForMainThread()->settings().width(), middle.y()));
        movement->setTime(800);
        movement->setLooped(false);


        auto* instance = new sad::animations::Instance();
        instance->setObject(local_sprite);
        instance->setAnimation(movement);
        instance->end([=] { local_sprite->scene()->removeNode(local_sprite); this->changeSceneToWinScreen(); });

        this->rendererForMainThread()->animations()->add(instance);
    });
}

void Game::changeScene(const SceneTransitionOptions& opts, double mul_dark, double mul_light) const
{
    m_transition_process->setTimeMultiplier(mul_dark, mul_light);
    m_transition_process->start(opts);
}

void Game::changeSceneToStartingScreen()
{
    this->m_score_bar->deinit();

    m_sounds->stopWalkingSound();
    m_triggers.clear();
    m_actors.clear();
    m_delayed_tasks.clear();
    m_snow_particles->clearParticleList();
    this->clearProjectiles();

    m_is_rendering_world_bodies = false;
    this->destroyWorld();
    SceneTransitionOptions options;

    m_inventory_popup = nullptr;
    clearLevelStorageLoader();


    sad::Renderer* main_renderer = m_main_thread->renderer();
    sad::Renderer* inventory_renderer = m_inventory_thread->renderer();

    const Game::MainMenuState  state = m_main_menu_state;
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
    SL_LOCAL_DEBUG("Starting to change screen to starting", *(this->rendererForMainThread()));
    changeScene(options);
}

void Game::changeSceneToLoseScreen()
{
    this->m_score_bar->deinit();

    m_sounds->stopWalkingSound();
    m_triggers.clear();
    m_actors.clear();
    m_delayed_tasks.clear();
    m_snow_particles->clearParticleList();
    this->clearProjectiles();

    m_is_rendering_world_bodies = false;
    this->destroyWorld();
    SceneTransitionOptions options;

    m_inventory_popup = nullptr;
    clearLevelStorageLoader();


    sad::Renderer* main_renderer = m_main_thread->renderer();
    sad::Renderer* inventory_renderer = m_inventory_thread->renderer();

    options.mainThread().LoadFunction = [this]() { this->tryLoadLoseScreen(false); };
    options.inventoryThread().LoadFunction = [this]() { this->m_player->reset(); this->tryLoadLoseScreen(true); };

    const std::function<void(sad::Renderer*)> init_screen = [=](sad::Renderer* r) {
        sad::db::Database* db = r->database("lose_screen");
        auto* label = db->objectByName<sad::Label>("ScoreValue");
        if (label)
        {
            label->setString(sad::String::number(this->score()));
            label->setPoint(r->settings().width() / 2.0 - 8 - label->area().width() / 2.0, label->point().y());
        }
        sad::db::populateScenesFromDatabase(r, db);
    };

    options.mainThread().OnLoadedFunction = [=]() {
        init_screen(main_renderer);
    };

    options.inventoryThread().OnLoadedFunction = [=]() {
        init_screen(inventory_renderer);
    };

    options.mainThread().OnFinishedFunction = [this]() {
        this->playSound("lose_screen"); this->m_state_machine.enterState("lose_screen"); this->enterPlayingState();
        this->rendererForMainThread()->pipeline()->append(new sad::pipeline::DelayedTask([=] { this->changeSceneToStartingScreen();  }, 1500));
    };
    options.inventoryThread().OnFinishedFunction = [this]() {  this->enterPlayingState(); };

    this->enterTransitioningState();
    this->waitForPipelineTasks();
    SL_LOCAL_DEBUG("Starting to change screen to losing", *(this->rendererForMainThread()));
    changeScene(options);
}

void Game::changeSceneToWinScreen()
{
    this->m_score_bar->deinit();

    m_sounds->stopWalkingSound();
    m_triggers.clear();
    m_actors.clear();
    m_delayed_tasks.clear();
    m_snow_particles->clearParticleList();
    this->clearProjectiles();

    m_is_rendering_world_bodies = false;
    this->destroyWorld();
    SceneTransitionOptions options;

    m_inventory_popup = nullptr;
    clearLevelStorageLoader();


    sad::Renderer* main_renderer = m_main_thread->renderer();
    sad::Renderer* inventory_renderer = m_inventory_thread->renderer();

    options.mainThread().LoadFunction = [this]() { this->tryLoadWinScreen(false); };
    options.inventoryThread().LoadFunction = [this]() { this->m_player->reset(); this->tryLoadWinScreen(true); };
    const std::function<void(sad::Renderer*)> init_screen_for_renderer = [=](sad::Renderer* r) {
        sad::db::Database* db = r->database("win_screen");
        auto* label = db->objectByName<sad::Label>("ScoreValue");
        if (label)
        {
            label->setString(sad::String::number(this->score()));
            label->setPoint(r->settings().width() / 2.0 - label->area().width() / 2.0, label->point().y());
        }
        sad::db::populateScenesFromDatabase(r, db);
    };
    options.mainThread().OnLoadedFunction = [=]() {
        init_screen_for_renderer(main_renderer);
    };

    options.inventoryThread().OnLoadedFunction = [=]() {
        init_screen_for_renderer(inventory_renderer);
    };

    options.mainThread().OnFinishedFunction = [this]() {
        this->playSound("win_screen"); this->m_state_machine.enterState("win_screen"); this->enterPlayingState();
        this->rendererForMainThread()->pipeline()->append(new sad::pipeline::DelayedTask([=] { this->changeSceneToStartingScreen();  }, 2100));
        auto* group = this->rendererForMainThread()->database("win_screen")->objectByName<sad::animations::Group>("Group1");
        this->rendererForMainThread()->animations()->add(group);
    };
    options.inventoryThread().OnFinishedFunction = [this]() {
        auto* group = this->rendererForInventoryThread()->database("win_screen")->objectByName<sad::animations::Group>("Group1");
        this->rendererForInventoryThread()->animations()->add(group);

        this->enterPlayingState();
    };

    this->enterTransitioningState();
    this->waitForPipelineTasks();
    SL_LOCAL_DEBUG("Starting to change screen to winning", *(this->rendererForMainThread()));
    changeScene(options);
}

void Game::changeSceneToPlayingScreen()
{
    this->destroyWorld();
    this->setScore(0);
    m_sounds->stopWalkingSound();
    m_actors.clear();
    m_triggers.clear();
    m_moving_platform_registry.clear();
    m_delayed_tasks.clear();
    m_actor_on_rest_callbacks.clear();
    m_platforms_to_actors.clear();
    m_wind_speed = 0;
    m_winning = false;
    m_enemy_counter->reset();
    this->clearProjectiles();

    SceneTransitionOptions options;

    m_inventory_popup = nullptr;
    m_is_rendering_world_bodies = false;
    

    sad::Renderer* main_renderer = m_main_thread->renderer();
    sad::Renderer* inventory_renderer = m_inventory_thread->renderer();

    m_player->reset();
    m_player->inventory()->setOwner(m_player->actor());
    clearLevelStorageLoader();

    auto* timer = new sad::Timer();
    timer->start();

    options.mainThread().LoadFunction = [this]() {  this->tryLoadGameScreen(); };

    options.mainThread().OnLoadedFunction = [=]() {
        timer->stop();
        SL_LOCAL_DEBUG(str(fmt::Format("Loaded level in {0} ms") << timer->elapsed()), *main_renderer);
        timer->start();
        sad::db::Database* db = main_renderer->database("gamescreen");
        auto* pause_scene = db->objectByName<sad::Scene>("pause");
        if (pause_scene)
        {
            pause_scene->setActive(false);
        }
        this->m_moving_platform_registry.setDatabase(this, db);
        this->m_score_bar->init();
        this->m_camera_movement->init();
        timer->stop();
        SL_LOCAL_DEBUG(str(fmt::Format("Inititalization of primitive objects took {0} ms") << timer->elapsed()), *main_renderer);
        timer->start();
        sad::db::populateScenesFromDatabase(main_renderer, db);
        timer->stop();
        SL_LOCAL_DEBUG(str(fmt::Format("sad::db::populateScenesFromDatabase took {0} ms") << timer->elapsed()), *main_renderer);
        timer->start();
        auto* scene = db->objectByName<sad::Scene>("gui");
        auto* bar = new game::HealthBar(this);
        scene->addNode(bar);
        scene->setLayer(bar, 0);
        m_snow_particles->setGame(this);
        timer->stop();
        SL_LOCAL_DEBUG(str(fmt::Format("Post-populating init took {0} ms") << timer->elapsed()), *main_renderer);
        timer->start();
        this->initGamePhysics();
        timer->stop();
        SL_LOCAL_DEBUG(str(fmt::Format("initGamePhysics() took {0} ms") << timer->elapsed()), *main_renderer);
        timer->start();
        // When loaded we should evaluate initialization script
        game::clearItemPenetrationDepths();
        this->clearItemDefinitions();
        timer->stop();
        SL_LOCAL_DEBUG(str(fmt::Format("Clearing of item penetration depths and definitions took {0} ms") << timer->elapsed()), *main_renderer);
        timer->start();
        this->evaluateInitializationScript();
        timer->stop();
        SL_LOCAL_DEBUG(str(fmt::Format("evaluateInitializationScript() took {0} ms") << timer->elapsed()), *main_renderer);
        delete timer;
    };

    options.inventoryThread().OnLoadedFunction = [=]() {
        auto* scene = new sad::Scene();
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
        const sad::MaybePoint3D maybe_point_3d = inventory_renderer->cursorPosition();
        if (maybe_point_3d.exists())
        {
            const sad::Point2D pnt(maybe_point_3d.value().x(), maybe_point_3d.value().y());
            this->tryShowInventoryPopup(pnt);
        }
    };

    this->enterTransitioningState();
    this->waitForPipelineTasks();

    changeScene(options, 2.0);
}

void Game::changeSceneToOptions()
{
    m_sounds->stopWalkingSound();
    m_triggers.clear();
    m_actors.clear();
    this->clearProjectiles();


    m_is_rendering_world_bodies = false;
    this->destroyWorld();
    this->m_player->reset();


    m_inventory_popup = nullptr;
    clearLevelStorageLoader();


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

    std::function<void(void)> on_finished_function = [this]() {  this->enterOptionsState(); this->enterPlayingState(); };
    options.mainThread().OnFinishedFunction = on_finished_function;
    options.inventoryThread().OnFinishedFunction = on_finished_function;

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

void Game::tryLoadGameScreen() const
{
    this->m_main_level_loader->loadGameScreen();
}

OptionsScreen& Game::optionsScreen()
{
    return m_options_screen;
}

void Game::playSound(const sad::String& sound_name) const
{
    m_sounds->playSound(sound_name, m_options.SoundVolume);
}

void Game::playWalkingSound() const
{
    m_sounds->playWalkingSound(m_options.MusicVolume);
}

void Game::stopWalkingSound() const
{
    m_sounds->stopWalkingSound();
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
    const sad::Maybe<sad::Triplet<int, int, game::Item*> > result_data = m_player->inventory()->getItemWhichIsUnderCursor(p);
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
                sad::String title = item->title();
                if (item->givenWeapon() == m_player->weapon())
                {
                    title += " [ACTIVE]";
                }
                m_inventory_popup->setDescription(title, item->description());
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
    const sad::Maybe<sad::String> maybe_script = sad::slurp("examples/multithreading/init.js", m_main_thread->renderer());
    if (maybe_script.exists())
    {
        // ReSharper disable CppExpressionWithoutSideEffects
        this->evalScript(maybe_script.value());
    }
}

game::Item* Game::makeItem(const sad::String& icon)
{
    if (!m_item_definitions.contains(icon))
    {
        return nullptr;
    }
    auto* item = new game::Item(m_item_definitions[icon]);
    item->setGame(this);
    return item;
}

void Game::makeItemUnpickable(game::Actor* actor)
{
    this->rendererForMainThread()->pipeline()->appendTask([=] {
        sad::p2d::Body* b = actor->body();
        b->addRef();
        this->m_physics_world->addBodyToGroup("items_unpickable", b);
        this->m_physics_world->removeBodyFromGroup("items", b);
        b->delRef();
    });
}

void Game::enableGravity(sad::p2d::Body* b)
{
    Game::setGravityForBody(b, Game::GravityForceValue);
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
    auto* actor = static_cast<game::Actor*>(body->userObject());  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    this->killProjectilesRelatedToActor(actor);
    this->rendererForMainThread()->animations()->stopProcessesRelatedToObject(actor->sprite());
    m_physics_world->removeBody(body);
    sad::Scene* scene = actor->sprite()->scene();
    scene->removeNode(actor->sprite());
    m_actors.remove(actor);
    eraseActorFromRestingBodiesToActors(actor);
}

void Game::killActorWithoutSprite(game::Actor* actor)
{
    this->killProjectilesRelatedToActor(actor);
    this->rendererForMainThread()->animations()->stopProcessesRelatedToObject(actor->sprite());
    m_physics_world->removeBody(actor->body());
    m_actors.remove(actor);
    eraseActorFromRestingBodiesToActors(actor);
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
            return;
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

bool Game::isDead(game::Actor* actor) const
{
    if (m_player->actor() == actor)
    {
        return m_player->isDead();
    }
    return m_actors.isDead(actor);
}

void Game::setStrategyForBot(game::Actor* actor, bots::shootingstrategies::ShootingStrategy* s)
{
    m_actors.setStrategyForBot(actor, s);
}

void Game::addDelayedTask(double time, const std::function<void()>& fn)
{
    m_delayed_tasks.add(DelayedTask(time, fn));
}

game::Actor* Game::makeEnemy(const sad::String& options_name, const sad::Point2D& middle)
{
    if (m_actor_options.contains(options_name))
    {
        game::ActorOptions* options = m_actor_options[options_name];

        auto* scene = this->rendererForMainThread()->database("gamescreen")->objectByName<sad::Scene>("main");
        if (scene)
        {
            auto* sprite = new sad::Sprite2D();
            sprite->setScene(scene);
            sprite->setTreeName(this->rendererForMainThread(), "");
            sprite->set(options->JumpingSprite);
            sprite->moveTo(middle);
            scene->add(sprite);

            auto* actor = new game::Actor();
            actor->setLives(Game::BasicEnemyLivesCount);
            actor->setGame(this);
            actor->setSprite(sprite);
            actor->setOptions(options);

            auto* body = new sad::p2d::Body();
            body->setCurrentAngularVelocity(0);
            body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
            auto* rect = new sad::p2d::Rectangle();
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
                this->incrementScore(1); me->die();
            });

            return actor;
        }
    }
    return nullptr;
}

game::Actor* Game::makeItemActor(const sad::String& options_name, const sad::Point2D& middle)
{
    const sad::String real_opt_name = options_name + "______item_actor";
    if (!m_actor_options.contains(real_opt_name)) {
        game::ActorOptions opts;
        opts.IsFloater = false;
        opts.CanEmitSound = false;
        opts.WalkerHorizontalVelocity = 1;
        opts.WalkerVerticalVelocity = 1;
        opts.FloaterHorizontalVelocity = 1;
        opts.FloaterVerticalVelocity = 1;
        opts.WalkingAnimationTime = 1;
        opts.JumpingAnimationTime = 1;

        opts.StandingSprite = options_name;
        opts.WalkingSprite = options_name;
        opts.WalkingAnimationOptions.push_back(options_name);
        opts.JumpingAnimationOptions.push_back(options_name);
        opts.JumpingSprite = options_name;
        opts.FallingSprite = options_name;
        opts.DuckingSprite = options_name;
        opts.FloaterSprite = options_name;
        opts.FloaterFlyAnimationOptions.push_back(options_name);

        addActorOptions(real_opt_name, opts);
    }
    game::ActorOptions* options = m_actor_options[real_opt_name];

    sad::db::Database* gamescreen = this->rendererForMainThread()->database("gamescreen");
    auto* scene = gamescreen->objectByName<sad::Scene>("main");
    auto* animation = gamescreen->objectByName<sad::animations::Animation>("item_falling");
    if (scene)
    {
        auto* sprite = new sad::Sprite2D();
        sprite->setScene(scene);
        sprite->setTreeName(this->rendererForMainThread(), "");
        sprite->set(options->JumpingSprite);
        sprite->moveTo(middle);
        scene->add(sprite);

        auto* actor = new game::Actor();
        actor->setLives(Game::BasicEnemyLivesCount);
        actor->setGame(this);
        actor->setSprite(sprite);
        actor->setOptions(options);

        // Make sprite larger (x3)
        sprite->setChangeSizeWhenOptionsAreChanged(false);
        const sad::Point2D wh(game::Item::SpriteSize / 2.0, game::Item::SpriteSize / 2.0);
        const sad::Rect2D area(middle - wh, middle + wh);
        sprite->setArea(area);

        auto* body = new sad::p2d::Body();
        body->setCurrentAngularVelocity(0);
        body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0)); // Just to ensure falling properly
        auto* rect = new sad::p2d::Rectangle();
        rect->setRect(sprite->area());
        body->setShape(rect);

        body->attachObject(actor);
        body->initPosition(sprite->middle());

        this->m_physics_world->addBodyToGroup("items", body);
        actor->setBody(body);
        actor->enableGravity();
        actor->toggleFixedSize(true);
        actor->init(true);
        actor->setHurtAnimation(m_hit_animation_for_enemies);
        // The item actor must not move
        this->addActor(actor, this->getFromRegistry("null")->clone());
        // Makes actor appear behind every other object on scene
        actor->scene()->setLayer(actor->sprite(), 0);

        if (animation)
        {
            auto* i = new sad::animations::Instance();
            i->setObject(sprite);
            i->setAnimation(animation);
            i->disableStateRestoringOnFinish();
            this->rendererForMainThread()->animations()->add(i);
        }
        
        return actor;
    }
    return nullptr;
}

bots::AbstractBot* Game::getFromRegistry(const sad::String& bot_name) const
{
    return m_bot_registry.get(bot_name);
}

bool Game::hasBotInRegistry(const sad::String& bot_name) const
{
    return m_bot_registry.contains(bot_name);
}

void Game::addActor(game::Actor* actor, bots::AbstractBot* bot)
{
    m_actors.add(actor, bot);
}

void Game::addToMainRenderer(sad::animations::Process* p) const
{
    this->rendererForMainThread()->animations()->add(p);
}

game::ActorOptions* Game::getActorOptions(const sad::String& name)
{
    return (m_actor_options.contains(name)) ? m_actor_options[name] : nullptr;
}

bool Game::addActorOptions(const sad::String& name, const game::ActorOptions& opts)
{
    if (m_actor_options.contains(name)) {
        return false;
    }
    auto* actor_options = new game::ActorOptions(opts);
    actor_options->addRef();
    m_actor_options.insert(name, actor_options);
    return true;
}

sad::p2d::BounceSolver* Game::bounceSolverForBullets() const
{
    return m_bounce_solver_for_bullets;
}

sad::dukpp03::Context* Game::context() const
{
    return m_eval_context;
}

void Game::clearItemDefinitions()
{
    for(sad::Hash<sad::String, game::Item::Definition*>::iterator it = m_item_definitions.begin(); it != m_item_definitions.end(); ++it)
    {
        game::Item::Definition* definition = it.value();
        if (definition)
        {
            definition->delRef();
        }
    }
    m_item_definitions.clear();
}

double Game::windSpeed() const
{
    return m_wind_speed;
}

void Game::setWindSpeed(double wind_speed)
{
    m_wind_speed = wind_speed;
}

bool Game::isPaused() const
{
    return this->m_paused_state_machine.isInState("paused");
}

bool Game::isNowPlaying() const
{
    return this->m_state_machine.isInState("playing");
}

double Game::maxLevelX() const
{
    return m_max_level_x;
}

bool Game::isWinning() const
{
    return m_winning;
}

void Game::tryEnterPause()
{
    if (this->isNowPlaying() && !this->isPaused() && !isWinning())
    {
        this->rendererForMainThread()->pipeline()->appendTask([=]() {
            sad::Renderer* r = this->rendererForMainThread();
            this->m_paused_state_machine.enterState("paused");
            this->rendererForMainThread()->animations()->pause();
            this->m_delayed_tasks.pause();
            this->m_player->pauseWeaponsReloading();
            this->m_actors.pause();
            this->m_pause_menu_state = Game::PauseMenuState::GPMS_RESUME;
            sad::db::Database* gamescreen = r->database("gamescreen");
            auto* pause_scene = gamescreen->objectByName<sad::Scene>("pause");
            if (pause_scene)
            {
                this->showCurrentPauseMenuOption();
                sad::Vector3D v = r->globalTranslationOffset();
                v.setX(v.x() * (-1.0));
                pause_scene->camera().setTranslationOffset(v);
                pause_scene->setActive(true);
            }
        });
        this->rendererForInventoryThread()->pipeline()->appendTask([=]() {
            this->rendererForInventoryThread()->animations()->pause();
            this->m_theme_playing->setIsPaused(true);
            Sounds::pause();
            this->playSound("pause");
        });
    }
}

void Game::tryExitPause()
{
    this->tryExitPause([]() {});
}

void Game::tryExitPause(const std::function<void()>& on_exit_main)
{
    if (this->isNowPlaying() && this->isPaused() && !isWinning())
    {
        this->rendererForMainThread()->pipeline()->appendTask([=]() {
            this->m_paused_state_machine.enterState("playing");
            this->rendererForMainThread()->animations()->resume();
            this->m_delayed_tasks.resume();
            this->m_player->resumeWeaponsReloading();
            this->m_actors.resume();
            auto* pause_scene = this->rendererForMainThread()->database("gamescreen")->objectByName<sad::Scene>("pause");
            if (pause_scene)
            {
                pause_scene->setActive(false);
            }
            on_exit_main();
        });
        this->rendererForInventoryThread()->pipeline()->appendTask([=]() {
            this->rendererForInventoryThread()->animations()->resume();
            Sounds::resume();
            this->m_theme_playing->setIsPaused(false);
            this->playSound("pause");
        });
    }
}

CameraMovement* Game::cameraMovement() const
{
    return m_camera_movement;
}


void Game::setWallsAccordingToOffset()
{
	const sad::Point2D p = this->rendererForMainThread()->globalTranslationOffset();
    this->m_walls.setLeftBound(p.x() * -1.0);
    this->m_walls.setRightBound(p.x() * -1.0 + this->rendererForMainThread()->settings().width());
}

void Game::removePlatform(const sad::String& name)
{
    sad::Renderer* r = this->rendererForMainThread();
    sad::db::Database* gamescreen = r->database("gamescreen");
    if (gamescreen)
    {
        auto* sprite = gamescreen->objectByName<sad::Sprite2D>(name);
        if (sprite)
        {
            sprite->addRef();
            sad::Vector<sad::p2d::Body*> bodies = m_physics_world->allBodiesInGroup("platforms");
            for (size_t i = 0; i < bodies.size(); i++)
            {
                bool found = false;
                sad::Vector<sad::Object*> o = bodies[i]->userObjects();
                for(size_t j = 0; j < o.size(); j++)
                {
                    if (o[j] == sprite)
                    {
                        found = true;
                    }
                }
                if (found)
                {
                    m_level_storage_loader->removeBody(bodies[i]);
                    m_level_storage_loader->removeSprite(sprite);
                    m_moving_platform_registry.remove(bodies[i]);
                    disableRestingForBodiesOnPlatform(bodies[i]);
                    m_physics_world->removeBody(bodies[i]);
                    bodies.removeAt(i);
                    --i;
                }
            }

            sprite->scene()->removeNode(sprite);
            sprite->delRef();
        }
    }
}

void Game::tryInvokeCallbackOnRestingOnPlatform(game::Actor* a, const sad::String& platform_name)
{
    if (m_actor_on_rest_callbacks.contains(a))
    {
        if (m_actor_on_rest_callbacks[a].contains(platform_name))
        {
            m_actor_on_rest_callbacks[a][platform_name]();
        }
    }
}

void Game::setCallbackOnRestingOnPlatform(game::Actor* a, const sad::String& platform_name, const std::function<void()>& cb)
{
    if (!m_actor_on_rest_callbacks.contains(a))
    {
        m_actor_on_rest_callbacks.insert(a, sad::Hash<sad::String, std::function<void()> >());
    }

    if (!m_actor_on_rest_callbacks[a].contains(platform_name))
    {
        m_actor_on_rest_callbacks[a].insert(platform_name, cb);
    }
    else
    {
        m_actor_on_rest_callbacks[a][platform_name] = cb;
    }
}

void Game::killPlatformBlinkingProcess(game::PlatformBlinking* process)
{
    for (sad::Hash<sad::Sprite2D*, game::PlatformBlinking*>::iterator it = m_blinking_platforms.begin(); it != m_blinking_platforms.end(); ++it)
    {
        if (it.value() == process)
        {
            delete it.value();
            m_blinking_platforms.remove(it.key());
            return;
        }
    }
}

void Game::disablePlatformBlinking(const sad::String& platform_name)
{
    sad::Renderer* renderer = this->rendererForMainThread();
    sad::db::Database* db = renderer->database("gamescreen");
    if (db)
    { 
        auto* sprite = db->objectByName<sad::Sprite2D>(platform_name);
        if (sprite)
        { 
            if (m_blinking_platforms.contains(sprite))
            {
                m_blinking_platforms[sprite]->disable();
            }
        }
    }
}

void Game::enablePlatformBlinking(const sad::String& platform_name, double time)
{
    sad::Renderer* renderer = this->rendererForMainThread();
    sad::db::Database* db = renderer->database("gamescreen");
    if (db)
    {
        auto* sprite = db->objectByName<sad::Sprite2D>(platform_name);
        if (sprite)
        {
            if (m_blinking_platforms.contains(sprite))
            {
                game::PlatformBlinking* blinking = m_blinking_platforms[sprite];
                blinking->enable();
                blinking->setTime(time);
            }
            else
            {
                auto* blinking = new game::PlatformBlinking(this, sprite, time);
                m_blinking_platforms.insert(sprite, blinking);
                blinking->enable();
                blinking->setTime(time);
            }
        }
        else
        {
            SL_LOCAL_DEBUG(sad::String("Cannot enable blinking for ") + platform_name + sad::String(": object not found"), *renderer);
        }
    }
}

void Game::registerRestingBody(sad::p2d::Body* b, game::Actor* a)
{
    if (!m_platforms_to_actors.contains(b))
    {
        m_platforms_to_actors.insert(b, sad::Vector<game::Actor*>());
    }
    sad::Vector<game::Actor*>&  v = m_platforms_to_actors[b];
    if (std::find(v.begin(), v.end(), a) == v.end())
    {
        v.push_back(a);
    }
}



void Game::unregisterRestingBody(sad::p2d::Body* b, game::Actor* a)
{
    if (m_platforms_to_actors.contains(b))
    {
        sad::Vector<game::Actor*>&  v = m_platforms_to_actors[b];
        v.removeAll(a);
    }
}

void Game::disableRestingForBodiesOnPlatform(sad::p2d::Body* b)
{
    if (m_platforms_to_actors.contains(b))
    {
        sad::Vector<game::Actor*>  v = m_platforms_to_actors[b];
        for(size_t i = 0; i < v.size(); i++)
        {
            v[i]->disableResting();
        }
    }
}

game::LevelStorageLoader* Game::levelStorageLoader() const
{
    return m_level_storage_loader;
}


void Game::runGameInitializationScript() const
{
    // Fetch and run game initialization script
    const sad::Maybe<sad::String> maybe_script = sad::slurp("examples/multithreading/game_init.js", m_main_thread->renderer());
    if (maybe_script.exists())
    {
        this->evalScript(maybe_script.value());
    }
    else
    {
        SL_LOCAL_FATAL("Unable to run game initialization script", *(m_main_thread->renderer()));
    }
}


// ==================================== PRIVATE METHODS ====================================

void Game::eraseActorFromRestingBodiesToActors(game::Actor* a)
{
    for (sad::Hash<sad::p2d::Body*, sad::Vector<game::Actor*> >::iterator it = m_platforms_to_actors.begin(); it != m_platforms_to_actors.end(); ++it)
    {
        sad::Vector<game::Actor*>&  actors = it.value();
        for (size_t i = 0; i < actors.size(); i++)
        {
            if (actors[i] == a)
            {
                actors.removeAt(i);
                break;
            }
        }
    }
}

void Game::showCurrentPauseMenuOption() const
{
    sad::Renderer* r = this->rendererForMainThread();
    sad::db::Database* gamescreen = r->database("gamescreen");
    auto* pause_scene = gamescreen->objectByName<sad::Scene>("pause");
    if (pause_scene)
    {
        const char* label_name = (this->m_pause_menu_state == Game::PauseMenuState::GPMS_RESUME) ? "PauseResume" : "PauseExit";
        auto* label = gamescreen->objectByName<sad::Label>(label_name);
        auto* sprite = gamescreen->objectByName<sad::Sprite2D>("PauseArrow");
        if (label && sprite)
        {
            const sad::Rect2D old_area = sprite->area();
            const double y = label->area().p0().y() - (label->area().height() - old_area.height()) / 2.0 - 3.0; // Simple offset to solve font size inconsistency
            sprite->setArea(sad::Rect2D(old_area.p0().x(), y - old_area.height(), old_area.p2().x(), y));
        }
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
    std::function<void(const sad::String&)> stop_moving_platform_on_way = [=](const sad::String& platform) {
        this->m_moving_platform_registry.remove(platform);
    };
    std::function<void(double, const sad::dukpp03::CompiledFunction&)> add_trigger = [=](double x, const sad::dukpp03::CompiledFunction& f) {
        this->m_triggers.add(x, f, false);
    };
    std::function<void(double, const sad::dukpp03::CompiledFunction&)> add_trigger_once = [=](double x, const sad::dukpp03::CompiledFunction& f) {
        this->m_triggers.add(x, f, true);
    };
    std::function<void(const sad::String&)> print = [=](const sad::String& message) {
        SL_LOCAL_DEBUG(message, *(m_main_thread->renderer()));
    };
    std::function<void()> trigger_win_game = [=]() {
        this->triggerWinGame();
    };
    std::function<void(int)> increment_score = [=](int d) {
        this->incrementScore(d);
    };
    std::function<void(int)> decrement_score = [=](int d) {
        this->decrementScore(d);
    };
    std::function<int()> local_score = [=]() {
        return this->score();
    };
    std::function<bool(const sad::String&,
                       const sad::String&,
                       const sad::String&,
                       bool,
                       sad::dukpp03::CompiledFunction,
                       sad::dukpp03::CompiledFunction,
                       sad::dukpp03::CompiledFunction
                       )> _add_item_definition = [=](
                       const sad::String& option_name,
                       const sad::String& title,
                       const sad::String& description,
                       bool delete_after_applied,
                       // ReSharper disable once CppParameterMayBeConst
                       sad::dukpp03::CompiledFunction on_item_added,  // NOLINT(performance-unnecessary-value-param)
                       // ReSharper disable once CppParameterMayBeConst
                       sad::dukpp03::CompiledFunction on_item_removed,  // NOLINT(performance-unnecessary-value-param)
                       // ReSharper disable once CppParameterMayBeConst
                       sad::dukpp03::CompiledFunction apply_item  // NOLINT(performance-unnecessary-value-param)
    ) -> bool {
        if (this->m_item_definitions.contains(option_name))
        {
            return false;
        }
        auto* definition = new game::Item::Definition(option_name, title, description, delete_after_applied);
        definition->OnItemAdded = on_item_added;
        definition->OnItemRemoved = on_item_removed;
        definition->ApplyCallback = apply_item;
        this->m_item_definitions.insert(option_name, definition);
        definition->addRef();
        return true;
    };

    std::function<void(game::Actor*)> _shedule_kill_actor_by_body = [=](game::Actor* a) {
        this->rendererForMainThread()->pipeline()->appendTask([=] {
            this->killActorByBody(a->body());
        });
    };

    std::function<void(game::Actor*)> _make_item_unpickable = [=](game::Actor* a) {
        this->makeItemUnpickable(a);
    };

    std::function<bool(const sad::String& options)> give_item_to_player = [=](const sad::String& opts) -> bool{
        if (this->m_item_definitions.contains(opts))
        {
            this->player()->inventory()->addItem(this->makeItem(opts));
        }
        return false;
    };
    std::function<void(game::Actor*, int)> increment_floater_state_counter_delayed = [=](game::Actor* actor, int delay) -> void {
        this->addDelayedTask(delay, [=]() { actor->incrementFloaterStateCounter(); });
    };
    std::function<void(game::Actor*, int)> decrement_floater_state_counter_delayed = [=](game::Actor* actor, int delay) -> void {
        this->addDelayedTask(delay, [=]() { actor->decrementFloaterStateCounter(); });
    };

    // ReSharper disable once CppParameterMayBeConst
    std::function<void(double, sad::dukpp03::CompiledFunction)> add_delayed_task = [=](double delay, sad::dukpp03::CompiledFunction f) {
        this->addDelayedTask(delay, [=]() {
            sad::dukpp03::CompiledFunction mf = f;
            mf.call(this->context());
        });
    };

    std::function<void(game::Actor*, bool)> turn_clipping_through_boundaries = [=](game::Actor* actor, bool value) -> void {
        actor->setClipThroughBoundaries(value);
    };
    std::function<game::Actor*(const sad::String&, const sad::Point2D& p)> _spawn_item = [=](const sad::String& name, const sad::Point2D& p) -> game::Actor* {
        game::Actor* a = this->makeItemActor(name, p);
        if (a)
        {
            this->playSound("item_drop");
            a->setVelocity(sad::p2d::Vector(0, 200)); // Made basic velocity small just to move item a little
        }
        return a;
    };
    std::function<void(game::Actor*, const sad::Hash<sad::String, int>&)> _set_loot_for_actor = [=](game::Actor* actor, const sad::Hash<sad::String, int>& loot) {
        actor->onBeforeDeath([=](game::Actor* a) {
            int sum = 0;
            for(sad::Hash<sad::String, int>::const_iterator it = loot.const_begin(); it != loot.const_end(); ++it) {
                sum += it.value();
            }
            sad::Maybe<sad::String> result;
            const double value = (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) * 100.0;
            // ReSharper disable once CppJoinDeclarationAndAssignment
            double begin;
            double end = 0;
            for (sad::Hash<sad::String, int>::const_iterator it = loot.const_begin(); it != loot.const_end(); ++it) 
            {
                if (!result.exists()) 
                {
                    // ReSharper disable once CppJoinDeclarationAndAssignment
                    begin = end;
                    end = begin + static_cast<double>(it.value()) / sum * 100.0;
                    if (value >= begin && value <= end) 
                    {
                      result.setValue(it.key());
                    }
                }
            }
            if (result.exists())
            { 
                _spawn_item(result.value(), a->middle());
            }
        });
    };

    std::function<void(const sad::String&)> play_sound = [=](const sad::String& name) {
        this->playSound(name);
    };
    std::function<void(double)> set_wind_speed = [=](double speed) {
        this->setWindSpeed(speed);
        this->player()->updateHorizontalVelocity();
    };
    std::function<game::SnowParticles*()> snow_particles = [=]() -> game::SnowParticles* {
        return m_snow_particles;
    };
    std::function<CameraMovement*()> camera_movement = [=]() -> CameraMovement* {
        return m_camera_movement;
    };
    std::function<void(const sad::Point2D&)> set_global_offset = [=](const sad::Point2D& p) {
        this->rendererForMainThread()->setGlobalTranslationOffset(p);
    };
    std::function<void(double)> set_max_level_x = [=](double x)  {
        this->m_max_level_x = x;
    };
    std::function<void(double)> set_left_bound = [=](double x) {
        this->m_walls.setLeftBound(x);
    };
    std::function<void(double)> set_right_bound = [=](double x) {
        this->m_walls.setRightBound(x);
    };
    std::function<void()> lock_screen = [=]() {
        this->setWallsAccordingToOffset();
        this->m_camera_movement->lock();
    };

    std::function<void()> unlock_screen = [=]() {
        this->m_walls.setLeftBound(0);
        this->m_walls.setRightBound(this->maxLevelX());
        this->m_camera_movement->unlock();
        this->m_camera_movement->showArrow();
    };

    std::function<void()> reset_enemy_counter = [=]() {
        this->m_enemy_counter->reset();
    };
    std::function<void()> clear_enemy_counter = [=]() {
        this->m_enemy_counter->clear();
    };
    std::function<void(int)> set_enemy_counter = [=](int d) {
        this->m_enemy_counter->set(d);
    };
    std::function<void()> decrement_enemy_counter = [=]() {
        this->m_enemy_counter->decrement();
    };
    std::function<void()> increment_enemy_counter = [=]() {
        this->m_enemy_counter->increment();
    };
    std::function<void(sad::dukpp03::CompiledFunction)> on_zero_enemies = [=](sad::dukpp03::CompiledFunction f) {
        this->m_enemy_counter->onZeroEnemies([=] {
            sad::dukpp03::CompiledFunction mf = f;
            mf.call(this->context());
        });
    };
    std::function<void(game::Actor*)> decrement_counter_on_actor_death = [=](game::Actor* a) {
        a->onBeforeDeath([=](game::Actor*) {
            this->m_enemy_counter->decrement();
        });
    };
    std::function<void(game::Actor*, sad::dukpp03::CompiledFunction)> add_on_actor_death_action = [=](game::Actor* a, sad::dukpp03::CompiledFunction f) -> void {
        a->onBeforeDeath([=](game::Actor*) {
            sad::dukpp03::CompiledFunction mf = f;
            mf.call(this->context());
        });
    };
    std::function<void(double, double)> start_playing_camera_lock_animation = [=](double finishing_offset, double total_time) {
        this->m_camera_movement->lock();
        this->rendererForMainThread()->pipeline()->prepend(new  CameraLockAnimation(this, finishing_offset, total_time));
    };
    std::function<void()> shake_camera = [=]() {
        sad::Renderer* r = this->rendererForMainThread();
        sad::db::Database* db = r->database("gamescreen");
        if (db)
        {
            sad::animations::Group* quake = db->objectByName<sad::animations::Group>("quake");
            quake->clearFinished();
            r->animations()->add(quake);
        }
    };
    std::function<void(const sad::String& s)> remove_platform = [=](const sad::String& s) {
        this->removePlatform(s);
    };
    std::function<void(const sad::String& platform_name, sad::dukpp03::CompiledFunction)> set_on_player_resting_on_platform = [=](const sad::String& platform_name, sad::dukpp03::CompiledFunction f) -> void {
        setCallbackOnRestingOnPlatform(this->player()->actor(), platform_name, [=]() {
            sad::dukpp03::CompiledFunction mf = f;
            mf.call(this->context());
        });
    };
    std::function<void(const sad::String&, double)> enable_platform_blinking = [=](const sad::String& platform_name, double time) {
        this->enablePlatformBlinking(platform_name, time);
    };
    std::function<void(const sad::String&)> disable_platform_blinking = [=](const sad::String& platform_name) {
        this->disablePlatformBlinking(platform_name);
    };


    m_eval_context->registerCallable("makePlatformGoOnWay", sad::dukpp03::make_lambda::from(make_platform_go_on_way));
    m_eval_context->registerCallable("stopMovingPlatformOnWay", sad::dukpp03::make_lambda::from(stop_moving_platform_on_way));
    m_eval_context->registerCallable("addTrigger", sad::dukpp03::make_lambda::from(add_trigger));
    m_eval_context->registerCallable("addTriggerOnce", sad::dukpp03::make_lambda::from(add_trigger_once));
    m_eval_context->registerCallable("gamePrint", sad::dukpp03::make_lambda::from(print));
    m_eval_context->registerCallable("triggerWinGame", sad::dukpp03::make_lambda::from(trigger_win_game));
    m_eval_context->registerCallable("incrementScore", sad::dukpp03::make_lambda::from(increment_score));
    m_eval_context->registerCallable("decrementScore", sad::dukpp03::make_lambda::from(decrement_score));
    m_eval_context->registerCallable("score", sad::dukpp03::make_lambda::from(local_score));
    m_eval_context->registerCallable("setItemPenetrationDepth", sad::dukpp03::make_function::from(game::setItemPenetrationDepth));
    m_eval_context->registerCallable("_addItemDefinition", sad::dukpp03::make_lambda::from(_add_item_definition));
    m_eval_context->registerCallable("_sheduleKillActorByBody", sad::dukpp03::make_lambda::from(_shedule_kill_actor_by_body));
    m_eval_context->registerCallable("_makeItemUnpickable", sad::dukpp03::make_lambda::from(_make_item_unpickable));
    m_eval_context->registerCallable("giveItemToPlayer", sad::dukpp03::make_lambda::from(give_item_to_player));
    m_eval_context->registerCallable("incrementFloaterStateCounterDelayed", sad::dukpp03::make_lambda::from(increment_floater_state_counter_delayed));
    m_eval_context->registerCallable("decrementFloaterStateCounterDelayed", sad::dukpp03::make_lambda::from(decrement_floater_state_counter_delayed));
    m_eval_context->registerCallable("turnClippingThroughBoundaries", sad::dukpp03::make_lambda::from(turn_clipping_through_boundaries)); // turns for actor
    m_eval_context->registerCallable("_spawnItem", sad::dukpp03::make_lambda::from(_spawn_item));
    m_eval_context->registerCallable("playSound", sad::dukpp03::make_lambda::from(play_sound));
    m_eval_context->registerCallable("_setLootForActor", sad::dukpp03::make_lambda::from(_set_loot_for_actor));
    m_eval_context->registerCallable("addOnActorDeathAction", sad::dukpp03::make_lambda::from(add_on_actor_death_action));
    m_eval_context->registerCallable("setWindSpeed", sad::dukpp03::make_lambda::from(set_wind_speed));
    m_eval_context->registerCallable("snowParticles", sad::dukpp03::make_lambda::from(snow_particles));
    m_eval_context->registerCallable("setGlobalOffset", sad::dukpp03::make_lambda::from(set_global_offset));
    m_eval_context->registerCallable("setMaxLevelX", sad::dukpp03::make_lambda::from(set_max_level_x));
    m_eval_context->registerCallable("setLeftBound", sad::dukpp03::make_lambda::from(set_left_bound));
    m_eval_context->registerCallable("setRightBound", sad::dukpp03::make_lambda::from(set_right_bound));
    m_eval_context->registerCallable("cameraMovement", sad::dukpp03::make_lambda::from(camera_movement));
    m_eval_context->registerCallable("lockScreen", sad::dukpp03::make_lambda::from(lock_screen));
    m_eval_context->registerCallable("unlockScreen", sad::dukpp03::make_lambda::from(unlock_screen));
    m_eval_context->registerCallable("resetEnemyCounter", sad::dukpp03::make_lambda::from(reset_enemy_counter));
    m_eval_context->registerCallable("clearEnemyCounter", sad::dukpp03::make_lambda::from(clear_enemy_counter));
    m_eval_context->registerCallable("setEnemyCounter", sad::dukpp03::make_lambda::from(set_enemy_counter));
    m_eval_context->registerCallable("decrementEnemyCounter", sad::dukpp03::make_lambda::from(decrement_enemy_counter));
    m_eval_context->registerCallable("incrementEnemyCounter", sad::dukpp03::make_lambda::from(increment_enemy_counter));
    m_eval_context->registerCallable("onZeroEnemies", sad::dukpp03::make_lambda::from(on_zero_enemies));
    m_eval_context->registerCallable("decrementCounterOnActorDeath", sad::dukpp03::make_lambda::from(decrement_counter_on_actor_death));
    m_eval_context->registerCallable("startPlayingCameraLockAnimation", sad::dukpp03::make_lambda::from(start_playing_camera_lock_animation));
    m_eval_context->registerCallable("shakeCamera", sad::dukpp03::make_lambda::from(shake_camera));
    m_eval_context->registerCallable("removePlatform", sad::dukpp03::make_lambda::from(remove_platform));
    m_eval_context->registerCallable("setOnPlayerRestingOnPlatform", sad::dukpp03::make_lambda::from(set_on_player_resting_on_platform));
    m_eval_context->registerCallable("addDelayedTask", sad::dukpp03::make_lambda::from(add_delayed_task));
    m_eval_context->registerCallable("enablePlatformBlinking", sad::dukpp03::make_lambda::from(enable_platform_blinking));
    m_eval_context->registerCallable("disablePlatformBlinking", sad::dukpp03::make_lambda::from(disable_platform_blinking));

    scripting::exposeSpawnEnemy(m_eval_context, this);
    game::exposeActorOptions(m_eval_context, this);
    weapons::exposeSwingSettings(m_eval_context);
    weapons::exposeBulletSettings(m_eval_context);
    weapons::exposeLaserSettings(m_eval_context);
    weapons::exposeWeapon(m_eval_context);
    game::exposePlayer(m_eval_context, this);
    game::exposeActor(m_eval_context);
    game::exposeItem(m_eval_context);
    game::exposeSnowParticles(m_eval_context);
    exposeCameraMovement(m_eval_context);
    bots::shootingstrategies::exposeShootingStrategy(m_eval_context);
    bots::shootingstrategies::exposeFixedAngleStrategy(m_eval_context);
    bots::shootingstrategies::exposePlayerLocationStrategy(m_eval_context);
    bots::shootingstrategies::exposeRandomStrategy(m_eval_context);
    bots::shootingstrategies::exposeTurningStrategy(m_eval_context);
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

void Game::destroyWorld()
{
    if (m_physics_world)
    {
        m_step_task->setWorld(nullptr);
        m_physics_world->delRef();
        m_physics_world = nullptr;
    }
}

void Game::initGamePhysics()
{
    m_physics_world = new sad::p2d::World();
    initPhysicsWorld(m_physics_world);
    m_max_level_x = 0;

    sad::Renderer* renderer = m_main_thread->renderer();
    sad::db::Database* db = renderer->database("gamescreen");
    m_player->initPhysics(m_physics_world, db);
    auto* main_scene = db->objectByName<sad::Scene>("main");
    if (main_scene)
    {
        auto* container = new game::StaticObjectContainer();
        initPhysicsPlatforms(m_physics_world, main_scene, &m_moving_platform_registry, container);
        initCoins(this, m_physics_world, db, this->rendererForMainThread(), &m_unanimated_coins, container);
        m_level_storage_loader = new game::LevelStorageLoader(container, m_physics_world, m_player->area());
        delete container;
    }
    m_max_level_x = 800;
    m_walls.init(0, m_max_level_x, 600, 0);
    m_walls.addToWorld(m_physics_world);

    // Handle all collision as non-resilient, enabling sliding
    const std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_player_and_platforms = [=](const sad::p2d::BasicCollisionEvent & ev) {
        if (ev.m_object_2 == this->m_walls.bottomWall())
        {
            m_player->onBottomWallCollision();
            return;
        }
        this->m_player->onPlatformCollision(ev);
    };
    const std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_enemy_and_platforms = [=](const sad::p2d::BasicCollisionEvent & ev) {
        auto* a = dynamic_cast<game::Actor*>(ev.m_object_1->userObject());
        if (a)
        {
            if (!a->isFloater() && ev.m_object_2 == this->m_walls.bottomWall())
            {
                a->fireOnDeathEvents();
                this->killActorWithoutSprite(a);
                a->playDeathAnimation();
                return;
            }
            a->onPlatformCollision(ev);
        }
    };
    const std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_enemy_and_walls = [=](const sad::p2d::BasicCollisionEvent & ev) {
        auto* a = dynamic_cast<game::Actor*>(ev.m_object_1->userObject());
        if (a)
        {
            if (!a->isFloater() && ev.m_object_2 == this->m_walls.bottomWall())
            {
                a->fireOnDeathEvents();
                this->killActorWithoutSprite(a);
                a->playDeathAnimation();
                return;
            }
            if (!a->isClipThroughBoundaries())
            { 
                a->onPlatformCollision(ev);
            }
        }
    };

    const std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_item_and_platforms = [=](const sad::p2d::BasicCollisionEvent & ev) {
        auto* a = dynamic_cast<game::Actor*>(ev.m_object_1->userObject());
        if (a)
        {
            if (!a->isFloater() && ev.m_object_2 == this->m_walls.bottomWall())
            {
                this->rendererForMainThread()->pipeline()->appendTask([=] {
                    this->killActorByBody(ev.m_object_1);
                    });
                return;
            }
            a->onPlatformCollision(ev);
            if (a->isResting())
            {
                a->body()->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
                a->onPlatformCollision(ev);
                this->rendererForMainThread()->pipeline()->appendTask([=] {
                    this->rendererForMainThread()->animations()->stopProcessesRelatedToObject(a->sprite());
                    const sad::String opts = a->sprite()->optionsName();
                    const int depth = game::getPenetrationDepthForItem(opts);
                    if (depth != 0)
                    {
                        a->sprite()->setAngle(M_PI / 4);
                        sad::Rect2D area = a->sprite()->area();
                        a->sprite()->setArea(sad::Rect2D(area[0].x(), area[0].y() - depth, area[2].x(), area[2].y() - depth));
                    }
                });
            }
        }
    };

    const std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_item_and_player = [=](const sad::p2d::BasicCollisionEvent & ev) {
        // Avoid hard computations for item on ground
        if (!this->player()->inventory()->isFull()) {
            auto* a = dynamic_cast<game::Actor*>(ev.m_object_1->userObject());
            if (!a)
            {
                return;
            }
            const sad::String& options = a->sprite()->optionsName();
            if (this->m_item_definitions.contains(options)) 
            {
                this->player()->inventory()->addItem(this->makeItem(options));
                this->playSound("item_taken");
                this->rendererForMainThread()->pipeline()->appendTask([=] {
                    this->killActorByBody(a->body());
                });
            } 
            else
            {
                SL_LOCAL_DEBUG(sad::String("No item definition: ") + options, *(this->rendererForMainThread()));
                this->makeItemUnpickable(a);
            }
        }
    };

    m_physics_world->addHandler("player", "platforms", collision_between_player_and_platforms);
    m_physics_world->addHandler("enemies", "platforms", collision_between_enemy_and_platforms);
    m_physics_world->addHandler("items", "platforms", collision_between_item_and_platforms);
    m_physics_world->addHandler("items_unpickable", "platforms", collision_between_item_and_platforms);

    m_physics_world->addHandler("player", "walls", collision_between_player_and_platforms);
    m_physics_world->addHandler("enemies", "walls", collision_between_enemy_and_walls);
    m_physics_world->addHandler("items", "walls", collision_between_item_and_platforms);
    m_physics_world->addHandler("items_unpickable", "walls", collision_between_item_and_platforms);

    m_physics_world->addHandler("items", "player", collision_between_item_and_player);

    const std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_walls_and_bullet = [=](const sad::p2d::BasicCollisionEvent & ev) {
        sad::Object* a = ev.m_object_2->userObject();
        // This conditions only holds only in case of bullets,
        static_cast<weapons::Projectile*>(a)->onWallHit(ev.m_object_1);
    };
    m_physics_world->addHandler("walls", "player_bullets", collision_between_walls_and_bullet);
    m_physics_world->addHandler("walls", "enemy_bullets", collision_between_walls_and_bullet);
    const std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_platform_and_bullet = [=](const sad::p2d::BasicCollisionEvent & ev) {
        sad::p2d::Body* maybe_bullet_body = ev.m_object_2;
        sad::Object* obj = maybe_bullet_body->userObject();
        static_cast<weapons::Projectile*>(obj)->onPlatformHit(ev.m_object_1);
    };
    m_physics_world->addHandler("platforms", "player_bullets", collision_between_platform_and_bullet);
    m_physics_world->addHandler("platforms", "enemy_bullets", collision_between_platform_and_bullet);

    const std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_enemy_and_bullet = [=](const sad::p2d::BasicCollisionEvent & ev) {
        auto* actor = static_cast<game::Actor*>(ev.m_object_1->userObject());
        auto* projectile = static_cast<weapons::Projectile*>(ev.m_object_2->userObject());
        projectile->onEnemyHit(actor);
        actor->takeDamage(projectile->damage());
    };

    const std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_player_and_bullet = [=](const sad::p2d::BasicCollisionEvent & ev) {
        auto* projectile = static_cast<weapons::Projectile*>(ev.m_object_2->userObject());
        const int damage = projectile->damage();
        projectile->onPlayerHit(m_player);
        this->player()->takeDamage(damage);
    };

    const std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_player_and_enemies = [=](const sad::p2d::BasicCollisionEvent & ev) {
        if (!this->player()->isInvincible()) this->playSound("hurt");
        this->player()->tryDecrementLives(1);
    };

    m_physics_world->addHandler("enemies", "player_bullets", collision_between_enemy_and_bullet);
    m_physics_world->addHandler("player", "enemy_bullets", collision_between_player_and_bullet);
    m_physics_world->addHandler("enemies", "player", collision_between_player_and_enemies);

    m_step_task->setWorld(m_physics_world);
}

void Game::tryRenderDebugShapes() const
{
    if (!m_is_rendering_world_bodies)
    {
        return;
    }
    sad::Renderer* renderer = m_main_thread->renderer();
    if (renderer->scenes().empty())
    {
        return;
    }
    sad::Scene* scene = renderer->scenes()[renderer->scenes().size() - 1];
    sad::Vector<sad::p2d::Body*> bodies = m_physics_world->allBodies();
    // As we already reset view matrix, shift rendered shapes by offset manually
    sad::Point2D p = renderer->globalTranslationOffset();
    //p = sad::Point2D(0, 0);
    if (!bodies.empty())
    {
        for (size_t i = 0; i < bodies.size(); i++)
        {
            sad::p2d::CollisionShape* shape = bodies[i]->currentShape();
            if (shape->metaIndex() == sad::p2d::Rectangle::globalMetaIndex())
            {
                sad::p2d::Rectangle* shape_rect = dynamic_cast<sad::p2d::Rectangle*>(shape);
                if (shape_rect)
                {
                    sad::Rect2D r = shape_rect->rect();
                    sad::moveBy(p, r);
                    renderer->render()->rectangle(scene, r, sad::AColor(0, 0, 255));
                }
            }
            if (shape->metaIndex() == sad::p2d::Circle::globalMetaIndex())
            {
                sad::Vector<sad::p2d::Point> list_of_points;
                shape->populatePoints(list_of_points);
                for (size_t j = 0; j < (list_of_points.size() - 1); j++)
                {
                    renderer->render()->line(scene, list_of_points[j] + p, list_of_points[j + 1] + p, sad::AColor(0, 0, 255));
                }
            }
            if (shape->metaIndex() == sad::p2d::Line::globalMetaIndex())
            {
                auto* line = dynamic_cast<sad::p2d::Line*>(shape);
                if (line)
                {
                    renderer->render()->line(scene, line->cutter().p1() + p, line->cutter().p2() + p, sad::AColor(0, 0, 255));
                }
            }
        }
    }
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
	const int index = (is_inventory_thread) ? 1 : 0;
    sad::Renderer* renderer = (is_inventory_thread) ? (m_inventory_thread->renderer()) : (m_main_thread->renderer());
    if (loaded[index])
    {
        renderer->database(db_name)->restoreSnapshot();
    }
    else
    {
        auto* database = new sad::db::Database();
        database->setRenderer(renderer);
        database->tryLoadFrom(file_name);
        database->saveSnapshot();
        renderer->addDatabase(db_name, database);
        loaded[index] = true;
    }
}

void Game::clearLevelStorageLoader()
{
    if (m_level_storage_loader)
    {
        delete m_level_storage_loader;
        m_level_storage_loader = nullptr;
    }
}

Game::Game(const Game&)  // NOLINT
    :
    m_main_thread(nullptr),
    m_inventory_thread(nullptr),
    m_is_quitting(false),
    m_main_menu_state(Game::MainMenuState::GMMS_PLAY),
    m_pause_menu_state(Game::PauseMenuState::GPMS_RESUME),
    m_score(0),
    m_highscore(0),
    m_loaded_options_database{false, false},
    m_loaded_lose_screen_database{false, false},
    m_loaded_win_screen_database{false, false},
    m_main_level_loader(nullptr),
    m_theme_playing(nullptr),
    m_sounds(nullptr),
    m_transition_process(nullptr),
    m_inventory_node(nullptr),
    m_inventory_popup(nullptr),
    m_player(nullptr),
    m_eval_context(nullptr),
    m_physics_world(nullptr),
    m_step_task(nullptr),
    m_bounce_solver(nullptr),
    m_bounce_solver_for_bullets(nullptr),
    m_is_rendering_world_bodies(false),
    m_max_level_x(0),
    m_hit_animation_for_enemies(nullptr),
    m_hit_animation_for_players(nullptr),
    m_score_bar(nullptr),
    m_wind_speed(0),
    m_snow_particles(nullptr),
    m_camera_movement(nullptr),
    m_winning(false), 
    m_enemy_counter(nullptr),
    m_level_storage_loader(nullptr)
{
    throw std::logic_error("Not implemented");
}

Game& Game::operator=(const Game&)
{
    throw std::logic_error("Not implemented");
    // ReSharper disable once CppUnreachableCode
    return *this;
}
