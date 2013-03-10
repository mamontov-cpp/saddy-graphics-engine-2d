#include "mainpanel.h"
#include <QDialog>
#include <QTimer>
#include <QFontDatabase>
#include <QItemDelegate>
#include <QPainter>
#include <QColorDialog>
#include <QInputDialog>
#include <QTimer>
#include <marshal/actioncontext.h>
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
#include "history/propertychangecommand.h"

#define IGNORE_SELFCHANGING if (m_selfchanged) { m_selfchanged = false; return; }

MainPanel::MainPanel(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	m_selfchanged = false;
	connect(ui.btnPickFontColor,SIGNAL(clicked()),this,SLOT(addNewFontColor()));
	connect(ui.btnPickFontSize,SIGNAL(clicked()), this, SLOT(addNewFontSize()));
	connect(ui.btnAddLabel, SIGNAL(clicked()), this, SLOT(addFontObject()));
	
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

	// Add SpriteViewer
	QGridLayout* grPadLayout = new QGridLayout;
	QPoint pointPad = ui.spriteViewerPad->pos();
	QPoint pointGroupPad = pointPad + ui.grpSprites->pos();
	
	QRectF contentRect = QRectF(pointGroupPad,ui.spriteViewerPad->size());
	m_spriteTableWidget = new QSpriteTableWidget(ui.cmbSpriteConfig,
													grPadLayout);


	ui.spriteViewerPad->setLayout(grPadLayout);
	connect(ui.cmbFontColor, SIGNAL(currentIndexChanged(int)), this, SLOT(colorChanged(int)));
	connect(ui.cmbFonts, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(fontChanged(const QString&)));
	connect(ui.cmbFontSize, SIGNAL(currentIndexChanged(int)), this, SLOT(sizeChanged(int))); 
	connect(ui.dblAngle, SIGNAL(valueChanged(double)), this, SLOT(angleChanged(double)));
	connect(ui.txtLabelText, SIGNAL(textChanged()), this, SLOT(textChanged()));
	connect(m_spriteTableWidget, SIGNAL(spriteSelected(QString,QString,int)), this, SLOT(spriteSelected(QString,QString,int)));	
}

void MainPanel::setEditor(IFaceEditor * editor) 
{  
	m_editor = editor; 
	connect(ui.btnDelete, SIGNAL(clicked()), m_editor, SLOT(tryEraseObject()));
}

void MainPanel::spriteSelected(QString config, QString group, int index)
{
	// TODO: Reimplement
	// QMessageBox::critical(NULL, "IFaceEditor", config + QString(",") + group + QString(",") + QString::number(index));
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

void MainPanel::setAddingEnabled(bool enabled)
{
	this->ui.btnAddLabel->setEnabled(enabled);
	this->ui.btnAddSprite->setEnabled(enabled);
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
	if (o) 
	{
		this->m_editor->lockRendering();
		_property = o->getProperty(prop);
		sad::Variant * old = _property->get(this->m_editor->log());
		if (_property) 
		{
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
				bool pending = this->m_editor->behaviourSharedData()->isRotationCommandPending();
				sad::Variant new_v(v);
				float new_val_escaped = new_v.get<float>(this->m_editor->log());
				float old_val_escaped = old->get<float>(this->m_editor->log());
				if (pending) {
					this->m_editor->behaviourSharedData()->submitRotationCommand(t, o, new_val_escaped, false);
				} else {
					this->m_editor->behaviourSharedData()->submitRotationCommand(t, o, new_val_escaped, true, old_val_escaped);
				}
				QObject::connect(t, SIGNAL(timeout()), this->m_editor, SLOT(appendRotationCommand()));
				t->start(MAX_ROTATION_TIME);
			}
			else
			{
				this->m_editor->history()->add(new PropertyChangeCommand<T>(o, prop, v, this->m_editor->log()));
			}
		}
		this->m_editor->unlockRendering();
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
	IGNORE_SELFCHANGING
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

void MainPanel::textChanged()
{
	IGNORE_SELFCHANGING
	hst::string s = ui.txtLabelText->toPlainText().toStdString().c_str();
	trySetProperty("text",s);
}

void MainPanel::updateObjectStats(AbstractScreenObject * o)
{
	AbstractProperty * prop = NULL;
	EditorLog * l = this->m_editor->log();		
	// Get text
	prop = o->getProperty("text");
	if (prop)
	{
		m_selfchanged = true;
		ui.txtLabelText->setPlainText(prop->get(l)->get<hst::string>(l).data());
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
			ui.cmbFontSize->setCurrentIndex(index);
		} 
		else 
		{
			ui.cmbFontSize->addItem(QString::number(size), (int)size);
			ui.cmbFontSize->setCurrentIndex(ui.cmbFontSize->count() - 1);
		}
	}
	prop = o->getProperty("color");
	if (prop)
	{
		m_selfchanged = true;
		hst::color c = prop->get(l)->get<hst::color>(l);
		QColor clr(c.r(), c.g(), c.b()); 
		int index = ui.cmbFontColor->findData(clr);
		if (index != -1) 
		{
			ui.cmbFontColor->setCurrentIndex(index);
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
			ui.cmbFonts->setCurrentIndex(index);
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
		ui.dblAngle->setValue(c);
	}
}
