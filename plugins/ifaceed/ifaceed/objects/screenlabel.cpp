#include "screenlabel.h"
#include "screentemplate.h"

#include "../core/fonttemplatesdatabase.h"
#include "../core/fontdatabase.h"

#include <geometry2d.h>
#include <3rdparty/format/format.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>


DECLARE_SOBJ_INHERITANCE(ScreenLabel, AbstractScreenObject);

ScreenLabel::ScreenLabel() : AbstractScreenObject()
{
	m_font = NULL;

	this->addProperty("font" ,new MappedField<sad::String>(&m_font_name, ""));
	this->addProperty("size" ,new MappedField<unsigned int>(&m_font_size, 0));
	this->addProperty("color",new MappedField<sad::Color>(&m_font_color, sad::Color(0,0,0)));
	this->addProperty("text" ,new MappedField<sad::String>(&m_text, ""));
	this->addProperty("angle",new MappedField<float>(&m_angle, 0.0f));
	this->addProperty("pos"  ,new MappedField<sad::Point2D>(&m_point, sad::Point2D(0,0)));
	this->addProperty("alpha"  ,new MappedField<int>(&m_alpha, 0));

}


void ScreenLabel::moveCenterTo(const sad::Point2D & p)
{
	sad::Size2D r = m_font->size(m_text);
	m_point.setX(p.x() - r.Width/2);
	m_point.setY(p.y() + r.Height/2);
}

sad::String ScreenLabel::_description()
{
	sad::String result = m_text.subString(0,10);
	if (result.length() > 10)
		result<<"... ";
	else
		result<<" ";
	result<<"(";
	result<<m_font_name;
	result<<",";
	result<<sad::String::number(m_font_size);
	result<<")";
	return result; 
}

sad::String ScreenLabel::typeName()
{
	return "ScreenLabel";
}

void ScreenLabel::_render()
{
	if (!m_font)
	{
		return;
	}
	m_font->setColor(sad::AColor(m_font_color.r(),m_font_color.g(),m_font_color.b(),(sad::uchar)m_alpha));
	m_font->setSize(m_font_size);

	sad::Size2D s = m_font->size(m_text);
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glTranslatef(m_point.x() +  s.Width /2, m_point.y() - s.Height / 2, 0.0f);
	glRotatef(m_angle / M_PI*180.0f, 0.0f, 0.0f, 1.0f);
	sad::Point2D p(s.Width / -2 , s.Height / 2);
	m_font->render(m_text, p);	
	
	glPopMatrix();

}

bool ScreenLabel::isValid(FontTemplateDatabase * db, sad::Vector<sad::String> * errors)
{
	IFaceEditorFontList & d = db->fonts();
	bool result = d.hasFont(m_font_name.data());
	if (result == false && errors != NULL)
	{
		*errors << str(
					fmt::Print("Font \"{0}\" is absent in database\n") 
					<< m_font_name.data() 
					).c_str();
	}
	return result;
}

bool ScreenLabel::tryReload(FontTemplateDatabase * db)
{
	if (!isValid(db))
		return false;
	IFaceEditorFontList & d = db->fonts();
	m_font = d.font(m_font_name.data())->sadFont();
	return true;
}


bool ScreenLabel::isWithin(const sad::Point2D & p)
{
	sad::Rect2D r = this->region();
	return sad::isWithin(p, r);
}

sad::Rect2D ScreenLabel::region()
{
	if (!m_font)
	{
		return sad::Rect2D();
	}
	m_font->setSize(m_font_size);

	sad::Size2D s = m_font->size(m_text);
	sad::Rect2D non_rotated_region( sad::Point2D(s.Width / -2.0f, s.Height / -2.0f),
		                       sad::Point2D(s.Width / 2.0f, s.Height / 2.0f));
	sad::Point2D results[4];
	float cos_angle = cos(m_angle);
	float sin_angle = sin(m_angle);
	float centerx = m_point.x() + s.Width / 2;
	float centery = m_point.y() - s.Height / 2;
	for (int i=0;i<4;i++)
	{
		results[i].setX(centerx + non_rotated_region[i].x()*cos_angle - non_rotated_region[i].y() * sin_angle);
		results[i].setY(centery + non_rotated_region[i].x()*sin_angle + non_rotated_region[i].y() * cos_angle);
	}
	return sad::Rect2D(results[0],results[1], results[2], results[3]);
}


bool ScreenLabel::resizable()
{
	return false;
}

void ScreenLabel::initializeGraphicAfterLoad(FontTemplateDatabase * db)
{
	tryReload(db);
}

