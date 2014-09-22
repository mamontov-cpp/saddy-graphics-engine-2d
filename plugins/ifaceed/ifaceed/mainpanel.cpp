#include "mainpanel.h"

#include "core/editor.h"
#include "core/fonttemplatesdatabase.h"
#include "core/fontdatabase.h"
#include "core/spritedatabase.h"
#include "core/editorbehaviour.h"
#include "core/shared.h"

#include "objects/screenlabel.h"
#include "objects/screensprite.h"
#include "objects/screentemplate.h"

#include "history/propertychangecommand.h"
#include "history/layercommands.h"
#include "history/movecommand.h"
#include "history/newcommand.h"

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

#include <geometry2d.h>
#include <keymouseconditions.h>
#include <keycodes.h>

#include <p2d/vector.h>
#include <p2d/point.h>

#include <db/save.h>
#include <db/load.h>
#include <db/dbdatabase.h>
#include <db/dbtable.h>
#include <db/dbstoredproperty.h>

#include <QDialog>
#include <QTimer>
#include <QFontDatabase>
#include <QItemDelegate>
#include <QPainter>
#include <QColorDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QTimer>
#include <QListWidget>
#include <QListWidgetItem>

#include <cstdio>

Q_DECLARE_METATYPE(sad::Scene*)
Q_DECLARE_METATYPE(sad::SceneNode*)


#define IGNORE_SELFCHANGING if (m_selfchanged) { m_selfchanged = false; return; }

#ifndef UNUSED
#ifdef GCC
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
#endif

//====================  PUBLIC METHODS HERE ====================

MainPanel::MainPanel(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), m_selfchanged(false)
{
	ui.setupUi(this);
	// Set default sprite adding model
	ui.rbPlaceAndRotate->setChecked(true);

    this->fillDatabasePropertyTypesCombo();

    ui.twDatabaseProperties->setColumnCount(3);
	double width = ui.twDatabaseProperties->width() - ui.twDatabaseProperties->verticalHeader()->width();
	ui.twDatabaseProperties->setColumnWidth(2, width / 6 - 12); // 12 is  a padding for header
	ui.twDatabaseProperties->setColumnWidth(1, width / 2); 
	ui.twDatabaseProperties->horizontalHeader()->hide();
	
    ui.txtLabelText->setPlainText("Test");

	m_scene_node_actions = new gui::SceneNodeActions();
	m_scene_node_actions->setPanel(this);

	m_label_actions = new gui::LabelActions();
	m_label_actions->setPanel(this);

	m_sprite2d_actions = new gui::Sprite2DActions();
	m_sprite2d_actions->setPanel(this);
	
	m_list.setWidget(ui.lstSceneObjects);
}


MainPanel::~MainPanel()
{
	delete m_label_actions;
	delete m_scene_node_actions;
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
	// TODO: Here, cleanup any kind of widget that is linked to resources
	if (!enabled)
	{
		
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
    sad::String s = "selected";

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

	// A bindings for adding node actions
    sad::Renderer::ref()->controls()->add(
        *sad::input::ET_MouseWheel & (m * s),
        m_scene_node_actions,
        &gui::SceneNodeActions::rotate
    );
	sad::Renderer::ref()->controls()->add(
		*sad::input::ET_KeyPress & sad::Esc & (m * s),
        m_scene_node_actions,
        &gui::SceneNodeActions::cancelSelection
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

	
	connect(ui.btnDatabasePropertiesAdd, SIGNAL(clicked()), this, SLOT(addDatabaseProperty()));
	
	connect(ui.btnSceneAdd, SIGNAL(clicked()), this, SLOT(addScene()));
	connect(ui.btnSceneDelete, SIGNAL(clicked()), this, SLOT(removeScene()));
	connect(ui.lstScenes, SIGNAL(currentRowChanged(int)), this, SLOT(currentSceneChanged(int)));
	connect(ui.txtSceneName, SIGNAL(textEdited(const QString&)), this, SLOT(sceneNameChanged(const QString&)));
	connect(ui.btnScenesMoveBack, SIGNAL(clicked()), this, SLOT(sceneMoveBack()));
	connect(ui.btnScenesMoveFront, SIGNAL(clicked()), this, SLOT(sceneMoveFront()));
	connect(ui.btnSceneClear, SIGNAL(clicked()), this, SLOT(clearScene()));
	
	connect(ui.btnRedo, SIGNAL(clicked()), this, SLOT(redo()));
	connect(ui.btnUndo, SIGNAL(clicked()), this, SLOT(undo()));

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
    
	connect(ui.btnReloadResources, SIGNAL(clicked()), this->m_editor, SLOT(reload()));
	connect(ui.btnDatabaseSave, SIGNAL(clicked()), this->m_editor, SLOT(save()));
	connect(ui.btnDatabaseLoad, SIGNAL(clicked()), this->m_editor, SLOT(load()));
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
		delete it.value();
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

	const sad::Vector<sad::Scene*>& scenes = sad::Renderer::ref()->scenes(); 
	for(unsigned int i = 0; i < scenes.size(); i++)
	{
		addSceneToSceneList(scenes[i]);
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

	if (s == n1 || s == n2)
	{
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

//====================  PUBLIC SLOTS METHODS HERE ====================

void MainPanel::updateUIForSelectedItem()
{
	QTimer::singleShot(0, this, SLOT(updateUIForSelectedItemNow()));
}

void MainPanel::updateUIForSelectedItemNow()
{
	
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
	if (db->table("scenes") == NULL)
	{
		db->addTable("scenes", new sad::db::Table());
	}
	if (db->table("scenenodes") == NULL)
	{
		db->addTable("scenenodes", new sad::db::Table());
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
}

void MainPanel::currentSceneChanged(int index)
{
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

void MainPanel::sceneNameChanged(const QString& name)
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
	if (index != -1) 
	{
		QListWidgetItem* i = ui.lstSceneObjects->item(index);
		sad::SceneNode* s = i->data(Qt::UserRole).value<sad::SceneNode*>();
		bool b = ui.txtObjectName->blockSignals(true);
		ui.txtObjectName->setText(s->objectName().c_str());
		ui.txtObjectName->blockSignals(b);

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
	sad::Scene* scene = currentScene();
	if (scene)
	{
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
	ui.txtMousePosX->setText(QString::number((int)(m_mousemove_point.x())));
	ui.txtMousePosY->setText(QString::number((int)(m_mousemove_point.y())));
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
			}
		}
	}
}


void MainPanel::selected(sad::String item)
{
	QMessageBox::warning(NULL, "1", item.c_str());
}

void MainPanel::synchronizeDatabase()
{
	ui.rtwSpriteSprite->setFilter("sad::Sprite2D::Options");
	ui.rtwLabelFont->setFilter("sad::freetype::Font|sad::TextureMappedFont");
	// TODO: Erase when not needed
	/*
	bool oldfontsstate = ui.cmbFonts->blockSignals(true);
	bool oldspritestate = m_spriteTableWidget->blockSignals(true);

	ui.cmbFonts->clear();
	FontTemplateDatabase * db = m_editor->database();
	core::EditorFontList & list =db->fonts();
	
	for(core::EditorFontListCursor it = list.begin(); !(it.end()); it++)
	{
		ui.cmbFonts->addItem(it.name(), QVariant(it.fonts()->qtFont()));
	}
	SpriteDatabase & sprites = db->sprites();
	AbstractSpriteDatabaseIterator * it = sprites.begin();
    for(; !(it->isEnd());it->next())
	{
		m_spriteTableWidget->add(*it);
	}
	m_spriteTableWidget->finishSyncronizing();
	delete it;

	ui.cmbFonts->blockSignals(oldfontsstate);
	m_spriteTableWidget->blockSignals(oldspritestate);
	*/
}

void MainPanel::addFontObject()
{
	if (ui.txtLabelText->toPlainText().length() == 0)
	{
		QMessageBox::critical(NULL, "IFace Editor", "You must specify label text to add it");
	}
	else
	{
		ScreenLabel * label = new ScreenLabel();
		label->setActive(true);
		label->setVisible(true);
		
		// TODO: Remake font
		// Set font
		// sad::String fontName=ui.cmbFonts->currentText().toStdString().c_str();
		// label->getProperty("font")->set(fontName);
		// TODO: Remake color
		// QColor qcolor = ui.cmbFontColor->itemData(ui.cmbFontColor->currentIndex()).value<QColor>();
		// sad::Color hcolor(qcolor.red(), qcolor.green(), qcolor.blue());

		//label->getProperty("pos")->set(sad::Point2D(0,0));
		//float angle = ui.dblAngle->value();
		//label->getProperty("angle")->set(angle);
		// TODO: Reimplement
		// unsigned int size = ui.cmbFontSize->itemData(ui.cmbFontSize->currentIndex()).value<int>();
		// label->getProperty("size")->set(size);
		sad::String text=ui.txtLabelText->toPlainText().toStdString().c_str();
		//label->getProperty("text")->set(text);


		label->tryReload(this->m_editor->database());
		sad::Scene* scene = this->m_editor->scene();
		label->setScene(scene);
		this->m_editor->shared()->setActiveObject(label);
		
		this->m_editor->currentBehaviour()->enterState("label_adding");
	}
}


void MainPanel::addSpriteObject()
{
	sad::String newstate = "";
	if (ui.rbPlaceAndRotate->isChecked()) newstate = "sprite_adding_simple";
	if (ui.rbTwoClicksPlaces->isChecked()) newstate = "sprite_adding_diagonal";
	if (newstate.length())
	{
		// TODO: Remake
		/*
		QSpriteTableWidgetSelection selection = m_spriteTableWidget->selection();
		if (selection.invalid() == false)
		{
			sad::String config = selection.config().toStdString().c_str();
			sad::String group = selection.group().toStdString().c_str();
			int index = selection.index();
			//  Adding a small screen sprite
			ScreenSprite * a = new ScreenSprite();
			sad::Sprite2DConfig * c = m_editor->database()->sprites().hconfigs()[config];
			sad::Sprite2DTemplate t = c->getTemplates()[group][index];
			sad::Point2D p1 = sad::Point2D(320,240);
			sad::Point2D p2 = p1 + t.size();
			sad::Rect2D rect(p1, p2);
			a->setProp<sad::Rect2D>("rect",rect, m_editor->log());
			a->setProp<float>("angle",ui.dblAngle->value(), m_editor->log());
			a->setProp<sad::String>("config",config, m_editor->log());
			a->setProp<sad::String>("group",group, m_editor->log());
			a->setProp<int>("index",index, m_editor->log());
			a->tryReload(m_editor->database());
			InterlockedScene * scene = static_cast<InterlockedScene*>(this->m_editor->scene());
			a->setScene(scene);

			this->m_editor->behaviourSharedData()->setActiveObject(a);

			this->m_editor->currentBehaviour()->enterState(newstate);
		}
		else
		{
			QMessageBox::critical(NULL, "IFace Editor", "You must select a sprite for adding");
		}
		*/
	}
	else
	{
		QMessageBox::critical(NULL, "IFace Editor", "You must select a mode for adding");
	}
}


void MainPanel::setAddingEnabled(bool enabled)
{
	this->ui.lstSceneObjects->setEnabled(enabled);
	this->ui.btnLabelAdd->setEnabled(enabled);
	this->ui.btnSpriteAdd->setEnabled(enabled);
}

void MainPanel::trySetProperty(const sad::String & prop, float v)
{
	/*
	core::Shared * data = this->m_editor->shared();
	AbstractScreenObject * o = NULL;
	sad::db::Property * _property = NULL;
	bool selected = false;
	if (data->activeObject()) 
	{
		o = data->activeObject();
	} 
	else 
	{
		o = data->selectedObject();	
		selected = true;
	}
	if (o) 
	{
		sad::Renderer::ref()->lockRendering();
		_property = o->SerializableObject::getProperty(prop);
        float  old = 0;
		if (_property) 
		{
			sad::log::Log * sl = sad::log::Log::ref();
			//old = _property->get<float>().value();	
			//_property->set(v);
		}
		if (selected) 
		{
			if (prop == "angle") 
			{
				QTimer * t =new QTimer();
				t->setSingleShot(true);
				bool pending = this->m_editor->shared()->isRotationCommandPending();
				sad::db::Variant new_v(v);
				float new_val_escaped = new_v.get<float>().value();
				float old_val_escaped = old;
				if (pending) {
					this->m_editor->shared()->submitRotationCommand(t, o, new_val_escaped, false);
				} else {
					this->m_editor->shared()->submitRotationCommand(t, o, new_val_escaped, true, old_val_escaped);
				}
				QObject::connect(t, SIGNAL(timeout()), this->m_editor, SLOT(appendRotationCommand()));
				t->start(MAX_ROTATION_TIME);
			}
			else
			{
				sad::log::Log * log = sad::log::Log::ref();
				this->m_editor->history()->add(new PropertyChangeCommand<float>(
					o, prop, v, 
					old,
					log
				));
			}
		}
		sad::Renderer::ref()->unlockRendering();
		this->updateList();
	}
	*/
}


template<typename T> void MainPanel::trySetProperty(const sad::String & prop, T v)
{
	core::Shared * data = this->m_editor->shared();
	AbstractScreenObject * o = NULL;
	sad::db::Property * _property = NULL;
	bool selected = false;
	if (data->activeObject()) 
	{
		//o = data->activeObject();
	} 
	else 
	{
		//o = data->selectedObject();	
		selected = true;
	}
	// Ignore color change for anyone but label
    if (!o) return;
	sad::String type_name = o->type();
    if (prop == "color" && type_name != "ScreenLabel")
		return;
	if (o) 
	{
		sad::Renderer::ref()->lockRendering();
		_property = o->SerializableObject::getProperty(prop);
		T  old;
		if (_property) 
		{
			sad::log::Log * sl = sad::log::Log::ref();;
			//old = _property->get<T>().value();	
			_property->set(NULL, v);
		}
		if (prop == "font")
		{
			o->tryReload(this->m_editor->database());
		}
		if (selected) 
		{
			if (prop == "angle") 
			{
				QTimer * t =new QTimer();
				t->setSingleShot(true);
				bool pending = this->m_editor->shared()->isRotationCommandPending();
				sad::db::Variant new_v(v);
				sad::db::Variant old_escaped(old);
				float new_val_escaped = new_v.get<float>().value();
				float old_val_escaped = old_escaped.get<float>().value();
				if (pending) {
					this->m_editor->shared()->submitRotationCommand(t, o, new_val_escaped, false);
				} else {
					this->m_editor->shared()->submitRotationCommand(t, o, new_val_escaped, true, old_val_escaped);
				}
				QObject::connect(t, SIGNAL(timeout()), this->m_editor, SLOT(appendRotationCommand()));
				t->start(MAX_ROTATION_TIME);
			}
			else
			{
				sad::log::Log * log = sad::log::Log::ref();
				this->m_editor->history()->add(new PropertyChangeCommand<T>(
					o, prop, v, 
					old,
					log
				));
			}
		}
		sad::Renderer::ref()->unlockRendering();
		this->updateList();
	}	
}


void MainPanel::angleChanged(double angle)
{
	//IGNORE_SELFCHANGING
	float fangle = angle;
	trySetProperty("angle", fangle);
}

void MainPanel::colorChanged(int index)
{
	IGNORE_SELFCHANGING
	if (index!=-1) 
	{
		// TODO: Remake this
		// QColor clr = ui.cmbFontColor->itemData(index).value<QColor>();
		// sad::Color c(clr.red(),clr.green(),clr.blue());
		// trySetProperty("color", c);
	}
}



void MainPanel::nameChanged(const QString & name)
{
	trySetProperty("name", sad::String(name.toStdString().c_str()));
	this->updateList();
}

void MainPanel::textChanged()
{
	IGNORE_SELFCHANGING
	sad::String s = ui.txtLabelText->toPlainText().toStdString().c_str();
	trySetProperty("text",s);
}

// Applying this macro, we can temporarily block 
// signals from Qt elements and restore them to
// original form
#define BLOCK_SIGNALS_AND_CALL(X, O)              \
		{                                         \
		  bool ___2___ = X -> blockSignals(true); \
		  X -> O ;                                \
		  X -> blockSignals(___2___);             \
		}
void MainPanel::updateObjectStats(AbstractScreenObject * o)
{
	sad::db::Property * prop = NULL;
	sad::log::Log * l = sad::log::Log::ref();		
	// Get text
	prop = o->SerializableObject::getProperty("text");
	if (prop)
	{
		m_selfchanged = true;
		/*
		BLOCK_SIGNALS_AND_CALL(
			ui.txtLabelText,
			setPlainText(prop->get<sad::String>().value().data())
		);
		*/
	}
	// Get size
	prop = o->SerializableObject::getProperty("size");
	if (prop)
	{
		m_selfchanged = true;
		/*
		TODO: Reimplement
		unsigned int size = prop->get<unsigned int>().value();
		int index = ui.cmbFontSize->findData((int)size);
		if (index != -1) 
		{
			BLOCK_SIGNALS_AND_CALL(
				ui.cmbFontSize,
				setCurrentIndex(index)
			);
		} 
		else 
		{
			ui.cmbFontSize->addItem(QString::number(size), (int)size);
			BLOCK_SIGNALS_AND_CALL(
				ui.cmbFontSize,
				setCurrentIndex(ui.cmbFontSize->count() - 1)
			);
		}
		*/
		m_selfchanged = false;
	}
	prop = o->SerializableObject::getProperty("color");
	if (prop && o->type() == "ScreenLabel")
	{
		m_selfchanged = true;
		//sad::Color c = prop->get<sad::Color>().value();
		//QColor clr(c.r(), c.g(), c.b()); 
		// TODO: Remake
		/*
		int index = ui.cmbFontColor->findData(clr);
		if (index != -1) 
		{
			BLOCK_SIGNALS_AND_CALL(
				ui.cmbFontColor,
				setCurrentIndex(index)
			);
		}
		else 
		{
			m_selfchanged = false;
		}
		*/
		m_selfchanged = false;
	}
	prop = o->SerializableObject::getProperty("font");
	if (prop)
	{
		m_selfchanged = true;
		//sad::String c = prop->get<sad::String>().value();
		//QString s = c.data();
		// TODO: Remake data
		/*
		int index = ui.cmbFonts->findText(s);
		if (index != -1) 
		{
			BLOCK_SIGNALS_AND_CALL(
				ui.cmbFonts,
				setCurrentIndex(index)
			);
		} 
		else 
		{
			m_selfchanged = false;
		}
		*/
	}
	/*
	prop = o->getProperty("angle");
	if (prop)
	{
		m_selfchanged = true;
		float c = prop->get<float>().value();
		BLOCK_SIGNALS_AND_CALL(ui.dblAngle, setValue(c));
		m_selfchanged = false;
	}
	*/
	prop = o->SerializableObject::getProperty("name");
	if (prop)
	{	
		m_selfchanged = true;
		//sad::String c= prop->get<sad::String>().value();
		// BLOCK_SIGNALS_AND_CALL(ui.txtObjectName, setText(c.data()));
		m_selfchanged = false;
	}
	if (o->SerializableObject::getProperty("config") != NULL)
	{
		this->setRegionParameters();
		
		sad::log::Log * log = sad::log::Log::ref();
		sad::String config = o->prop<sad::String>("config", log);
		sad::String group = o->prop<sad::String>("group", log);
		int index = o->prop<int>("index", log);
		
		//QSpriteTableWidgetSelection sel(config.data(), group.data(), index);
		//BLOCK_SIGNALS_AND_CALL(m_spriteTableWidget,setSelection(sel));
	}
	// This added to prevent cases when selfchanging does not work and flag is not resetted.
	m_selfchanged = false;
}
#undef BLOCK_SIGNALS_AND_CALL

void MainPanel::updateList()
{
	//m_list.updateWidget(m_editor->result(), m_editor->shared()->selectedObject());
}


void MainPanel::selectedObjectChanged(int index)
{
	/*
	if (index != -1 && m_list.selfChanged() == false)
	{
		AbstractScreenObject * o = m_list.row(index);
		this->m_editor->shared()->setSelectedObject(o);
		m_editor->showObjectStats(o);
		m_editor->currentBehaviour()->enterState("selected");
	}
	*/
}

void MainPanel::moveObjectBack()
{
	/*
	AbstractScreenObject * o = m_editor->shared()->selectedObject();
	if (o && m_editor->currentBehaviour()->state() == "selected") 
	{
		unsigned int  my  = o->scene()->findLayer(o);
		unsigned int  min = m_editor->result()->minLayer();
		if (my != min) 
		{
			LayerCommand * c = new LayerCommand(o, my, my - 1);
			c->commit(m_editor);
			m_editor->history()->add(c);
		}
	}
	*/
}

void MainPanel::moveObjectFront()
{
	/*
	AbstractScreenObject * o = m_editor->shared()->selectedObject();
	if (o && m_editor->currentBehaviour()->state() == "selected") 
	{
		unsigned int  my  = o->scene()->findLayer(o);
		unsigned int  max = m_editor->result()->maxLayer();
		if (my != max) 
		{
			LayerCommand * c = new LayerCommand(o, my, my + 1);
			c->commit(m_editor);
			m_editor->history()->add(c);
		}
	}
	*/
}

void MainPanel::setAngleChangingEnabled(bool enabled)
{
	ui.awSceneNodeAngle->setEnabled(enabled);
	if (enabled == false)
	{
		ui.awSceneNodeAngle->setValue(0);
	}
}

void MainPanel::setSpriteChangingEnabled(bool enabled)
{
	this->ui.rwSceneNodeRect->setEnabled(enabled);
}


void MainPanel::setRegionParameters()
{
	/*
	AbstractScreenObject * o1 = m_editor->shared()->activeObject();
	AbstractScreenObject * o2 = m_editor->shared()->selectedObject();
	AbstractScreenObject * o = (o1) ? o1 : o2;
	// Get rect	
	if (o)
	{
		sad::Rect2D rect = o->region();
		if (o->typeName() == "ScreenSprite")
		{
			m_selfchanged = true;
			ui.dblSpriteX->setValue(rect[0].x());
			ui.dblSpriteY->setValue(rect[0].y());
			ui.dblSpriteWidth->setValue(rect[0].distance(rect[1]));
			ui.dblSpriteHeight->setValue(rect[0].distance(rect[3]));
			m_selfchanged = false;
		}
	}
	*/
}


void MainPanel::spriteSelected(QString config, QString group, int index)
{
	if (m_selfchanged)
		return;
	/*
	AbstractScreenObject * o1 = m_editor->shared()->activeObject();
	AbstractScreenObject * o2 = m_editor->shared()->selectedObject();
	AbstractScreenObject * o = (o1) ? o1 : o2;
	// TODO: Reimplement	
	if (o)
	{
		
		QSpriteTableWidgetSelection sel(config, group, index);
		if (sel.invalid() == false  && o->getProperty("config") != NULL) 
		{
			sad::String oconf =  o->prop<sad::String>("config", m_editor->log());
			sad::String ogroup =  o->prop<sad::String>("group", m_editor->log());
			int oindex =  o->prop<int>("index", m_editor->log());

			o->setProp<sad::String>("config", config.toStdString().c_str(), m_editor->log());
			o->setProp<sad::String>("group",  group.toStdString().c_str(), m_editor->log());
			o->setProp<int>("index",    index, m_editor->log());
			
			bool set_rect = false;
			sad::Rect2D rect = o->region();
			float angle = o->prop<float>("angle", m_editor->log());
			set_rect = this->m_editor->currentBehaviour()->state() != "sprite_adding_simple";
			
			o->tryReload(this->m_editor->database());
			
			if (set_rect)
			{
				static_cast<ScreenSprite *>(o)->setRotatedRectangle(rect,angle);
			}

			if (o2 == o)
			{

				SpritePropertyChangeCommandInfo * _old = new SpritePropertyChangeCommandInfo();
	
				_old->config = oconf;
				_old->group = ogroup;
				_old->index = oindex;
				_old->rect = rect;
				_old->angle = angle;

				SpritePropertyChangeCommandInfo * _new = new SpritePropertyChangeCommandInfo();

				_new->config = config.toStdString().c_str();
				_new->group = group.toStdString().c_str();
				_new->index = index;
				_new->rect = rect;
				_new->angle = angle;

				SpritePropertyChangeCommand * c = new SpritePropertyChangeCommand(
					static_cast<ScreenSprite *>(o),
					m_editor->database(), 
					m_editor->log(), 
					*_old, 
					*_new
				);

				m_editor->history()->add(c);

				delete _old;
				delete _new;

			}
		}
	}
	*/
}

void SpritePropertyChangeCommand::commit(core::Editor * ob )
{
	SL_SCOPE("SpritePropertyChangeCommand::commit");
	m_sprite->setProp<sad::String>("config", m_new.config, m_log);
	m_sprite->setProp<sad::String>("group",  m_new.group, m_log);
	m_sprite->setProp<int>("index",    m_new.index, m_log);
	m_sprite->tryReload(m_db);
	m_sprite->setRotatedRectangle(m_new.rect, m_new.angle);
	ob->submitEvent("SpritePropertyChangeCommand::commit", sad::db::Variant(0));
}

void SpritePropertyChangeCommand::rollback(core::Editor * ob)
{
	SL_SCOPE("SpritePropertyChangeCommand::rollback");
	m_sprite->setProp<sad::String>("config", m_old.config, m_log);
	m_sprite->setProp<sad::String>("group",  m_old.group, m_log);
	m_sprite->setProp<int>("index",    m_old.index, m_log);
	m_sprite->tryReload(m_db);
	m_sprite->setRotatedRectangle(m_old.rect, m_old.angle);
	ob->submitEvent("SpritePropertyChangeCommand::rollback", sad::db::Variant(0));
}

void MainPanel::spriteRectChanged()
{
	if (m_selfchanged)
		return;
	/*
	AbstractScreenObject * o1 = m_editor->shared()->activeObject();
	AbstractScreenObject * o2 = m_editor->shared()->selectedObject();
	AbstractScreenObject * o = (o1) ? o1 : o2;
	// TODO: implement	
	if (o)
	{
		if (o->getProperty("rect") != NULL && o->typeName() == "ScreenSprite")
		{
			float angle = o->prop<float>("angle", this->m_editor->log());
			sad::Rect2D oldrect = o->region();
			sad::Point2D newpoint(ui.dblSpriteX->value(), ui.dblSpriteY->value());
			sad::Point2D size(ui.dblSpriteWidth->value(), ui.dblSpriteHeight->value());
			float comparisonprec = 0.0001f;
			float oldwidth = oldrect[1].distance(oldrect[0]);
			float oldheight =oldrect[3].distance(oldrect[0]);
			if (fabs(newpoint.x() - oldrect[0].x()) < comparisonprec
				&& fabs(newpoint.y() - oldrect[0].y()) < comparisonprec
				&& fabs(size.x() - oldwidth) < comparisonprec
				&& fabs(size.y() - oldheight) < comparisonprec
				)
			{
				return;
			}
			sad::Point2D horizontal = sad::p2d::unit(oldrect[1] - oldrect[0]);
			sad::Point2D vertical = sad::p2d::unit(oldrect[3] - oldrect[0]);
			sad::Rect2D newrect;
			ScreenSprite * oo = static_cast<ScreenSprite *>(o);
			newrect[0] = newpoint;
			newrect[1] = newpoint;
			newrect[1] +=  horizontal * size.x(); 
			newrect[2] = newpoint;
			newrect[2] +=  (horizontal * size.x())  + (vertical * size.y());
			newrect[3] = newpoint;
			newrect[3] +=  (vertical * size.y());
			oo->setRotatedRectangle(newrect, angle);
			if (o == o2)
			{
				m_editor->history()->add(new  SpriteRectChangeCommand(oo, angle, oldrect, newrect));
			}
			
		}
	}
	*/
}

void SpriteRectChangeCommand::commit(core::Editor * ob)
{
	m_sprite->setRotatedRectangle(m_old_rect, m_angle);
	ob->submitEvent("SpriteRectChangeCommand::commit", sad::db::Variant(0));
}


void SpriteRectChangeCommand::rollback(core::Editor * ob)
{
	m_sprite->setRotatedRectangle(m_new_rect, m_angle);
	ob->submitEvent("SpriteRectChangeCommand::rollback", sad::db::Variant(0));
}


void MainPanel::makeBackground()
{
	SL_SCOPE("MainPanel::makeBackground()");
	AbstractScreenObject * o2 = NULL;
	if (o2)
	{
		if (o2->hasProperty("rect") && o2->rotatable())
		{
			history::Command * d = new MakeBackgroundCommand(o2);
			m_editor->history()->add(d);
			d->commit(m_editor);
			SL_DEBUG("MakeBackgroundCommand() comitted");
		}
		else
		{
			SL_DEBUG("Selected object has no \"rect\" or rotatable");
		}
	}
	else 
	{
		SL_DEBUG("No object selected");
	}
}


void MainPanel::clearScreenTemplate()
{
	if (m_editor->currentBehaviour()->state() == "idle"
		|| m_editor->currentBehaviour()->state() == "selected"
		)
	{
	if (m_editor->result()->count() )
	{
		if (m_editor->currentBehaviour()->state() == "selected")
		{
			m_editor->shared()->setSelectedObject(NULL);
			m_editor->currentBehaviour()->enterState("idle");
		}
		ScreenClearCommand * c =new ScreenClearCommand(m_editor->result());
		m_editor->history()->add(c);
		c->commit(m_editor);
		m_editor->currentBehaviour()->enterState("idle");
	}
	}
}

void MainPanel::colorChanged(QColor c)
{
	QString a = QString("%1%2%3%4\n").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
	qDebug(a.toStdString().c_str());
}
