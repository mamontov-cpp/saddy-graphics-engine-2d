#include "labelactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../core/typeconverters/qcolortosadacolor.h"

#include "../history/scenenodes/scenenodesnew.h"

#include "../history/label/changefontname.h"
#include "../history/label/changefontsize.h"
#include "../history/label/changetext.h"
#include "../history/label/changelinespacing.h"

#include <label.h>
#include <geometry2d.h>

#include <QDebug>


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
			CLOSURE
			CLOSURE_DATA( gui::rectwidget::RectWidget* w; sad::Rect2D v; )
			CLOSURE_CODE(
				bool blocked =  w->blockSignals(true);
				w->setValue(QRectF(v[0].x(), v[0].y(), v.width(), v.height()));
				w->blockSignals(blocked);
			)
			INITCLOSURE( CLSET(w, m_panel->UI()->rwSceneNodeRect); CLSET(v, v); )
			SUBMITCLOSURE(	m_panel->editor()->emitClosure );
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

void gui::LabelActions::rotateLabelWhenAdding(const sad::input::MouseWheelEvent& e)
{
	sad::SceneNode* active = m_panel->editor()->shared()->activeObject();
	sad::Maybe<float> maybeangle = active->getProperty<float>("angle");
	if (maybeangle.exists())
	{
		float angle = maybeangle.value() / M_PI * 180;
		if (sad::is_fuzzy_equal(0, angle, 0.001) && e.Delta < 0)
		{
			angle = 359.0f;
		} 
		else 
		{ 
			if (sad::is_fuzzy_equal(360, angle, 0.001) && e.Delta > 0)
			{
				angle = 1.0f;
			} 
			else
			{
				float delta = (e.Delta > 0) ? 1 : -1;
				angle += delta;
			}
		}

		angle  = angle / 180 * M_PI;
		active->setProperty("angle", angle);

		// Update label
		CLOSURE
		CLOSURE_DATA(double __angle; MainPanel* m_panel;)
		CLOSURE_CODE(  
			bool b =  m_panel->UI()->awSceneNodeAngle->blockSignals(true);
			m_panel->UI()->awSceneNodeAngle->setValue((double)__angle);
			m_panel->UI()->awSceneNodeAngle->blockSignals(b);				
		);
		INITCLOSURE( CLSET(__angle, angle); CLSET(m_panel, m_panel);  )
		SUBMITCLOSURE(	m_panel->editor()->emitClosure )
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
		bool blocked = m_panel->UI()->cbSceneNodeVisible->blockSignals(true);
		m_panel->UI()->cbSceneNodeVisible->setCheckState(Qt::Checked);
		m_panel->UI()->cbSceneNodeVisible->blockSignals(blocked);

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

void gui::LabelActions::labelFontChanged(sad::String s)
{
	if (m_panel->editor()->shared()->activeObject() != NULL)
	{
		m_panel->editor()->shared()->activeObject()->setProperty("font", s);
	}
	else
	{
		sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
		if (node)
		{
			sad::Maybe<sad::String> oldvalue = node->getProperty<sad::String>("font");
			if (oldvalue.exists())
			{
				if (oldvalue.value() != s)
				{
					node->setProperty("font", s);
					m_panel->editor()->history()->add(new history::label::ChangeFontName(node, oldvalue.value(), s));
				}
			}
		}
	}
}

void gui::LabelActions::labelSizeChanged(unsigned int s)
{
    if (m_panel->editor()->shared()->activeObject() != NULL)
    {
        m_panel->editor()->shared()->activeObject()->setProperty("fontsize", s);
        this->updateRegionForLabel();
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<unsigned int> oldvalue = node->getProperty<unsigned int>("fontsize");
            if (oldvalue.exists())
            {
                if (oldvalue.value() != s)
                {
                    node->setProperty("fontsize", s);
                    this->updateRegionForLabel();
                    m_panel->editor()->history()->add(new history::label::ChangeFontSize(node, oldvalue.value(), s));
                }
            }
        }
    }
}


void gui::LabelActions::labelTextChanged()
{
    sad::String newvalue = m_panel->UI()->txtLabelText->toPlainText().toStdString();
    if (m_panel->editor()->shared()->activeObject() != NULL)
    {
        m_panel->editor()->shared()->activeObject()->setProperty("text", newvalue);
        this->updateRegionForLabel();
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::String> oldvalue = node->getProperty<sad::String>("text");
            if (oldvalue.exists())
            {
                if (oldvalue.value() != newvalue)
                {
                    node->setProperty("text", newvalue);
                    this->updateRegionForLabel();
                    m_panel->editor()->history()->add(new history::label::ChangeText(node, oldvalue.value(), newvalue));
                }
            }
        }
    }
}

void gui::LabelActions::labelLineSpacingChanged(double newvalue)
{
	if (m_panel->editor()->shared()->activeObject() != NULL)
    {
        m_panel->editor()->shared()->activeObject()->setProperty("linespacing", newvalue);
        this->updateRegionForLabel();
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<float> oldvalue = node->getProperty<float>("linespacing");
            if (oldvalue.exists())
            {
				if (sad::is_fuzzy_equal(oldvalue.value(), newvalue, 0.0001) == false)
                {
                    node->setProperty("linespacing", newvalue);
                    this->updateRegionForLabel();
                    m_panel->editor()->history()->add(new history::label::ChangeLineSpacing(node, oldvalue.value(), newvalue));
                }
            }
        }
    }
}
