#include "screenlabel.h"
#include "screentemplate.h"
#include "../core/fonttemplatesdatabase.h"
#include "../core/fontdatabase.h"
#include "../core/ifaceeditor.h"

ScreenLabel::ScreenLabel() : AbstractScreenObject()
{
	this->invalidateCache();

	this->addProperty("font" ,new MappedField<hst::string>(&m_font_name, ""));
	this->addProperty("size" ,new MappedField<unsigned int>(&m_font_size, 0));
	this->addProperty("color",new MappedField<hst::color>(&m_font_color, hst::color(0,0,0)));
	this->addProperty("text" ,new MappedField<hst::string>(&m_text, ""));
	this->addProperty("angle",new MappedField<float>(&m_angle, 0.0f));
	this->addProperty("pos"  ,new MappedField<hPointF>(&m_point, hPointF(0,0)));
}


void ScreenLabel::invalidateCache()
{
	m_font = NULL;
}

hst::string ScreenLabel::type()
{
	return "ScreenLabel";
}

void ScreenLabel::_render()
{
	if (this->screenTemplate() == NULL)
		return;
	if (!m_font && !isValid(this->screenTemplate()))
	{
		return;
	}
	if (!m_font)
	{
		IFaceEditorFontList & d = static_cast<IFaceEditor*>(this->screenTemplate()->editor())->database()->fonts();
		IFaceEditorFont * fnt = d.font(m_font_name.data());
		if (!fnt)
			return;
		m_font = fnt->sadFont();
	}
	m_font->setColor(hst::acolor(m_font_color.r(),m_font_color.g(),m_font_color.b(),0));
	m_font->setHeight(m_font_size);
	m_font->render(m_text, m_point.x(), m_point.y());	
	
	glDisable(GL_TEXTURE_2D);
	hRectF s = m_font->size(m_text);
	

	
	GLint   clr[4]={};
	glGetIntegerv(GL_CURRENT_COLOR,clr);
	glColor4ub(255,0,0,255);
    glBegin(GL_LINES);
	glVertex2f(m_point.x(), m_point.y());
	glVertex2f(m_point.x() + s.width(), m_point.y());

	glVertex2f(m_point.x() + s.width(), m_point.y());
	glVertex2f(m_point.x() + s.width(), m_point.y() - s.height());

	glVertex2f(m_point.x() + s.width(), m_point.y() - s.height());
	glVertex2f(m_point.x() , m_point.y() - s.height());	

	glVertex2f(m_point.x() , m_point.y() - s.height());	
	glVertex2f(m_point.x(), m_point.y());

	glEnd();
	glColor4iv(clr);
	glEnable(GL_TEXTURE_2D);


}

bool ScreenLabel::isValid(ScreenTemplate * t)
{
	IFaceEditorFontList & d = static_cast<IFaceEditor*>(t->editor())->database()->fonts();
	return d.hasFont(m_font_name.data());
}


bool ScreenLabel::isWithin(const hPointF & p)
{
	return false; //!< TODO: Implement
}

hRectF ScreenLabel::region()
{
	return hRectF();
}
