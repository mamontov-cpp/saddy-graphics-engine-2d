#include "mainpanel.h"
#include "gridandoffset.h"
#include "blockedclosuremethodcall.h"
#include "reloadfilelist.h"
#include "qstdstring.h"

#include "../scripting/database/databasebindings.h"


#include "core/editor.h"
#include "core/shared.h"
#include "core/selection.h"

#include "gui/actions/actions.h"
#include "gui/actions/sceneactions.h"
#include "gui/actions/scenenodeactions.h"
#include "gui/actions/labelactions.h"
#include "gui/actions/sprite2dactions.h"
#include "gui/actions/customobjectactions.h"
#include "gui/actions/animationactions.h"
#include "gui/actions/animationinstanceactions.h"
#include "gui/actions/animationgroupactions.h"
#include "gui/actions/dialogueactions.h"
#include "gui/actions/wayactions.h"
#include "gui/actions/gridactions.h"

#include "gui/rendergrids.h"
#include "gui/rendereditorgrid.h"

#include "core/borders/selectionborder.h"

#include "history/database/newproperty.h"

#include "gui/codeedit/highlighter.h"
#include "gui/codeedit/completer.h"

#include "gui/layouts/layoutcelledit.h"

#include "scripting/scripting.h"

#include <keymouseconditions.h>
#include <keycodes.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>
#include <db/dbdatabase.h>
#include <db/dbtable.h>
#include <db/dbstoredproperty.h>
#include <db/dbpopulatescenesfromdatabase.h>

#include <resource/tree.h>
#include <freetype/font.h>

#include <QFileDialog>
// ReSharper disable once CppUnusedIncludeDirective
#include <QHeaderView>
#include <QStringListModel>
#include <QLinkedList>
// ReSharper disable once CppUnusedIncludeDirective
#include <QSet>

// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>

#include "closuremethodcall.h"

Q_DECLARE_METATYPE(sad::animations::Group*) //-V566

//====================  PUBLIC METHODS HERE ====================

MainPanel::MainPanel(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags), m_selfchanged(false)
{
    ui.setupUi(this);

    this->fillDatabasePropertyTypesCombo();

    ui.tabTypes->setCurrentIndex(0);
    ui.tabObjects->setCurrentIndex(0);

    ui.tabAnimationTypes->setCurrentIndex(0);

    ui.twDatabaseProperties->setColumnCount(3);
    const int header_padding = 12;  // 12 is  a padding for header
    double width = ui.twDatabaseProperties->width() - ui.twDatabaseProperties->verticalHeader()->width();
    ui.twDatabaseProperties->setColumnWidth(2, width / 6 - header_padding);
    ui.twDatabaseProperties->setColumnWidth(1, width / 2); 
    ui.twDatabaseProperties->horizontalHeader()->hide();


    width = ui.twCustomObjectProperties->width() - ui.twCustomObjectProperties->verticalHeader()->width();
    ui.twCustomObjectProperties->setColumnCount(2);
    ui.twCustomObjectProperties->setColumnWidth(0, width / 2);
    ui.twCustomObjectProperties->setColumnWidth(1, width / 2 - header_padding * 3.5);
    ui.twCustomObjectProperties->horizontalHeader()->hide();

    QComboBox* list[] = {
        ui.cmbLabelBreakText,
        ui.cmbLabelOverflowStrategy,
        ui.cmbLabelTextEllipsis,
        ui.cmbLabelOverflowStrategyForLines,
        ui.cmbLabelTextEllipsisForLines,
        NULL
    };
    size_t i = 0; 
    while(list[i] != 0)
    {
        list[i]->setCurrentIndex(0);
        i++;
    }

    ui.txtLabelText->setPlainText("Test");

    QCompleter * c = new QCompleter();
    c->setModel(new QStringListModel());
    ui.txtFontListList->setCompleter(c);

     c = new QCompleter();
    c->setModel(new QStringListModel());
    ui.txtOptionListList->setCompleter(c);

    c = new QCompleter();
    c->setModel(new QStringListModel());
    ui.txtTextureCoordinatesList->setCompleter(c);

    m_scripting = new scripting::Scripting();
    
    // Set tab size to 4
    QFont editor_font = ui.txtConsoleCode->currentFont();
    const int tabStop = 4;  // 4 characters

    QFontMetrics editor_font_width(editor_font);
    ui.txtConsoleCode->setTabStopWidth(tabStop * editor_font_width.width(' '));

    // Set background colors for palette
    ui.cwColorStartingColor->setBackgroundColor(QColor(255, 0, 0));
    ui.cwColorEndingColor->setBackgroundColor(QColor(255, 0, 0));

    const char* easing_function_types[] = {
        "Linear",
        "InSine",
        "OutSine",
        "InOutSine",
        "InQuad",
        "OutQuad",
        "InOutQuad",
        "InCubic",
        "OutCubic",
        "InOutCubic",
        "InQuart",
        "OutQuart",
        "InOutQuart",
        "InQuint",
        "OutQuint",
        "InOutQuint",
        "InExpo",
        "OutExpo",
        "InOutExpo",
        "InCirc",
        "OutCirc",
        "InOutCirc",
        "InElastic",
        "OutElastic",
        "InOutElastic",
        "InBack",
        "OutBack",
        "InOutBack",
        "InBounce",
        "OutBounce",
        "InOutBounce",
        NULL
    };
    i = 0;
    while(easing_function_types[i]) {        
        ui.cmbAnimationEasingType->addItem(easing_function_types[i]);
        ++i;
    }

    m_offsets_window = new GridAndOffsets(NULL);

    scripting::database::initializeInvisiblePropertiesList();
}


MainPanel::~MainPanel()
{
    delete m_scripting;
    for(sad::PtrHash<sad::String, gui::table::Delegate>::iterator it = m_property_delegates.begin();
        it != m_property_delegates.end();
        ++it)
    {
        it.value()->delRef();
    }
    m_property_delegates.clear();
    delete m_offsets_window;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::toggleEditingButtons(bool enabled)
{
    const int affectedpushbuttonscount = 53;
    QPushButton* affectedpushbuttons[affectedpushbuttonscount] = {
        ui.btnReloadResources,
        ui.btnUndo,
        ui.btnRedo,

        ui.btnDatabaseSave,
        ui.btnDatabaseLoad,
        ui.btnDatabasePropertiesAdd,

        ui.btnScenesMoveFront,
        ui.btnScenesMoveBack,
        ui.btnSceneDelete,

        ui.btnSceneAdd,
        ui.btnSceneNodeMoveFront,
        ui.btnSceneNodeMoveBack,

        ui.btnSceneClear,
        ui.btnSceneNodeDelete,
        ui.btnSceneNodeSpanBetweenTwoPoints,
        ui.btnLabelAdd,

        ui.btnSpriteMakeBackground,
        ui.btnSpriteAdd,
        ui.btnCustomObjectAdd,

        ui.btnWayAdd,
        ui.btnWayRemove,
        
        ui.btnWayPointAdd,
        ui.btnWayPointRemove,
        ui.btnWayPointMoveBack,
        ui.btnWayPointMoveFront,

        ui.btnDialogueAdd,
        ui.btnDialogueRemove,

        ui.btnPhraseAdd,
        ui.btnPhraseRemove,
        ui.btnPhraseMoveBack,
        ui.btnPhraseMoveFront,

        ui.btnAnimationsAdd,
        ui.btnAnimationsRemove,
        ui.btnAnimationsStart,
        ui.btnAnimationsCancel,

        ui.btnCompositeAnimationAddToList,
        ui.btnCompositeAnimationRemoveFromList,
        ui.btnCompositeAnimationMoveBack,
        ui.btnCompositeAnimationMoveFront,

        ui.btnAnimationsInstanceAdd,
        ui.btnAnimationsInstanceRemove,
        ui.btnAnimationsInstanceStart,
        ui.btnAnimationsInstanceCancel,

        ui.btnAnimationsGroupAdd,
        ui.btnAnimationsGroupRemove,
        ui.btnAnimationsGroupStart,
        ui.btnAnimationsGroupCancel,
        ui.btnAnimationsGroupRemoveFromList,
        ui.btnAnimationsGroupAddToList,

        ui.btnConsoleRun,

        ui.btnLayoutAdd,
        ui.btnLayoutAddByStretching,
        ui.btnLayoutRemove        
    };
    for(int i = 0; i < affectedpushbuttonscount; i++)
    {
        affectedpushbuttons[i]->setEnabled(enabled);
    }
    ui.cbLayoutShow->setEnabled(enabled);
}

bool MainPanel::isEditingEnabled() const
{
    return ui.btnLabelAdd->isEnabled() 
        && ui.btnSpriteAdd->isEnabled()
        && ui.btnCustomObjectAdd->isEnabled();
}

void MainPanel::setEditor(core::Editor* editor)
{  
    m_editor = editor; 


    m_offsets_window->setProxy(m_editor->panelProxy());

    m_scripting->setEditor(editor);
    this->initConsoleAutocompletion();


    sad::hfsm::Machine* m = editor->machine();
    sad::String cgo = "changing_global_offset";

    sad::String la = "adding/label";
    sad::String ssa = "adding/sprite";
    sad::String sda = "adding/sprite_diagonal";
    sad::String sdap = "adding/sprite_diagonal/point_placed";
    sad::String coa = "adding/customobject";
    sad::String coad = "adding/customobject_diagonal";
    sad::String coadp = "adding/customobject_diagonal/point_placed";

    sad::String i = "idle";

    sad::String s = "selected";
    sad::String sm = "selected/moving";
    sad::String sr = "selected/resizing";
    sad::String ssfp = "selected/spanning/firstpoint";
    sad::String sssp = "selected/spanning/secondpoint";

    sad::String wi = "ways/idle";
    sad::String ws = "ways/selected";
    sad::String wsm = "ways/selected/moving";


    sad::String psmp = "picking_simple_movement_point";

    sad::String ladd = "layouts/adding";
    sad::String lmove = "layouts/moving";
    sad::String lsfp = "layouts/adding/firstpoint";
    sad::String lssp = "layouts/adding/secondpoint";
    sad::String lrp = "layouts/resizing";	

    gui::actions::SceneNodeActions* sn_actions = m_editor->actions()->sceneNodeActions();
    gui::actions::SceneActions* s_actions = m_editor->actions()->sceneActions();
    gui::actions::CustomObjectActions* co_actions = m_editor->actions()->customObjectActions();
    gui::actions::AnimationActions* a_actions = m_editor->actions()->animationActions();
    gui::actions::LabelActions* l_actions = m_editor->actions()->labelActions();
    gui::actions::Sprite2DActions* sp_actions = m_editor->actions()->sprite2DActions();
    gui::actions::DialogueActions* d_actions = m_editor->actions()->dialogueActions();
    gui::actions::AnimationInstanceActions* ai_actions = m_editor->actions()->instanceActions();
    gui::actions::AnimationGroupActions* ag_actions = m_editor->actions()->groupActions();
    gui::actions::WayActions* w_actions = m_editor->actions()->wayActions();    
    gui::actions::GridActions* ga_actions = m_editor->actions()->gridActions();


    // A bindings for idle state
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * i),
        m_editor->selection(),
        &core::Selection::cancelGridSelectionOrQuit
    );

    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * i),
        m_editor->selection(),
        &core::Selection::trySelect
    );

    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & (m * i),
        this,
        &MainPanel::onKeyPressedInIdleState
    );

    // A bindings for changing global offset
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseLeave & (m * cgo),
        this,
        &MainPanel::onWindowLeaveWhenChangingGlobalOffset
    );

    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * cgo),
        this,
        &MainPanel::onMouseMoveWhenChangingGlobalOffset
    );

    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseRelease & (m * cgo),
        this,
        &MainPanel::onMouseReleaseWhenChangingGlobalOffset
    );


    // A bindings for moving object
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * sm),
        sn_actions,
        &gui::actions::SceneNodeActions::moveObject
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseRelease & (m * sm),
        sn_actions,
        &gui::actions::SceneNodeActions::commitObjectMoving
    );

    // A bindings for resizing object
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * sr),
        sn_actions,
        &gui::actions::SceneNodeActions::resizeObject
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseRelease & (m * sr),
        sn_actions,
        &gui::actions::SceneNodeActions::commitObjectResizing
    );
    
    // A bindings for selected node actions
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseWheel & (m * s),
        sn_actions,
        &gui::actions::SceneNodeActions::navigateOrRotate
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * s),
        sn_actions,
        &gui::actions::SceneNodeActions::cancelSelection
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * s),
        m_editor->selection(),
        &core::Selection::trySelect
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & (m * s),
        sn_actions,
        &gui::actions::SceneNodeActions::tryChangeAreaViaKeyPress
    );

    // A bindings for adding label
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * la), 
        l_actions, 
        &gui::actions::LabelActions::cancelAddLabel
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * la),
        l_actions,
        &gui::actions::LabelActions::moveLabel
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * la),
        l_actions,
        &gui::actions::LabelActions::commitLabelAdd
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseWheel & (m * la),
        sn_actions,
        &gui::actions::SceneNodeActions::rotate
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & (m * la),
        sn_actions,
        &gui::actions::SceneNodeActions::tryChangeAreaViaKeyPress
    );


    // A binding for adding sprite actions (just placing)
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * ssa), 
        sp_actions, 
        &gui::actions::Sprite2DActions::cancelAddSprite
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * ssa),
        sp_actions,
        &gui::actions::Sprite2DActions::moveCenterOfSprite
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * ssa),
        sp_actions,
        &gui::actions::Sprite2DActions::commitAdd
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseWheel & (m * ssa),
        sn_actions,
        &gui::actions::SceneNodeActions::rotate
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & (m * ssa),
        sn_actions,
        &gui::actions::SceneNodeActions::tryChangeAreaViaKeyPress
    );

    // A binding for adding sprite (after first click)
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * sdap), 
        sp_actions, 
        &gui::actions::Sprite2DActions::cancelAddSprite
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * sdap),
        sp_actions, 
        &gui::actions::Sprite2DActions::commitAdd
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * sdap),
        sp_actions,
        &gui::actions::Sprite2DActions::moveLowerPointOfSprite
    );

    // A binding for adding sprite (when first click determines upper-left corner)
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * sda), 
        sp_actions, 
        &gui::actions::Sprite2DActions::cancelAddSprite
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * sda),
        sp_actions, 
        &gui::actions::Sprite2DActions::placeFirstPointForSprite
    );

    // A binding for adding custom object actions (just placing)
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * coa), 
        co_actions, 
        &gui::actions::CustomObjectActions::cancelAdd
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * coa),
        co_actions,
        &gui::actions::CustomObjectActions::moveCenterOfObject
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * coa),
        co_actions,
        &gui::actions::CustomObjectActions::commitAdd
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseWheel & (m * coa),
        sn_actions,
        &gui::actions::SceneNodeActions::rotate
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & (m * coa),
        sn_actions,
        &gui::actions::SceneNodeActions::tryChangeAreaViaKeyPress
    );

    // A binding for adding sprite (after first click)
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * coadp), 
        co_actions, 
        &gui::actions::CustomObjectActions::cancelAdd
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * coadp),
        co_actions,
        &gui::actions::CustomObjectActions::commitAdd
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * coadp),
        co_actions,
        &gui::actions::CustomObjectActions::moveLowerPoint
    );

    // A binding for adding sprite (when first click determines upper-left corner)
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * coad), 
        co_actions, 
        &gui::actions::CustomObjectActions::cancelAdd
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * coad),
        co_actions, 
        &gui::actions::CustomObjectActions::placeFirstPoint
    );

    // A binding for layouts/adding
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * ladd),
        ga_actions,
        &gui::actions::GridActions::cancelAddGrid
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * ladd),
        ga_actions,
        &gui::actions::GridActions::moveByCenter
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * ladd),
        ga_actions,
        &gui::actions::GridActions::commitGridAdd
    );

    // A bindings for layouts/adding/secondpoint
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * lssp),
        ga_actions,
        &gui::actions::GridActions::cancelAddGrid
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * lssp),
        ga_actions,
        &gui::actions::GridActions::moveByBottomRightCorner
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * lssp),
        ga_actions,
        &gui::actions::GridActions::commitGridAdd
    );

    // A bindings for layouts/adding/firstpoint
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * lsfp),
        ga_actions,
        &gui::actions::GridActions::cancelAddGrid
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * lsfp),
        ga_actions,
        &gui::actions::GridActions::commitTopLeftPoint
    );

    // A bindings for layouts/moving
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * lmove),
        ga_actions,
        &gui::actions::GridActions::moveByPivotPoint
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseRelease & (m * lmove),
        ga_actions,
        &gui::actions::GridActions::commitMovingGrid
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & (m * lmove),
        ga_actions,
        &gui::actions::GridActions::tryMoveSelectedGridByKeyboard
    );

    // A bindings for layouts/resizing
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * lrp),
        ga_actions,
        &gui::actions::GridActions::resizeGridUsingHotspot
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseRelease & (m * lrp),
        ga_actions,
        &gui::actions::GridActions::commitGridResizingUsingHotspot
    );

    // A binding for ways/selected/moving
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * wsm),
        w_actions,
        &gui::actions::WayActions::moveWayPoint
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseRelease & (m * wsm),
        w_actions,
        &gui::actions::WayActions::commitWayPointMoving
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & (m * wsm),
        w_actions,
        &gui::actions::WayActions::tryMoveWayByKeys
    );

    // A binding for ways/selected
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * ws),
        m_editor->selection(),
        &core::Selection::trySelect
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & (m * ws),
        w_actions,
        &gui::actions::WayActions::tryMoveWayByKeys
    );

    // A binding for ways/idle
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * wi),
        m_editor->selection(),
        &core::Selection::trySelect
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & (m * wi),
        w_actions,
        &gui::actions::WayActions::tryMoveWayByKeys
    );

    // A binding for picking_simple_movement_point
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * psmp),
        a_actions,
        &gui::actions::AnimationActions::pickedPointForSimpleMovement
    );
     sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & sad::MouseLeft & (m * psmp),
        a_actions,
        &gui::actions::AnimationActions::cancelPickingPointForSimpleMovement
    );
    
    // A bindings for placing second point, when user are going to span object between two points
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseRelease & (m * sssp),
        sn_actions,
        &gui::actions::SceneNodeActions::commitSecondPointForSpanning
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * sssp),
        sn_actions,
        &gui::actions::SceneNodeActions::moveSecondPointForSpanning
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * sssp) ,
        sn_actions,
        &gui::actions::SceneNodeActions::cancelSpanningObject
    );

    // A bindings for placing first point, when user are going to span object between two points
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseRelease & (m * ssfp),
        sn_actions,
        &gui::actions::SceneNodeActions::placeFirstPointForSpanning
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_KeyPress & sad::Esc & (m * ssfp) ,
        sn_actions,
        &gui::actions::SceneNodeActions::cancelSpanningObject
    );

    connect(ui.tabTypes, SIGNAL(currentChanged(int)), this, SLOT(tabTypeChanged(int)));

    connect(ui.btnDatabaseSave, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui.btnDatabaseSaveAs, SIGNAL(clicked()), this, SLOT(saveAs()));
    connect(ui.btnDatabaseLoad, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui.btnLoadResources, SIGNAL(clicked()), this, SLOT(loadResources()));
    connect(ui.btnReloadResources, SIGNAL(clicked()), this, SLOT(reloadResources()));
    connect(ui.btnUndo, SIGNAL(clicked()), m_editor, SLOT(undo()));
    connect(ui.btnRedo, SIGNAL(clicked()), m_editor, SLOT(redo()));
    
    connect(ui.btnDatabasePropertiesAdd, SIGNAL(clicked()), this, SLOT(addDatabaseProperty()));
    
    connect(ui.btnSceneAdd, SIGNAL(clicked()), s_actions, SLOT(addScene()));
    connect(ui.btnSceneDelete, SIGNAL(clicked()), s_actions, SLOT(removeScene()));
    connect(ui.lstScenes, SIGNAL(currentRowChanged(int)), s_actions, SLOT(currentSceneChanged(int)));
    connect(ui.txtSceneName, SIGNAL(textEdited(const QString&)), s_actions, SLOT(sceneNameChanged(const QString&)));
    connect(ui.btnScenesMoveBack, SIGNAL(clicked()), s_actions, SLOT(sceneMoveBack()));
    connect(ui.btnScenesMoveFront, SIGNAL(clicked()), s_actions, SLOT(sceneMoveFront()));
    connect(ui.btnSceneClear, SIGNAL(clicked()), s_actions, SLOT(clearScene()));
    
    connect(ui.txtObjectName, SIGNAL(textEdited(const QString&)), sn_actions, SLOT(nameEdited(const QString&)));
    connect(ui.cbSceneNodeVisible, SIGNAL(clicked(bool)), sn_actions, SLOT(visibilityChanged(bool)));
    connect(ui.clpSceneNodeColor, SIGNAL(selectedColorChanged(QColor)), sn_actions, SLOT(colorChanged(QColor)));
    connect(ui.rwSceneNodeRect, SIGNAL(valueChanged(QRectF)), sn_actions, SLOT(areaChanged(QRectF)));
    connect(ui.awSceneNodeAngle, SIGNAL(valueChanged(double)), sn_actions, SLOT(angleChanged(double)));
    connect(ui.lstSceneObjects, SIGNAL(currentRowChanged(int)), sn_actions, SLOT(currentSceneNodeChanged(int)));
    connect(ui.btnSceneNodeDelete, SIGNAL(clicked()), sn_actions, SLOT(removeSceneNode()));
    connect(ui.btnSceneNodeMoveBack, SIGNAL(clicked()), sn_actions, SLOT(sceneNodeMoveBack()));
    connect(ui.btnSceneNodeMoveFront, SIGNAL(clicked()), sn_actions, SLOT(sceneNodeMoveFront()));
    connect(ui.btnSceneNodeSpanBetweenTwoPoints, SIGNAL(clicked()), sn_actions, SLOT(enterSpanningObjectBetweenTwoPoints()));
    connect(ui.btnSceneNodeCopySizeAndPosition, SIGNAL(clicked()), sn_actions, SLOT(copyPositionAndSizeFromOtherNodeClicked()));
    

    connect(ui.btnLabelAdd, SIGNAL(clicked()), l_actions, SLOT(addLabel()));
    connect(ui.rtwLabelFont, SIGNAL(selectionChanged(sad::String)), l_actions, SLOT(labelFontChanged(sad::String)));
    connect(ui.fswLabelFontSize, SIGNAL(valueChanged(unsigned int)), l_actions, SLOT(labelSizeChanged(unsigned int)));
    connect(ui.txtLabelText, SIGNAL(textChanged()), l_actions, SLOT(labelTextChanged()));
    connect(ui.dsbLineSpacingRatio, SIGNAL(valueChanged(double)), l_actions, SLOT(labelLineSpacingChanged(double)));
    connect(ui.spbMaximalLineWidth, SIGNAL(valueChanged(int)), l_actions, SLOT(labelMaximalLineWidthChanged(int)));
    connect(ui.cmbLabelBreakText, SIGNAL(currentIndexChanged(int)), l_actions, SLOT(labelBreakTextChanged(int)));
    connect(ui.cmbLabelOverflowStrategy, SIGNAL(currentIndexChanged(int)), l_actions, SLOT(labelOverflowStrategyChanged(int)));
    connect(ui.cmbLabelTextEllipsis, SIGNAL(currentIndexChanged(int)), l_actions, SLOT(labelTextEllipsisChanged(int)));	
    connect(ui.spbMaximalLinesCount, SIGNAL(valueChanged(int)), l_actions, SLOT(labelMaximalLinesCountChanged(int)));
    connect(ui.cmbLabelOverflowStrategyForLines, SIGNAL(currentIndexChanged(int)), l_actions, SLOT(labelOverflowStrategyForLinesChanged(int)));
    connect(ui.cmbLabelTextEllipsisForLines, SIGNAL(currentIndexChanged(int)), l_actions, SLOT(labelTextEllipsisForLinesChanged(int)));	
    connect(ui.cbLabelHasFormatting, SIGNAL(clicked(bool)), l_actions, SLOT(labelHasFormattingChanged(bool)));


    connect(ui.btnSpriteAdd, SIGNAL(clicked()), sp_actions, SLOT(add()));
    connect(ui.rtwSpriteSprite, SIGNAL(selectionChanged(sad::String)), sp_actions, SLOT(spriteOptionsChanged(sad::String)));
    connect(ui.btnSpriteMakeBackground, SIGNAL(clicked()), sp_actions, SLOT(makeBackground()));
    connect(ui.cbFlipX, SIGNAL(clicked(bool)), sp_actions, SLOT(flipXChanged(bool)));
    connect(ui.cbFlipY, SIGNAL(clicked(bool)), sp_actions, SLOT(flipYChanged(bool)));
    
    connect(ui.btnCustomObjectAdd, SIGNAL(clicked()), co_actions, SLOT(add()));
    connect(ui.rtwCustomObjectSchemas, SIGNAL(selectionChanged(sad::String)), co_actions, SLOT(schemaChanged(sad::String)));

    connect(ui.cbLayoutShow, SIGNAL(clicked(bool)), ga_actions, SLOT(showGridsClicked(bool)));
    connect(ui.btnLayoutAdd, SIGNAL(clicked()), ga_actions, SLOT(addGridClicked()));
    connect(ui.btnLayoutAddByStretching, SIGNAL(clicked()), ga_actions, SLOT(addGridByStretchingClicked()));
    connect(ui.lstLayoutGridList, SIGNAL(currentRowChanged(int)), ga_actions, SLOT(currentGridChanged(int)));
    connect(ui.txtLayoutGridName, SIGNAL(textChanged(const QString&)), ga_actions, SLOT(nameChanged(const QString&)));
    connect(ui.btnLayoutRemove, SIGNAL(clicked()), ga_actions, SLOT(removeGridClicked()));
    connect(ui.rwLayoutArea, SIGNAL(valueChanged(QRectF)), ga_actions, SLOT(areaChanged(QRectF)));
    connect(ui.spnLayoutGridRows, SIGNAL(valueChanged(int)), ga_actions, SLOT(rowCountChanged(int)));
    connect(ui.spnLayoutGridCols, SIGNAL(valueChanged(int)), ga_actions, SLOT(columnCountChanged(int)));
    connect(ui.cbLayoutFixedWidth, SIGNAL(clicked(bool)), ga_actions, SLOT(fixedWidthClicked(bool)));
    connect(ui.cbLayoutFixedHeight, SIGNAL(clicked(bool)), ga_actions, SLOT(fixedHeightClicked(bool)));
    connect(ui.dsbLayoutPaddingTop, SIGNAL(valueChanged(double)), ga_actions, SLOT(topPaddingChanged(double)));
    connect(ui.dsbLayoutPaddingBottom, SIGNAL(valueChanged(double)), ga_actions, SLOT(bottomPaddingChanged(double)));
    connect(ui.dsbLayoutPaddingLeft, SIGNAL(valueChanged(double)), ga_actions, SLOT(leftPaddingChanged(double)));
    connect(ui.dsbLayoutPaddingRight, SIGNAL(valueChanged(double)), ga_actions, SLOT(rightPaddingChanged(double)));
    connect(ui.btnLayoutMerge, SIGNAL(clicked()), ga_actions, SLOT(mergeButtonClicked()));
    connect(ui.btnLayoutSplit, SIGNAL(clicked()), ga_actions, SLOT(splitButtonClicked()));
    connect(ui.spnLayoutNavigateCellX , SIGNAL(valueChanged(int)), ga_actions, SLOT(navigateCellRow(int)));
    connect(ui.spnLayoutNavigateCellY , SIGNAL(valueChanged(int)), ga_actions, SLOT(navigateCellColumn(int)));

    connect(ui.lstWays, SIGNAL(currentRowChanged(int)), w_actions, SLOT(wayChanged(int)));
    connect(ui.btnWayAdd, SIGNAL(clicked()), w_actions, SLOT(addWay()));
    connect(ui.btnWayRemove, SIGNAL(clicked()), w_actions, SLOT(removeWay()));
    connect(ui.txtWayName, SIGNAL(textEdited(const QString&)), w_actions, SLOT(nameEdited(const QString&)));
    connect(ui.cbWayClosed, SIGNAL(clicked(bool)), w_actions, SLOT(closednessChanged(bool)));
    connect(ui.dsbWayTotalTime, SIGNAL(valueChanged(double)), w_actions, SLOT(totalTimeChanged(double)));
    connect(ui.lstWayPoints, SIGNAL(currentRowChanged(int)), w_actions, SLOT(viewPoint(int)));
    connect(ui.btnWayPointAdd, SIGNAL(clicked()), w_actions, SLOT(addWayPoint()));
    connect(ui.btnWayPointRemove, SIGNAL(clicked()), w_actions, SLOT(removeWayPoint()));
    connect(ui.dsbWayPointX, SIGNAL(valueChanged(double)), w_actions, SLOT(wayPointXChanged(double)));
    connect(ui.dsbWayPointY, SIGNAL(valueChanged(double)), w_actions, SLOT(wayPointYChanged(double)));
    connect(ui.btnWayPointMoveBack, SIGNAL(clicked()), w_actions, SLOT(wayPointMoveBack()));
    connect(ui.btnWayPointMoveFront, SIGNAL(clicked()), w_actions, SLOT(wayPointMoveFront()));

    connect(ui.lstDialogues, SIGNAL(currentRowChanged(int)), d_actions, SLOT(dialogueChanged(int)));
    connect(ui.lstPhrases, SIGNAL(currentRowChanged(int)), d_actions, SLOT(phraseChanged(int)));
    connect(ui.btnDialogueAdd, SIGNAL(clicked()), d_actions, SLOT(addDialogue()));
    connect(ui.btnDialogueRemove, SIGNAL(clicked()), d_actions, SLOT(removeDialogue()));
    connect(ui.txtDialogueName, SIGNAL(textEdited(const QString&)), d_actions, SLOT(nameEdited(const QString&)));
    connect(ui.btnPhraseAdd, SIGNAL(clicked()), d_actions, SLOT(addPhrase()));
    connect(ui.btnPhraseRemove, SIGNAL(clicked()), d_actions, SLOT(removePhrase()));
    connect(ui.btnPhraseMoveBack, SIGNAL(clicked()), d_actions, SLOT(movePhraseBack()));
    connect(ui.btnPhraseMoveFront, SIGNAL(clicked()), d_actions, SLOT(movePhraseFront()));
    connect(ui.dsbPhraseDuration, SIGNAL(valueChanged(double)), d_actions, SLOT(durationChanged(double)));
    connect(ui.txtPhrasePhrase, SIGNAL(textChanged()), d_actions, SLOT(phraseTextChanged()));
    connect(ui.txtPhraseActorName, SIGNAL(textEdited(const QString&)), d_actions, SLOT(actorNameChanged(const QString&)));
    connect(ui.txtPhraseActorPortrait, SIGNAL(textEdited(const QString&)), d_actions, SLOT(actorPortraitChanged(const QString&)));
    connect(ui.txtPhraseViewHint, SIGNAL(textEdited(const QString&)), d_actions, SLOT(viewHintChanged(const QString&)));

    connect(ui.btnAnimationsAdd, SIGNAL(clicked()), a_actions, SLOT(addAnimation()));
    connect(ui.btnAnimationsRemove, SIGNAL(clicked()), a_actions, SLOT(removeAnimation()));
    connect(ui.lstAnimations, SIGNAL(currentRowChanged(int)), a_actions, SLOT(currentAnimationChanged(int)));
    connect(ui.txtAnimationName, SIGNAL(textEdited(const QString&)), a_actions, SLOT(nameChanged(const QString&)));
    connect(ui.dsbAnimationTime, SIGNAL(valueChanged(double)), a_actions, SLOT(timeChanged(double)));
    connect(ui.cbAnimationLooped, SIGNAL(clicked(bool)), a_actions, SLOT(loopedChanged(bool)));
    connect(ui.cmbAnimationEasingType, SIGNAL(currentIndexChanged(int)), a_actions, SLOT(easingFunctionTypeChanged(int)));
    connect(ui.dsbAnimationEasingOvershootAmplitude, SIGNAL(valueChanged(double)), a_actions, SLOT(easingOvershootAmplitudeChanged(double)));
    connect(ui.dsbAnimationEasingPeriod, SIGNAL(valueChanged(double)), a_actions, SLOT(easingPeriodChanged(double)));
    connect(ui.sbBlinkingFrequency, SIGNAL(valueChanged(int)), a_actions, SLOT(blinkingFrequencyChanged(int)));
    connect(ui.btnAnimationsStart, SIGNAL(clicked()), a_actions, SLOT(startOnObject()));
    connect(ui.btnAnimationsCancel, SIGNAL(clicked()), a_actions, SLOT(stopOnObject()));
    connect(ui.cwColorStartingColor, SIGNAL(clicked()), a_actions, SLOT(colorChangeStartingColor()));
    connect(ui.cwColorEndingColor, SIGNAL(clicked()), a_actions, SLOT(colorChangeEndingColor()));
    connect(ui.dabResizeStartingSizeX, SIGNAL(valueChanged(double)), a_actions, SLOT(resizeChangeStartingSizeX(double)));
    connect(ui.dabResizeStartingSizeY, SIGNAL(valueChanged(double)), a_actions, SLOT(resizeChangeStartingSizeY(double)));
    connect(ui.dabResizeEndingSizeX, SIGNAL(valueChanged(double)), a_actions, SLOT(resizeChangeEndingSizeX(double)));
    connect(ui.dabResizeEndingSizeY, SIGNAL(valueChanged(double)), a_actions, SLOT(resizeChangeEndingSizeY(double)));	
    connect(ui.dsbRotateStartingAngle, SIGNAL(valueChanged(double)), a_actions, SLOT(rotateChangeStartingAngle(double)));
    connect(ui.dsbRotateEndingAngle, SIGNAL(valueChanged(double)), a_actions, SLOT(rotateChangeEndingAngle(double)));
    connect(ui.cmbWayAnimationWay, SIGNAL(currentIndexChanged(int)), a_actions, SLOT(wayMovingChangeWay(int)));
    connect(ui.txtFontListList, SIGNAL(textEditingFinished()), a_actions, SLOT(fontListEditingFinished()));
    connect(ui.sbFontSizeStartingSize, SIGNAL(valueChanged(int)), a_actions, SLOT(fontSizeChangeStartingSize(int)));
    connect(ui.sbFontSizeEndingSize, SIGNAL(valueChanged(int)), a_actions, SLOT(fontSizeChangeEndingSize(int)));
    connect(ui.txtOptionListList, SIGNAL(textEditingFinished()), a_actions, SLOT(optionListEditingFinished()));
    connect(ui.txtTextureCoordinatesList, SIGNAL(textEditingFinished()), a_actions, SLOT(textureCoordinatesListEditingFinished()));
    connect(ui.rctTCCStartingRect, SIGNAL(valueChanged(QRectF)), a_actions, SLOT(textureCoordinatesChangeStartRect(QRectF)));
    connect(ui.rctTCCEndingRect, SIGNAL(valueChanged(QRectF)), a_actions, SLOT(textureCoordinatesChangeEndRect(QRectF)));
    connect(ui.dsbCameraRotationPivotX, SIGNAL(valueChanged(double)), a_actions, SLOT(cameraRotationChangePivotX(double)));
    connect(ui.dsbCameraRotationPivotY, SIGNAL(valueChanged(double)), a_actions, SLOT(cameraRotationChangePivotY(double)));
    connect(ui.dsbCameraRotationStartingAngle, SIGNAL(valueChanged(double)), a_actions, SLOT(cameraRotationChangeStartingAngle(double)));
    connect(ui.dsbCameraRotationEndingAngle, SIGNAL(valueChanged(double)), a_actions, SLOT(cameraRotationChangeEndingAngle(double)));
    connect(ui.dsbCameraShakingOffsetX, SIGNAL(valueChanged(double)), a_actions, SLOT(cameraShakingChangeOffsetX(double)));
    connect(ui.dsbCameraShakingOffsetY, SIGNAL(valueChanged(double)), a_actions, SLOT(cameraShakingChangeOffsetY(double)));
    connect(ui.sbCameraShakingFrequency, SIGNAL(valueChanged(int)), a_actions, SLOT(cameraShakingChangeFrequency(int)));
    connect(ui.btnCompositeAnimationAddToList, SIGNAL(clicked()), a_actions, SLOT(addAnimationToComposite()));
    connect(ui.btnCompositeAnimationRemoveFromList, SIGNAL(clicked()), a_actions, SLOT(removeAnimationFromComposite()));
    connect(ui.btnCompositeAnimationMoveBack, SIGNAL(clicked()), a_actions, SLOT(moveBackInCompositeList()));
    connect(ui.btnCompositeAnimationMoveFront, SIGNAL(clicked()), a_actions, SLOT(moveFrontInCompositeList()));
    connect(ui.dabSimpleMovementStartingPointX, SIGNAL(valueChanged(double)), a_actions, SLOT(simpleMovementChangeStartingPointX(double)));
    connect(ui.dabSimpleMovementStartingPointY, SIGNAL(valueChanged(double)), a_actions, SLOT(simpleMovementChangeStartingPointY(double)));
    connect(ui.dabSimpleMovementEndingPointX, SIGNAL(valueChanged(double)), a_actions, SLOT(simpleMovementChangeEndingPointX(double)));
    connect(ui.dabSimpleMovementEndingPointY, SIGNAL(valueChanged(double)), a_actions, SLOT(simpleMovementChangeEndingPointY(double)));
    connect(ui.btnSimpleMovementPickStartingPoint, SIGNAL(clicked()), a_actions, SLOT(startPickingStartingPointForSimpleMovement()));
    connect(ui.btnSimpleMovementPickEndingPoint, SIGNAL(clicked()), a_actions, SLOT(startPickingEndingPointForSimpleMovement()));

    connect(ui.btnAnimationsInstanceAdd, SIGNAL(clicked()), ai_actions, SLOT(addInstance()));
    connect(ui.btnAnimationsInstanceRemove, SIGNAL(clicked()), ai_actions, SLOT(removeInstance()));	
    connect(ui.lstAnimationInstances, SIGNAL(currentRowChanged(int)), ai_actions, SLOT(currentInstanceChanged(int)));
    connect(ui.txtAnimationInstanceName, SIGNAL(textEdited(const QString&)), ai_actions, SLOT(nameChanged(const QString&)));
    connect(ui.rbAnimationInstanceFromDatabase, SIGNAL(toggled(bool)), ai_actions, SLOT(databaseLinkStateChanged(bool)));
    connect(ui.rbAnimationInstanceFromTree, SIGNAL(toggled(bool)), ai_actions, SLOT(treeLinkStateChanged(bool)));
    connect(ui.cmbAnimationInstanceAnimationFromTree, SIGNAL(currentIndexChanged(int)), ai_actions, SLOT(treeElementChanged(int)));
    connect(ui.cmbAnimationInstanceAnimationFromDatabase, SIGNAL(currentIndexChanged(int)), ai_actions, SLOT(databaseElementChanged(int)));
    connect(ui.cmbAnimationInstanceObject, SIGNAL(currentIndexChanged(int)), ai_actions, SLOT(objectChanged(int)));
    connect(ui.dsbAnimationInstanceStartTime, SIGNAL(valueChanged(double)), ai_actions, SLOT(startTimeChanged(double)));
    connect(ui.cmbWayAnimationInstanceWay, SIGNAL(currentIndexChanged(int)), ai_actions, SLOT(wayChanged(int)));
    connect(ui.btnAnimationsInstanceStart, SIGNAL(clicked()), ai_actions, SLOT(start()));
    connect(ui.btnAnimationsInstanceCancel, SIGNAL(clicked()), ai_actions, SLOT(stop()));
    
    connect(ui.btnAnimationsGroupAdd, SIGNAL(clicked()), ag_actions, SLOT(addGroup()));
    connect(ui.btnAnimationsGroupRemove, SIGNAL(clicked()), ag_actions, SLOT(removeGroup()));	
    connect(ui.lstAnimationsGroup, SIGNAL(currentRowChanged(int)), ag_actions, SLOT(currentGroupChanged(int)));
    connect(ui.txtAnimationsGroupName, SIGNAL(textEdited(const QString&)), ag_actions, SLOT(nameChanged(const QString&)));
    connect(ui.cbAnimationsGroupLooped, SIGNAL(clicked(bool)), ag_actions, SLOT(loopedChanged(bool)));
    connect(ui.btnAnimationsGroupAddToList, SIGNAL(clicked()), ag_actions, SLOT(addInstance()));	
    connect(ui.btnAnimationsGroupRemoveFromList, SIGNAL(clicked()), ag_actions, SLOT(removeInstance()));	
    connect(ui.btnAnimationsGroupStart, SIGNAL(clicked()), ag_actions, SLOT(start()));	
    connect(ui.btnAnimationsGroupCancel, SIGNAL(clicked()), ag_actions, SLOT(stop()));
    connect(ui.rbAnimationsGroupSequential, SIGNAL(toggled(bool)), ag_actions, SLOT(toggledSequential(bool)));
    connect(ui.rbAnimationsGroupParallel, SIGNAL(toggled(bool)), ag_actions, SLOT(toggledParallel(bool)));
    

    connect(ui.btnConsoleRun, SIGNAL(clicked()), m_scripting, SLOT(runScript()));
    connect(ui.btnConsoleHelp, SIGNAL(clicked()), m_scripting, SLOT(showHelp()));
    connect(ui.btnConsoleSave, SIGNAL(clicked()), m_scripting, SLOT(saveScript()));
    connect(ui.btnConsoleLoad, SIGNAL(clicked()), m_scripting, SLOT(loadScript()));

    connect(ui.btnClearObjectSelection, SIGNAL(clicked()), this, SLOT(clearObjectSelection()));
    connect(ui.btnFixTextureCoordinates, SIGNAL(clicked()), this, SLOT(fixTextureCoordinates()));

    connect(ui.btnClearCounters, SIGNAL(clicked()), m_editor, SLOT(clearFastModeCounter()));
    connect(ui.btnFastModeHelp, SIGNAL(clicked()), this, SLOT(showFastModeHelp()));
    connect(ui.btnGridAndOffset, SIGNAL(clicked()), this, SLOT(showGridAndOffsetWindow()));

    // Initialize UI from editor
    if (editor)
    {
        sad::Vector<sad::String> names = editor->animationFactory()->registeredClasses();
        std::sort(names.begin(), names.end());
        ui.cmbAddedAnimationType->clear();
        for(size_t ii = 0; ii < names.size(); ii++)
        {
            QString name = names[ii].c_str();
            name.replace("sad::animations::", "");
            ui.cmbAddedAnimationType->addItem(name);
        }
    }
    
    ui.cmbAddedAnimationInstanceType->clear();
    ui.cmbAddedAnimationInstanceType->addItem("Instance");
    ui.cmbAddedAnimationInstanceType->addItem("WayInstance");

    if (editor)
    {
        editor->selectionBorder()->toggleShowHotspot(ui.tabTypes->currentIndex() == 0);
    }
}

core::Editor* MainPanel::editor() const
{
    return m_editor;
}

sad::Hash<sad::String, gui::table::Delegate*>& MainPanel::delegatesByName()
{
    return m_delegates_by_names;
}

Ui::MainPanelClass* MainPanel::UI()
{
    return &ui;
}

void MainPanel::viewDatabase()
{
    this->fixDatabase();
    sad::db::Database* db = sad::Renderer::ref()->database("");
    // Update palette
    ui.clpSceneNodeColor->setPalette(db->getProperty<QList<QList<QColor> > >("palette").value());
    // Update global offset
    sad::Point3D global_offset = db->getProperty<sad::Point3D>("global_renderer_offset").value();
    sad::Renderer::ref()->setGlobalTranslationOffset(global_offset);
    m_offsets_window->updateGlobalOffsetInUI(global_offset);
    // Update grid enabled
    bool grid_enabled = db->getProperty<bool>("global_renderer_grid_enabled").value();
    m_offsets_window->updateGridEnabled(grid_enabled);
    gui::RenderEditorGrid* reg =  m_editor->renderEditorGrid();
    if (grid_enabled)
    {
        reg->enable();
    }
    else
    {
        reg->disable();        
    }
    // Update settings for grid
    sad::Point2I grid_settings = db->getProperty<sad::Point2I>("global_renderer_grid_settings").value();
    m_offsets_window->updateGridSettings(grid_settings);
    reg->GridSpaceX = grid_settings.x();
    reg->GridSpaceY = grid_settings.y();
    // Update grid color
    sad::AColor grid_color = db->getProperty<sad::AColor>("global_renderer_grid_color").value();
    m_offsets_window->setGridColor(grid_color);
    reg->Color = grid_color;

    // Remove old delegates
    for(sad::PtrHash<sad::String, gui::table::Delegate>::iterator it = m_property_delegates.begin();
        it != m_property_delegates.end();
        ++it)
    {
        it.value()->delRef();
    }
    m_delegates_by_names.clear();

    const QSet<QString>& invisible_properties = scripting::database::getInvisibleProperties();

    for(sad::db::Database::Properties::const_iterator it = db->begin();
        it != db->end();
        ++it)
    {
        // Skip invisible properties
        QString fkey = STD2QSTRING(it.key());
        if ((invisible_properties.contains(fkey) == false) && (it.value()->pointerStarsCount() == 0))
        {
            gui::table::Delegate* d = m_dbdelegate_factory.create(STD2QSTRING(it.value()->baseType().c_str()));
            if (d)
            {
                d->makeLinkedTo(ui.twDatabaseProperties, m_editor);
                d->setPropertyName(STD2QSTRING(it.key().c_str()));
                d->linkToDatabase();
                d->add();

                m_delegates_by_names.insert(it.key(), d);
                m_property_delegates.insert(it.key(), d);
            }
        }
    }

    sad::db::populateScenesFromDatabase(sad::Renderer::ref(), sad::Renderer::ref()->database(""));

    sad::db::Object* nullobject = NULL;
    QVariant nullobjectvariant;
    nullobjectvariant.setValue(nullobject);
    ui.cmbAnimationInstanceObject->addItem("Not set", nullobjectvariant);
    const sad::Vector<sad::Scene*>& scenes = sad::Renderer::ref()->scenes(); 

    sad::Vector<sad::SceneNode*> nodes;
    gui::actions::SceneActions* s_actions = m_editor->actions()->sceneActions();
    for(unsigned int i = 0; i < scenes.size(); i++)
    {
        s_actions->addSceneToSceneList(scenes[i]);
        nodes << scenes[i]->objects();
    }
    gui::actions::SceneNodeActions* sn_actions = m_editor->actions()->sceneNodeActions();
    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        QVariant v;
        v.setValue(static_cast<sad::db::Object*>(nodes[i]));

        ui.cmbAnimationInstanceObject->addItem(sn_actions->fullNameForNode(nodes[i]), v);
    }

    // Cleanup a combo for animation way
    sad::p2d::app::Way* nullway = NULL;
    QVariant nullwayvariant;
    nullwayvariant.setValue(nullway);
    ui.cmbWayAnimationWay->addItem("Not set", nullwayvariant);
    ui.cmbWayAnimationInstanceWay->addItem("Not set", nullwayvariant);

    sad::Vector<sad::db::Object*> wayslist;
    db->table("ways")->objects(wayslist);
    gui::actions::WayActions* w_actions = m_editor->actions()->wayActions();    
    for(unsigned int i = 0; i < wayslist.size(); i++)
    {
        if (wayslist[i]->isInstanceOf("sad::p2d::app::Way"))
        {
            sad::p2d::app::Way* w = static_cast<sad::p2d::app::Way*>(wayslist[i]);
            w_actions->addLastWayToEnd(w);
        }
    }

    sad::Vector<sad::db::Object*> dialoguelist;
    db->table("dialogues")->objects(dialoguelist);
    gui::actions::DialogueActions* d_actions = m_editor->actions()->dialogueActions();
    for(unsigned int i = 0; i < dialoguelist.size(); i++)
    {
        if (dialoguelist[i]->isInstanceOf("sad::dialogue::Dialogue"))
        {
            sad::dialogue::Dialogue* w = static_cast<sad::dialogue::Dialogue*>(dialoguelist[i]);
            d_actions->addDialogueToDialogueList(w);
        }
    }

    sad::animations::Animation* nullanimation = NULL;
    QVariant nullanimationvariant;
    nullanimationvariant.setValue(nullanimation);
    ui.cmbAnimationInstanceAnimationFromDatabase->addItem("Not set", nullanimationvariant);
    sad::Vector<sad::db::Object*> animationlist;
    sad::db::Table* animationtable = db->table("animations");
    animationtable->objects(animationlist);
    static const int validtypescount = 17;
    static sad::String validtypes[] = {
        "sad::animations::Animation",      // 1
        "sad::animations::Blinking",       // 2
        "sad::animations::CameraRotation", // 3
        "sad::animations::CameraShaking",  // 4
        "sad::animations::Color",          // 5
        "sad::animations::FontList",       // 6
        "sad::animations::FontSize",       // 7
        "sad::animations::OptionList",     // 8
        "sad::animations::Parallel",       // 9
        "sad::animations::Resize",         // 10
        "sad::animations::Rotate",         // 11
        "sad::animations::SimpleMovement", // 12
        "sad::animations::Sequential",     // 13
        "sad::animations::TextureCoordinatesContinuous",// 14
        "sad::animations::TextureCoordinatesList",      // 15
        "sad::animations::Typing",                      // 16
        "sad::animations::WayMoving"                    // 17
    };
    gui::actions::AnimationActions* a_actions = m_editor->actions()->animationActions();
    for(unsigned int i = 0; i < animationlist.size(); i++)
    {
        bool valid = false;
        for(size_t j  = 0; (j < validtypescount) && (valid == false); j++)
        {
            valid = valid || animationlist[i]->isInstanceOf(validtypes[j]);
        }
        if (valid)
        {
            sad::animations::Animation* w = static_cast<sad::animations::Animation*>(animationlist[i]);
            a_actions->addAnimationToViewingLists(w);
        }
    }

    sad::Vector<sad::db::Object*> animationinstancelist;
    sad::db::Table* animationinstancetable = db->table("animationinstances");
    animationinstancetable->objects(animationinstancelist);
    gui::actions::AnimationInstanceActions* ai_actions = m_editor->actions()->instanceActions();
    for(unsigned int i = 0; i < animationinstancelist.size(); i++)
    {
        sad::db::Object* o = animationinstancelist[i];
        if (o->isInstanceOf("sad::animations::Instance") || o->isInstanceOf("sad::animations::WayInstance"))
        {
            sad::animations::Instance* w = static_cast<sad::animations::Instance*>(o);
            QVariant v;
            v.setValue(w);

            QString name = ai_actions->nameForInstance(w);

            QListWidgetItem* item = new QListWidgetItem(name);
            item->setData(Qt::UserRole, v);

            ui.lstAnimationInstances->addItem(item);
        }
    }

    ai_actions->updateGroupInstanceList();

    sad::Vector<sad::db::Object*> animationgrouplist;
    sad::db::Table* animationgrouptable = db->table("animationgroups");
    animationgrouptable->objects(animationgrouplist);
    gui::actions::AnimationGroupActions* ag_actions = m_editor->actions()->groupActions();
    for(unsigned int i = 0; i < animationgrouplist.size(); i++)
    {
        sad::db::Object* o = animationgrouplist[i];
        if (o->isInstanceOf("sad::animations::Group"))
        {
            sad::animations::Group* g = static_cast<sad::animations::Group*>(o);
            ag_actions->addGroupToList(g);
        }
    }

    // 
    sad::Renderer::ref()->lockRendering();
    m_editor->renderGrids()->disable();

    sad::db::Table* table = db->table("layouts");
    sad::Vector<sad::layouts::Grid*> grids;
    gui::RenderGrids::gridsFromTable(table, grids);
    gui::actions::GridActions* grid_actions = m_editor->actions()->gridActions();
    grid_actions->clearNodeToGridCache();
    grid_actions->clearGridCellsBrowser();
    for(size_t i = 0; i < grids.size(); i++)
    {
        grid_actions->addGridToGridList(grids[i]);
    }


    m_editor->renderGrids()->synchronizeWithDatabase();
    m_editor->renderGrids()->setEnabled(ui.cbLayoutShow->checkState() == Qt::Checked);
    sad::Renderer::ref()->unlockRendering();
}

QList<QList<QColor> >  MainPanel::colorPalette() const
{
    return ui.clpSceneNodeColor->palette();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::setColorPalette(const QList<QList<QColor> >& palette)
{
    ui.clpSceneNodeColor->setPalette(palette);
    sad::db::Database* db = sad::Renderer::ref()->database("");
    db->setProperty("palette", palette);
}

bool  MainPanel::takeDelegateByPropertyName(const QString & name)
{
    bool owns = false;
    std::string dname = Q2STDSTRING(name);
    if (m_property_delegates.contains(dname))
    {
        m_property_delegates.remove(dname);
    }
    return owns;
}


void MainPanel::updateMousePosition(const sad::input::MouseMoveEvent & e)
{
    m_mousemove_point = e.pos2D();
    QTimer::singleShot(0, this, SLOT(updateMousePositionNow()));
}

void MainPanel::updateResourceViews()
{
    ui.rtwLabelFont->setTree("");
    ui.rtwLabelFont->setFilter("sad::freetype::Font|sad::TextureMappedFont");
    ui.rtwLabelFont->updateTree();

    ui.rtwSpriteSprite->setTree("");
    ui.rtwSpriteSprite->setFilter("sad::Sprite2D::Options");
    ui.rtwSpriteSprite->updateTree();

    ui.rtwCustomObjectSchemas->setTree("");
    ui.rtwCustomObjectSchemas->setFilter("sad::db::custom::Schema");
    ui.rtwCustomObjectSchemas->updateTree();

    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    ui.txtFontListList->completer()->setModel(new QStringListModel(
        this->resourcesByFilter(tree->root(), "", "sad::freetype::Font|sad::TextureMappedFont")
    ));

    ui.txtOptionListList->completer()->setModel(new QStringListModel(
        this->resourcesByFilter(tree->root(), "", "sad::Sprite2D::Options")
    ));

    ui.txtTextureCoordinatesList->completer()->setModel(new QStringListModel(
        this->resourcesByFilter(tree->root(), "", "sad::Sprite2D::Options")
    ));

    this->updateAnimationsListFromTree();
}

void MainPanel::highlightState(const sad::String & text)
{
    m_highlight_state = text.c_str();
    QTimer::singleShot(0, this, SLOT(highlightStateNow()));
}

void MainPanel::highlightIdleState()
{
    this->highlightState("Idle");
}

void MainPanel::highlightSelectedState()
{
    this->highlightState("Editing item...");
}

void MainPanel::highlightLabelAddingState()
{
    this->highlightState("Click, where you want label to be placed");
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::toggleAnimationPropertiesEditable(bool flag)
{
    QWidget* widgets[] = {
        ui.btnLoadResources,
        ui.btnReloadResources,
        ui.btnUndo,
        ui.btnRedo,
        ui.txtAnimationName,
        ui.btnAnimationsAdd,
        ui.btnAnimationsRemove,
        ui.dsbAnimationTime,
        ui.cbAnimationLooped,
        ui.sbBlinkingFrequency,
        ui.cwColorStartingColor,
        ui.cwColorEndingColor,
        ui.dabResizeStartingSizeX,
        ui.dabResizeStartingSizeY,
        ui.dabResizeEndingSizeX,
        ui.dabResizeEndingSizeY,
        ui.dsbRotateStartingAngle,
        ui.dsbRotateEndingAngle,
        ui.cmbWayAnimationWay,
        ui.txtFontListList,
        ui.sbFontSizeStartingSize,
        ui.sbFontSizeEndingSize,
        ui.txtOptionListList,
        ui.txtTextureCoordinatesList,
        ui.rctTCCStartingRect,
        ui.rctTCCEndingRect,
        ui.dsbCameraRotationPivotX,
        ui.dsbCameraRotationPivotY,
        ui.dsbCameraRotationStartingAngle,
        ui.dsbCameraRotationEndingAngle,
        ui.dsbCameraShakingOffsetX,
        ui.dsbCameraShakingOffsetY,
        ui.sbCameraShakingFrequency,
        ui.btnCompositeAnimationRemoveFromList,
        ui.btnCompositeAnimationAddToList,
        ui.btnCompositeAnimationMoveBack,
        ui.btnCompositeAnimationMoveFront,
        ui.lstAnimations, // !!! Toggle animation list too to be disabled, to force user not to change animation
        
        ui.lstAnimationInstances,
        ui.cmbAnimationInstanceAnimationFromTree,
        ui.cmbAnimationInstanceAnimationFromDatabase,
        ui.rbAnimationInstanceFromTree,
        ui.rbAnimationInstanceFromDatabase,
        ui.cmbAnimationInstanceObject,
        ui.dsbAnimationInstanceStartTime,
        ui.cmbWayAnimationInstanceWay,
        ui.cmbAddedAnimationInstanceType,
        ui.txtAnimationInstanceName,
        ui.btnAnimationsInstanceAdd,
        ui.btnAnimationsInstanceRemove,

        ui.lstAnimationsGroup,
        ui.lstAnimationsGroupInGroup,
        ui.lstAnimationsGroupAllAnimations,
        ui.btnAnimationsGroupRemoveFromList,
        ui.btnAnimationsGroupAddToList,
        ui.txtAnimationsGroupName,
        ui.cbAnimationsGroupLooped,
        ui.btnAnimationsGroupAdd,
        ui.btnAnimationsGroupRemove,
        ui.rbAnimationsGroupParallel,
        ui.rbAnimationsGroupSequential,

        ui.btnConsoleRun,

        ui.btnClearObjectSelection,

        NULL
    };
    size_t i = 0;
    while(widgets[i] != NULL)
    {
        widgets[i]->setEnabled(flag);
        ++i;
    }
}

//====================  PUBLIC SLOTS METHODS HERE ====================

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::clearObjectSelection()
{
   if (m_editor->shared()->isAnyKindOfAnimationIsRunning() == false
       && m_editor->isInEditingState() == false
      ) {
        m_editor->shared()->setSelectedObject(NULL); 
        if (m_editor->machine()->isInState("selected"))
        {
            m_editor->machine()->enterState("idle");
        }
   }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::fixTextureCoordinates()
{
    if (m_editor->shared()->isAnyKindOfAnimationIsRunning() == false && m_editor->isInEditingState() == false)
    {
        sad::Renderer::ref()->lockRendering();
        sad::db::Database* db = sad::Renderer::ref()->database("");
        sad::Vector<sad::db::Object*> objects;
        db->table("scenes")->objects(objects);
        for(size_t i = 0; i < objects.size(); i++)
        {
            if (objects[i]->isInstanceOf("sad::Scene"))
            {
                const sad::Vector<sad::SceneNode*>& nodes = static_cast<sad::Scene*>(objects[i])->objects();
                for(size_t j = 0; j < nodes.size(); j++)
                {
                    sad::SceneNode* node = nodes[j];
                    if (node->isInstanceOf("sad::Sprite2D"))
                    {
                        sad::Sprite2D* sprite = static_cast<sad::Sprite2D*>(node);
                        if (sprite->optionsName().length())
                        {
                            sad::Sprite2D::Options* opts = sad::Renderer::ref()
                                                        ->tree("")
                                                        ->get<sad::Sprite2D::Options>(sprite->optionsName());
                            if (opts)
                            {
                                sprite->setTextureCoordinates(opts->TextureRectangle);
                            }
                        }
                    }
                    if (node->isInstanceOf("sad::db::custom::Object"))
                    {
                        sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(node);
                        if (o->innerTypeIs("sad::Sprite2D") && o->options().length())
                        {
                            sad::Sprite2D::Options* opts = sad::Renderer::ref()
                                                        ->tree("")
                                                        ->get<sad::Sprite2D::Options>(o->options());
                            if (opts)
                            {
                                o->setTextureCoordinates(opts->TextureRectangle);
                            }                            
                        }
                    }
                }
            }            
        }
        sad::Renderer::ref()->unlockRendering();
    }
}

void MainPanel::showFastModeHelp()
{
    QMessageBox::information(
        this,
        "Help on fast mode",
        "Fast mode allows you to make simpler tedious placement of large amount of objects with similar names.\n"
        "It changes behaviour, so every placement of old objects just adds movable copy of it, until Escape is pressed.\n"
        "Names for objects are created via adding to current name a special counter, which could be cleared with \"Clear counters\" button."
    );
}

void MainPanel::showGridAndOffsetWindow() const
{
    if (m_offsets_window->isVisible() == false)
    {
        m_offsets_window->show();
    }
}

//====================  PROTECTED METHODS HERE ====================

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::fillDatabasePropertyTypesCombo()
{
    const unsigned int typescount = 21;
    const QString types[typescount] = {
        "bool",

        "unsigned char",
        "signed char",
        "char",

        "short",
        "unsigned short",

        "int",
        "unsigned int",

        "long",
        "unsigned long",

        "long long",
        "unsigned long long",

        "float",
        "double",

        "sad::String",
        "sad::Color",
        "sad::AColor",

        "sad::Point2D",
        "sad::Point2I",

        "sad::Size2D",
        "sad::Size2I"
    };
    for(unsigned int i = 0; i < typescount; i++)
    {
        ui.cmbDatabasePropertyType->addItem(types[i]);
    }
}

void MainPanel::closeEvent(QCloseEvent* ev)
{
    this->QMainWindow::closeEvent(ev);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void MainPanel::fixDatabase()
{
    sad::db::Database* db = sad::Renderer::ref()->database("");
    // Contains sad::Scene
    if (db->table("scenes") == NULL)
    {
        db->addTable("scenes", new sad::db::Table());
    }
    // Contains sad::SceneNode
    if (db->table("scenenodes") == NULL)
    {
        db->addTable("scenenodes", new sad::db::Table());
    }
    // Contains sad::p2d::app::Way
    if (db->table("ways") == NULL)
    {
        db->addTable("ways", new sad::db::Table());
    }
    // Contains sad::dialogue::Dialogue
    if (db->table("dialogues") == NULL)
    {
        db->addTable("dialogues", new sad::db::Table());
    }    
    // Contains sad::animations::Animation
    if (db->table("animations") == NULL)
    {
        db->addTable("animations", new sad::db::Table());
    }
    // Contains sad::animations::Instance
    if (db->table("animationinstances") == NULL)
    {
        db->addTable("animationinstances", new sad::db::Table());
    }
    // Contains sad::animations::Group
    if (db->table("animationgroups") == NULL)
    {
        db->addTable("animationgroups", new sad::db::Table());
    }
    // Contains sad::layouts::Grid
    if (db->table("layouts") == NULL)
    {
        db->addTable("layouts", new sad::db::Table());        
    }

    bool needtosetpalette = false;
    if (db->propertyByName("palette") != NULL)
    {
        if (db->propertyByName("palette")->baseType() != "sad::Vector<sad::Vector<sad::AColor> >"
            || db->propertyByName("palette")->pointerStarsCount() != 0)
        {
            needtosetpalette = true;
            db->removeProperty("palette");
            db->addProperty(
                "palette", 
                new sad::db::StoredProperty<sad::Vector<sad::Vector<sad::AColor> > >()
            );
        }
    } 
    else
    {
        needtosetpalette = true;
        db->addProperty(
            "palette", 
            new sad::db::StoredProperty<sad::Vector<sad::Vector<sad::AColor> > >()
        );
    }
    // Set global renderer offset
    if (db->propertyByName("global_renderer_offset") != NULL)
    {
        if (db->propertyByName("global_renderer_offset")->baseType() != "sad::Point3D"
            || db->propertyByName("global_renderer_offset")->pointerStarsCount() != 0)
        {
            db->removeProperty("global_renderer_offset");
            db->addProperty(
                "global_renderer_offset", 
                new sad::db::StoredProperty<sad::Point3D >(m_offsets_window->globalOffset())
            );
        }
    }
    else
    {
        db->addProperty(
            "global_renderer_offset", 
            new sad::db::StoredProperty<sad::Point3D >(m_offsets_window->globalOffset())
        );
    }
    // Set whether grid is enabled
    if (db->propertyByName("global_renderer_grid_enabled") != NULL)
    {
        if (db->propertyByName("global_renderer_grid_enabled")->baseType() != "bool"
            || db->propertyByName("global_renderer_grid_enabled")->pointerStarsCount() != 0)
        {
            db->removeProperty("global_renderer_grid_enabled");
            db->addProperty(
                "global_renderer_grid_enabled", 
                new sad::db::StoredProperty<bool>(m_offsets_window->gridEnabled())
            );
        }
    }
    else
    {
        db->addProperty(
            "global_renderer_grid_enabled", 
            new sad::db::StoredProperty<bool>(m_offsets_window->gridEnabled())
        );
    }
    // Set grid settings
    if (db->propertyByName("global_renderer_grid_settings") != NULL)
    {
        if (db->propertyByName("global_renderer_grid_settings")->baseType() != "sad::Point2I"
            || db->propertyByName("global_renderer_grid_settings")->pointerStarsCount() != 0)
        {
            db->removeProperty("global_renderer_grid_settings");
            db->addProperty(
                "global_renderer_grid_settings", 
                new sad::db::StoredProperty<sad::Point2I>(m_offsets_window->gridSettings())
            );
        }
    }
    else
    {
        db->addProperty(
            "global_renderer_grid_settings", 
            new sad::db::StoredProperty<sad::Point2I>(m_offsets_window->gridSettings())
        );
    }
    // Set grid color
    if (db->propertyByName("global_renderer_grid_color") != NULL)
    {
        if (db->propertyByName("global_renderer_grid_color")->baseType() != "sad::AColor"
            || db->propertyByName("global_renderer_grid_color")->pointerStarsCount() != 0)
        {
            db->removeProperty("global_renderer_grid_color");
            db->addProperty(
                "global_renderer_grid_color", 
                new sad::db::StoredProperty<sad::AColor>(m_offsets_window->gridColor())
            );
        }
    }
    else
    {
        db->addProperty(
            "global_renderer_grid_color", 
            new sad::db::StoredProperty<sad::AColor>(m_offsets_window->gridColor())
        );
    }
    // Init palette
    if (needtosetpalette)
    {
        sad::Vector<sad::Vector<sad::AColor> > default_palette;

        default_palette << sad::Vector<sad::AColor >();
        default_palette[0] << sad::AColor(255, 255, 255, 0);
        default_palette[0] << sad::AColor(255, 0, 0, 0);
        default_palette[0] << sad::AColor(0, 255, 0, 0);
        default_palette[0] << sad::AColor(0, 255, 255, 0);
        default_palette[0] << sad::AColor(0, 0, 255, 0);

        default_palette << sad::Vector<sad::AColor >();
        default_palette[1] << sad::AColor(192, 192, 192, 0);
        default_palette[1] << sad::AColor(192, 0, 0, 0);
        default_palette[1] << sad::AColor(0, 192, 0, 0);
        default_palette[1] << sad::AColor(0, 192, 192, 0);
        default_palette[1] << sad::AColor(0, 0, 192, 0);

        default_palette << sad::Vector<sad::AColor >();
        default_palette[2] << sad::AColor(164, 164, 164, 0);
        default_palette[2] << sad::AColor(164, 0, 0, 0);
        default_palette[2] << sad::AColor(0, 164, 0, 0);
        default_palette[2] << sad::AColor(0, 164, 164, 0);
        default_palette[2] << sad::AColor(0, 0, 164, 0);

        default_palette << sad::Vector<sad::AColor >();
        default_palette[3] << sad::AColor(128, 128, 128, 0);
        default_palette[3] << sad::AColor(128, 0, 0, 0);
        default_palette[3] << sad::AColor(0, 128, 0, 0);
        default_palette[3] << sad::AColor(0, 128, 128, 0);
        default_palette[3] << sad::AColor(0, 0, 128, 0);
        db->setProperty("palette", default_palette);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::initConsoleAutocompletion()
{
    QStringList constantslist;
    QStringList functionlist;

    m_scripting->propertiesAndFunctions(constantslist, functionlist);

    gui::codeedit::Highlighter::setPredefinedConstants(constantslist);
    gui::codeedit::Highlighter::setPredefinedFunctions(functionlist);

    QHash<QString, QString> replaceincompleter;
    replaceincompleter.insert("p2d","p2d(0, 0)");
    replaceincompleter.insert("p3d","p3d(0, 0, 0)");
    replaceincompleter.insert("p2i","p2i(0, 0)");
    replaceincompleter.insert("p3i","p3i(0, 0, 0)");
    replaceincompleter.insert("r2d","r2d(0, 0, 0, 0)");
    replaceincompleter.insert("r2i","r2i(0, 0, 0, 0)");
    replaceincompleter.insert("clr","clr(255, 255, 255)");
    replaceincompleter.insert("aclr","clr(255, 255, 255, 0)");
    replaceincompleter.insert("s2d","s2d(0, 0)");
    replaceincompleter.insert("s2i","s2i(0, 0)");

    gui::codeedit::Completer* consolecompleter = new gui::codeedit::Completer();
    ui.txtConsoleCode->setCompleter(consolecompleter);
    consolecompleter->setModel(
        consolecompleter->modelFromEngine(
            m_scripting->context(),
            replaceincompleter,
            m_scripting->commonProperties()
        )
    );
}

//====================  PROTECTED SLOTS HERE ====================

void MainPanel::addDatabaseProperty()
{
    this->scriptableAddProperty(
        Q2STDSTRING(ui.cmbDatabasePropertyType->currentText()),
        Q2STDSTRING(ui.txtDatabasePropertyName->text()),
        true
    );
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::updateMousePositionNow()
{
    ui.txtMousePosX->setText(QString::number(static_cast<int>(m_mousemove_point.x())));
    ui.txtMousePosY->setText(QString::number(static_cast<int>(m_mousemove_point.y())));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::highlightStateNow()
{
    ui.txtEditorState->setText(m_highlight_state);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::undo()
{
    m_editor->undo();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::redo()
{
    m_editor->redo();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::synchronizeDatabase()
{
    ui.rtwSpriteSprite->setFilter("sad::Sprite2D::Options");
    ui.rtwLabelFont->setFilter("sad::freetype::Font|sad::TextureMappedFont");
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::setAddingEnabled(bool enabled)
{
    this->ui.lstSceneObjects->setEnabled(enabled);
    this->ui.btnLabelAdd->setEnabled(enabled);
    this->ui.btnSpriteAdd->setEnabled(enabled);
    this->ui.btnCustomObjectAdd->setEnabled(enabled);
    this->ui.btnLayoutAdd->setEnabled(enabled);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::setSpriteChangingEnabled(bool enabled)
{
    this->ui.rwSceneNodeRect->setEnabled(enabled);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::setAngleChangingEnabled(bool enabled)
{
    ui.awSceneNodeAngle->setEnabled(enabled);
    if (enabled == false)
    {
        ui.awSceneNodeAngle->setValue(0);
    }
}

void MainPanel::clearDatabaseProperties()
{
    ui.twDatabaseProperties->clear();
    ui.twDatabaseProperties->setRowCount(0);
    for(sad::PtrHash<sad::String, gui::table::Delegate>::iterator it = m_property_delegates.begin();
        it != m_property_delegates.end();
        ++it)
    {
        it.value()->delRef();
    }
    m_property_delegates.clear();
}

void MainPanel::lockTypesTab(bool lock)
{
    if (lock)
    {
        m_type_locked_tab.setValue(ui.tabTypes->currentIndex());
    }
    else
    {
        m_type_locked_tab.clear();
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::updateAnimationsListFromTree()
{
    ui.cmbAnimationInstanceAnimationFromTree->clear();
    ui.cmbAnimationInstanceAnimationFromTree->addItem("Not set");
    
    QLinkedList< QPair<QString, sad::resource::Folder*> > folderstobeviewed;
    folderstobeviewed << QPair<QString, sad::resource::Folder*>("", sad::Renderer::ref()->tree("")->root());

    while(folderstobeviewed.count())
    {
        QPair<QString, sad::resource::Folder*> current = folderstobeviewed.front();
        folderstobeviewed.pop_front();

        QString prefix = current.first;
        sad::resource::Folder* folder  = current.second;
        sad::resource::FolderIterator it = folder->folderListBegin();
        for(; it != folder->folderListEnd(); ++it)
        {
            QString tmp = prefix;
            if (tmp.length())
            {
                tmp = tmp + "/" + STD2QSTRING(it.key().c_str());
            }
            else
            {
                tmp = STD2QSTRING(it.key().c_str());
            }
            folderstobeviewed << QPair<QString, sad::resource::Folder*>(tmp, it.value());
        }

        sad::resource::ResourceIterator rit = folder->resourceListBegin();
        for(; rit != folder->resourceListEnd(); ++ rit)
        {
            sad::resource::Resource* resource = rit.value();
            if (resource->metaData()->canBeCastedTo("sad::animations::Animation"))
            {
                QString tmp = prefix;
                if (tmp.length())
                {
                    tmp = tmp + "/" + STD2QSTRING(rit.key().c_str());
                }
                else
                {
                    tmp = STD2QSTRING(rit.key().c_str());
                }
                ui.cmbAnimationInstanceAnimationFromTree->addItem(tmp);
            }
        }
    }
    int pos = 0;
    if (m_editor)
    {
        sad::animations::Instance* instance = m_editor->shared()->selectedInstance();
        if (instance)
        {
            sad::String string = instance->animationName();
            if (string.size())
            {
                QString selectedanimationname = STD2QSTRING(string.c_str());
                for(size_t i = 0; i < ui.cmbAnimationInstanceAnimationFromTree->count(); i++)
                {
                    if (ui.cmbAnimationInstanceAnimationFromTree->itemText(i) == selectedanimationname)
                    {
                        pos = i;
                    }
                }
            }
        }
    }
    invoke_blocked(ui.cmbAnimationInstanceAnimationFromTree, &QComboBox::setCurrentIndex, pos);
}

QStringList MainPanel::resourcesByFilter(
    sad::resource::Folder* root, 
    const QString& prefix, 
    const QString& filter
)
{
    QStringList result;
    QStringList filterlist = filter.split("|");

    sad::resource::ResourceIterator cur = root->resourceListBegin();
    for(; cur != root->resourceListEnd(); ++cur)
    {
        bool shouldshowresource = true;
        if (filterlist.count())
        {
            const sad::String & name = cur.value()->metaData()->name();
            shouldshowresource = filterlist.indexOf(STD2QSTRING(name.c_str())) != -1;
        }
        if (shouldshowresource)
        {
            QString name = STD2QSTRING(cur.key().c_str());
            if (prefix.length() != 0)
            {
                name = prefix + "/" + name;
            }
            result << name;
        }
    }

    sad::resource::FolderIterator subfolders = root->folderListBegin();
    for(; subfolders != root->folderListEnd(); ++subfolders)
    {
        QString name = STD2QSTRING(subfolders.key().c_str());
        if (prefix.length() != 0)
        {
            name = prefix + "/" + name;
        }
        QStringList tmp = resourcesByFilter(subfolders.value(), name, filter);
        result << tmp;
    }

    return result;
}

bool MainPanel::scriptableAddProperty(const sad::String& propertytype, const sad::String& propertyname, bool fromeditor)
{
    sad::db::Database* db = sad::Renderer::ref()->database("");
    bool result = false;
    if (db->propertyByName(propertyname) == NULL && propertyname.size()!= 0)
    {
        gui::table::Delegate* d  = m_dbdelegate_factory.create(propertytype.c_str());
        sad::db::Property* prop = m_property_factory.create(propertytype);
        if (d && prop)
        {
            result = true;

            sad::Renderer::ref()->database("")->addProperty(propertyname, prop);
            d->setPropertyName(STD2QSTRING(propertyname.c_str()));
            d->linkToDatabase();
            d->makeLinkedTo(ui.twDatabaseProperties, m_editor);
            d->add();

            m_delegates_by_names.insert(propertyname, d);
            
            history::database::NewProperty* p = new history::database::NewProperty(d);
            if (fromeditor)
            {
                m_editor->history()->add(p);
            }
            else
            {
                m_editor->currentBatchCommand()->add(p);
            }
        }
        else
        {
            delete d;
            delete prop;
        }
    }
    return result;
}

gui::table::DelegateFactory* MainPanel::delegateFactory() const
{
    return &(const_cast<MainPanel*>(this)->m_dbdelegate_factory);
}

GridAndOffsets* MainPanel::gridAndOffset() const
{
    return m_offsets_window; 
}

void MainPanel::onWindowLeaveWhenChangingGlobalOffset()
{
    m_editor->machine()->enterState(m_editor->shared()->OldEditorState);
    this->highlightState(Q2STDSTRING(m_editor->shared()->OldHighlightString));
}


void MainPanel::enterGlobalOffsetEditingState(const sad::Point2D& p)
{
    // Do not allow to enter this state twice
    if (m_editor->machine()->isInState("changing_global_offset"))
    {
        return;
    }
    core::Shared* s = m_editor->shared();
    s->OldHighlightString = ui.txtEditorState->text();
    s->OldGlobalOffset = sad::Renderer::ref()->globalTranslationOffset();
    s->setOldPoint(p);
    s->OldEditorState = m_editor->machine()->currentState();
    m_editor->machine()->enterState("changing_global_offset");
    this->highlightState("Move mouse to change offset");
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::onMouseMoveWhenChangingGlobalOffset(const sad::input::MouseMoveEvent& ev)
{
    core::Shared* s = m_editor->shared();
    sad::Renderer* r = sad::Renderer::ref();
    sad::Point2D offset(r->globalTranslationOffset().x(), r->globalTranslationOffset().y());
    sad::Point2D oldOffset(s->OldGlobalOffset.x(), s->OldGlobalOffset.y());
    sad::Point2D diff = (ev.pos2D() + offset - oldOffset)  - s->oldPoint();
    sad::Vector3D v  = s->OldGlobalOffset;
    v -= sad::Point3D(diff.x(), diff.y(), 0);

    void (GridAndOffsets::*f)(const sad::Point3D&) const = &GridAndOffsets::updateGlobalOffsetInUI;
    m_editor->emitClosure(::bind(m_offsets_window,
        f,
        v
    ));
    sad::Renderer::ref()->database("")->setProperty("global_renderer_offset", static_cast<sad::Point3D>(v));
    sad::Renderer::ref()->setGlobalTranslationOffset(v);
}

void MainPanel::onMouseReleaseWhenChangingGlobalOffset(const sad::input::MouseReleaseEvent& ev)
{
    sad::input::MouseMoveEvent e;
    e.Point3D = ev.Point3D;
    this->onMouseMoveWhenChangingGlobalOffset(e);
    // Commit changes
    this->onWindowLeaveWhenChangingGlobalOffset();
}

void MainPanel::onKeyPressedInIdleState(const sad::input::KeyPressEvent& ev) const
{
    bool shouldchangegrid = false;
    if (m_editor->isInGridEditingState())
    {
        shouldchangegrid = m_editor->shared()->selectedGrid() != NULL;
    }
    if (shouldchangegrid)
    {
        m_editor->actions()->gridActions()->tryMoveSelectedGridByKeyboard(ev);
    }
    else
    {
        this->handleGlobalOffsetChange(ev);
    }
}

void MainPanel::handleGlobalOffsetChange(const sad::input::KeyPressEvent& ev) const
{
    sad::Point3D p(0, 0, 0);
    switch (ev.Key)
    {
        case sad::KeyLeft:  {p.setX(-1); break;}
        case sad::KeyRight:  {p.setX(1); break;}
        case sad::KeyUp:  {p.setY(1); break;}
        case sad::KeyDown:  {p.setY(-1); break;}
        default: break;
    };
    if (sad::non_fuzzy_zero(p.x()) || sad::non_fuzzy_zero(p.y()))
    {
        sad::Point3D pp = sad::Renderer::ref()->globalTranslationOffset() + p;
        sad::Renderer::ref()->setGlobalTranslationOffset(pp);
        sad::Renderer::ref()->database("")->setProperty("global_renderer_offset", pp);
        m_offsets_window->updateGlobalOffsetInUI(pp);
    }
}

void MainPanel::save()
{
    if (m_editor->shared()->fileName().length() == 0)
    {
        this->saveAs();
    }
    else
    {
        this->saveToFile();
    }
}

void MainPanel::saveAs()
{
    QString name = QFileDialog::getSaveFileName(this, "Enter file, where we should store database", "", "*.json");
    if (name.length() != 0)
    {
        m_editor->shared()->setFileName(name);
        this->saveToFile();
    }
}

void MainPanel::saveToFile() const
{
    sad::db::Database* db = sad::Renderer::ref()->database("");
    gui::RenderEditorGrid* render_editor_grid = m_editor->renderEditorGrid();
    db->setProperty("palette", ui.clpSceneNodeColor->palette());
    db->setProperty("global_renderer_offset", static_cast<sad::Point3D>(sad::Renderer::ref()->globalTranslationOffset()));
    db->setProperty("global_renderer_grid_enabled", render_editor_grid->isEnabled());
    db->setProperty("global_renderer_grid_settings", sad::Point2I(render_editor_grid->GridSpaceX, render_editor_grid->GridSpaceY));
    db->setProperty("global_renderer_grid_color", render_editor_grid->Color);


    db->saveToFile(Q2STDSTRING(m_editor->shared()->fileName()));
}

void MainPanel::load()
{
    if (m_editor->isInEditingState())
    {
        return;
    }
    QString name = QFileDialog::getOpenFileName(this, "Enter file, where database was stored", "", "*.json");
    if (name.length() != 0)
    {
        sad::db::Database* tmp = new sad::db::Database();
        tmp->setRenderer(sad::Renderer::ref());
        tmp->setDefaultTreeName("");
        std::string filename = Q2STDSTRING(name);
        if (tmp->loadFromFile(filename, sad::Renderer::ref()))
        {
            m_editor->shared()->setFileName(name);
            sad::Renderer::ref()->lockRendering();
            m_editor->cleanDatabase();
            sad::Renderer::ref()->addDatabase("", tmp);
            this->viewDatabase();
            sad::Renderer::ref()->unlockRendering();
        }
        else
        {
            delete tmp;
            QMessageBox::critical(NULL, "Failed to load database", "Failed to load database");
        }
    }
}

void MainPanel::loadResources()
{
    if (m_editor->isInEditingState())
    {
        return;
    }
    bool message_box_needed;
    bool proceed = true;
    if (ui.btnUndo->isEnabled() == false)
    {
        message_box_needed = false;
    }
    else
    {
        message_box_needed = (this->editor()->isDatabaseEmpty() == false);
    }
    if (message_box_needed) {
        QMessageBox::StandardButton btn = QMessageBox::warning(NULL, "Database will be erased", "Database will be erased. Proceed loading?", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
        proceed = (btn == QMessageBox::Ok);    
    }
    if (proceed)
    {
        QString name = QFileDialog::getOpenFileName(this, "Enter new resource file", "", "*.json");
        if (name.length())
        {
            sad::resource::Tree* tree = new sad::resource::Tree();
            tree->setRenderer(sad::Renderer::ref());
            tree->setStoreLinks(true);
            tree->factory()->registerResource<sad::freetype::Font>();
            
            sad::Vector<sad::resource::Error * > errors;
            errors = tree->loadFromFile(Q2STDSTRING(name));
            if (errors.size())
            {
                delete tree;
                this->m_editor->reportResourceLoadingErrors(errors, name.toStdString());               
            }
            else
            {
                sad::Renderer::ref()->lockRendering();
                m_editor->cleanDatabase();
                sad::Renderer::ref()->addDatabase("", new sad::db::Database());
                this->viewDatabase();
                sad::Renderer::ref()->unlockRendering();

                sad::Renderer::ref()->removeTree("");
                sad::Renderer::ref()->addTree("", tree);

                ui.txtFontListList->completer()->setModel(new QStringListModel(
                    this->resourcesByFilter(tree->root(), "", "sad::freetype::Font|sad::TextureMappedFont")
                ));

                ui.txtOptionListList->completer()->setModel(new QStringListModel(
                    this->resourcesByFilter(tree->root(), "", "sad::Sprite2D::Options")
                ));

                ui.txtTextureCoordinatesList->completer()->setModel(new QStringListModel(
                    this->resourcesByFilter(tree->root(), "", "sad::Sprite2D::Options")
                ));

                this->updateAnimationsListFromTree();

                this->updateResourceViews();
                this->toggleEditingButtons(true);
            }
        }
    }
}

void MainPanel::reloadResources()
{
    if (m_editor->isInEditingState())
    {
        return;
    }
    ReloadFileList list(this);
    if (list.exec() == QDialog::Accepted && list.selectedFile() != NULL)
    {
        m_editor->shared()->setActiveObject(NULL);
        m_editor->shared()->setSelectedObject(NULL);
        m_editor->machine()->enterState("idle");

        sad::resource::ResourceFile* file = list.selectedFile();
        sad::Renderer::ref()->lockRendering();
        sad::Vector<sad::resource::Error*> errors = file->reload();
        sad::Renderer::ref()->unlockRendering();
        if (errors.size() == 0)
        {
            sad::resource::Tree* tree = sad::Renderer::ref()->tree("");

            ui.txtFontListList->completer()->setModel(new QStringListModel(
                this->resourcesByFilter(tree->root(), "", "sad::freetype::Font|sad::TextureMappedFont")
            ));

            ui.txtOptionListList->completer()->setModel(new QStringListModel(
                this->resourcesByFilter(tree->root(), "", "sad::Sprite2D::Options")
            ));

            ui.txtTextureCoordinatesList->completer()->setModel(new QStringListModel(
                this->resourcesByFilter(tree->root(), "", "sad::Sprite2D::Options")
            ));

            this->updateAnimationsListFromTree();

            this->updateResourceViews();
        }
        else
        {
            m_editor->reportResourceLoadingErrors(errors, file->name());
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainPanel::tabTypeChanged(int index)
{
    // If tab is locked, switch to locked tab
    if (m_type_locked_tab.exists())
    {
        ui.tabTypes->setCurrentIndex(m_type_locked_tab.value());
        return;
    }

    int oldtypeindex = (m_editor->machine()->isInState("ways")) ? 1 : 0;
    if (oldtypeindex != index)
    {
        if (m_editor->isInEditingState())
        {
            invoke_blocked(ui.tabTypes, &QTabWidget::setCurrentIndex, oldtypeindex);
            return;
        }
        if (index == 0)
        {
            m_editor->tryEnterObjectEditingState();
        }
        if (index == 1)
        {
            m_editor->tryEnterWayEditingState();
        }
    }

    if (index == 2)
    {
        int row = ui.lstDialogues->currentRow(); 
        if (row >= 0)
        {
            sad::dialogue::Dialogue* w = ui.lstDialogues->item(row)->data(Qt::UserRole).value<sad::dialogue::Dialogue*>();
            m_editor->shared()->setSelectedDialogue(w);
            m_editor->actions()->dialogueActions()->dialogueChanged(row);
        }
    }

    if (index == 3)
    {
        int row = ui.lstAnimations->currentRow(); 
        if (row >= 0)
        {
            sad::animations::Animation* w = ui.lstAnimations->item(row)->data(Qt::UserRole).value<sad::animations::Animation*>();
            gui::actions::AnimationActions* actions = m_editor->actions()->animationActions();
            m_editor->shared()->setSelectedAnimation(w);
            actions->currentAnimationChanged(row);
        }
    }

    if (index == 4)
    {
        int row = ui.lstAnimationInstances->currentRow(); 
        if (row >= 0)
        {
            sad::animations::Instance* w = ui.lstAnimationInstances->item(row)->data(Qt::UserRole).value<sad::animations::Instance*>();
            m_editor->shared()->setSelectedInstance(w);
            m_editor->actions()->instanceActions()->currentInstanceChanged(row);
        }
    }

    if (index == 5)
    {
        int row = ui.lstAnimationsGroup->currentRow(); 
        if (row >= 0)
        {
            gui::actions::AnimationGroupActions* ag_actions = m_editor->actions()->groupActions();
            sad::animations::Group* w = ui.lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();
            m_editor->shared()->setSelectedGroup(w);
            ag_actions->currentGroupChanged(row);
        }
    }
    m_editor->selectionBorder()->toggleShowHotspot(index == 0);
}

