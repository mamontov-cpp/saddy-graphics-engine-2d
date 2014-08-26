#include "mainpanel.h"

#include "core/ifaceeditor.h"
#include "core/fonttemplatesdatabase.h"
#include "core/fontdatabase.h"
#include "core/spritedatabase.h"

#include "editorcore/editorbehaviour.h"
#include "editorcore/editorbehaviourshareddata.h"

#include "objects/screenlabel.h"
#include "objects/screensprite.h"
#include "objects/screentemplate.h"

#include "history/propertychangecommand.h"
#include "history/layercommands.h"
#include "history/movecommand.h"
#include "history/newcommand.h"

#include <geometry2d.h>

#include <p2d/vector.h>
#include <p2d/point.h>

#include <db/save.h>
#include <db/load.h>

#include <QDialog>
#include <QTimer>
#include <QFontDatabase>
#include <QItemDelegate>
#include <QPainter>
#include <QColorDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QTimer>

#define IGNORE_SELFCHANGING if (m_selfchanged) { m_selfchanged = false; return; }

#ifndef UNUSED
#ifdef GCC
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
#endif

MainPanel::MainPanel(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	// Init non-random palette
	QList< QList<QColor> > defaultpalette;

	defaultpalette << QList<QColor>();
	defaultpalette[0] << QColor(255, 255, 0);
	defaultpalette[0] << QColor(255, 0, 0);
	defaultpalette[0] << QColor(0, 255, 0);
	defaultpalette[0] << QColor(0, 0, 255);

	defaultpalette << QList<QColor>();
	defaultpalette[1] << QColor(192, 192, 0);
	defaultpalette[1] << QColor(192, 0, 0);
	defaultpalette[1] << QColor(0, 192, 0);
	defaultpalette[1] << QColor(0, 0, 192);

	defaultpalette << QList<QColor>();
	defaultpalette[2] << QColor(128, 128, 0);
	defaultpalette[2] << QColor(128, 0, 0);
	defaultpalette[2] << QColor(0, 128, 0);
	defaultpalette[2] << QColor(0, 0, 128);

	defaultpalette << QList<QColor>();
	defaultpalette[3] << QColor(64, 64, 0);
	defaultpalette[3] << QColor(64, 0, 0);
	defaultpalette[3] << QColor(0, 64, 0);
	defaultpalette[3] << QColor(0, 0, 64);

	ui.clpSceneNodeColor->setPalette(defaultpalette);

	m_selfchanged = false;
	connect(ui.btnLabelAdd, SIGNAL(clicked()), this, SLOT(addFontObject()));
	connect(ui.btnSpriteAdd, SIGNAL(clicked()), this, SLOT(addSpriteObject()));
	
	
	// Set default sprite adding model
	ui.rbPlaceAndRotate->setChecked(true);

	

	m_list.setWidget(ui.lstSceneObjects);

	connect(ui.txtLabelText, SIGNAL(textChanged()), this, SLOT(textChanged()));
	connect(ui.lstSceneObjects, SIGNAL(currentRowChanged(int)), this, SLOT(selectedObjectChanged(int)));
	connect(ui.btnSceneNodeMoveBack, SIGNAL(clicked()), this, SLOT(moveObjectBack()));
	connect(ui.btnSceneNodeMoveFront, SIGNAL(clicked()), this, SLOT(moveObjectFront()));
	connect(ui.txtObjectName, SIGNAL(textEdited(const QString&)), this, SLOT(nameChanged(const QString&)));
	connect(ui.btnSpriteMakeBackground, SIGNAL(clicked()), this, SLOT(makeBackground()));
	connect(ui.btnSceneClear, SIGNAL(clicked()), this, SLOT(clearScreenTemplate()));
	connect(ui.btnRedo, SIGNAL(clicked()), this, SLOT(repeatHistoryChange()));
	connect(ui.btnUndo, SIGNAL(clicked()), this, SLOT(rollbackHistoryChange()));
	connect(ui.clpSceneNodeColor, SIGNAL(selectedColorChanged(QColor)), this, SLOT(colorChanged(QColor)));
}

void MainPanel::setEditor(IFaceEditor * editor) 
{  
	m_editor = editor; 
	connect(ui.btnSceneNodeDelete, SIGNAL(clicked()), m_editor, SLOT(tryEraseObject()));
	connect(ui.btnReloadResources, SIGNAL(clicked()), this->m_editor, SLOT(reload()));
	connect(ui.btnDatabaseSave, SIGNAL(clicked()), this->m_editor, SLOT(save()));
	connect(ui.btnDatabaseLoad, SIGNAL(clicked()), this->m_editor, SLOT(load()));

	connect(ui.rtwSpriteSprite, SIGNAL(selectionChanged(sad::String)), this, SLOT(selected(sad::String)));
}



MainPanel::~MainPanel()
{

}



void MainPanel::closeEvent(QCloseEvent* ev)
{
 this->QMainWindow::closeEvent(ev);
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
	IFaceEditorFontList & list =db->fonts();
	
	for(IFaceEditorFontListCursor it = list.begin(); !(it.end()); it++)
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





void MainPanel::setMouseMovePosView(float x, float y)
{
	m_tmp_mov_x = x;
	m_tmp_mov_y = y;

	QTimer::singleShot(0, this, SLOT(setMouseMovePosViewImpl()));
}

void MainPanel::setMouseMovePosViewImpl()
{
	ui.txtMousePosX->setText(QString::number(m_tmp_mov_x));
	ui.txtMousePosY->setText(QString::number(m_tmp_mov_y));
}

void MainPanel::highlightStateImpl()
{
	ui.txtEditorState->setText(m_tmp_state);
}

void MainPanel::highlightState(const sad::String & hints)
{
	m_tmp_state = hints.data();
	QTimer::singleShot(0, this, SLOT(highlightStateImpl()));
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
		InterlockedScene * scene = static_cast<InterlockedScene*>(this->m_editor->scene());
		label->setScene(static_cast<InterlockedScene*>(this->m_editor->scene()));
		this->m_editor->behaviourSharedData()->setActiveObject(label);
		
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
	EditorBehaviourSharedData * data = this->m_editor->behaviourSharedData();
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
		this->m_editor->lockRendering();
		_property = o->SerializableObject::getProperty(prop);
        float  old = 0;
		if (_property) 
		{
			sad::log::Log * sl = this->m_editor->log();
			//old = _property->get<float>().value();	
			//_property->set(v);
		}
		if (selected) 
		{
			if (prop == "angle") 
			{
				QTimer * t =new QTimer();
				t->setSingleShot(true);
				bool pending = this->m_editor->shdata()->isRotationCommandPending();
				sad::db::Variant new_v(v);
				float new_val_escaped = new_v.get<float>().value();
				float old_val_escaped = old;
				if (pending) {
					this->m_editor->shdata()->submitRotationCommand(t, o, new_val_escaped, false);
				} else {
					this->m_editor->shdata()->submitRotationCommand(t, o, new_val_escaped, true, old_val_escaped);
				}
				QObject::connect(t, SIGNAL(timeout()), this->m_editor, SLOT(appendRotationCommand()));
				t->start(MAX_ROTATION_TIME);
			}
			else
			{
				this->m_editor->history()->add(new PropertyChangeCommand<float>(
					o, prop, v, 
					old,
					this->m_editor->log()
				));
			}
		}
		this->m_editor->unlockRendering();
		this->updateList();
	}	
}


template<typename T> void MainPanel::trySetProperty(const sad::String & prop, T v)
{
	EditorBehaviourSharedData * data = this->m_editor->behaviourSharedData();
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
	// Ignore color change for anyone but label
    if (!o) return;
	sad::String type_name = o->type();
    if (prop == "color" && type_name != "ScreenLabel")
		return;
	if (o) 
	{
		this->m_editor->lockRendering();
		_property = o->SerializableObject::getProperty(prop);
		T  old;
		if (_property) 
		{
			sad::log::Log * sl = this->m_editor->log();
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
				bool pending = this->m_editor->shdata()->isRotationCommandPending();
				sad::db::Variant new_v(v);
				sad::db::Variant old_escaped(old);
				float new_val_escaped = new_v.get<float>().value();
				float old_val_escaped = old_escaped.get<float>().value();
				if (pending) {
					this->m_editor->shdata()->submitRotationCommand(t, o, new_val_escaped, false);
				} else {
					this->m_editor->shdata()->submitRotationCommand(t, o, new_val_escaped, true, old_val_escaped);
				}
				QObject::connect(t, SIGNAL(timeout()), this->m_editor, SLOT(appendRotationCommand()));
				t->start(MAX_ROTATION_TIME);
			}
			else
			{
				this->m_editor->history()->add(new PropertyChangeCommand<T>(
					o, prop, v, 
					old,
					this->m_editor->log()
				));
			}
		}
		this->m_editor->unlockRendering();
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
	sad::log::Log * l = this->m_editor->log();		
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
		
		sad::String config = o->prop<sad::String>("config", m_editor->log());
		sad::String group = o->prop<sad::String>("group", m_editor->log());
		int index = o->prop<int>("index", m_editor->log());
		
		//QSpriteTableWidgetSelection sel(config.data(), group.data(), index);
		//BLOCK_SIGNALS_AND_CALL(m_spriteTableWidget,setSelection(sel));
	}
	// This added to prevent cases when selfchanging does not work and flag is not resetted.
	m_selfchanged = false;
}
#undef BLOCK_SIGNALS_AND_CALL

void MainPanel::updateList()
{
	m_list.updateWidget(m_editor->result(), m_editor->behaviourSharedData()->selectedObject());
}


void MainPanel::selectedObjectChanged(int index)
{
	if (index != -1 && m_list.selfChanged() == false)
	{
		AbstractScreenObject * o = m_list.row(index);
		this->m_editor->behaviourSharedData()->setSelectedObject(o);
		m_editor->showObjectStats(o);
		m_editor->currentBehaviour()->enterState("selected");
	}
}

void MainPanel::moveObjectBack()
{
	AbstractScreenObject * o = m_editor->behaviourSharedData()->selectedObject();
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
}

void MainPanel::moveObjectFront()
{
	AbstractScreenObject * o = m_editor->behaviourSharedData()->selectedObject();
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
	AbstractScreenObject * o1 = m_editor->behaviourSharedData()->activeObject();
	AbstractScreenObject * o2 = m_editor->behaviourSharedData()->selectedObject();
	AbstractScreenObject * o = (o1) ? o1 : o2;
	// Get rect
	/*
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
	AbstractScreenObject * o1 = m_editor->behaviourSharedData()->activeObject();
	AbstractScreenObject * o2 = m_editor->behaviourSharedData()->selectedObject();
	AbstractScreenObject * o = (o1) ? o1 : o2;
	// TODO: Reimplement
	/*
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

void SpritePropertyChangeCommand::commit(core::CommandChangeObserver * ob )
{
	SL_SCOPE("SpritePropertyChangeCommand::commit");
	m_sprite->setProp<sad::String>("config", m_new.config, m_log);
	m_sprite->setProp<sad::String>("group",  m_new.group, m_log);
	m_sprite->setProp<int>("index",    m_new.index, m_log);
	m_sprite->tryReload(m_db);
	m_sprite->setRotatedRectangle(m_new.rect, m_new.angle);
	ob->submitEvent("SpritePropertyChangeCommand::commit", sad::db::Variant(0));
}

void SpritePropertyChangeCommand::rollback(core::CommandChangeObserver * ob)
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
	AbstractScreenObject * o1 = m_editor->behaviourSharedData()->activeObject();
	AbstractScreenObject * o2 = m_editor->behaviourSharedData()->selectedObject();
	AbstractScreenObject * o = (o1) ? o1 : o2;
	// TODO: implement
	
	/*
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

void SpriteRectChangeCommand::commit(core::CommandChangeObserver * ob)
{
	m_sprite->setRotatedRectangle(m_old_rect, m_angle);
	ob->submitEvent("SpriteRectChangeCommand::commit", sad::db::Variant(0));
}


void SpriteRectChangeCommand::rollback(core::CommandChangeObserver * ob)
{
	m_sprite->setRotatedRectangle(m_new_rect, m_angle);
	ob->submitEvent("SpriteRectChangeCommand::rollback", sad::db::Variant(0));
}


void MainPanel::makeBackground()
{
	SL_SCOPE("MainPanel::makeBackground()");
	AbstractScreenObject * o2 = m_editor->behaviourSharedData()->selectedObject();
	if (o2)
	{
		if (o2->hasProperty("rect") && o2->rotatable())
		{
			AbstractCommand * d = new MakeBackgroundCommand(o2);
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
			m_editor->shdata()->setSelectedObject(NULL);
			m_editor->currentBehaviour()->enterState("idle");
		}
		ScreenClearCommand * c =new ScreenClearCommand(m_editor->result());
		m_editor->history()->add(c);
		c->commit(m_editor);
		m_editor->currentBehaviour()->enterState("idle");
	}
	}
}


void MainPanel::repeatHistoryChange()
{
	m_editor->history()->commit(m_editor);
}

void MainPanel::rollbackHistoryChange()
{
	m_editor->history()->rollback(m_editor);
}

void MainPanel::colorChanged(QColor c)
{
	QString a = QString("%1%2%3%4\n").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
	qDebug(a.toStdString().c_str());
}
