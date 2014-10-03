#include "selection.h"

#include "editor.h"

#include "../mainpanel.h"
#include "../closuremethodcall.h"

#include <geometry2d.h>

// =========================================== PUBLIC METHODS =========================================== 

core::Selection::Selection() : m_editor(NULL), m_selection_change(false), m_current_position(0)
{
	 connect(&m_timer, SIGNAL(timeout()), this, SLOT(disableSelectionNavigation()));
}

core::Selection::~Selection()
{
	
}

void core::Selection::setEditor(core::Editor* e)
{
	m_editor = e;
}

core::Editor* core::Selection::editor() const
{
	return m_editor;
}

const int core::Selection::TIMEOUT = 700;


void core::Selection::trySelect(const sad::input::MousePressEvent& e)
{
	// Stop selection
	m_selection_change = false;
	m_editor->emitClosure( bind(&m_timer, &QTimer::stop) );

	if (m_editor->shared()->selectedObject())
	{
		sad::Vector<sad::Rect2D> regions;
		m_editor->shared()->selectedObject()->regions(regions);
		if (sad::isWithin(e.pos2D(), regions))
		{			
			// TODO: Implement moving sprite
			
			return;
		}
	}


	// Fill navigation chain
	if (m_editor->panel()->currentScene())
	{
		m_selection_chain.clear();
		const sad::Vector<sad::SceneNode*>& objects = m_editor->panel()->currentScene()->objects();		
		for(int i = objects.size() - 1; i > -1; i--)
		{
			sad::Vector<sad::Rect2D> regions;
			objects[i]->regions(regions);
			if (sad::isWithin(e.pos2D(), regions))
			{
				m_selection_chain << objects[i];
			}
		}

		if (m_selection_chain.count() == 0) 
		{
			m_editor->machine()->enterState("idle");
			m_editor->shared()->setSelectedObject(NULL);
		}
		else
		{
			m_editor->shared()->setSelectedObject(m_selection_chain[0]);
			m_editor->machine()->enterState("selected");
			m_editor->panel()->updateUIForSelectedItem();
			m_current_position = 0;
			m_editor->emitClosure( bind(this, &core::Selection::startTimer));
		}
	}
}

void core::Selection::navigateSelection(const sad::input::MouseWheelEvent& e)
{
	int last_position = m_current_position;
	if (e.Delta > 0)
	{
		if (m_current_position == m_selection_chain.size() - 1)
		{
			m_current_position = 0;
		}
		else
		{
			m_current_position++;
		}
	}
	else
	{
		if (m_current_position == 0)
		{
			m_current_position = m_selection_chain.size() - 1;
		}
		else
		{
			m_current_position--;
		}
	}

	if (last_position != m_current_position)
	{
		m_editor->shared()->setSelectedObject(m_selection_chain[m_current_position]);
		m_editor->machine()->enterState("selected");
		m_editor->panel()->updateUIForSelectedItem();
		m_editor->emitClosure( bind(this, &core::Selection::startTimer));
	}
}

bool core::Selection::isSelectionPending() const
{
	return m_selection_change;
}

// =========================================== PROTECTED METHODS =========================================== 

void core::Selection::disableSelectionNavigation()
{
	m_selection_change = false;
}

void core::Selection::startTimer()
{
	if (m_selection_change)
	{
		m_timer.stop();
	}
	m_timer.setSingleShot(true);
	m_timer.setInterval(core::Selection::TIMEOUT);
	m_timer.start();
	m_selection_change = true;
}