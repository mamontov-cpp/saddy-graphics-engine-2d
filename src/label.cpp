#include "label.h"
#include "renderer.h"

SAD_DECLARE(Label,sad::BasicNode)

void Label::operator()(const sad::ResizeEvent & o)
{
	float old_width=(float)(m_renderer->settings().width());
	float old_height=(float)(m_renderer->settings().height());
	m_rend_point.setX(m_p.x() * o.new_width/old_width);
	m_rend_point.setY(m_p.y() * o.new_height/old_height);
}
Label::Label(sad::Renderer * renderer)
{
	m_renderer = renderer;
	m_renderer->controls()->addResizeHandler(this,false);
}
Label::Label(
		      LabelFont fnt,
		      const hst::string & str,
			  const pointf      & p,
			  sad::Renderer * renderer
			)
{
	m_renderer = renderer;
    m_fnt=fnt;
	m_str=str;
	m_p=p;
	m_rend_point=p;
	m_renderer->controls()->addResizeHandler(this,false);
}

Label::~Label()
{
	m_renderer->controls()->removeResizeHandler(this);
}

void Label::render()
{
	m_fnt->render(m_str,m_rend_point);
}
