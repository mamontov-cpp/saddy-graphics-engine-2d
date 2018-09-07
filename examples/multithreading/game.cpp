#include "game.h"

#include "threads/gamethread.h"

#include "nodes/background.h"
#include "nodes/inventorynode.h"
#include "nodes/inventorypopup.h"

#include <input/controls.h>
#include <pipeline/pipeline.h>

#include <keymouseconditions.h>

#include <sprite2d.h>
#include <label.h>
#include <sadsleep.h>

#include <db/dbdatabase.h>
#include <db/dbpopulatescenesfromdatabase.h>

#include <animations/animationsinstance.h>

#include <slurpjson.h>
#include <spitjson.h>

#include <functional>

#include <dukpp-03-irrklang/dukpp-03-irrklang.h>

#include <p2d/collides1d.h>
#include <p2d/force.h>


// A precision error for designer's editor when designing level
#define DESIGNER_PRECISION_ERROR  (2.0)

const sad::Point2D Game::GravityForceValue(0.0, -300.0); // -300 is arbitrarily defined, to make player fall slowly

class BS: public sad::p2d::BounceSolver
{
public:
    BS() {}
    virtual ~BS() {}
    void logFCPError(const char * m) { printf("%s: %s\n", m, this->dump().c_str()); };
};

sad::p2d::Body* moving_platform = NULL;
bool moving_top = false;

// ==================================== PUBLIC METHODS ====================================

Game::Game()  : m_is_quitting(false),  // NOLINT(cppcoreguidelines-pro-type-member-init)
m_main_menu_state(Game::GMMS_PLAY),
m_highscore(0),
m_loaded_options_database{false, false},
m_loaded_game_screen(false),
m_inventory_node(NULL),
m_inventory_popup(NULL),
m_physics_world(NULL),
m_is_rendering_world_bodies(false),
max_level_x(0.0),
m_running_tasks(0) // NOLINT
{
    m_eval_context = new sad::dukpp03::Context();
    sad::dukpp03irrklang::init(m_eval_context);
    m_main_thread = new threads::GameThread();
    m_inventory_thread = new threads::GameThread();

    m_player = new game::Player();

    m_main_menu_states_to_labels.insert(Game::GMMS_PLAY   , "Play");
    m_main_menu_states_to_labels.insert(Game::GMMS_OPTIONS, "Options");
    m_main_menu_states_to_labels.insert(Game::GMMS_EXIT   , "Exit");

    // Init common state machine
    m_state_machine.addState("starting_screen", new sad::hfsm::State());
    m_state_machine.addState("options", new sad::hfsm::State());
    m_state_machine.addState("playing", new sad::hfsm::State());

    m_state_machine.addTransition("starting_screen", "options", new sad::hfsm::Transition());
    m_state_machine.addTransition("options", "starting_screen", new sad::hfsm::Transition());

    m_state_machine.addTransition("starting_screen", "playing", new sad::hfsm::Transition());
    m_state_machine.addTransition("playing", "starting_screen", new sad::hfsm::Transition());

    m_state_machine.enterState("starting_screen");

    // Init state machine for handling pause
    m_paused_state_machine.addState("playing", new sad::hfsm::State());
    // This third stated indicates, that we are transitioning between multiple scenes
    m_paused_state_machine.addState("transitioning", new sad::hfsm::State());
    m_paused_state_machine.addState("paused", new sad::hfsm::State());

    m_paused_state_machine.addTransition("playing", "paused", new sad::hfsm::Transition());
    m_paused_state_machine.addTransition("playing", "transitioning", new sad::hfsm::Transition());
    m_paused_state_machine.addTransition("transitioning", "playing", new sad::hfsm::Transition());

    m_paused_state_machine.addTransition("paused", "playing", new sad::hfsm::Transition());
    m_paused_state_machine.addTransition("paused", "transitioning", new sad::hfsm::Transition());
    m_paused_state_machine.addTransition("transitioning", "paused", new sad::hfsm::Transition());

    m_paused_state_machine.enterState("playing");

    m_transition_process = new SceneTransitionProcess(this);

    m_loaded_options_database[0] = false;
    m_loaded_options_database[1] = false;

    m_options_screen.init(this, m_main_thread->renderer(), m_inventory_thread->renderer());
    
    m_step_task = new sad::p2d::WorldStepTask(NULL, m_main_thread->renderer());
    
    m_bounce_solver = new BS();
    m_bounce_solver->enableDebug();
}

Game::~Game()  // NOLINT
{
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
        if (this->m_inventory_popup)
        {
            if (!(this->m_player->inventory()->isStartedDraggingItem()))
            {
                if (!(this->m_paused_state_machine.isInState("paused")))
                {
                    this->m_inventory_popup->render();
                }
            }
        }
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

void Game::setControlsForMainThread(sad::Renderer* renderer, sad::db::Database* db)
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
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.LeftKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() {
        this->m_player->sprite()->setFlipX(true);
        if (this->m_player->isResting())
        {
            this->m_player->sprite()->set("enemies_list/playerRed_walk1ng");
        }
        this->m_player->setHorizontalVelocity(game::Player::MaxHorizontalVelocity * -1);
    });
    renderer->controls()->addLambda(
        *sad::input::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.LeftKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() {  
        if (this->m_player->isResting())
        {
            this->m_player->sprite()->set("enemies_list/playerRed_standng");
        }
        else
        {
            this->m_player->sprite()->set("enemies_list/playerRed_up2ng");
        }
        this->m_player->setHorizontalVelocity(0);
    });
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.RightKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() {
        this->m_player->sprite()->setFlipX(false);
        if (this->m_player->isResting())
        {
            this->m_player->sprite()->set("enemies_list/playerRed_walk1ng");
        }
        this->m_player->setHorizontalVelocity(game::Player::MaxHorizontalVelocity);
    });
    renderer->controls()->addLambda(
        *sad::input::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.RightKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() {
        if (this->m_player->isResting())
        {
            this->m_player->sprite()->set("enemies_list/playerRed_standng");
        }
        else
        {
            this->m_player->sprite()->set("enemies_list/playerRed_up2ng");
        }
        this->m_player->setHorizontalVelocity(0);
    });
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.UpKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this] {
        if (this->m_player->canJump()) {
            this->m_player->incrementVerticalVelocity(game::Player::MaxVerticalVelocity);
            this->m_player->disableResting();
            this->m_player->sprite()->set("enemies_list/playerRed_up1ng");
        }
    });
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this] {
        if (!this->m_player->isResting()) {
            this->m_player->incrementVerticalVelocity(game::Player::MaxVerticalVelocity * -1);
            this->m_player->pushOptions("enemies_list/playerRed_fallng");
        } else {
            this->m_player->pushOptions("enemies_list/playerRed_duckng");
        }
    });
    renderer->controls()->addLambda(
        *sad::input::ET_KeyRelease
        & m_conditions.ConditionsForMainRenderer.DownKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this] {
        if (!this->m_player->isResting()) {
            this->m_player->incrementVerticalVelocity(game::Player::MaxVerticalVelocity);
        } else {
            this->m_player->popOptions();
        }
    });
    renderer->controls()->addLambda(
        *sad::input::ET_KeyPress
        & m_conditions.ConditionsForMainRenderer.JumpActionConditions[game::Conditions::CS_PLAYGAME_PLAYING]
        & ((&m_state_machine) * sad::String("playing"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this] { this->changeSceneToStartingScreen();  }
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
                m_running_tasks_lock.lock();
                ++m_running_tasks;
                m_running_tasks_lock.unlock();
                if (moving_platform)
                {
                    sad::Point2D a = dynamic_cast<sad::p2d::Rectangle*>(moving_platform->currentShape())->rect()[0];
                    //printf("Platform: %lf,%lf\n", a.x(), a.y());
                    if (a.y() <= 310)
                    {
                        if (a.y() >= 140)
                        {
                            if (moving_top)
                            {
                                moving_platform->setCurrentTangentialVelocity(sad::Point2D(50, 50));
                            } 
                            else
                            {
                                moving_platform->setCurrentTangentialVelocity(sad::Point2D(-50, -50));
                            }
                        }
                        else
                        {
                            moving_platform->setCurrentTangentialVelocity(sad::Point2D(50, 50));
                            moving_top = true;
                        }
                    }
                    else
                    {
                        moving_platform->setCurrentTangentialVelocity(sad::Point2D(-50, -50));
                        moving_top = false;
                    }
                    //printf("Velocity: %lf,%lf\n", moving_platform->tangentialVelocity().x(), moving_platform->tangentialVelocity().y());
                }
                this->m_player->clearFixedFlags();
                this->m_step_task->enable();
                this->m_step_task->process();
                // If player went too far into left or right, block the way
                sad::Rect2D area = this->m_player->area(); 
                if ((area[0].x() < 0) && (!sad::is_fuzzy_equal(area[0].x(), 0.0)))
                {
                    printf("Boundary collision\n");
                    if (this->m_player->body()->willPositionChange())
                    {
                        sad::Point2D cp = this->m_player->body()->position();
                        sad::Point2D p = this->m_player->body()->nextPosition();
                        this->m_player->body()->shedulePosition(sad::Point2D(cp.x()  + area[0].x() * - 1, p.y()));
                        this->m_player->setXCoordinateFixed(true);
                    } 
                    else 
                    {
                        this->m_player->move(sad::Point2D(area[0].x() * - 1, 0.0));
                        this->m_player->setXCoordinateFixed(true);
                    }
                    this->m_player->setHorizontalVelocity(0.0);
                }
                if (area[2].x() > max_level_x && (!sad::is_fuzzy_equal(area[2].x(), max_level_x)))
                {
                    printf("Boundary collision\n");
                    if (this->m_player->body()->willPositionChange())
                    {
                        sad::Point2D cp = this->m_player->body()->position();
                        sad::Point2D p = this->m_player->body()->nextPosition();
                        this->m_player->body()->shedulePosition(sad::Point2D(cp.x() - area[2].x(), p.y()));
                        this->m_player->setXCoordinateFixed(true);
                    }
                    else
                    {
                        this->m_player->move(sad::Point2D(max_level_x - area[2].x(), 0.0));
                        this->m_player->setXCoordinateFixed(true);
                    }
                }
                m_running_tasks_lock.lock();
                --m_running_tasks;
                m_running_tasks_lock.unlock();
            }
        }
    });
    // A debug collision shape rendering
    renderer->pipeline()->appendProcess([=]() {
        if (this->m_state_machine.isInState("playing"))
        {
            if (this->m_paused_state_machine.isInState("playing"))
            {
                if (m_is_rendering_world_bodies)
                {
                    m_running_tasks_lock.lock();
                    ++m_running_tasks;
                    m_running_tasks_lock.unlock();
                    sad::Vector<sad::p2d::Body*> bodies = m_physics_world->allBodies();
                    if (!bodies.empty())
                    {
                        for(size_t i = 0; i < bodies.size(); i++) 
                        {
                            sad::p2d::CollisionShape* shape =  bodies[i]->currentShape();
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
                    m_running_tasks_lock.lock();
                    --m_running_tasks;
                    m_running_tasks_lock.unlock();
                }
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

void Game::changeScene(const SceneTransitionOptions& opts) const
{
    m_transition_process->start(opts);
}

void Game::changeSceneToStartingScreen()
{
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

void Game::changeSceneToPlayingScreen()
{
    this->destroyWorld();

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
        sad::db::populateScenesFromDatabase(main_renderer, main_renderer->database("gamescreen"));
        this->initGamePhysics();
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
    int index = (is_inventory_thread) ? 1 : 0;
    sad::Renderer* renderer =(is_inventory_thread) ? (m_inventory_thread->renderer()) : (m_main_thread->renderer());
    if (m_loaded_options_database[index])
    {
        renderer->database("optionsscreen")->restoreSnapshot();
    }
    else
    {
        sad::db::Database* database = new sad::db::Database();
        database->setRenderer(renderer);
        database->tryLoadFrom("examples/multithreading/optionsscreen.json");
        database->saveSnapshot();
        renderer->addDatabase("optionsscreen", database);
        m_loaded_options_database[index] = true;
    }
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
    // ReSharper disable once CppInitializedValueIsAlwaysRewritten
    int amount = 0;
    do
    {
        sad::sleep(100);
        m_running_tasks_lock.lock();
        amount = m_running_tasks;
        m_running_tasks_lock.unlock();
    }
    while(amount > 0);
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
    m_physics_world->addRef();
    m_physics_world->addGroup("player");
    m_physics_world->addGroup("platforms");

    max_level_x = 0;

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
        std::function<void (sad::p2d::Vector)> move_listener = [this](sad::p2d::Vector) {
            this->m_player->testResting();
        };
        body->addMoveListener(move_listener);

        m_physics_world->addBodyToGroup("player", body);
        m_player->setBody(body);
        m_player->enableGravity();
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
                        max_level_x = std::max(max_level_x, node->area()[2].x());
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
            if (ungrouped_platform_sprites[i]->objectName() == "MovingPlatform4")
            {
                moving_platform = body;
            }

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

            m_physics_world->addBodyToGroup("platforms", body);
        }
    }

    // Handle all collision as non-resilient, enabling sliding
    std::function<void(const sad::p2d::BasicCollisionEvent &)> collision_between_player_and_platforms = [=](const sad::p2d::BasicCollisionEvent & ev) {
        printf("Event\n");
        double tick = this->m_physics_world->timeStep();

        sad::p2d::Vector force_value;
        ev.m_object_1->tangentialForces().value(force_value);

        bool willVelocityChange = ev.m_object_1->willTangentialVelocityChange();
        sad::p2d::Vector v = ev.m_object_2->tangentialVelocity();
        sad::p2d::Vector player_velocity = ev.m_object_1->tangentialVelocity();
        sad::p2d::Vector next_velocity;
        if (willVelocityChange)
        {
            next_velocity = ev.m_object_1->nextTangentialVelocity();
        }
        sad::Point2D current_position_1 = ev.m_object_1->position();
        sad::Point2D current_position_2 = ev.m_object_2->position();

        this->m_bounce_solver->pushResilienceCoefficient(0.0);
        this->m_bounce_solver->pushRotationFriction(0.0);
        if (!this->m_bounce_solver->bounce(ev.m_object_1, ev.m_object_2))
        {
            return;
        }
        double ctoi = this->m_bounce_solver->correctedTOI();
        sad::Point2D next_position_1 = ev.m_object_1->nextPosition();
        sad::Point2D next_position_2 = ev.m_object_2->nextPosition();

        sad::Rect2D shape_1 = dynamic_cast<sad::p2d::Rectangle*>(ev.m_object_1->currentShape())->rect();
        sad::Rect2D shape_2 = dynamic_cast<sad::p2d::Rectangle*>(ev.m_object_2->currentShape())->rect();

        sad::moveBy(next_position_1 - current_position_1, shape_1);
        sad::moveBy(next_position_2 - current_position_2, shape_2);

        double min_player_y = std::min(shape_1[0].y(), shape_1[2].y());
        double max_platform_y = std::max(shape_2[0].y(), shape_2[2].y());

        sad::p2d::Cutter1D player_part(std::min(shape_1[0].x(), shape_1[2].x()), std::max(shape_1[0].x(), shape_1[2].x()));
        sad::p2d::Cutter1D platform_part(std::min(shape_2[0].x(), shape_2[2].x()), std::max(shape_2[0].x(), shape_2[2].x()));

        if ((sad::is_fuzzy_equal(max_platform_y, min_player_y) || (min_player_y > max_platform_y))
            && (sad::p2d::collides(player_part, platform_part)))
        {
            if (ev.m_object_1->willPositionChange())
            {
                double x = ev.m_object_1->nextPosition().x();
                double y = ev.m_object_1->position().y() + player_velocity.y() * (ctoi * 0.99);
                if (!(this->m_player->isXCoordinateFixed()))
                {
                    x = ev.m_object_1->position().x() + player_velocity.x() * tick + force_value.x() * tick * tick / 2.0;
                }
                if (ev.m_object_2->tangentialVelocity().y() > 0)
                {
                    y += ev.m_object_2->tangentialVelocity().y() * tick;
                }
                ev.m_object_1->shedulePosition(sad::Point2D(x, y));
                this->m_player->setYCoordinateFixed(true);

            }
            else
            {
                double x = ev.m_object_1->position().x() + player_velocity.x() * tick + force_value.x() * tick * tick / 2.0;
                double y = ev.m_object_1->position().y() + player_velocity.y() * (ctoi * 0.99);
                if (ev.m_object_2->tangentialVelocity().y() > 0)
                {
                    y += ev.m_object_2->tangentialVelocity().y() * tick;
                }
                ev.m_object_1->shedulePosition(sad::Point2D(x, y));
                this->m_player->setYCoordinateFixed(true);
            }
            this->m_player->restOnPlatform(ev.m_object_2, v);
            this->m_player->setYCoordinateFixed(true);
        }
        else
        {
            ev.m_object_1->setCurrentAngularVelocity(0);
            ev.m_object_1->sheduleAngularVelocity(0);

            // Force sliding by offsetting objects after collision
            if (sad::p2d::collides(player_part, platform_part))
            {
                if (ev.m_object_1->willPositionChange())
                {
                    double x = ev.m_object_1->nextPosition().x();
                    double y = ev.m_object_1->position().y() + player_velocity.y() * (ctoi * 0.99);
                    if (!(this->m_player->isXCoordinateFixed()))
                    {
                        x = ev.m_object_1->position().x() + player_velocity.x() * tick + force_value.x() * tick * tick / 2.0;
                    }
                    if (ev.m_object_2->tangentialVelocity().y() < 0)
                    {
                        y += ev.m_object_2->tangentialVelocity().y() * tick;
                    }
                    ev.m_object_1->shedulePosition(sad::Point2D(x, y));
                    this->m_player->setYCoordinateFixed(true);

                }
                else
                {
                    double x = ev.m_object_1->position().x() + player_velocity.x() * tick + force_value.x() * tick * tick / 2.0;
                    double y = ev.m_object_1->position().y() + player_velocity.y() * (ctoi * 0.99);
                    if (ev.m_object_2->tangentialVelocity().y() < 0)
                    {
                        y += ev.m_object_2->tangentialVelocity().y() * tick;
                    }
                    ev.m_object_1->shedulePosition(sad::Point2D(x, y));
                    this->m_player->setYCoordinateFixed(true);
                }
            }
            else
            {
                double correction = 0;
                if ((player_part.p2() < platform_part.p1() && v.x() < 0) || (v.x() > 0))
                {
                   correction = v.x() * tick;
                }
                double x = ev.m_object_1->position().x() + player_velocity.x() * (ctoi * 0.99) + correction;
                if (ev.m_object_1->willPositionChange())
                {
                    double y = ev.m_object_1->nextPosition().y();
                    if (!(this->m_player->isYCoordinateFixed()))
                    {
                        y = ev.m_object_1->position().y() + player_velocity.y() * tick + force_value.y() * tick * tick / 2.0;
                    }
                    ev.m_object_1->shedulePosition(sad::Point2D(x, y));
                    this->m_player->setXCoordinateFixed(true);

                }
                else
                {
                    double y = ev.m_object_1->position().y() + player_velocity.y() * tick + force_value.y() * tick * tick / 2.0;
                    ev.m_object_1->shedulePosition(sad::Point2D(x, y));
                    this->m_player->setXCoordinateFixed(true);
                }
            }
            if (!willVelocityChange)
            {
                player_velocity += force_value * ev.m_time;
                ev.m_object_1->sheduleTangentialVelocity(player_velocity);
            }
            else
            {
                ev.m_object_1->sheduleTangentialVelocity(next_velocity);
            }
        }
        ev.m_object_2->setCurrentTangentialVelocity(v);
        ev.m_object_2->setCurrentAngularVelocity(0);
        ev.m_object_2->sheduleTangentialVelocity(v);
        ev.m_object_2->sheduleAngularVelocity(0);
        ev.m_object_2->shedulePosition(ev.m_object_2->position() + v * tick);
    };
    m_physics_world->addHandler("player", "platforms", collision_between_player_and_platforms);
    m_step_task->setWorld(m_physics_world);
}

Game::Game(const Game&)  // NOLINT
    : m_main_thread(NULL), m_inventory_thread(NULL), m_is_quitting(false), m_main_menu_state(Game::GMMS_PLAY),
      m_highscore(0), m_loaded_options_database{false, false}, m_loaded_game_screen(false), m_theme_playing(NULL),
      m_transition_process(NULL),
      m_inventory_node(NULL), m_inventory_popup(NULL), m_eval_context(NULL), m_physics_world(NULL),
      m_step_task(NULL), m_bounce_solver(NULL), m_is_rendering_world_bodies(false), max_level_x(0),
      m_running_tasks(0)
{
    throw std::logic_error("Not implemented");
}

Game& Game::operator=(const Game&)
{
    throw std::logic_error("Not implemented");
    // ReSharper disable once CppUnreachableCode
    return *this;
}
