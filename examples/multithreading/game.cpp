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

// ==================================== PUBLIC METHODS ====================================

Game::Game()  : m_is_quitting(false) // NOLINT
{
    m_main_thread = new threads::GameThread();
    m_inventory_thread = new threads::GameThread();
}

Game::~Game()  // NOLINT
{
    delete m_main_thread;
    delete m_inventory_thread;
}

void Game::runMainGameThread()
{
    sad::Renderer& renderer= *(m_main_thread->renderer());
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
            sad::Sprite2D* sprite = static_cast<sad::Sprite2D*>(node);
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
            sad::Label* label = static_cast<sad::Label*>(node);
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

    nodes::Background* background = new nodes::Background(true);
    scene->addNode(background);
    scene->setLayer(background, 0); 
    
    // Play animations
    sad::animations::Instance* player_walk = db->objectByName<sad::animations::Instance>("player_walk");
    sad::animations::Instance* player_walk2 = db->objectByName<sad::animations::Instance>("player_walk2");
    renderer.animations()->add(player_walk);
    renderer.animations()->add(player_walk2);

	// Pointer for the menu options
	sad::Sprite2D* choisePointer = db->objectByName<sad::Sprite2D>("PlayerPick");
	choisePointer->setMiddle(sad::Point2D(330, 275));

	enum state {
		play,
		exit
	};
	state GameState;

	renderer.controls()->addLambda(
		*sad::input::ET_KeyPress & sad::KeyUp,
		[this, choisePointer, &GameState]() -> void {
		choisePointer->setMiddle(sad::Point2D(330, 275));
		GameState = state::play;
	}
	);

	renderer.controls()->addLambda(
		*sad::input::ET_KeyPress & sad::KeyDown,
		[this, choisePointer, &GameState]() -> void {
		choisePointer->setMiddle(sad::Point2D(330, 230));
		GameState = state::exit;
	}
	);

	renderer.controls()->addLambda(
		*sad::input::ET_KeyPress & sad::Enter,
		[this, &GameState]() -> void {
		if (GameState == state::exit)
		{
		}
		else if (GameState == state::play)
		{
		}
	}
	);

    // TODO: Khomich loading database code here
    SL_LOCAL_DEBUG("Starting\n", renderer);
    renderer.controls()->addLambda(
        *sad::input::ET_Quit,
        [this]() -> void {
            if (!m_is_quitting) {
                m_is_quitting = true;
                m_inventory_thread->sendKillSignalFrom(m_main_thread);
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


    // TODO: Khomich loading database code here
    SL_LOCAL_DEBUG("Starting new renderer\n", renderer);
    m_inventory_thread->markAsRendererStarted();
    // Kill other window, if closed
    renderer.controls()->addLambda(
        *sad::input::ET_Quit,
        [this]() -> void {
            if (!m_is_quitting) {
                m_is_quitting = true;
                m_main_thread->sendKillSignalFrom(m_inventory_thread);
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
	}
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
