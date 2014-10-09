/*! \file renderways.h
	\author HiddenSeeker

	Defines a simple process to render some ways from database
 */
#pragma once
#include <pipeline/pipelineprocess.h>
#include <sprite2d.h>
#include <sadcolor.h>

class MainPanel;

namespace gui
{

/*! A process to render some ways from database
 */
class RenderWays: public sad::pipeline::AbstractProcess
{
public:
	/*! Construct new delegate for rendering
	 */
	RenderWays(MainPanel* panel);
	/*! Can be inherited
	 */
	virtual ~RenderWays();
protected:
	/*! A panel for rendering ways
	 */
	MainPanel * m_panel;
	/*! Whether sprites are init
	 */
	bool m_init;
	/*! A default sprite to render points
	 */
	sad::Sprite2D* m_default_sprite;
	/*! A red sprite to render points
	 */
	sad::Sprite2D* m_red_sprite;
	/*! A default color for selected point
	 */
	sad::AColor m_default;
	/*! A red color
	 */
	sad::AColor m_red;
	/*! Invokes a delegate inside of process
	 */ 
	virtual void _process();
};

}
