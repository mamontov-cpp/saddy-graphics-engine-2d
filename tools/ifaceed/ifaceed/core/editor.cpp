#include <QFontComboBox>
// ReSharper disable once CppUnusedIncludeDirective
#include <QTimer>

#include "editor.h"
#include "closuremethodcall.h"
#include "gridandoffset.h"

#include <freetype/font.h>

#include <renderer.h>
#include <keymouseconditions.h>

#include <input/controls.h>

#include <pipeline/pipeline.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <db/load.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>
#include <db/dbdatabase.h>

#include <resource/tree.h>

#include "core/saddythread.h"
#include "core/synchronization.h"
#include "core/selection.h"

#include "core/borders/activeborder.h"
#include "core/borders/selectionborder.h"

#include "gui/eventfilter.h"
#include "gui/renderways.h"
#include "gui/rendereditorgrid.h"
#include "gui/rendergrids.h"
#include "gui/actions/wayactions.h"
#include "gui/mainpanelproxy.h"

#include "gui/uiblocks/uiblocks.h"
#include "gui/actions/actions.h"
#include "gui/actions/gridactions.h"

#include "typeconverters/save.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "typeconverters/load.h"
#include "typeconverters/qcolortosadcolor.h"
#include "typeconverters/qcolortosadacolor.h"
#include "typeconverters/qstringtosadstring.h"
#include "typeconverters/qlistqlistqcolortosadvectorsadvectoracolor.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "typeconverters/qrectftosadrect2d.h"
#include "typeconverters/sadcolortoqcolor.h"
#include "typeconverters/sadacolortoqcolor.h"
#include "typeconverters/sadstringtoqstring.h"
#include "typeconverters/sadvectorsadvectoracolortoqlistqlistqcolor.h"
#include "typeconverters/sadrect2dtoqrectf.h"

#include "../blockedclosuremethodcall.h"

#include "../mainpanel.h"

#include "../gui/actions/customobjectactions.h"
#include "../gui/actions/scenenodeactions.h"

Q_DECLARE_METATYPE(sad::layouts::Grid*) //-V566
Q_DECLARE_METATYPE(sad::Way*) //-V566
Q_DECLARE_METATYPE(sad::SceneNode*) //-V566
// =================== PUBLIC METHODS ===================

core::Editor::Editor()
{
    // Add message data
    m_qt_target = new core::QtTarget(this);
    sad::Renderer::ref()->log()->addTarget(m_qt_target);

    // Add small user log
    sad::log::FileTarget * t = new sad::log::FileTarget("{0}: [{1}] {3}{2}{4}", sad::log::Priority::MESSAGE);
    t->open("user.txt");
    sad::log::Log::ref()->addTarget(t);

    // Add large debug log
    t = new sad::log::FileTarget();
    t->open("full.txt");
    sad::log::Log::ref()->addTarget(t);

    m_args = nullptr;
    m_render_thread = new core::SaddyThread(this);
    m_qt_app = nullptr;
    m_current_batch_command = nullptr;
    m_history = new history::History();

    m_machine = new sad::hfsm::Machine();
    // A states for editing objects
    m_machine->addState("idle", new sad::hfsm::State(), true);
    m_machine->addState("changing_global_offset", new sad::hfsm::State(), true);
    m_machine->addState("selected", new sad::hfsm::State(), true);
    m_machine->addState("selected/moving", new sad::hfsm::State(), true);
    m_machine->addState("selected/resizing", new sad::hfsm::State(), true);
    m_machine->addState("selected/spanning/firstpoint", new sad::hfsm::State(), true);
    m_machine->addState("selected/spanning/secondpoint", new sad::hfsm::State(), true);
    m_machine->addState("adding/label", new sad::hfsm::State(), true);
    m_machine->addState("adding/sprite", new sad::hfsm::State(), true);
    m_machine->addState("adding/sprite_diagonal", new sad::hfsm::State(), true);
    m_machine->addState("adding/sprite_diagonal/point_placed", new sad::hfsm::State(), true);
    m_machine->addState("adding/customobject", new sad::hfsm::State(), true);
    m_machine->addState("adding/customobject_diagonal", new sad::hfsm::State(), true);
    m_machine->addState("adding/customobject_diagonal/point_placed", new sad::hfsm::State(), true);
    // A states for editing ways
    m_machine->addState("ways/idle", new sad::hfsm::State(), true);
    m_machine->addState("ways/selected", new sad::hfsm::State(), true);
    m_machine->addState("ways/selected/moving", new sad::hfsm::State(), true);
    m_machine->addState("picking_simple_movement_point", new sad::hfsm::State(), true);
    // A state for adding, moving and resizing layout
    m_machine->addState("layouts/adding", new sad::hfsm::State(), true);
    m_machine->addState("layouts/adding/firstpoint", new sad::hfsm::State(), true);
    m_machine->addState("layouts/adding/secondpoint", new sad::hfsm::State(), true);
    m_machine->addState("layouts/moving", new sad::hfsm::State(), true);
    m_machine->addState("layouts/resizing", new sad::hfsm::State(), true);

    // We create grids more early to ensure binding events on it
    m_render_grids = new gui::RenderGrids(this);   
    
    // Disable rendering resize hotspots when editing objects.
    // Note that this is done to avoid interlocked non-atomic changes between machine and grids
    const char* editing_states[] = {
        "selected/moving",
        "selected/resizing",
        "selected/spanning/firstpoint",
        "selected/spanning/secondpoint",
        "adding/label", 
        "adding/sprite", 
        "adding/sprite_diagonal", 
        "adding/sprite_diagonal/point_placed", 
        "adding/customobject", 
        "adding/customobject_diagonal", 
        "adding/customobject_diagonal/point_placed", 
        "ways/selected/moving", 
        "picking_simple_movement_point", 
        "layouts/adding", 
        "layouts/adding/firstpoint", 
        "layouts/adding/secondpoint", 
        nullptr
    };
    size_t i = 0;
    while(editing_states[i])
    {
        sad::hfsm::State* s = m_machine->state(editing_states[i]);
        s->addEnterHandler(m_render_grids, &gui::RenderGrids::disableResizeHotspots);
        s->addLeaveHandler(m_render_grids, &gui::RenderGrids::enableResizeHotspots);
        ++i;
    }

    m_machine->enterState("idle");

    m_shared = new core::Shared();
    m_shared->setEditor(this);

    m_active_border = new core::borders::ActiveBorder(m_shared);
    m_selection_border = new core::borders::SelectionBorder(m_shared);
    
    m_synchronization = new core::Synchronization();

    sad::Renderer::ref()->controls()->add(*sad::input::EventType::ET_KeyPress & sad::KeyboardKey::Z & sad::SpecialKey::HoldsControl, this, &core::Editor::undo);
    sad::Renderer::ref()->controls()->add(*sad::input::EventType::ET_KeyPress & sad::KeyboardKey::R & sad::SpecialKey::HoldsControl, this, &core::Editor::redo);

    sad::Renderer::ref()->pipeline()->append(m_selection_border);
    sad::Renderer::ref()->pipeline()->append(m_active_border);
    m_render_ways = new gui::RenderWays(this);
    m_render_editor_grid  = new gui::RenderEditorGrid();
    sad::Renderer::ref()->pipeline()->prepend(m_render_editor_grid);
    sad::Renderer::ref()->pipeline()->append(m_render_ways);
    sad::Renderer::ref()->pipeline()->append(m_render_grids);

    m_selection = new core::Selection();
    m_selection->setEditor(this);

    // Fill conversion table with converters
    this->initConversionTable();

    m_ui_blocks = new gui::uiblocks::UIBlocks();

    m_actions = new gui::actions::Actions();
    m_actions->setEditor(this);

    m_panel_proxy = new gui::MainPanelProxy();
    m_panel_proxy->setEditor(this);

    m_fast_mode_counter = 0;

}
core::Editor::~Editor()
{
    delete m_args;
    delete m_render_thread;
    delete m_parsed_args;
    delete m_history;
    delete m_shared;
    delete m_machine;
    delete m_synchronization;
    delete m_selection;
    delete m_main_window;
    delete m_qt_app;
    delete m_ui_blocks;
    delete m_actions;
    delete m_panel_proxy;
}

void core::Editor::init(int argc,char ** argv)
{
    // Create and parse command line arguments
    m_args = new sad::cli::Args(argc, argv);
    m_parsed_args = new sad::cli::Parser();
    m_parsed_args->addSingleValuedOption("resources");
    m_parsed_args->addSingleValuedOption("width");
    m_parsed_args->addSingleValuedOption("height");
    m_parsed_args->addFlag("debug");
    m_parsed_args->parse(argc, const_cast<const char **>(argv));
    
#ifdef HAVE_QT5
    QCoreApplication::addLibraryPath(".");
#endif

    m_qt_app = new QApplication(m_args->count(), m_args->arguments());

    // This thread only runs qt event loop. SaddyThread runs only event loop of renderer of Saddy.
    m_synchronization->startSynchronizationWithSaddyThread();
    m_render_thread->start();
    // Wait for Saddy's window to show up
    m_synchronization->waitForSaddyThread();
    this->runQtEventLoop();
    m_render_thread->wait();
}

MainPanel* core::Editor::panel() const
{
    return m_main_window;
}

sad::hfsm::Machine* core::Editor::machine() const
{
    return m_machine;
}

core::Shared* core::Editor::shared() const
{
    return m_shared;
}

sad::cli::Parser * core::Editor::parsedArgs() const
{
    return m_parsed_args;
}

history::History* core::Editor::history() const
{
    return m_history;
}

QApplication* core::Editor::app() const
{
    return m_qt_app;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
sad::Renderer* core::Editor::renderer() const
{
    return sad::Renderer::ref();
}

core::Synchronization* core::Editor::synchronization() const
{
    return m_synchronization;
}

core::borders::ActiveBorder* core::Editor::activeBorder() const
{
    return m_active_border;
}

core::borders::SelectionBorder* core::Editor::selectionBorder() const
{
    return m_selection_border;
}

gui::RenderWays* core::Editor::renderWays() const
{
    return m_render_ways;
}

gui::RenderGrids* core::Editor::renderGrids() const
{
    return m_render_grids;
}

gui::RenderEditorGrid* core::Editor::renderEditorGrid() const
{
    return m_render_editor_grid;
}

core::Selection* core::Editor::selection() const
{
    return m_selection;
}

sad::animations::Factory* core::Editor::animationFactory() const
{
    return const_cast<sad::animations::Factory*>(&m_animation_factory);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void core::Editor::quit()
{
    sad::Renderer::ref()->quit();
}

void core::Editor::emitClosure(sad::ClosureBasic * closure)
{
    emit closureArrived(closure);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void core::Editor::cleanupBeforeAdding()
{
    if (this->machine()->isInState("adding"))
    {
        sad::Renderer::ref()->lockRendering();

        sad::SceneNode* node = this->shared()->activeObject();
        this->shared()->setActiveObject(nullptr);
        node->scene()->remove(node);

        sad::Renderer::ref()->unlockRendering();

        m_actions->customObjectActions()->clearCustomObjectPropertiesTable();
    }
}

bool core::Editor::isNodeSelected(sad::SceneNode* node) const
{
    return node  == m_shared->selectedObject() && m_machine->isInState("selected");
}

void core::Editor::enteredIdleState()
{
    m_main_window->highlightIdleState();
    this->emitClosure( bind(m_actions->customObjectActions(), &gui::actions::CustomObjectActions::clearCustomObjectPropertiesTable));
}

static const size_t CoreEditorEditingStatesCount = 10;

static sad::String CoreEditorEditingStates[CoreEditorEditingStatesCount] = {
    sad::String("adding"),
    sad::String("selected/moving"),
    sad::String("selected/resizing"),
    sad::String("selected/spanning/firstpoint"),
    sad::String("selected/spanning/secondpoint"),
    sad::String("ways/selected/moving"),
    sad::String("picking_simple_movement_point"),
    sad::String("layouts/adding"),
    sad::String("layouts/moving"),
    sad::String("layouts/resizing")
};

bool core::Editor::isInEditingState() const
{
    bool result = false;
    for (size_t i = 0; i < CoreEditorEditingStatesCount; ++i)
    {
        result = result || m_machine->isInState(CoreEditorEditingStates[i]);
    }
    return result;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void core::Editor::cleanDatabase()
{
    m_machine->enterState("idle");
    m_shared->setSelectedObject(nullptr);
    m_shared->setSelectedWay(nullptr);
    m_shared->setSelectedDialogue(nullptr);
    m_shared->setActiveObject(nullptr);
    m_shared->setSelectedAnimation(nullptr);
    m_shared->setSelectedInstance(nullptr);
    m_shared->setSelectedGroup(nullptr);
    m_shared->setActiveGrid(nullptr);
    m_shared->setSelectedGrid(nullptr);
    m_history->clear();
    m_main_window->clearDatabaseProperties();
    sad::Renderer::ref()->clear();
    m_main_window->UI()->lstSceneObjects->clear();
    m_main_window->UI()->lstScenes->clear();
    m_main_window->UI()->lstWays->clear();
    m_main_window->UI()->lstWayPoints->clear();
    m_main_window->UI()->lstDialogues->clear();
    m_main_window->UI()->lstPhrases->clear();
    m_main_window->UI()->lstAnimations->clear();
    m_main_window->UI()->lstAnimationInstances->clear();
    m_main_window->UI()->lstCompositeCandidates->clear();
    m_main_window->UI()->lstCompositeList->clear();
    m_main_window->UI()->lstLayoutGridList->clear();
    m_main_window->UI()->cmbAnimationInstanceObject->clear();
    m_main_window->UI()->cmbAnimationInstanceAnimationFromDatabase->clear();
    m_main_window->UI()->cmbWayAnimationInstanceWay->clear();
    m_main_window->UI()->cmbWayAnimationWay->clear();
    m_main_window->UI()->lstAnimationInstances->clear();
    m_main_window->UI()->lstAnimationsGroupAllAnimations->clear();
    m_main_window->UI()->lstAnimationsGroupInGroup->clear();
    m_main_window->UI()->lstLayoutGridList->clear();
    m_actions->gridActions()->clearGridCellsBrowser();

    // Cleanup layout view
    QGridLayout* table = new QGridLayout();
    QWidget* w = new QWidget();
    w->setLayout(table);
    m_main_window->UI()->tblLayoutCells->setWidget(w);

    m_main_window->clearDatabaseProperties();
    sad::Renderer::ref()->removeDatabase("");
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void core::Editor::reportResourceLoadingErrors(
        sad::Vector<sad::resource::Error *> & errors,
        const sad::String& name
)
{
    sad::String errorlist = sad::resource::format(errors);
    sad::String resultmessage = "There was errors while loading ";
    resultmessage += name;
    resultmessage += ":\n";
    resultmessage += errorlist;
    sad::util::free(errors);
    errors.clear();
    SL_FATAL(resultmessage);
}

bool core::Editor::isInObjectEditingState() const
{
    return m_main_window->UI()->tabTypes->currentIndex() == 0;
}

bool core::Editor::isInWaysEditingState() const
{
    return m_main_window->UI()->tabTypes->currentIndex() == 1;
}

bool core::Editor::isInGridEditingState() const
{
    return m_main_window->UI()->tabTypes->currentIndex() == 0
        && m_main_window->UI()->tabObjects->currentIndex() == 7;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void core::Editor::tryEnterObjectEditingState()
{
    if (this->isInEditingState())
    {
        return;
    }
    this->m_machine->enterState("idle");
    m_shared->setSelectedObject(nullptr);
    m_shared->setActiveObject(nullptr);
    m_shared->setSelectedWay(nullptr);
    m_shared->setSelectedGrid(nullptr);

    int currentrow = m_main_window->UI()->lstSceneObjects->currentRow();
    if (currentrow > -1)
    {
        QVariant v = m_main_window->UI()->lstSceneObjects->item(currentrow)->data(Qt::UserRole);
        m_shared->setSelectedObject(v.value<sad::SceneNode*>());
    }
    currentrow = m_main_window->UI()->lstLayoutGridList->currentRow();
    if (currentrow > - 1)
    {
        QVariant v = m_main_window->UI()->lstLayoutGridList->item(currentrow)->data(Qt::UserRole);
        m_shared->setSelectedGrid(v.value<sad::layouts::Grid*>());
    }
    invoke_blocked(m_main_window->UI()->tabTypes, &QTabWidget::setCurrentIndex, 0);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void core::Editor::tryEnterWayEditingState()
{
    if (this->isInEditingState())
    {
        return;
    }
    this->m_machine->enterState("idle");
    this->m_machine->enterState("ways/idle");
    m_shared->setSelectedObject(nullptr);
    m_shared->setActiveObject(nullptr);
    m_shared->setSelectedWay(nullptr);	
    m_shared->setSelectedGrid(nullptr);
    invoke_blocked(m_main_window->UI()->tabTypes, &QTabWidget::setCurrentIndex, 1);
    if (m_main_window->UI()->lstWays->currentRow() >= 0)
    {
        this->m_machine->enterState("ways/selected");
        int row = m_main_window->UI()->lstWays->currentRow();
        sad::Way* way = m_main_window->UI()->lstWays->item(row)->data(Qt::UserRole).value<sad::Way*>();
        m_shared->setSelectedWay(way);
        this->actions()->wayActions()->wayChanged(row);
    }
}

void core::Editor::setCurrentBatchCommand(history::BatchCommand* c)
{
    m_current_batch_command = c;
}

history::BatchCommand* core::Editor::currentBatchCommand() const
{
    return m_current_batch_command;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
bool core::Editor::isDatabaseEmpty() const
{
    bool result = true;
    sad::db::Database* db = sad::Renderer::ref()->database("");
      for(sad::db::Database::Properties::const_iterator it = db->begin();
        it != db->end();
        ++it)
    {
        // Skip palette
        if (it.key() != "palette")
        {
            result = false;
        }
    }
    result = result && db->tablesAreEmpty();
    return result; 
}

gui::uiblocks::UIBlocks* core::Editor::uiBlocks() const
{
    return m_ui_blocks;
}

gui::actions::Actions* core::Editor::actions() const
{
    return m_actions;
}

gui::MainPanelProxy* core::Editor::panelProxy() const
{
    return m_panel_proxy;
}

QWidget* core::Editor::panelAsWidget() const
{
    return m_main_window;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void core::Editor::addToHistory(history::Command* c, bool from_editor)
{
    if (from_editor)
    {
        this->history()->add(c);
    }
    else
    {
        this->currentBatchCommand()->add(c);
    }
}

unsigned int core::Editor::fastModeCounter() const
{
    return m_fast_mode_counter;
}

void core::Editor::incrementFastModeCounter()
{
    ++m_fast_mode_counter;
}

void core::Editor::tryUpdateParentGridForNode(sad::SceneNode* node) const
{
    this->actions()->sceneNodeActions()->tryUpdateParentGridForNode(node);
}

core::Editor::NodePickingDialog  core::Editor::dialogForSelectingNode(
    QWidget* parent,
    const QString& window_title,
    const QVector<QPair<QString, unsigned long long> >& pairs
)
{
    double starting_offset= 5;
    double dialog_size_x = 320, dialog_size_y = 240;  
    double button_height = 25;
    QDialog* dlg = new QDialog(parent);
    dlg->setObjectName("nodepicker");
    dlg->resize(dialog_size_x, dialog_size_y);
    dlg->setMinimumSize(QSize(dialog_size_x, dialog_size_y));
    dlg->setMaximumSize(QSize(dialog_size_x, dialog_size_y));
    dlg->setWindowTitle(window_title);
        
    QListWidget* list = new QListWidget(dlg);
    list->setGeometry(starting_offset, starting_offset, dialog_size_x - starting_offset * 2, dialog_size_y - button_height - starting_offset * 4);
    for(size_t i = 0; i < pairs.size(); i++)
    {
        QListWidgetItem* item = new QListWidgetItem(pairs[i].first);
        item->setData(Qt::UserRole, QVariant(pairs[i].second));
        list->addItem(item);
    }
    list->setCurrentRow(0);

    QPushButton* btn_ok = new QPushButton(dlg);
    btn_ok->setText("Ok");
    btn_ok->setGeometry(starting_offset, dialog_size_y - button_height - starting_offset, dialog_size_x / 2 - 2 * starting_offset, button_height);

    QPushButton* btn_cancel = new QPushButton(dlg);
    btn_cancel->setText("Cancel");
    btn_cancel->setGeometry(dialog_size_x / 2 + starting_offset, dialog_size_y - button_height - starting_offset, dialog_size_x / 2 - 2 * starting_offset, button_height);

    connect(btn_ok, SIGNAL(clicked()), dlg, SLOT(accept()));
    connect(btn_cancel, SIGNAL(clicked()), dlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(dlg);

    core::Editor::NodePickingDialog result;
    result.Dialog = dlg;
    result.NodeList = list;
    return result;
}

// =================== PUBLIC SLOTS METHODS ===================

void core::Editor::start()
{
    SL_SCOPE("core::Editor::start()");
    bool mustquit = false;

    // Try load icons resources
    sad::Renderer::ref()->addTree("icons", new sad::resource::Tree());
    sad::Renderer::ref()->tree("icons")->factory()->registerResource<sad::freetype::Font>();
    sad::Vector<sad::resource::Error * > errors;
    errors = sad::Renderer::ref()->tree("icons")->loadFromFile(ICONS_PATH);
    if (errors.size())
    {
        mustquit = true;
        this->reportResourceLoadingErrors(errors, ICONS_PATH);
    }

    // Try load specified resources, if need to
    sad::Renderer::ref()->tree("")->factory()->registerResource<sad::freetype::Font>();
    sad::Renderer::ref()->tree("")->setStoreLinks(true);
    sad::Maybe<sad::String> maybefilename = this->parsedArgs()->single("resources");
    if (maybefilename.exists() && this->parsedArgs()->specified("resources"))
    {
        errors = sad::Renderer::ref()->tree("")->loadFromFile(maybefilename.value());
        if (errors.size())
        {
            mustquit = true;
            this->reportResourceLoadingErrors(errors, maybefilename.value());
        }
        else
        {
            this->m_main_window->updateResourceViews();
        }
    }
    else
    {
        this->m_main_window->toggleEditingButtons(false);
    }
    
    bool database_loaded = false;
    if (this->m_main_window->isEditingEnabled() && this->parsedArgs()->defaultOption().exists() && !mustquit)
    {		
        // Try load database	
        sad::String value = this->parsedArgs()->defaultOption().value();
        sad::db::Database* tmp = new sad::db::Database();
        tmp->setRenderer(sad::Renderer::ref());
        tmp->setDefaultTreeName("");
        if (tmp->loadFromFile(value, sad::Renderer::ref()))
        {
            this->shared()->setFileName(STD2QSTRING(value.c_str()));
            sad::Renderer::ref()->addDatabase("", tmp);
            database_loaded = true;
        }
        else
        {
            delete tmp;
            mustquit = true;
            SL_FATAL("Failed to load database");
        }
    }

    // If no database loaded, init default database, add a palette to it.
    if (database_loaded == false)
    {
        // Create a default database
        sad::db::Database * db = new sad::db::Database();
        // Default database has empty name
        sad::Renderer::ref()->addDatabase("", db);
    }
    this->m_main_window->viewDatabase();

    if (mustquit)
    {
         QTimer::singleShot(0, this->panel(), SLOT(close()));
    }
    else
    {
        m_render_ways->enable();
    }
}

void core::Editor::undo()
{
    if (m_shared->isAnyKindOfAnimationIsRunning() == false)
    {
        m_history->rollback(this);
    }
}

void core::Editor::redo()
{
    if (m_shared->isAnyKindOfAnimationIsRunning() == false)
    {
        m_history->commit(this);
    }
}

void core::Editor::clearFastModeCounter()
{
    m_fast_mode_counter = 0;
}

// =================== PROTECTED METHODS ===================

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void core::Editor::initConversionTable()
{
    sad::db::ConversionTable::ref()->add(
        "sad::Color", 
        "QColor", 
        new core::typeconverters::SadColorToQColor()
    );
    sad::db::ConversionTable::ref()->add(
        "sad::AColor", 
        "QColor", 
        new core::typeconverters::SadAColorToQColor()
    );
    sad::db::ConversionTable::ref()->add(
        "sad::String", 
        "QString", 
        new core::typeconverters::SadStringToQString()
    );
    sad::db::ConversionTable::ref()->add(
        "sad::Vector<sad::Vector<sad::AColor> >", 
        "QList<QList<QColor> >", 
        new core::typeconverters::SadVectorSadVectorToAColorToQListQListQColor()
    );
    sad::db::ConversionTable::ref()->add(
        "sad::Rect2D",
        "QRectF",
        new core::typeconverters::SadRect2DToQRectF()
    );
    sad::db::ConversionTable::ref()->add(
        "QColor", 
        "sad::Color", 		
        new core::typeconverters::QColorToSadColor()
    );
    sad::db::ConversionTable::ref()->add(
        "QColor", 		
        "sad::AColor", 
        new core::typeconverters::QColorToSadAColor()
    );
    sad::db::ConversionTable::ref()->add(
        "QString",
        "sad::String",  
        new core::typeconverters::QStringToSadString()
    );
    sad::db::ConversionTable::ref()->add(
        "QList<QList<QColor> >", 
        "sad::Vector<sad::Vector<sad::AColor> >", 
        new core::typeconverters::QListQListQColorToSadVectorSadVectorToAColor()
    );
    sad::db::ConversionTable::ref()->add(
        "sad::Rect2D",
        "QRectF",
        new core::typeconverters::SadRect2DToQRectF()
    );
}

void core::Editor::saddyQuitSlot()
{
    if (m_quit_reason == core::QuitReason::QR_NOTSET) {
        m_quit_reason = core::QuitReason::QR_SADDY;
        QTimer::singleShot(0,this,SLOT(onQuitActions()));
    }
}

// =================== PROTECTED SLOTS METHODS ===================

void core::Editor::runQtEventLoop()
{
    m_main_window = new MainPanel();
    m_ui_blocks->init(m_main_window);
    m_main_window->setEditor(this);

    gui::actions::GridActions* ga = m_actions->gridActions();

    m_machine->state("idle")->addEnterHandler(this, &core::Editor::enteredIdleState);
    m_machine->state("selected")->addEnterHandler(m_main_window, &MainPanel::highlightSelectedState);
    m_machine->state("selected")->addEnterHandler(m_actions->sceneNodeActions(), &gui::actions::SceneNodeActions::updateUIForSelectedSceneNode);
    m_machine->state("adding/label")->addEnterHandler(m_main_window, &MainPanel::highlightLabelAddingState);
    m_machine->state("ways/idle")->addEnterHandler(this, &core::Editor::enteredIdleState);
    m_machine->state("ways/selected")->addEnterHandler(m_main_window, &MainPanel::highlightSelectedState);
    m_machine->state("layouts/adding")->addEnterHandler(ga, &gui::actions::GridActions::highlightAddingState);
    m_machine->state("layouts/adding/firstpoint")->addEnterHandler(ga, &gui::actions::GridActions::highlightPlaceFirstPointState);
    m_machine->state("layouts/adding/secondpoint")->addEnterHandler(ga, &gui::actions::GridActions::highlightPlaceSecondPointState);
    m_machine->state("layouts/moving")->addEnterHandler(ga, &gui::actions::GridActions::highlightMovingState);
    m_machine->state("layouts/resizing")->addEnterHandler(ga, &gui::actions::GridActions::highlightResizingState);

    // This should be called this explicitly,
    // because state machine entered idle state
    // before we bind a handler on it
    m_main_window->highlightIdleState();

    sad::Renderer::ref()->controls()->add(*sad::input::EventType::ET_MouseMove, m_main_window, &MainPanel::updateMousePosition);

    gui::EventFilter* filter = new gui::EventFilter();
    filter->setEditor(this);
    QCoreApplication::instance()->installEventFilter(filter);

    QObject::connect(
        this->m_qt_app,
        SIGNAL(lastWindowClosed()),
        this,
        SLOT(qtQuitSlot()));
    this->m_main_window->show();

    QObject::connect(
        this, 
        SIGNAL(closureArrived(sad::ClosureBasic*)), 
        this, 
        SLOT(runClosure(sad::ClosureBasic*))
    );
    m_qt_target->enable();
    QTimer::singleShot(0, this, SLOT(start()));
    m_qt_app->exec();
    m_qt_target->disable();
}

void core::Editor::runSaddyEventLoop() 
{
    m_quit_reason = core::QuitReason::QR_NOTSET;
    sad::Renderer::ref()->run();
    // Quit reason can be set by main thread, when window is closed
    if (m_quit_reason == core::QuitReason::QR_NOTSET)
        this->saddyQuitSlot();
}

void core::Editor::qtQuitSlot()
{
    if (m_quit_reason == core::QuitReason::QR_NOTSET) {
        m_quit_reason = core::QuitReason::QR_QTWINDOW;
        this->onQuitActions();
    }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void core::Editor::onQuitActions()
{
    if (m_quit_reason == core::QuitReason::QR_SADDY) {
        this->m_main_window->close();
        GridAndOffsets* gao = this->m_main_window->gridAndOffset();
        if (gao->isVisible())
        {
            gao->close();
        }
    }
    if (m_quit_reason == core::QuitReason::QR_QTWINDOW) {
        sad::Renderer::ref()->quit();
    }
}

void core::Editor::runClosure(sad::ClosureBasic * closure)
{
    closure->run();
    delete closure;
}

