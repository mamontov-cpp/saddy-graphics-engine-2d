#include "mainpanel.h"
#include <QDialog>
#include <QTimer>
#include <QFontDatabase>
#include <QItemDelegate>
#include <QPainter>
#include <QColorDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QTimer>
#include <marshal/actioncontext.h>
#include <extra/geometry2d.h>
#include "gui/fontdelegate.h"
#include "gui/colordelegate.h"
#include "core/ifaceeditor.h"
#include "core/fonttemplatesdatabase.h"
#include "core/fontdatabase.h"
#include "core/spritedatabase.h"
#include "core/mockspritetablewidget.h"
#include "editorcore/editorbehaviour.h"
#include "editorcore/editorbehaviourshareddata.h"
#include "objects/screenlabel.h"
#include "objects/screensprite.h"
#include "objects/screentemplate.h"
#include "history/propertychangecommand.h"
#include "history/layercommands.h"
#include "history/movecommand.h"
#include "history/newcommand.h"

#define IGNORE_SELFCHANGING if (m_selfchanged) { m_selfchanged = false; return; }

MainPanel::MainPanel(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	m_selfchanged = false;
	connect(ui.btnPickFontColor,SIGNAL(clicked()),this,SLOT(addNewFontColor()));
	connect(ui.btnPickFontSize,SIGNAL(clicked()), this, SLOT(addNewFontSize()));
	connect(ui.btnAddLabel, SIGNAL(clicked()), this, SLOT(addFontObject()));
	connect(ui.btnAddSprite, SIGNAL(clicked()), this, SLOT(addSpriteObject()));
	
	ui.cmbFonts->setItemDelegate(new FontDelegate());
	ui.cmbFontColor->setItemDelegate(new ColorDelegate());

	QColor colors[] = { Qt::red, 
						Qt::darkRed,
						Qt::blue,
						Qt::darkBlue,
						Qt::green,
						Qt::darkGreen,
						Qt::white, 
						Qt::magenta, 
						Qt::yellow, 
						Qt::black 
	                  };
	for (int i=0;i<10;i++)
	{
		QString text = QString::number(colors[i].red()) + QString(",")
			         + QString::number(colors[i].green()) + QString(",")
					 + QString::number(colors[i].blue());
		text = QString("(") + text + QString(")");
		ui.cmbFontColor->addItem(text, QVariant(colors[i]));
	}
	// Populate font size
	for (int i=5;i<201;i++)
	{
		ui.cmbFontSize->addItem(QString::number(i),QVariant(i));
	}
	// Set default sprite adding model
	ui.rbPlaceAndRotate->setChecked(true);

	// Add SpriteViewer
	QGridLayout* grPadLayout = new QGridLayout;
	QPoint pointPad = ui.spriteViewerPad->pos();
	QPoint pointGroupPad = pointPad + ui.grpSprites->pos();
	
	QRectF contentRect = QRectF(pointGroupPad,ui.spriteViewerPad->size());
	m_spriteTableWidget = new QSpriteTableWidget(ui.cmbSpriteConfig,
													grPadLayout);

	m_list.setWidget(ui.lstObjects);


	ui.spriteViewerPad->setLayout(grPadLayout);
	connect(ui.cmbFontColor, SIGNAL(currentIndexChanged(int)), this, SLOT(colorChanged(int)));
	connect(ui.cmbFonts, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(fontChanged(const QString&)));
	connect(ui.cmbFontSize, SIGNAL(currentIndexChanged(int)), this, SLOT(sizeChanged(int))); 
	connect(ui.dblAngle, SIGNAL(valueChanged(double)), this, SLOT(angleChanged(double)));
	connect(ui.txtLabelText, SIGNAL(textChanged()), this, SLOT(textChanged()));
	connect(m_spriteTableWidget, SIGNAL(spriteSelected(QString,QString,int)), this, SLOT(spriteSelected(QString,QString,int)));	
	connect(ui.lstObjects, SIGNAL(currentRowChanged(int)), this, SLOT(selectedObjectChanged(int)));
	connect(ui.btnMoveBack, SIGNAL(clicked()), this, SLOT(moveObjectBack()));
	connect(ui.btnMoveFront, SIGNAL(clicked()), this, SLOT(moveObjectFront()));
	connect(ui.txtName, SIGNAL(textEdited(const QString&)), this, SLOT(nameChanged(const QString&)));
	connect(ui.dblSpriteX, SIGNAL(editingFinished()), this, SLOT(spriteRectChanged()));
	connect(ui.dblSpriteY, SIGNAL(editingFinished()), this, SLOT(spriteRectChanged()));
	connect(ui.dblSpriteWidth, SIGNAL(editingFinished()), this, SLOT(spriteRectChanged()));
	connect(ui.dblSpriteHeight, SIGNAL(editingFinished()), this, SLOT(spriteRectChanged()));
	connect(ui.btnMakeBackground, SIGNAL(clicked()), this, SLOT(makeBackground()));
	connect(ui.btnClear, SIGNAL(clicked()), this, SLOT(clearScreenTemplate()));
	connect(ui.btnRedo, SIGNAL(clicked()), this, SLOT(repeatHistoryChange()));
	connect(ui.btnUndo, SIGNAL(clicked()), this, SLOT(rollbackHistoryChange()));
}

void MainPanel::setEditor(IFaceEditor * editor) 
{  
	m_editor = editor; 
	connect(ui.btnDelete, SIGNAL(clicked()), m_editor, SLOT(tryEraseObject()));
	connect(ui.btnReloadDB, SIGNAL(clicked()), this->m_editor, SLOT(reload()));
	connect(ui.btnSave, SIGNAL(clicked()), this->m_editor, SLOT(save()));
}



MainPanel::~MainPanel()
{
	delete m_spriteTableWidget;
}



void MainPanel::closeEvent(QCloseEvent* ev)
{
 this->QMainWindow::closeEvent(ev);
}

void MainPanel::synchronizeDatabase()
{
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
	for(it; !(it->isEnd());it->next())
	{
		m_spriteTableWidget->add(*it);
	}
	m_spriteTableWidget->finishSyncronizing();
	delete it;

	ui.cmbFonts->blockSignals(oldfontsstate);
	m_spriteTableWidget->blockSignals(oldspritestate);
}

void MainPanel::addNewFontColor()
{
	QColor initial(0,0,0);
	if (ui.cmbFontColor->currentIndex()!=-1)
	{
		initial = ui.cmbFontColor->itemData(ui.cmbFontColor->currentIndex()).value<QColor>();
	}

	QColor to_add = QColorDialog::getColor(initial,this,"Pick a new color for label");
	if (to_add.isValid())
	{
		QString text = QString::number(to_add.red()) + QString(",")
			         + QString::number(to_add.green()) + QString(",")
					 + QString::number(to_add.blue());
		text = QString("(") + text + QString(")");
		ui.cmbFontColor->addItem(text, QVariant(to_add));
		ui.cmbFontColor->setCurrentIndex(ui.cmbFontColor->count()-1);
	}
}


void MainPanel::addNewFontSize()
{
	QInputDialog d(this);
	d.setInputMode(QInputDialog::IntInput);
	d.setWindowTitle("Input some font size");
	d.setIntRange(201,1000);
	d.exec();
	if (d.result() == QDialog::Accepted)
	{
		int size = d.intValue();
		ui.cmbFontSize->addItem(QString::number(size), QVariant(size));
		ui.cmbFontSize->setCurrentIndex(ui.cmbFontSize->count()-1);
	}
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

void MainPanel::highlightState(const hst::string & hints)
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
		
		// Set props
		ActionContext * c = this->m_editor->log();
		hst::string fontName=ui.cmbFonts->currentText().toStdString().c_str();
		label->getProperty("font")->set(sad::Variant(fontName),c);
		QColor qcolor = ui.cmbFontColor->itemData(ui.cmbFontColor->currentIndex()).value<QColor>();
		hst::color hcolor(qcolor.red(), qcolor.green(), qcolor.blue());
		label->getProperty("color")->set(sad::Variant(hcolor), c);
		label->getProperty("pos")->set(sad::Variant(hPointF(0,0)), c);
		float angle = ui.dblAngle->value();
		label->getProperty("angle")->set(sad::Variant(angle), c);
		unsigned int size = ui.cmbFontSize->itemData(ui.cmbFontSize->currentIndex()).value<int>();
		label->getProperty("size")->set(sad::Variant(size), c);
		hst::string text=ui.txtLabelText->toPlainText().toStdString().c_str();
		label->getProperty("text")->set(sad::Variant(text), c);


		label->tryReload(this->m_editor->database());
		InterlockedScene * scene = static_cast<InterlockedScene*>(this->m_editor->scene());
		label->setScene(static_cast<InterlockedScene*>(this->m_editor->scene()));
		this->m_editor->behaviourSharedData()->setActiveObject(label);
		scene->add(label);
		
		this->m_editor->currentBehaviour()->enterState("label_adding");
	}
}


void MainPanel::addSpriteObject()
{
	hst::string newstate = "";
	if (ui.rbPlaceAndRotate->isChecked()) newstate = "sprite_adding_simple";
	if (ui.rbTwoClicksPlaces->isChecked()) newstate = "sprite_adding_diagonal";
	if (newstate.length())
	{
		QSpriteTableWidgetSelection selection = m_spriteTableWidget->selection();
		if (selection.invalid() == false)
		{
			hst::string config = selection.config().toStdString().c_str();
			hst::string group = selection.group().toStdString().c_str();
			int index = selection.index();
			//  Adding a small screen sprite
			ScreenSprite * a = new ScreenSprite();
			Sprite2DConfig * c = m_editor->database()->sprites().hconfigs()[config];
			Sprite2DTemplate t = c->getTemplates()[group][index];
			hPointF p1 = hPointF(320,240);
			hPointF p2 = p1 + t.size();
			hRectF rect(p1, p2);
			a->setProp<hRectF>("rect",rect, m_editor->log());
			a->setProp<float>("angle",ui.dblAngle->value(), m_editor->log());
			a->setProp<hst::string>("config",config, m_editor->log());
			a->setProp<hst::string>("group",group, m_editor->log());
			a->setProp<int>("index",index, m_editor->log());
			a->tryReload(m_editor->database());
			InterlockedScene * scene = static_cast<InterlockedScene*>(this->m_editor->scene());
			a->setScene(scene);

			this->m_editor->behaviourSharedData()->setActiveObject(a);

			scene->add(a);
			this->m_editor->currentBehaviour()->enterState(newstate);
		}
		else
		{
			QMessageBox::critical(NULL, "IFace Editor", "You must select a sprite for adding");
		}
	}
	else
	{
		QMessageBox::critical(NULL, "IFace Editor", "You must select a mode for adding");
	}
}


void MainPanel::setAddingEnabled(bool enabled)
{
	this->ui.lstObjects->setEnabled(enabled);
	this->ui.btnAddLabel->setEnabled(enabled);
	this->ui.btnAddSprite->setEnabled(enabled);
}

void MainPanel::trySetProperty(const hst::string & prop, float v)
{
	EditorBehaviourSharedData * data = this->m_editor->behaviourSharedData();
	AbstractScreenObject * o = NULL;
	AbstractProperty * _property = NULL;
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
		_property = o->getProperty(prop);
		float  old;
		if (_property) 
		{
			sad::Log * sl = this->m_editor->log();
			old = _property->get(sl)->get<float>(sl);	
			_property->set(v, this->m_editor->log());
		}
		if (selected) 
		{
			if (prop == "angle") 
			{
				QTimer * t =new QTimer();
				t->setSingleShot(true);
				bool pending = this->m_editor->shdata()->isRotationCommandPending();
				sad::Variant new_v(v);
				float new_val_escaped = new_v.get<float>(this->m_editor->log());
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


template<typename T> void MainPanel::trySetProperty(const hst::string & prop, T v)
{
	EditorBehaviourSharedData * data = this->m_editor->behaviourSharedData();
	AbstractScreenObject * o = NULL;
	AbstractProperty * _property = NULL;
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
	if (prop == "color" && o->type() != "ScreenLabel")
		return;
	if (o) 
	{
		this->m_editor->lockRendering();
		_property = o->getProperty(prop);
		T  old;
		if (_property) 
		{
			sad::Log * sl = this->m_editor->log();
			old = _property->get(sl)->get<T>(sl);	
			_property->set(v, this->m_editor->log());
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
				sad::Variant new_v(v);
				sad::Variant old_escaped(old);
				float new_val_escaped = new_v.get<float>(this->m_editor->log());
				float old_val_escaped = old_escaped.get<float>(this->m_editor->log());
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

void MainPanel::fontChanged(const QString & s)
{
	IGNORE_SELFCHANGING
	hst::string hs = s.toStdString().c_str();
	trySetProperty("font", hs);
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
		QColor clr = ui.cmbFontColor->itemData(index).value<QColor>();
		hst::color c(clr.red(),clr.green(),clr.blue());
		trySetProperty("color", c);
	}
}

void MainPanel::sizeChanged(int index)
{
	IGNORE_SELFCHANGING
	if (index!=-1)
	{
		unsigned int size = ui.cmbFontSize->itemData(index).value<int>();
		trySetProperty("size", size);
	}
}

void MainPanel::nameChanged(const QString & name)
{
	trySetProperty("name", hst::string(name.toStdString().c_str()));
	this->updateList();
}

void MainPanel::textChanged()
{
	IGNORE_SELFCHANGING
	hst::string s = ui.txtLabelText->toPlainText().toStdString().c_str();
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
	AbstractProperty * prop = NULL;
	sad::Log * l = this->m_editor->log();		
	// Get text
	prop = o->getProperty("text");
	if (prop)
	{
		m_selfchanged = true;
		BLOCK_SIGNALS_AND_CALL(
			ui.txtLabelText,
			setPlainText(prop->get(l)->get<hst::string>(l).data())
		);
	}
	// Get size
	prop = o->getProperty("size");
	if (prop)
	{
		m_selfchanged = true;
		unsigned int size = prop->get(l)->get<unsigned int>(l);
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
		m_selfchanged = false;
	}
	prop = o->getProperty("color");
	if (prop && o->type() == "ScreenLabel")
	{
		m_selfchanged = true;
		hst::color c = prop->get(l)->get<hst::color>(l);
		QColor clr(c.r(), c.g(), c.b()); 
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
	}
	prop = o->getProperty("font");
	if (prop)
	{
		m_selfchanged = true;
		hst::string c = prop->get(l)->get<hst::string>(l);
		QString s = c.data();
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
	}
	prop = o->getProperty("angle");
	if (prop)
	{
		m_selfchanged = true;
		float c = prop->get(l)->get<float>(l);
		BLOCK_SIGNALS_AND_CALL(ui.dblAngle, setValue(c));
		m_selfchanged = false;
	}
	prop = o->getProperty("name");
	if (prop)
	{	
		m_selfchanged = true;
		hst::string c= prop->get(l)->get<hst::string>(l);
		BLOCK_SIGNALS_AND_CALL(ui.txtName, setText(c.data()));
		m_selfchanged = false;
	}
	if (o->getProperty("config") != NULL)
	{
		this->setRegionParameters();
		
		hst::string config = o->prop<hst::string>("config", m_editor->log());
		hst::string group = o->prop<hst::string>("group", m_editor->log());
		int index = o->prop<int>("index", m_editor->log());
		
		QSpriteTableWidgetSelection sel(config.data(), group.data(), index);
		BLOCK_SIGNALS_AND_CALL(m_spriteTableWidget,setSelection(sel));
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
			c->commit(m_editor->log(), m_editor);
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
			c->commit(m_editor->log(), m_editor);
			m_editor->history()->add(c);
		}
	}
}

void MainPanel::setAngleChangingEnabled(bool enabled)
{
	ui.dblAngle->setEnabled(enabled);
	if (enabled == false)
	{
		ui.dblAngle->setValue(0.0);
	}
}

void MainPanel::setSpriteChangingEnabled(bool enabled)
{
	this->ui.dblSpriteX->setEnabled(enabled);
	this->ui.dblSpriteY->setEnabled(enabled);
	this->ui.dblSpriteWidth->setEnabled(enabled);
	this->ui.dblSpriteHeight->setEnabled(enabled);
}


void MainPanel::setRegionParameters()
{
	AbstractScreenObject * o1 = m_editor->behaviourSharedData()->activeObject();
	AbstractScreenObject * o2 = m_editor->behaviourSharedData()->selectedObject();
	AbstractScreenObject * o = (o1) ? o1 : o2;
	if (o)
	{
		hRectF rect = o->region();
		if (o->typeName() == "ScreenSprite")
		{
			m_selfchanged = true;
			ui.dblSpriteX->setValue(rect[0].x());
			ui.dblSpriteY->setValue(rect[0].y());
			ui.dblSpriteWidth->setValue(dist(rect[0], rect[1]));
			ui.dblSpriteHeight->setValue(dist(rect[0], rect[3]));
			m_selfchanged = false;
		}
	}
}


void MainPanel::spriteSelected(QString config, QString group, int index)
{
	if (m_selfchanged)
		return;
	AbstractScreenObject * o1 = m_editor->behaviourSharedData()->activeObject();
	AbstractScreenObject * o2 = m_editor->behaviourSharedData()->selectedObject();
	AbstractScreenObject * o = (o1) ? o1 : o2;
	if (o)
	{
		
		QSpriteTableWidgetSelection sel(config, group, index);
		if (sel.invalid() == false  && o->getProperty("config") != NULL) 
		{
			hst::string oconf =  o->prop<hst::string>("config", m_editor->log());
			hst::string ogroup =  o->prop<hst::string>("group", m_editor->log());
			int oindex =  o->prop<int>("index", m_editor->log());

			o->setProp<hst::string>("config", config.toStdString().c_str(), m_editor->log());
			o->setProp<hst::string>("group",  group.toStdString().c_str(), m_editor->log());
			o->setProp<int>("index",    index, m_editor->log());
			
			bool set_rect = false;
			hRectF rect = o->region();
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
}

void SpritePropertyChangeCommand::commit(ActionContext *c, CommandChangeObserver * ob )
{
	SL_SCOPE("SpritePropertyChangeCommand::commit");
	hst::string rd = SaveLoadCallback<hRectF>::save(m_new.rect).data();
	m_sprite->setProp<hst::string>("config", m_new.config, m_log);
	m_sprite->setProp<hst::string>("group",  m_new.group, m_log);
	m_sprite->setProp<int>("index",    m_new.index, m_log);
	m_sprite->tryReload(m_db);
	m_sprite->setRotatedRectangle(m_new.rect, m_new.angle);
	ob->submitEvent("SpritePropertyChangeCommand::commit", sad::Variant(0));
}

void SpritePropertyChangeCommand::rollback(ActionContext *c, CommandChangeObserver * ob)
{
	SL_SCOPE("SpritePropertyChangeCommand::rollback");
	m_sprite->setProp<hst::string>("config", m_old.config, m_log);
	m_sprite->setProp<hst::string>("group",  m_old.group, m_log);
	m_sprite->setProp<int>("index",    m_old.index, m_log);
	m_sprite->tryReload(m_db);
	m_sprite->setRotatedRectangle(m_old.rect, m_old.angle);
	ob->submitEvent("SpritePropertyChangeCommand::rollback", sad::Variant(0));
}

void MainPanel::spriteRectChanged()
{
	if (m_selfchanged)
		return;
	AbstractScreenObject * o1 = m_editor->behaviourSharedData()->activeObject();
	AbstractScreenObject * o2 = m_editor->behaviourSharedData()->selectedObject();
	AbstractScreenObject * o = (o1) ? o1 : o2;
	if (o)
	{
		if (o->getProperty("rect") != NULL && o->typeName() == "ScreenSprite")
		{
			float angle = o->prop<float>("angle", this->m_editor->log());
			hRectF oldrect = o->region();
			hPointF newpoint(ui.dblSpriteX->value(), ui.dblSpriteY->value());
			hPointF size(ui.dblSpriteWidth->value(), ui.dblSpriteHeight->value());
			float comparisonprec = 0.0001f;
			float oldwidth = dist(oldrect[1], oldrect[0]);
			float oldheight = dist(oldrect[3], oldrect[0]);
			if (fabs(newpoint.x() - oldrect[0].x()) < comparisonprec
				&& fabs(newpoint.y() - oldrect[0].y()) < comparisonprec
				&& fabs(size.x() - oldwidth) < comparisonprec
				&& fabs(size.y() - oldheight) < comparisonprec
				)
			{
				return;
			}
			hPointF horizontal = normalize(oldrect[1] - oldrect[0]);
			hPointF vertical = normalize(oldrect[3] - oldrect[0]);
			hRectF newrect;
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
}

void SpriteRectChangeCommand::commit(ActionContext *c, CommandChangeObserver * ob)
{
	m_sprite->setRotatedRectangle(m_old_rect, m_angle);
	ob->submitEvent("SpriteRectChangeCommand::commit", sad::Variant(0));
}


void SpriteRectChangeCommand::rollback(ActionContext *c, CommandChangeObserver * ob)
{
	m_sprite->setRotatedRectangle(m_new_rect, m_angle);
	ob->submitEvent("SpriteRectChangeCommand::rollback", sad::Variant(0));
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
			d->commit(m_editor->log(), m_editor);
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
		c->commit(m_editor->log(), m_editor);
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

