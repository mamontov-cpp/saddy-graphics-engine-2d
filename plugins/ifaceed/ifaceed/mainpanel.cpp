#include "mainpanel.h"
#include "blockedclosuremethodcall.h"
#include "reloadfilelist.h"

#include "core/editor.h"
#include "core/shared.h"
#include "core/selection.h"

#include "history/database/newproperty.h"

#include "history/scenes/scenesadd.h"
#include "history/scenes/scenesremove.h"
#include "history/scenes/scenesclear.h"
#include "history/scenes/sceneschangename.h"
#include "history/scenes/sceneslayerswap.h"

#include "history/scenenodes/scenenodeslayerswap.h"

#include "gui/scenenodeactions.h"
#include "gui/labelactions.h"
#include "gui/sprite2dactions.h"
#include "gui/customobjectactions.h"
#include "gui/wayactions.h"
#include "gui/dialogueactions.h"
#include "gui/animationactions.h"
#include "gui/updateelement.h"

#include <keymouseconditions.h>
#include <keycodes.h>

#include <db/save.h>
#include <db/dbdatabase.h>
#include <db/dbtable.h>
#include <db/dbstoredproperty.h>
#include <db/dbpopulatescenesfromdatabase.h>

#include <resource/tree.h>
#include <freetype/font.h>

#include <QFileDialog>
#include <QCompleter>
#include <QStringListModel>

#include <cstdio>

Q_DECLARE_METATYPE(sad::Scene*)
Q_DECLARE_METATYPE(sad::SceneNode*)
Q_DECLARE_METATYPE(sad::p2d::app::Way*)
Q_DECLARE_METATYPE(sad::dialogue::Dialogue*)
Q_DECLARE_METATYPE(sad::animations::Animation*)

//====================  PUBLIC METHODS HERE ====================

MainPanel::MainPanel(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), m_selfchanged(false)
{
	ui.setupUi(this);

    this->fillDatabasePropertyTypesCombo();

	ui.tabTypes->setCurrentIndex(0);

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

	m_scene_node_actions = new gui::SceneNodeActions();
	m_scene_node_actions->setPanel(this);

	m_label_actions = new gui::LabelActions();
	m_label_actions->setPanel(this);

	m_sprite2d_actions = new gui::Sprite2DActions();
	m_sprite2d_actions->setPanel(this);

	m_custom_object_actions = new gui::CustomObjectActions();
	m_custom_object_actions->setPanel(this);

	m_way_actions = new gui::WayActions();
	m_way_actions->setPanel(this);
	
	m_dialogue_actions = new gui::DialogueActions();
	m_dialogue_actions->setPanel(this);

	m_animation_actions = new gui::AnimationActions();
	m_animation_actions->setPanel(this);
}


MainPanel::~MainPanel()
{
	delete m_label_actions;
	delete m_sprite2d_actions;
	delete m_scene_node_actions;
	delete m_custom_object_actions;
	delete m_way_actions;
	delete m_dialogue_actions;
	delete m_animation_actions;
	for(sad::PtrHash<sad::String, gui::table::Delegate>::iterator it = m_property_delegates.begin();
		it != m_property_delegates.end();
		++it)
	{
		it.value()->delRef();
	}
	m_property_delegates.clear();
}

void MainPanel::toggleEditingButtons(bool enabled)
{
	const int affectedpushbuttonscount = 18;
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
		ui.btnLabelAdd,

		ui.btnSpriteMakeBackground,
		ui.btnSpriteAdd,
		ui.btnCustomObjectAdd
	};
	for(int i = 0; i < affectedpushbuttonscount; i++)
	{
		affectedpushbuttons[i]->setEnabled(enabled);
	}	
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

	sad::hfsm::Machine* m = editor->machine();
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

	sad::String wi = "ways/idle";
	sad::String ws = "ways/selected";
	sad::String wsm = "ways/selected/moving";

	// A bindings for idle state
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MousePress & sad::MouseLeft & (m * i),
		m_editor->selection(),
		&core::Selection::trySelect
	);

	// A bindings for moving object
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseMove & (m * sm),
		m_scene_node_actions,
		&gui::SceneNodeActions::moveObject
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseRelease & (m * sm),
		m_scene_node_actions,
		&gui::SceneNodeActions::commitObjectMoving
	);

	// A bindings for resizing object
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseMove & (m * sr),
		m_scene_node_actions,
		&gui::SceneNodeActions::resizeObject
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseRelease & (m * sr),
		m_scene_node_actions,
		&gui::SceneNodeActions::commitObjectResizing
	);

	// A bindings for selected node actions
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseWheel & (m * s),
        m_scene_node_actions,
        &gui::SceneNodeActions::navigateOrRotate
    );
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_KeyPress & sad::Esc & (m * s),
        m_scene_node_actions,
        &gui::SceneNodeActions::cancelSelection
    );
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MousePress & sad::MouseLeft & (m * s),
		m_editor->selection(),
		&core::Selection::trySelect
	);

	// A bindings for adding label
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_KeyPress & sad::Esc & (m * la), 
		m_label_actions, 
		&gui::LabelActions::cancelAddLabel
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseMove & (m * la),
		m_label_actions,
		&gui::LabelActions::moveLabel
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MousePress & sad::MouseLeft & (m * la),
		m_label_actions,
		&gui::LabelActions::commitLabelAdd
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseWheel & (m * la),
        m_scene_node_actions,
        &gui::SceneNodeActions::rotate
	);


	// A binding for adding sprite actions (just placing)
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_KeyPress & sad::Esc & (m * ssa), 
		m_sprite2d_actions, 
		&gui::Sprite2DActions::cancelAddSprite
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseMove & (m * ssa),
		m_sprite2d_actions,
		&gui::Sprite2DActions::moveCenterOfSprite
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MousePress & sad::MouseLeft & (m * ssa),
		m_sprite2d_actions,
		&gui::Sprite2DActions::commitAdd
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseWheel & (m * ssa),
        m_scene_node_actions,
        &gui::SceneNodeActions::rotate
	);

	// A binding for adding sprite (after first click)
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_KeyPress & sad::Esc & (m * sdap), 
		m_sprite2d_actions, 
		&gui::Sprite2DActions::cancelAddSprite
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MousePress & sad::MouseLeft & (m * sdap),
		m_sprite2d_actions, 
		&gui::Sprite2DActions::commitAdd
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseMove & (m * sdap),
		m_sprite2d_actions,
		&gui::Sprite2DActions::moveLowerPointOfSprite
	);

	// A binding for adding sprite (when first click determines upper-left corner)
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_KeyPress & sad::Esc & (m * sda), 
		m_sprite2d_actions, 
		&gui::Sprite2DActions::cancelAddSprite
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MousePress & sad::MouseLeft & (m * sda),
		m_sprite2d_actions, 
		&gui::Sprite2DActions::placeFirstPointForSprite
	);

	// A binding for adding custom object actions (just placing)
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_KeyPress & sad::Esc & (m * coa), 
		m_custom_object_actions, 
		&gui::CustomObjectActions::cancelAdd
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseMove & (m * coa),
		m_custom_object_actions,
		&gui::CustomObjectActions::moveCenterOfObject
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MousePress & sad::MouseLeft & (m * coa),
		m_custom_object_actions,
		&gui::CustomObjectActions::commitAdd
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseWheel & (m * coa),
        m_scene_node_actions,
        &gui::SceneNodeActions::rotate
	);

	// A binding for adding sprite (after first click)
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_KeyPress & sad::Esc & (m * coadp), 
		m_custom_object_actions, 
		&gui::CustomObjectActions::cancelAdd
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MousePress & sad::MouseLeft & (m * coadp),
		m_custom_object_actions,
		&gui::CustomObjectActions::commitAdd
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MouseMove & (m * coadp),
		m_custom_object_actions,
		&gui::CustomObjectActions::moveLowerPoint
	);

	// A binding for adding sprite (when first click determines upper-left corner)
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_KeyPress & sad::Esc & (m * coad), 
		m_custom_object_actions, 
		&gui::CustomObjectActions::cancelAdd
	);
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_MousePress & sad::MouseLeft & (m * coad),
		m_custom_object_actions, 
		&gui::CustomObjectActions::placeFirstPoint
	);

    // A binding for ways/selected/moving
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseMove & (m * wsm),
        m_way_actions,
        &gui::WayActions::moveWayPoint
    );
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseRelease & (m * wsm),
        m_way_actions,
        &gui::WayActions::commitWayPointMoving
    );

    // A binding for ways/selected
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * ws),
        m_editor->selection(),
        &core::Selection::trySelect
    );

    // A binding for ways/idle
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MousePress & sad::MouseLeft & (m * wi),
        m_editor->selection(),
        &core::Selection::trySelect
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
	
	connect(ui.btnSceneAdd, SIGNAL(clicked()), this, SLOT(addScene()));
	connect(ui.btnSceneDelete, SIGNAL(clicked()), this, SLOT(removeScene()));
	connect(ui.lstScenes, SIGNAL(currentRowChanged(int)), this, SLOT(currentSceneChanged(int)));
	connect(ui.txtSceneName, SIGNAL(textEdited(const QString&)), this, SLOT(sceneNameChanged(const QString&)));
	connect(ui.btnScenesMoveBack, SIGNAL(clicked()), this, SLOT(sceneMoveBack()));
	connect(ui.btnScenesMoveFront, SIGNAL(clicked()), this, SLOT(sceneMoveFront()));
	connect(ui.btnSceneClear, SIGNAL(clicked()), this, SLOT(clearScene()));
	
	connect(ui.txtObjectName, SIGNAL(textEdited(const QString&)), m_scene_node_actions, SLOT(nameEdited(const QString&)));
	connect(ui.cbSceneNodeVisible, SIGNAL(clicked(bool)), m_scene_node_actions, SLOT(visibilityChanged(bool)));
    connect(ui.clpSceneNodeColor, SIGNAL(selectedColorChanged(QColor)), m_scene_node_actions, SLOT(colorChanged(QColor)));
    connect(ui.rwSceneNodeRect, SIGNAL(valueChanged(QRectF)), m_scene_node_actions, SLOT(areaChanged(QRectF)));
    connect(ui.awSceneNodeAngle, SIGNAL(valueChanged(double)), m_scene_node_actions, SLOT(angleChanged(double)));
	connect(ui.lstSceneObjects, SIGNAL(currentRowChanged(int)), this, SLOT(currentSceneNodeChanged(int)));
	connect(ui.btnSceneNodeDelete, SIGNAL(clicked()), m_scene_node_actions, SLOT(removeSceneNode()));
	connect(ui.btnSceneNodeMoveBack, SIGNAL(clicked()), this, SLOT(sceneNodeMoveBack()));
	connect(ui.btnSceneNodeMoveFront, SIGNAL(clicked()), this, SLOT(sceneNodeMoveFront()));

	connect(ui.btnLabelAdd, SIGNAL(clicked()), m_label_actions, SLOT(addLabel()));
	connect(ui.rtwLabelFont, SIGNAL(selectionChanged(sad::String)), m_label_actions, SLOT(labelFontChanged(sad::String)));
    connect(ui.fswLabelFontSize, SIGNAL(valueChanged(unsigned int)), m_label_actions, SLOT(labelSizeChanged(unsigned int)));
    connect(ui.txtLabelText, SIGNAL(textChanged()), m_label_actions, SLOT(labelTextChanged()));
	connect(ui.dsbLineSpacingRatio, SIGNAL(valueChanged(double)), m_label_actions, SLOT(labelLineSpacingChanged(double)));

	connect(ui.btnSpriteAdd, SIGNAL(clicked()), m_sprite2d_actions, SLOT(add()));
	connect(ui.rtwSpriteSprite, SIGNAL(selectionChanged(sad::String)), m_sprite2d_actions, SLOT(spriteOptionsChanged(sad::String)));
	connect(ui.btnSpriteMakeBackground, SIGNAL(clicked()), m_sprite2d_actions, SLOT(makeBackground()));
	connect(ui.cbFlipX, SIGNAL(clicked(bool)), m_sprite2d_actions, SLOT(flipXChanged(bool)));
	connect(ui.cbFlipY, SIGNAL(clicked(bool)), m_sprite2d_actions, SLOT(flipYChanged(bool)));
    
	connect(ui.btnCustomObjectAdd, SIGNAL(clicked()), m_custom_object_actions, SLOT(add()));
	connect(ui.rtwCustomObjectSchemas, SIGNAL(selectionChanged(sad::String)), m_custom_object_actions, SLOT(schemaChanged(sad::String)));

    connect(ui.lstWays, SIGNAL(currentRowChanged(int)), m_way_actions, SLOT(wayChanged(int)));
    connect(ui.btnWayAdd, SIGNAL(clicked()), m_way_actions, SLOT(addWay()));
    connect(ui.btnWayRemove, SIGNAL(clicked()), m_way_actions, SLOT(removeWay()));
    connect(ui.txtWayName, SIGNAL(textEdited(const QString&)), m_way_actions, SLOT(nameEdited(const QString&)));
    connect(ui.cbWayClosed, SIGNAL(clicked(bool)), m_way_actions, SLOT(closednessChanged(bool)));
    connect(ui.dsbWayTotalTime, SIGNAL(valueChanged(double)), m_way_actions, SLOT(totalTimeChanged(double)));
    connect(ui.lstWayPoints, SIGNAL(currentRowChanged(int)), m_way_actions, SLOT(viewPoint(int)));
    connect(ui.btnWayPointAdd, SIGNAL(clicked()), m_way_actions, SLOT(addWayPoint()));
    connect(ui.btnWayPointRemove, SIGNAL(clicked()), m_way_actions, SLOT(removeWayPoint()));
	connect(ui.dsbWayPointX, SIGNAL(valueChanged(double)), m_way_actions, SLOT(wayPointXChanged(double)));
	connect(ui.dsbWayPointY, SIGNAL(valueChanged(double)), m_way_actions, SLOT(wayPointYChanged(double)));
	connect(ui.btnWayPointMoveBack, SIGNAL(clicked()), m_way_actions, SLOT(wayPointMoveBack()));
	connect(ui.btnWayPointMoveFront, SIGNAL(clicked()), m_way_actions, SLOT(wayPointMoveFront()));

	connect(ui.lstDialogues, SIGNAL(currentRowChanged(int)), m_dialogue_actions, SLOT(dialogueChanged(int)));
	connect(ui.lstPhrases, SIGNAL(currentRowChanged(int)), m_dialogue_actions, SLOT(phraseChanged(int)));
	connect(ui.btnDialogueAdd, SIGNAL(clicked()), m_dialogue_actions, SLOT(addDialogue()));
	connect(ui.btnDialogueRemove, SIGNAL(clicked()), m_dialogue_actions, SLOT(removeDialogue()));
	connect(ui.txtDialogueName, SIGNAL(textEdited(const QString&)), m_dialogue_actions, SLOT(nameEdited(const QString&)));
	connect(ui.btnPhraseAdd, SIGNAL(clicked()), m_dialogue_actions, SLOT(addPhrase()));
    connect(ui.btnPhraseRemove, SIGNAL(clicked()), m_dialogue_actions, SLOT(removePhrase()));
	connect(ui.btnPhraseMoveBack, SIGNAL(clicked()), m_dialogue_actions, SLOT(movePhraseBack()));
	connect(ui.btnPhraseMoveFront, SIGNAL(clicked()), m_dialogue_actions, SLOT(movePhraseFront()));
	connect(ui.dsbPhraseDuration, SIGNAL(valueChanged(double)), m_dialogue_actions, SLOT(durationChanged(double)));
	connect(ui.txtPhrasePhrase, SIGNAL(textChanged()), m_dialogue_actions, SLOT(phraseTextChanged()));
	connect(ui.txtPhraseActorName, SIGNAL(textEdited(const QString&)), m_dialogue_actions, SLOT(actorNameChanged(const QString&)));
	connect(ui.txtPhraseActorPortrait, SIGNAL(textEdited(const QString&)), m_dialogue_actions, SLOT(actorPortraitChanged(const QString&)));
	connect(ui.txtPhraseViewHint, SIGNAL(textEdited(const QString&)), m_dialogue_actions, SLOT(viewHintChanged(const QString&)));

	connect(ui.btnAnimationsAdd, SIGNAL(clicked()), m_animation_actions, SLOT(addAnimation()));
	connect(ui.lstAnimations, SIGNAL(currentRowChanged(int)), m_animation_actions, SLOT(currentAnimationChanged(int)));
	connect(ui.txtAnimationName, SIGNAL(textEdited(const QString&)), m_animation_actions, SLOT(nameChanged(const QString&)));
	connect(ui.dsbAnimationTime, SIGNAL(valueChanged(double)), m_animation_actions, SLOT(timeChanged(double)));
	connect(ui.cbAnimationLooped, SIGNAL(clicked(bool)), m_animation_actions, SLOT(loopedChanged(bool)));
    connect(ui.sbBlinkingFrequency, SIGNAL(valueChanged(int)), m_animation_actions, SLOT(blinkingFrequencyChanged(int)));
	connect(ui.btnAnimationsStart, SIGNAL(clicked()), m_animation_actions, SLOT(startOnObject()));
	connect(ui.btnAnimationsCancel, SIGNAL(clicked()), m_animation_actions, SLOT(stopOnObject()));
	connect(ui.cwColorStartingColor, SIGNAL(clicked()), m_animation_actions, SLOT(colorChangeStartingColor()));
	connect(ui.cwColorEndingColor, SIGNAL(clicked()), m_animation_actions, SLOT(colorChangeEndingColor()));
	connect(ui.dabResizeVectorX, SIGNAL(valueChanged(double)), m_animation_actions, SLOT(resizeChangeVectorX(double)));
	connect(ui.dabResizeVectorY, SIGNAL(valueChanged(double)), m_animation_actions, SLOT(resizeChangeVectorY(double)));
	connect(ui.dsbRotateStartingAngle, SIGNAL(valueChanged(double)), m_animation_actions, SLOT(rotateChangeStartingAngle(double)));
	connect(ui.dsbRotateEndingAngle, SIGNAL(valueChanged(double)), m_animation_actions, SLOT(rotateChangeEndingAngle(double)));
	connect(ui.cmbWayAnimationWay, SIGNAL(currentIndexChanged(int)), m_animation_actions, SLOT(wayMovingChangeWay(int)));
	connect(ui.txtFontListList, SIGNAL(textEditingFinished()), m_animation_actions, SLOT(fontListEditingFinished()));
	connect(ui.sbFontSizeStartingSize, SIGNAL(valueChanged(int)), m_animation_actions, SLOT(fontSizeChangeStartingSize(int)));
	connect(ui.sbFontSizeEndingSize, SIGNAL(valueChanged(int)), m_animation_actions, SLOT(fontSizeChangeEndingSize(int)));
	
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
}

core::Editor* MainPanel::editor() const
{
    return m_editor;
}

gui::SceneNodeActions* MainPanel::sceneNodeActions() const
{
    return m_scene_node_actions;
}

gui::LabelActions* MainPanel::labelActions() const
{
    return m_label_actions;
}

gui::Sprite2DActions* MainPanel::sprite2DActions() const
{
	return m_sprite2d_actions;
}

gui::CustomObjectActions* MainPanel::customObjectActions() const
{
	return m_custom_object_actions;
}

gui::WayActions* MainPanel::wayActions() const
{
	return m_way_actions;
}

gui::DialogueActions* MainPanel::dialogueActions() const
{
	return m_dialogue_actions;
}

gui::AnimationActions* MainPanel::animationActions() const
{
	return m_animation_actions;
}

Ui::MainPanelClass* MainPanel::UI()
{
    return &ui;
}

void MainPanel::viewDatabase()
{
	this->fixDatabase();
	sad::db::Database* db = sad::Renderer::ref()->database("");
	ui.clpSceneNodeColor->setPalette(db->getProperty<QList<QList<QColor> > >("palette").value());

	// Remove old delegates
	for(sad::PtrHash<sad::String, gui::table::Delegate>::iterator it = m_property_delegates.begin();
		it != m_property_delegates.end();
		++it)
	{
		it.value()->delRef();
	}

	for(sad::db::Database::Properties::const_iterator it = db->begin();
		it != db->end();
		++it)
	{
		// Skip palette
		if (it.key() != "palette" && it.value()->pointerStarsCount() == 0)
		{
			gui::table::Delegate* d = m_dbdelegate_factory.create(it.value()->baseType().c_str());
			if (d)
			{
				d->makeLinkedTo(ui.twDatabaseProperties, m_editor);
				d->setPropertyName(it.key().c_str());
				d->linkToDatabase();
				d->add();				
			}
		}
	}

	sad::db::populateScenesFromDatabase(sad::Renderer::ref(), sad::Renderer::ref()->database(""));

	const sad::Vector<sad::Scene*>& scenes = sad::Renderer::ref()->scenes(); 
	for(unsigned int i = 0; i < scenes.size(); i++)
	{
		addSceneToSceneList(scenes[i]);
	}

	// Cleanup a combo for animation way
	sad::p2d::app::Way* nullway = NULL;
	QVariant nullwayvariant;
	nullwayvariant.setValue(nullway);
	ui.cmbWayAnimationWay->addItem("Not set", nullwayvariant);
	ui.cmbWayAnimationInstanceWay->addItem("Not set", nullwayvariant);

    sad::Vector<sad::db::Object*> wayslist;
    db->table("ways")->objects(wayslist);
    for(unsigned int i = 0; i < wayslist.size(); i++)
    {
        sad::p2d::app::Way* w = static_cast<sad::p2d::app::Way*>(wayslist[i]);
        addLastWayToEnd(w);
    }

	sad::Vector<sad::db::Object*> dialoguelist;
    db->table("dialogues")->objects(dialoguelist);
    for(unsigned int i = 0; i < dialoguelist.size(); i++)
    {
		sad::dialogue::Dialogue* w = static_cast<sad::dialogue::Dialogue*>(dialoguelist[i]);
        addDialogueToDialogueList(w);
    }
}

QList<QList<QColor> >  MainPanel::colorPalette() const
{
	return ui.clpSceneNodeColor->palette();
}

void MainPanel::setColorPalette(const QList<QList<QColor> >& palette)
{
	ui.clpSceneNodeColor->setPalette(palette);
	sad::db::Database* db = sad::Renderer::ref()->database("");
	db->setProperty("palette", palette);
}

bool  MainPanel::takeDelegateByPropertyName(const QString & name)
{
	bool owns = false;
	if (m_property_delegates.contains(name.toStdString()))
	{
		m_property_delegates.remove(name.toStdString());
	}
	return owns;
}

void MainPanel::addSceneToSceneList(sad::Scene* s)
{
	QString name = this->viewableObjectName(s);
	QListWidgetItem* i =  new QListWidgetItem();
	i->setText(name);
	
	QVariant v;
	v.setValue(s);
	i->setData(Qt::UserRole, v);
	ui.lstScenes->addItem(i);
}

void MainPanel::removeLastSceneFromSceneList()
{
	if (ui.lstScenes->count())
	{
		QListWidgetItem* i = ui.lstScenes->takeItem(ui.lstScenes->count() - 1);
		delete i;
	}
}

void MainPanel::insertSceneToSceneList(sad::Scene* s, int position)
{
	QString name = this->viewableObjectName(s);
	QListWidgetItem* i =  new QListWidgetItem();
	i->setText(name);
	
	QVariant v;
	v.setValue(s);
	i->setData(Qt::UserRole, v);
	ui.lstScenes->insertItem(position, i);
}

void MainPanel::removeSceneFromSceneList(int position)
{
	QListWidgetItem* i =  ui.lstScenes->takeItem(position);
	delete i;
}

sad::Scene* MainPanel::currentScene()
{
	QListWidgetItem* item = ui.lstScenes->currentItem();
	sad::Scene* scene = NULL;
	if (item)
	{
		scene = item->data(Qt::UserRole).value<sad::Scene*>();
	}
	return scene;
}

void MainPanel::updateSceneName(sad::Scene* s)
{
	int row = this->findSceneInList(s);
	if (row != -1)
	{
		ui.lstScenes->item(row)->setText(this->viewableObjectName(s));
	}
	if (s == currentScene())
	{
		bool b = ui.txtSceneName->blockSignals(true);
		ui.txtSceneName->setText(s->objectName().c_str());
		ui.txtSceneName->blockSignals(b);
	}
}

int MainPanel::findSceneInList(sad::Scene* s)
{
	int row = -1;
	for(int i = 0; i < ui.lstScenes->count(); i++)
	{	
		if (ui.lstScenes->item(i)->data(Qt::UserRole).value<sad::Scene*>() == s)
		{
			row = i;
		}
	}
	return row;
}

void MainPanel::setScenesInList(sad::Scene* s1, sad::Scene* s2, int pos1, int pos2)
{
	sad::Scene* s = this->currentScene();
	ui.lstScenes->item(pos1)->setText(this->viewableObjectName(s1));
	QVariant v1;
	v1.setValue(s1);
	ui.lstScenes->item(pos1)->setData(Qt::UserRole, v1);

	ui.lstScenes->item(pos2)->setText(this->viewableObjectName(s2));
	QVariant v2;
	v2.setValue(s2);
	ui.lstScenes->item(pos2)->setData(Qt::UserRole, v2);

	if (s == s1 || s == s2)
	{
		this->currentSceneChanged(ui.lstScenes->currentRow());
	}
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

void MainPanel::addSceneNodeToSceneNodeList(sad::SceneNode* s)
{
	QString name = this->viewableObjectName(s);
	QListWidgetItem* i =  new QListWidgetItem();
	i->setText(name);
	
	QVariant v;
	v.setValue(s);
	i->setData(Qt::UserRole, v);
	ui.lstSceneObjects->addItem(i);
}

void MainPanel::removeLastSceneNodeFromSceneNodeList()
{
	if (ui.lstSceneObjects->count())
	{
		QListWidgetItem* i = ui.lstSceneObjects->takeItem(ui.lstSceneObjects->count() - 1);
		delete i;
	}
}

void MainPanel::insertSceneNodeToSceneNodeList(sad::SceneNode* s, int position)
{
	QString name = this->viewableObjectName(s);
	QListWidgetItem* i =  new QListWidgetItem();
	i->setText(name);
	
	QVariant v;
	v.setValue(s);
	i->setData(Qt::UserRole, v);
	ui.lstSceneObjects->insertItem(position, i);
}

void MainPanel::removeSceneNodeFromSceneNodeList(int position)
{
	QListWidgetItem* i =  ui.lstSceneObjects->takeItem(position);
	delete i;
}

void MainPanel::removeSceneNodeFromSceneNodeListByNode(sad::SceneNode* s)
{
	int position = this->findSceneNodeInList(s);
	if (position >= 0)
	{
		removeSceneNodeFromSceneNodeList(position);
	}
}

void MainPanel::setSceneNodesInList(sad::SceneNode* n1, sad::SceneNode* n2, int pos1, int pos2)
{
	sad::SceneNode* s = this->editor()->shared()->selectedObject();
	ui.lstSceneObjects->item(pos1)->setText(this->viewableObjectName(n1));
	QVariant v1;
	v1.setValue(n1);
	ui.lstSceneObjects->item(pos1)->setData(Qt::UserRole, v1);

	ui.lstSceneObjects->item(pos2)->setText(this->viewableObjectName(n2));
	QVariant v2;
	v2.setValue(n2);
	ui.lstSceneObjects->item(pos2)->setData(Qt::UserRole, v2);

	if (s == n1)
	{
		void (QListWidget::*row)(int) = &QListWidget::setCurrentRow;
		invoke_blocked(ui.lstSceneObjects, row, pos1);
		this->currentSceneNodeChanged(ui.lstSceneObjects->currentRow());
	}
	if (s == n2)
	{
		void (QListWidget::*row)(int) = &QListWidget::setCurrentRow;
		invoke_blocked(ui.lstSceneObjects, row, pos2);
		this->currentSceneNodeChanged(ui.lstSceneObjects->currentRow());
	}
}

int MainPanel::findSceneNodeInList(sad::SceneNode* s)
{
	int row = -1;
	for(int i = 0; i < ui.lstSceneObjects->count(); i++)
	{	
		if (ui.lstSceneObjects->item(i)->data(Qt::UserRole).value<sad::SceneNode*>() == s)
		{
			row = i;
		}
	}
	return row;
}

void MainPanel::updateSceneNodeName(sad::SceneNode* s)
{
	int row = this->findSceneNodeInList(s);
	if (row != -1)
	{
		ui.lstSceneObjects->item(row)->setText(this->viewableObjectName(s));
	}
	if (s == m_editor->shared()->selectedObject() || s == m_editor->shared()->activeObject())
	{
		bool b = ui.txtSceneName->blockSignals(true);
		ui.txtSceneName->setText(s->objectName().c_str());
		ui.txtSceneName->blockSignals(b);
	}
}

void MainPanel::addLastWayToEnd(sad::p2d::app::Way* way)
{
	QString nameforway = this->viewableObjectName(way);
    
	ui.lstWays->addItem(nameforway);
    
	QVariant v;
    v.setValue(way);
    ui.lstWays->item(ui.lstWays->count()-1)->setData(Qt::UserRole, v);

	ui.cmbWayAnimationWay->addItem(nameforway, v);
	ui.cmbWayAnimationInstanceWay->addItem(nameforway, v);

}

void MainPanel::removeLastWayFromWayList()
{
    if (ui.lstWays->count() > 0)
    {
        QVariant v = ui.lstWays->item(ui.lstWays->count() - 1)->data(Qt::UserRole);
        sad::p2d::app::Way* w  = v.value<sad::p2d::app::Way*>();
        if (w == m_editor->shared()->selectedWay())
        {
            m_editor->shared()->setSelectedWay(NULL);
            m_editor->machine()->enterState("ways/idle");
        }
        delete ui.lstWays->takeItem(ui.lstWays->count() - 1);

		int pos = this->findInComboBox(ui.cmbWayAnimationWay, w);
		if (pos >= 0)
		{
			ui.cmbWayAnimationWay->removeItem(pos);
		}

		pos = this->findInComboBox(ui.cmbWayAnimationInstanceWay, w);
		if (pos >= 0)
		{
			ui.cmbWayAnimationInstanceWay->removeItem(pos);
		}
    }
}

void MainPanel::insertWayToWayList(sad::p2d::app::Way* s, int position)
{
    QListWidgetItem* i = new QListWidgetItem(this->viewableObjectName(s));
    QVariant v;
    v.setValue(s);
    i->setData(Qt::UserRole, v);
    ui.lstWays->insertItem(position, i);
}

void MainPanel::removeWayFromWayList(int position)
{
    QVariant v = ui.lstWays->item(position)->data(Qt::UserRole);
    sad::p2d::app::Way* w  = v.value<sad::p2d::app::Way*>();
    if (w == m_editor->shared()->selectedWay())
    {
        m_editor->shared()->setSelectedWay(NULL);
        m_editor->machine()->enterState("ways/idle");
    }
    delete ui.lstWays->takeItem(position);
}

void MainPanel::removeWayFromWayList(sad::p2d::app::Way* s)
{
    int pos = this->findWayInList(s);
    if (s == m_editor->shared()->selectedWay())
    {
        m_editor->shared()->setSelectedWay(NULL);
        m_editor->machine()->enterState("ways/idle");
    }
    if (pos >= 0)
    {
        delete ui.lstWays->takeItem(pos);
    }
}

int MainPanel::findWayInList(sad::p2d::app::Way* s)
{
    for(int i = 0; i < ui.lstWays->count(); i++)
    {
        QVariant v = ui.lstWays->item(i)->data(Qt::UserRole);
        sad::p2d::app::Way* w  = v.value<sad::p2d::app::Way*>();
        if (w == s)
        {
            return i;
        }
    }
    return -1;
}

void MainPanel::updateWayName(sad::p2d::app::Way* s)
{
	QString name = this->viewableObjectName(s);
    int row = this->findWayInList(s);
    if (row != -1)
    {
        ui.lstWays->item(row)->setText(name);
    }

	int pos = this->findInComboBox(ui.cmbWayAnimationWay, s);
	if (pos >= 0)
	{
		ui.cmbWayAnimationWay->setItemText(pos, name);
	}

	pos = this->findInComboBox(ui.cmbWayAnimationInstanceWay, s);
	if (pos >= 0)
	{
		ui.cmbWayAnimationInstanceWay->setItemText(pos, name);
	}
}

void MainPanel::removeRowInWayPointList(int row)
{
    if (row >=0  && row < ui.lstWayPoints->count()) {
        delete ui.lstWayPoints->takeItem(row);
    }
}

QString MainPanel::nameForPoint(const sad::Point2D& p) const
{
    return QString("(%1,%2)")
           .arg(static_cast<int>(p.x()))
           .arg(static_cast<int>(p.y()));
}

void MainPanel::addDialogueToDialogueList(sad::dialogue::Dialogue* dialogue)
{
    ui.lstDialogues->addItem(this->viewableObjectName(dialogue));
    QVariant v;
    v.setValue(dialogue);
    ui.lstDialogues->item(ui.lstDialogues->count()-1)->setData(Qt::UserRole, v);
}

void MainPanel::removeLastDialogueFromDialogueList()
{
    if (ui.lstDialogues->count() > 0)
    {
        QVariant v = ui.lstDialogues->item(ui.lstDialogues->count() - 1)->data(Qt::UserRole);
		sad::dialogue::Dialogue* w  = v.value<sad::dialogue::Dialogue*>();
        if (w == m_editor->shared()->selectedDialogue())
        {
            m_editor->shared()->setSelectedDialogue(NULL);
        }
        delete ui.lstDialogues->takeItem(ui.lstDialogues->count() - 1);
    }
}

void MainPanel::insertDialogueToDialogueList(sad::dialogue::Dialogue* s, int position)
{
    QListWidgetItem* i = new QListWidgetItem(this->viewableObjectName(s));
    QVariant v;
    v.setValue(s);
    i->setData(Qt::UserRole, v);
    ui.lstDialogues->insertItem(position, i);
}

void MainPanel::removeDialogueFromDialogueList(int position)
{
    QVariant v = ui.lstDialogues->item(position)->data(Qt::UserRole);
    sad::dialogue::Dialogue* w  = v.value<sad::dialogue::Dialogue*>();
    if (w == m_editor->shared()->selectedDialogue())
    {
        m_editor->shared()->setSelectedDialogue(NULL);
    }
    delete ui.lstDialogues->takeItem(position);
}

void MainPanel::removeDialogueFromDialogueList(sad::dialogue::Dialogue* s)
{
    int pos = this->findDialogueInList(s);
    if (s == m_editor->shared()->selectedDialogue())
    {
        m_editor->shared()->setSelectedDialogue(NULL);
    }
    if (pos >= 0)
    {
        delete ui.lstDialogues->takeItem(pos);
    }
}

int MainPanel::findDialogueInList(sad::dialogue::Dialogue* s)
{
    for(int i = 0; i < ui.lstDialogues->count(); i++)
    {
        QVariant v = ui.lstDialogues->item(i)->data(Qt::UserRole);
        sad::dialogue::Dialogue* w  = v.value<sad::dialogue::Dialogue*>();
        if (w == s)
        {
            return i;
        }
    }
    return -1;
}

void MainPanel::updateDialogueName(sad::dialogue::Dialogue* s)
{
	int row = this->findDialogueInList(s);
	if (row != -1)
	{
		ui.lstDialogues->item(row)->setText(this->viewableObjectName(s));
	}
}

void MainPanel::removePhraseFromPhraseList(int row)
{
	delete ui.lstPhrases->takeItem(row);
}

QString MainPanel::nameForPhrase(const sad::dialogue::Phrase& p) const
{
	sad::String s = p.phrase();
	if (s.length() > 3)
	{
		s = s.subString(0, 3);
		s += "...";
	}
	return QString("%1(%2)")
		   .arg(p.actorName().c_str())
		   .arg(s.c_str());
}

QString MainPanel::nameForAnimation(sad::animations::Animation* a) const
{
	QString result = const_cast<MainPanel*>(this)->viewableObjectName(a);
	if (a)
	{
		sad::String s = a->serializableName();
		s.removeAllOccurences("sad::animations::");
		s.insert('[', 0);
		s << "] ";
		result = QString(s.c_str()) + result;
	}
	return result;
}

void MainPanel::addAnimationToViewingLists(sad::animations::Animation* a)
{
	QString name = this->nameForAnimation(a);

	QVariant v;
	v.setValue(a);

	QListWidget* listofanimations = ui.lstAnimations;
	listofanimations->addItem(name);
	listofanimations->item(listofanimations->count() - 1)->setData(Qt::UserRole, v);

	QComboBox* listofanimationsforinstances = ui.cmbAnimationInstanceAnimationFromDatabase;
	listofanimationsforinstances->addItem(name);
	listofanimationsforinstances->setItemData(listofanimationsforinstances->count() - 1, v, Qt::UserRole);

	QListWidget* listofanimationcandidates = ui.lstCompositeCandidates;
	listofanimationcandidates->addItem(name);
	listofanimationcandidates->item(listofanimations->count() - 1)->setData(Qt::UserRole, v);
}


void MainPanel::removeAnimationFromViewingLists(sad::animations::Animation* a)
{
	int pos = this->findInList(ui.lstAnimations, a);
	if (pos > -1)
	{
		delete ui.lstAnimations->takeItem(pos);
	}

	pos = this->findInComboBox(ui.cmbAnimationInstanceAnimationFromDatabase, a);
	if (pos > - 1)
	{
		ui.cmbAnimationInstanceAnimationFromDatabase->removeItem(pos);
	}

	pos = this->findInList(ui.lstCompositeCandidates, a);
	if (pos > -1)
	{
		delete ui.lstCompositeCandidates->takeItem(pos);
	}

}

void MainPanel::updateAnimationName(sad::animations::Animation* a)
{
	QString name = this->nameForAnimation(a);

	int pos = this->findInList(ui.lstAnimations, a);
	if (pos > -1)
	{
		ui.lstAnimations->item(pos)->setText(name);
	}

	pos = this->findInComboBox(ui.cmbAnimationInstanceAnimationFromDatabase, a);
	if (pos > - 1)
	{
		ui.cmbAnimationInstanceAnimationFromDatabase->setItemText(pos, name);
	}

	pos = this->findInList(ui.lstCompositeCandidates, a);
	if (pos > -1)
	{
		ui.lstCompositeCandidates->item(pos)->setText(name);
	}

	pos = this->findInList(ui.lstCompositeList, a);
	if (pos > -1)
	{
		ui.lstCompositeList->item(pos)->setText(name);
	}
}

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
		ui.dabResizeVectorX,
		ui.dabResizeVectorY,
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
		NULL
	};
	size_t i = 0;
	while(widgets[i] != NULL)
	{
		widgets[i]->setEnabled(flag);
		++i;
	}
}

QCheckBox* MainPanel::visibilityCheckbox() const
{
	return ui.cbSceneNodeVisible;	
}

QCheckBox* MainPanel::flipXCheckbox() const
{
	return ui.cbFlipX;	
}

QCheckBox* MainPanel::flipYCheckbox() const
{
	return ui.cbFlipY;	
}

void MainPanel::clearCustomObjectPropertiesTable()
{
    for(size_t i = 0; i < ui.twCustomObjectProperties->rowCount(); i++)
    {
        QVariant  v = ui.twCustomObjectProperties->item(i, 0)->data(Qt::UserRole);
        gui::table::Delegate* d = v.value<gui::table::Delegate*>();
        if (d)
        {
            d->disconnectSlots();
            delete d;
        }
    }
    ui.twCustomObjectProperties->clear();
	ui.twCustomObjectProperties->setRowCount(0);
}

 gui::table::Delegate* MainPanel::delegateForCustomObjectProperty(const QString& name)
 {
     for(size_t i = 0; i < ui.twCustomObjectProperties->rowCount(); i++)
     {
         if (ui.twCustomObjectProperties->item(i, 0)->text() == name) {
             QVariant  v = ui.twCustomObjectProperties->item(i, 0)->data(Qt::UserRole);
             gui::table::Delegate* d = v.value<gui::table::Delegate*>();
             return d;
         }
     }
     return NULL;
 }

void MainPanel::updateCustomObjectPropertyValue(
     sad::SceneNode* node,
     const sad::String& name,
     const sad::db::Variant& value
)
{
    if (m_editor->isNodeSelected(node))
    {
        m_custom_object_property_name = name;
        m_custom_object_property_value = value;
        QTimer::singleShot(0, this, SLOT(updateCustomObjectPropertyValueNow()));
    }
}

void MainPanel::fillCustomObjectProperties(
	sad::SceneNode* node	
)
{
	this->clearCustomObjectPropertiesTable();
	if (node)
	{
		if (node->metaData()->canBeCastedTo("sad::db::custom::Object"))
		{
			sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(node);
			const sad::Hash<sad::String, sad::db::Property*>& db = o->schemaProperties();
			for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = db.const_begin();
				it != db.const_end();
				++it)
			{
				gui::table::Delegate* d = m_dbdelegate_factory.create(it.value()->baseType().c_str());
				if (d)
				{
					d->makeLinkedTo(ui.twCustomObjectProperties, m_editor);
					d->setPropertyName(it.key().c_str());
					d->linkToCustomObject(o);
					d->add();				
				}
			}
		}
	}
}

void MainPanel::selectLastSceneNode()
{
	if (ui.lstSceneObjects->count() != 0)
	{
		bool b = ui.lstSceneObjects->blockSignals(true);
		ui.lstSceneObjects->setCurrentRow(ui.lstSceneObjects->count() - 1);
		ui.lstSceneObjects->blockSignals(b);
	}
}

//====================  PUBLIC SLOTS METHODS HERE ====================

void MainPanel::updateUIForSelectedItem()
{
	QTimer::singleShot(0, this, SLOT(updateUIForSelectedItemNow()));
}

void MainPanel::updateUIForSelectedItemNow()
{
	sad::SceneNode* node = m_editor->shared()->selectedObject();
	if (node)
	{
        // Scene tab
        int row = this->findSceneNodeInList(node);
        if (row != ui.lstSceneObjects->currentRow()) {
            void (QListWidget::*setRow)(int) = &QListWidget::setCurrentRow;
            invoke_blocked(ui.lstSceneObjects, setRow, row);
        }
        invoke_blocked(ui.txtObjectName, &QLineEdit::setText, node->objectName().c_str());

        // SceneNode tab
        m_scene_node_actions->updateRegionForNode();
        sad::Maybe<bool> maybevisible = node->getProperty<bool>("visible");
        if (maybevisible.exists())
        {
            invoke_blocked(ui.cbSceneNodeVisible, &QCheckBox::setCheckState, (maybevisible.value()) ? Qt::Checked : Qt::Unchecked);
        }
        gui::UpdateElement<double>::with(node, "angle", ui.awSceneNodeAngle, &gui::anglewidget::AngleWidget::setValue);
        gui::UpdateElement<QColor>::with(node, "color", ui.clpSceneNodeColor, &gui::colorpicker::ColorPicker::setSelectedColor);

        // Label tab
        gui::UpdateElement<sad::String>::with(node, "font", ui.rtwLabelFont, &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName);
        gui::UpdateElement<unsigned int>::with(node, "fontsize", ui.fswLabelFontSize, &gui::fontsizewidget::FontSizeWidget::setValue);
        gui::UpdateElement<float>::with(node, "linespacing", ui.dsbLineSpacingRatio, &QDoubleSpinBox::setValue);
        gui::UpdateElement<QString>::with(node, "text", ui.txtLabelText, &QPlainTextEdit::setPlainText);

        // Sprite2D tab
        gui::UpdateElement<sad::String>::with(node, "options", ui.rtwSpriteSprite, &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName);
        sad::Maybe<bool> maybeflipx = node->getProperty<bool>("flipx");
        if (maybeflipx.exists())
        {
            invoke_blocked(ui.cbFlipX, &QCheckBox::setCheckState, (maybeflipx.value()) ? Qt::Checked : Qt::Unchecked);
        }
        sad::Maybe<bool> maybeflipy = node->getProperty<bool>("flipy");
        if (maybeflipy.exists())
        {
            invoke_blocked(ui.cbFlipY, &QCheckBox::setCheckState, (maybeflipy.value()) ? Qt::Checked : Qt::Unchecked);
        }


        // Custom object tab
        gui::UpdateElement<sad::String>::with(node, "schema", ui.rtwCustomObjectSchemas, &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName);
		if (node->metaData()->canBeCastedTo("sad::db::custom::Object"))
		{
			this->fillCustomObjectProperties(node);
        }
        else
        {
            this->clearCustomObjectPropertiesTable();
        }
	}
}

void MainPanel::updateCustomObjectPropertyValueNow()
{
    gui::table::Delegate* d = this->delegateForCustomObjectProperty(m_custom_object_property_name.data());
    if (d)
    {
        d->set(m_custom_object_property_value);
    }
}

void MainPanel::updateUIForSelectedWay()
{
    QTimer::singleShot(0, this, SLOT(updateUIForSelectedWayNow()));
}

void MainPanel::updateUIForSelectedWayNow()
{
    if (m_editor->shared()->selectedWay())
    {
        ui.lstWayPoints->clear();
        sad::p2d::app::Way* p = m_editor->shared()->selectedWay();
        for(size_t i = 0; i < p->wayPoints().size(); ++i)
        {
            ui.lstWayPoints->addItem(this->nameForPoint(p->wayPoints()[i]));
        }
        invoke_blocked(ui.txtWayName, &QLineEdit::setText, p->objectName().c_str());
        invoke_blocked(ui.dsbWayTotalTime, &QDoubleSpinBox::setValue, p->totalTime());
        invoke_blocked(ui.cbWayClosed, &QCheckBox::setCheckState,  (p->closed()) ? Qt::Checked : Qt::Unchecked);
    }
}

//====================  PROTECTED METHODS HERE ====================

void MainPanel::fillDatabasePropertyTypesCombo()
{
    const unsigned int typescount = 20;
    const QString types[typescount] = {
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

//====================  PROTECTED SLOTS HERE ====================

void MainPanel::addDatabaseProperty()
{
	sad::db::Database* db = sad::Renderer::ref()->database("");
	sad::String propname = ui.txtDatabasePropertyName->text().toStdString();
	if (db->propertyByName(propname) == NULL && propname.size()!= 0)
	{
		gui::table::Delegate* d  = m_dbdelegate_factory.create(ui.cmbDatabasePropertyType->currentText());
		sad::db::Property* prop = m_property_factory.create(ui.cmbDatabasePropertyType->currentText().toStdString());
		if (d && prop)
		{
			sad::Renderer::ref()->database("")->addProperty(propname, prop);
			d->setPropertyName(propname.c_str());
			d->linkToDatabase();
			d->makeLinkedTo(ui.twDatabaseProperties, m_editor);
			d->add();
			history::database::NewProperty* p = new history::database::NewProperty(d);
			m_editor->history()->add(p);
		}
		else
		{
			delete d;
			delete prop;
		}
	}
}

void MainPanel::addScene()
{
	sad::Scene* s  = new sad::Scene();

	QString name = ui.txtSceneName->text();
	if (name.length())
	{
		s->setObjectName(name.toStdString());
	}

	sad::Renderer::ref()->add(s);
	sad::Renderer::ref()->database("")->table("scenes")->add(s);

	history::Command* c = new history::scenes::Add(s);
	c->commit(m_editor);
	m_editor->history()->add(c);

	invoke_blocked<QListWidget, void (QListWidget::*)(int), int>(ui.lstScenes, &QListWidget::setCurrentRow, ui.lstScenes->count() - 1);
}

void MainPanel::currentSceneChanged(int index)
{
	if (m_editor->machine()->isInState("adding")
		|| (m_editor->machine()->isInState("selected") 
		    && m_editor->machine()->currentState() != "selected"))
	{
		if (this->currentScene())
		{
			bool b = ui.lstScenes->blockSignals(true);
			int row = this->findSceneInList(this->currentScene());
			if (row != -1)
			{
				ui.lstScenes->setCurrentRow(row);
			}
			ui.lstScenes->blockSignals(b);
			return;
		}
	}
	if (index != -1) 
	{
		QListWidgetItem* i = ui.lstScenes->item(index);
		sad::Scene* s = i->data(Qt::UserRole).value<sad::Scene*>();
		bool b = ui.txtSceneName->blockSignals(true);
		ui.txtSceneName->setText(s->objectName().c_str());
		ui.txtSceneName->blockSignals(b);

		ui.lstSceneObjects->clear();
		const sad::Vector<sad::SceneNode*>& nodes = s->objects();
		for(size_t i = 0; i < nodes.size(); i++)
		{
			if (nodes[i]->active())
			{
				QListWidgetItem* ki = new QListWidgetItem();
				ki->setText(this->viewableObjectName(nodes[i]));
				
				QVariant v;
				v.setValue(nodes[i]);
				ki->setData(Qt::UserRole, v);
				ui.lstSceneObjects->addItem(ki);
			}
		}
	}
}

void MainPanel::sceneNameChanged(const QString&)
{
	sad::Scene* scene = currentScene();
	if (scene)
	{
		sad::String oldname = scene->objectName();
		sad::String newname = ui.txtSceneName->text().toStdString();
		
		history::Command* c = new history::scenes::ChangeName(scene, oldname, newname);
		this->m_editor->history()->add(c);
		c->commit(m_editor);
	}
}

void MainPanel::currentSceneNodeChanged(int index)
{
	if (m_editor->machine()->isInState("adding")
		|| (m_editor->machine()->isInState("selected") 
		    && m_editor->machine()->currentState() != "selected"))
	{
		if (this->editor()->shared()->selectedObject())
		{
			bool b = ui.lstSceneObjects->blockSignals(true);
			int row = this->findSceneNodeInList(this->editor()->shared()->selectedObject());
			if (row != -1)
			{
				ui.lstSceneObjects->setCurrentRow(row);
			}
			ui.lstSceneObjects->blockSignals(b);
			return;
		}
	}
	if (index != -1) 
	{
		QListWidgetItem* i = ui.lstSceneObjects->item(index);
		sad::SceneNode* s = i->data(Qt::UserRole).value<sad::SceneNode*>();

		if (m_editor->machine()->isInState("idle"))
		{
			m_editor->machine()->enterState("selected");
		}
		if (m_editor->machine()->isInState("selected"))
		{
			m_editor->shared()->setSelectedObject(s);
			this->updateUIForSelectedItem();
		}
	}
}

void MainPanel::removeScene()
{
	if (m_editor->isInEditingState())
	{
		return;
	}
	sad::Scene* scene = currentScene();
	if (scene)
	{
		if (m_editor->machine()->isInState("selected"))
		{
			m_editor->machine()->enterState("idle");
			m_editor->shared()->setSelectedObject(NULL);
		}
		int row = ui.lstScenes->currentRow();

		history::Command* c = new history::scenes::Remove(scene, row);
		this->m_editor->history()->add(c);
		c->commit(m_editor);
	}
}

void MainPanel::sceneMoveBack()
{
	sad::Scene* scene = currentScene();
	if (scene)
	{
		int row = ui.lstScenes->currentRow();
		if (row != 0)
		{
			sad::Scene* previousscene = ui.lstScenes->item(row -1)->data(Qt::UserRole).value<sad::Scene*>();

			history::Command* c = new history::scenes::LayerSwap(scene, previousscene, row, row - 1);
			this->m_editor->history()->add(c);
			c->commit(m_editor);
		}
	}
}

void MainPanel::sceneMoveFront()
{
	sad::Scene* scene = currentScene();
	if (scene)
	{
		int row = ui.lstScenes->currentRow();
		if (row < ui.lstScenes->count() - 1)
		{
			sad::Scene* nextscene = ui.lstScenes->item(row + 1)->data(Qt::UserRole).value<sad::Scene*>();

			history::Command* c = new history::scenes::LayerSwap(scene, nextscene, row, row + 1);
			this->m_editor->history()->add(c);
			c->commit(m_editor);
		}
	}
}

void MainPanel::clearScene()
{
	sad::Scene* scene = currentScene();
	if (scene)
	{
		history::Command* c = new history::scenes::Clear(scene);
		this->m_editor->history()->add(c);
		c->commit(m_editor);
	}
}

void MainPanel::updateMousePositionNow()
{
	ui.txtMousePosX->setText(QString::number(static_cast<int>(m_mousemove_point.x())));
	ui.txtMousePosY->setText(QString::number(static_cast<int>(m_mousemove_point.y())));
}

void MainPanel::highlightStateNow()
{
	ui.txtEditorState->setText(m_highlight_state);
}

void MainPanel::undo()
{
	m_editor->undo();
}

void MainPanel::redo()
{
	m_editor->redo();
}

void MainPanel::sceneNodeMoveBack()
{
	if (m_editor->machine()->isInState("selected"))
	{
		sad::SceneNode* node = m_editor->shared()->selectedObject();
		if (node)
		{
			int row = ui.lstSceneObjects->currentRow();
			int row2 = this->findSceneNodeInList(node);
			if (row > 0 && row == row2)
			{
				sad::SceneNode* previousnode = ui.lstSceneObjects->item(row - 1)->data(Qt::UserRole).value<sad::SceneNode*>();

				history::Command* c = new history::scenenodes::LayerSwap(node, previousnode, row, row - 1);
				this->m_editor->history()->add(c);
				c->commit(m_editor);

				invoke_blocked<QListWidget, void (QListWidget::*)(int), int>(ui.lstSceneObjects, &QListWidget::setCurrentRow, row - 1);
			}
		}
	}
}

void MainPanel::sceneNodeMoveFront()
{
	if (m_editor->machine()->isInState("selected"))
	{
		sad::SceneNode* node = m_editor->shared()->selectedObject();
		if (node)
		{
			int row = ui.lstSceneObjects->currentRow();
			int row2 = this->findSceneNodeInList(node);
			if (row < ui.lstSceneObjects->count() - 1 && row > -1 && row == row2)
			{
				sad::SceneNode* nextnode = ui.lstSceneObjects->item(row + 1)->data(Qt::UserRole).value<sad::SceneNode*>();

				history::Command* c = new history::scenenodes::LayerSwap(node, nextnode, row, row + 1);
				this->m_editor->history()->add(c);
				c->commit(m_editor);

				invoke_blocked<QListWidget, void (QListWidget::*)(int), int>(ui.lstSceneObjects, &QListWidget::setCurrentRow, row + 1);
			}
		}
	}
}

void MainPanel::synchronizeDatabase()
{
	ui.rtwSpriteSprite->setFilter("sad::Sprite2D::Options");
	ui.rtwLabelFont->setFilter("sad::freetype::Font|sad::TextureMappedFont");
}

void MainPanel::setAddingEnabled(bool enabled)
{
	this->ui.lstSceneObjects->setEnabled(enabled);
	this->ui.btnLabelAdd->setEnabled(enabled);
	this->ui.btnSpriteAdd->setEnabled(enabled);
}

void MainPanel::setSpriteChangingEnabled(bool enabled)
{
	this->ui.rwSceneNodeRect->setEnabled(enabled);
}

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

QString MainPanel::viewableObjectName(sad::db::Object* o)
{
	QString result = o->objectName().c_str();
	if (result.length() == 0)
	{
		char buffer[20];
		sprintf(buffer, "%p", o);
		result = QString(buffer);
	}
	return result;
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
			shouldshowresource = filterlist.indexOf(name.c_str()) != -1;
		}
		if (shouldshowresource)
		{
			QString name = cur.key().c_str();
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
		QString name = subfolders.key().c_str();
		if (prefix.length() != 0)
		{
			name = prefix + "/" + name;
		}
		QStringList tmp = resourcesByFilter(subfolders.value(), name, filter);
		result << tmp;
	}

	return result;
}

void MainPanel::save()
{
	if (m_editor->shared()->fileName().length() == 0)
	{
		this->saveAs();
	}
	else
	{
		sad::Renderer::ref()->database("")->saveToFile(m_editor->shared()->fileName().toStdString());
	}
}

void MainPanel::saveAs()
{
	QString name = QFileDialog::getSaveFileName(this, "Enter file, where we should store database", "", "*.json");
	if (name.length() != 0)
	{
		m_editor->shared()->setFileName(name);
		sad::Renderer::ref()->database("")->saveToFile(m_editor->shared()->fileName().toStdString());
	}
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
		if (tmp->loadFromFile(name.toStdString(), sad::Renderer::ref()))
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
    QMessageBox::StandardButton btn = QMessageBox::warning(NULL, "Database will be erased", "Database will be erased. Proceed loading?", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
    if (btn == QMessageBox::Ok)
    {
        QString name = QFileDialog::getOpenFileName(this, "Enter new resource file", "", "*.json");
        if (name.length())
        {
            sad::resource::Tree* tree = new sad::resource::Tree();
            tree->setRenderer(sad::Renderer::ref());
            tree->setStoreLinks(true);
            tree->factory()->registerResource<sad::freetype::Font>();
            
            sad::Vector<sad::resource::Error * > errors;
            errors = tree->loadFromFile(name.toStdString());
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

                if (ui.rtwLabelFont->filter().length() == 0)
                {
                    this->updateResourceViews();
                }
                else
                {
                    ui.rtwLabelFont->updateTree();
                    ui.rtwSpriteSprite->updateTree();
                    ui.rtwCustomObjectSchemas->updateTree();
                }
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

		sad::resource::PhysicalFile* file = list.selectedFile();
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

            if (ui.rtwLabelFont->filter().length() == 0)
            {
                this->updateResourceViews();
            }
            else
            {
                ui.rtwLabelFont->updateTree();
                ui.rtwSpriteSprite->updateTree();
                ui.rtwCustomObjectSchemas->updateTree();
            }
		}
		else
		{
			m_editor->reportResourceLoadingErrors(errors, file->name());
		}
	}
}


void MainPanel::tabTypeChanged(int index)
{
	// If tab is locked, switch to locked tab
	if (m_type_locked_tab.exists())
	{
		ui.tabTypes->setCurrentIndex(m_type_locked_tab.value());
		return;
	}

	int oldtypeindex = (m_editor->machine()->isInState("ways")) ? 1 : 0;
	if (oldtypeindex == index)
	{
		return;
	}
	if (m_editor->isInEditingState())
	{
		invoke_blocked(ui.tabTypes, &QTabWidget::setCurrentIndex, oldtypeindex);
	}
	if (index == 0)
	{
		m_editor->tryEnterObjectEditingState();
	}
	if (index == 1)
	{
		m_editor->tryEnterWayEditingState();
	}
	if (index == 2)
	{
		int row = ui.lstAnimations->currentRow(); 
		if (row >= 0)
		{
			sad::animations::Animation* w = ui.lstAnimations->item(row)->data(Qt::UserRole).value<sad::animations::Animation*>();
			m_editor->shared()->setSelectedAnimation(w);
		}
	}
}

