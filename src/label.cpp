#include "label.h"

SAD_DECLARE(Label,sad::BasicNode)


Label::Label(
		      sad::BasicFont * fnt,
		      const hst::string & str,
			  const hRectF      & rect,
			  float                z
			)
{
    m_fnt=fnt;
	m_str=str;
	m_rect=rect;
	m_z=z;
}

Label::~Label()
{

}

void Label::render()
{
	m_fnt->render(m_str,m_rect,m_z);
}
