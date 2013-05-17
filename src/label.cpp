#include "label.h"
#include "renderer.h"

SAD_DECLARE(Label,sad::BasicNode)

void Label::operator()(const sad::ResizeEvent & o)
{
	float old_width=(float)(sad::Renderer::ref()->settings().width());
	float old_height=(float)(sad::Renderer::ref()->settings().height());
	m_rend_point.setX(o.new_width/old_width);
	m_rend_point.setY(o.new_width/old_height);
}
Label::Label(sad::Input * input)
{
	m_input = input;
	m_input->addResizeHandler(this,false);
}
Label::Label(
		      LabelFont fnt,
		      const hst::string & str,
			  const pointf      & p,
			  sad::Input * input
			)
{
	m_input = input;
    m_fnt=fnt;
	m_str=str;
	m_p=p;
	m_rend_point=p;
	input->addResizeHandler(this,false);
}

Label::~Label()
{
	m_input->removeResizeHandler(this);
}

void Label::render()
{
	m_fnt->render(m_str,m_rend_point);
}
