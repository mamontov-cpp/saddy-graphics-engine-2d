#include "labelactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../core/typeconverters/qcolortosadacolor.h"

#include "../history/scenenodes/scenenodesnew.h"

#include <label.h>



// ===============================  PUBLIC METHODS ===============================

gui::LabelActions::LabelActions(QObject* parent) : QObject(parent), m_panel(NULL)
{
	
}

gui::LabelActions::~LabelActions()
{
	
}

void gui::LabelActions::setPanel(MainPanel* e)
{
	m_panel = e;
}

MainPanel* gui::LabelActions::panel() const
{
	return m_panel;
}

void gui::LabelActions::updateRegionForLabel()
{
	core::Shared* s = this->m_panel->editor()->shared();
	sad::SceneNode* node = s->activeObject();
	if (node == NULL)
	{
		node  = s->selectedObject(); 
	}
	if (node)
	{
		sad::Renderer::ref()->lockRendering();
		sad::Maybe<sad::Rect2D> maybearea = node->getProperty<sad::Rect2D>("area");
		sad::Renderer::ref()->unlockRendering();
		if (maybearea.exists())
		{
			const sad::Rect2D & v = maybearea.value();
			bool blocked =  this->m_panel->UI()->rwSceneNodeRect->blockSignals(true);
			this->m_panel->UI()->rwSceneNodeRect->setValue(QRectF(v[0].x(), v[0].y(), v.width(), v.height()));
			this->m_panel->UI()->rwSceneNodeRect->blockSignals(blocked);
		}
	}
}

void gui::LabelActions::cancelAddLabel()
{
	core::Shared* s = this->m_panel->editor()->shared();
	sad::SceneNode* label = s->activeObject();
	sad::Renderer::ref()->lockRendering();
	s->setActiveObject(NULL);
	label->scene()->remove(label);
	sad::Renderer::ref()->unlockRendering();
	
	sad::hfsm::Machine* m = m_panel->editor()->machine();
	m->enterState(m->previousState());
	if (m->isInState("adding") || (m->isInState("selected") && s->selectedObject() == NULL))
	{
		m->enterState("idle");
	}
}

void gui::LabelActions::moveLabel(const sad::input::MouseMoveEvent & e)
{
	const sad::Point2D& p = e.pos2D();
	sad::SceneNode* node = m_panel->editor()->shared()->activeObject();
	if (node)
	{
		sad::Maybe<sad::Rect2D> mayberect = node->getProperty<sad::Rect2D>("area");
		if (mayberect.exists())
		{
			sad::Rect2D r = mayberect.value();
			double width = (r[1].x() - r[0].x()) / 2.0;
			double height = (fabs(r[2].y() - r[0].y())) / 2.0;

			r = sad::Rect2D(p.x() - width, p.y() + height, p.x() + width, p.y() - height);
			node->setProperty("area", r);
			this->updateRegionForLabel();
		}
	}
}

void gui::LabelActions::commitLabelAdd(const sad::input::MousePressEvent& e)
{
	core::Shared* s = m_panel->editor()->shared();
	sad::SceneNode* node = s->activeObject();
	sad::Renderer::ref()->database("")->table("scenenodes")->add(node);
	history::Command* c = new history::scenenodes::New(node);
	s->setActiveObject(NULL);
	s->setSelectedObject(node);
	m_panel->editor()->history()->add(c);
	c->commit(m_panel->editor());
	m_panel->editor()->machine()->enterState("selected");
}

// ===============================  PUBLIC SLOTS METHODS ===============================

void gui::LabelActions::addLabel()
{
	bool valid = true;
	valid = valid && m_panel->currentScene() != NULL;
	valid = valid && m_panel->UI()->txtLabelText->toPlainText().length() != 0;
	valid = valid && m_panel->UI()->rtwLabelFont->selectedResourceName().exists();
	if (valid)
	{
		// Cleanup last adding item if was in that state
		m_panel->editor()->cleanupBeforeAdding();

		// Create and initialize label
		sad::Label* label = new sad::Label();
		label->setTreeName("");
		label->setFontName(m_panel->UI()->rtwLabelFont->selectedResourceName().value());

		const sad::Settings & settings = sad::Renderer::ref()->settings();
		label->setPoint(settings.width() / 2.0, settings.height() / 2.0);
		label->setString(m_panel->UI()->txtLabelText->toPlainText().toStdString());
		label->setSize(m_panel->UI()->fswLabelFontSize->value());
		label->setLineSpacingRatio(m_panel->UI()->dsbLineSpacingRatio->value());
		label->setAngle(m_panel->UI()->awSceneNodeAngle->value());

		sad::AColor clr;
		core::typeconverters::QColorToSadAColor::convert(m_panel->UI()->clpSceneNodeColor->selectedColor(), clr);
		label->setColor(clr);

		QString name = m_panel->UI()->txtObjectName->text();
		if (name.length())
		{
			label->setObjectName(name.toStdString());
		}

		// Actually add label
		m_panel->currentScene()->add(label);
		m_panel->editor()->shared()->setActiveObject(label);
		m_panel->editor()->shared()->toggleActiveBorder(true);

		m_panel->editor()->machine()->enterState("adding/label");
		m_panel->highlightState("Click, where you want label to be placed");

		this->updateRegionForLabel();
	}
}


