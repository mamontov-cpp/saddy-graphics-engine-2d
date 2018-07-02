#include "game.h"

#include "threads/gamethread.h"

#include "nodes/background.h"

#include <input/controls.h>

#include <keymouseconditions.h>

#include <sprite2d.h>
#include <label.h>

#include <db/dbdatabase.h>
#include <db/dbpopulatescenesfromdatabase.h>

#include <animations/animationsinstance.h>
#include <animations/animationscolor.h>
#include <utility>
#include <animations/animationsinstance.h>

#include <slurpjson.h>
#include <spitjson.h>

/*! A transition time, used to make transition between screens
 */
#define TRANSITION_TIME (1000)

// ==================================== PUBLIC METHODS ====================================

Game::Game()  : m_is_quitting(false), m_main_menu_state(Game::GMMS_PLAY), m_highscore(0) // NOLINT
{
    m_main_thread = new threads::GameThread();
    m_inventory_thread = new threads::GameThread();

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
}

Game::~Game()  // NOLINT
{
    delete m_main_thread;
    delete m_inventory_thread;
}

/*! A padding, that will be used in main menu between label and player choice
 */
const double padding_between_label_and_player_choice = 15;

void Game::runMainGameThread()
{
    sad::Renderer& renderer= *(m_main_thread->renderer());

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
        if (node->metaData()->name() == "sad::Sprite2D")
        {
            sad::Sprite2D* sprite = dynamic_cast<sad::Sprite2D*>(node);
            if (sprite->texture())
            {
                SL_LOCAL_DEBUG("Sprite has texture", renderer);
            }
            else
            {
                SL_LOCAL_DEBUG("Sprite has no texture", renderer);
            }
        }
        if (node->metaData()->name() == "sad::Label")
        {
            sad::Label* label = dynamic_cast<sad::Label*>(node);
            if (label->font())
            {
                SL_LOCAL_DEBUG("Label has font", renderer);
            }
            else
            {
                SL_LOCAL_DEBUG("Label has no font", renderer);
            }
        }

    }

    tryStartStartingState();

    // Set pointer for the menu options
    std::function<void(Game::MainMenuState)> put_player_pick_according_to_menu_state = [this, db](Game::MainMenuState state) {
        sad::Sprite2D* choice_pointer = db->objectByName<sad::Sprite2D>("PlayerPick");
        sad::Label* new_game_label = db->objectByName<sad::Label>(this->m_main_menu_states_to_labels[state]);
        double x = new_game_label->area().p0().x() - padding_between_label_and_player_choice - (choice_pointer->area().width() / 2.0);
        double y = (new_game_label->area().p0().y() - new_game_label->area().height() / 2.0);
        choice_pointer->setMiddle(sad::Point2D(x, y));
    };
    put_player_pick_according_to_menu_state(Game::GMMS_PLAY);

    renderer.controls()->addLambda(
        *sad::input::ET_KeyPress
        & sad::KeyUp
        & ((&m_state_machine) * sad::String("starting_screen"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this, put_player_pick_according_to_menu_state]() -> void {
            if (this->m_main_menu_state == Game::GMMS_PLAY)
            {
                this->m_main_menu_state = Game::GMMS_EXIT;
            }
            else
            {
                this->m_main_menu_state =  static_cast<Game::MainMenuState>(static_cast<int>(this->m_main_menu_state) - 1);
            }
            put_player_pick_according_to_menu_state(this->m_main_menu_state);
        }
    );

    renderer.controls()->addLambda(
        *sad::input::ET_KeyPress
        & sad::KeyDown
        & ((&m_state_machine) * sad::String("starting_screen"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this, put_player_pick_according_to_menu_state]() -> void {
            if (this->m_main_menu_state == Game::GMMS_EXIT)
            {
                this->m_main_menu_state = Game::GMMS_PLAY;
            }
            else
            {
                this->m_main_menu_state = static_cast<Game::MainMenuState>(static_cast<int>(this->m_main_menu_state) + 1);  // NOLINT(misc-misplaced-widening-cast)
            }
            put_player_pick_according_to_menu_state(this->m_main_menu_state);
        }
    );

    renderer.controls()->addLambda(
        *sad::input::ET_KeyPress & sad::Enter
        & ((&m_state_machine) * sad::String("starting_screen"))
        & ((&m_paused_state_machine) * sad::String("playing")),
        [this]() -> void {
            switch(this->m_main_menu_state)
            {
                case Game::GMMS_PLAY: this->changeScene([]()-> void { printf("Loading"); }, []() -> void { printf("On loaded"); }, []()-> void { printf("Stage loaded"); }); break;
                case Game::GMMS_OPTIONS:
                case Game::GMMS_EXIT: this->quitGame(); break;
            }
        }
    );

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

    sad::Scene* scene = renderer.scenes()[0];
    nodes::Background* background = new nodes::Background(false);
    scene->addNode(background);

    SL_LOCAL_DEBUG("Starting new renderer\n", renderer);
    m_inventory_thread->markAsRendererStarted();
    // Kill other window, if closed
    renderer.controls()->addLambda(
        *sad::input::ET_Quit,
        [this]() -> void {
            if (!m_is_quitting) {
                m_is_quitting = true;
                m_main_thread->sendKillSignalFrom(m_inventory_thread);
                sad::irrklang::Engine::eref()->stopAllSounds();
            }
        }
    );

    renderer.run();
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

    m_state_machine.enterState("starting_screen");
    m_paused_state_machine.enterState("playing");

    sad::irrklang::Sound* theme = m_inventory_thread->renderer()->tree("")->get<sad::irrklang::Sound>("main_theme");
    m_theme_playing = m_theme.play2D(theme, 1.0);
}

void Game::changeScene(std::function<void()> load_new_data, std::function<void()> on_loaded, std::function<void()> actions_after_transition)
{
    m_load_data_thread = new sad::Thread (load_new_data);
    m_load_data_thread->run();

    sad::Renderer& renderer = *(m_main_thread->renderer());

    sad::animations::Instance* darkeningScreen = this->setAnimationForScreenTransition(renderer, TRANSITION_TIME, true);
    darkeningScreen->end([this, &renderer, on_loaded, actions_after_transition]() {
        // Wait for other thread to complete
        this->m_load_data_thread->wait();
        delete this->m_load_data_thread;
        m_load_data_thread = NULL;

        on_loaded();

        sad::animations::Instance* brighteningScreen = this->setAnimationForScreenTransition(renderer, TRANSITION_TIME, false);
        brighteningScreen->end(actions_after_transition);

		renderer.animations()->add(brighteningScreen);
	});
    renderer.animations()->add(darkeningScreen);
}

sad::animations::Instance* Game::setAnimationForScreenTransition(sad::Renderer & renderer, long time, bool dark)
{
    sad::Texture* tex = new sad::Texture();
    tex->load("white_square.png", &renderer);
    tex->setRenderer(&renderer);

    sad::Sprite2D* sprite = new sad::Sprite2D();
    sprite->setTexture(tex);
    sprite->setTextureCoordinates(sad::Rect2D(0, 0, 2, 2));
    sprite->setArea(sad::Rect2D(0, 0, 800, 600));
    sprite->setColor(sad::AColor(0, 0, 0, 248));

    renderer.scenes()[renderer.scenes().size() - 1]->addNode(sprite);

    sad::animations::Color* color = new sad::animations::Color();
    unsigned int start = 0, end = 255;
    if (dark)
    {
        start = 255;
        end = 0;
    }
    color->setMinColor(sad::AColor(0, 0, 0, start));
    color->setMaxColor(sad::AColor(0, 0, 0, end));
    color->setTime(time);
    color->setLooped(false);

    sad::animations::Instance* animation = new sad::animations::Instance();
    animation->setAnimation(color);
    animation->setObject(sprite);
    animation->clearFinished();
    animation->disableStateRestoringOnFinish();
    return animation;
}


// ==================================== PRIVATE METHODS ====================================

Game::Game(const Game&)  // NOLINT
: m_main_thread(NULL), m_inventory_thread(NULL), m_is_quitting(false)
{
    throw std::logic_error("Not implemented");
}

Game& Game::operator=(const Game&)
{
    throw std::logic_error("Not implemented");
    // ReSharper disable once CppUnreachableCode
    return *this;
}
