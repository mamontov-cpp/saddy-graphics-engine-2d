#include "sprite2d.h"
#include <geometry2d.h>
#include <renderer.h>
#include <texturemanager.h>

#include <os/glheaders.h>

#include <math.h>

DECLARE_SOBJ_INHERITANCE(sad::Sprite2D,sad::SceneNode)

sad::Sprite2D::Sprite2D()
: 
m_angle(0),
m_flipx(false),
m_flipy(false),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(0,0,0,0),
m_middle(0,0),
m_size(0,0),
m_renderable_area(sad::Point2D(0, 0), sad::Point2D(0, 0)),
m_texture(NULL),
m_color(sad::AColor(255,255,255,0))
{

}

sad::Sprite2D::Sprite2D(		
	sad::Texture * texture,
	const sad::Rect2D & texturecoordinates,
	const sad::Rect2D & area,
	bool fast
)
: 
m_flipx(false),
m_flipy(false),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(texturecoordinates),
m_texture(texture),
m_color(sad::AColor(255,255,255,0))
{
	normalizeTextureCoordinates();
	if (fast)
	{
		initFromRectangleFast(area);
	}
	else
	{
		initFromRectangle(area);
		buildRenderableArea();
	}
}

sad::Sprite2D::Sprite2D(
		const sad::String& texture,
		const sad::Rect2D& texturecoordinates,
		const sad::Rect2D& area,
		bool fast
	)
: 
m_texture_name(texture),
m_flipx(false),
m_flipy(false),
m_angle(0),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(texturecoordinates),
m_texture(NULL),
m_color(sad::AColor(255,255,255,0))
{
	normalizeTextureCoordinates();
	if (fast)
	{
		initFromRectangleFast(area);
	}
	else
	{
		initFromRectangle(area);
		buildRenderableArea();
	}
}

sad::Sprite2D::~Sprite2D()
{

}

void sad::Sprite2D::render()
{
  if (!m_texture)
	  return;
   glGetIntegerv(GL_CURRENT_COLOR, m_current_color_buffer);   
   glColor4ub(m_color.r(),m_color.g(),m_color.b(),255-m_color.a());	   
   m_texture->bind();	
   glBegin(GL_QUADS);
   for (int i = 0;i < 4; i++)
   {
	  glTexCoord2f(m_normalized_texture_coordinates[i].x(),m_normalized_texture_coordinates[i].y());
	  glVertex2f(m_renderable_area[i].x(),m_renderable_area[i].y());
   }  
   glEnd();
   glColor4iv(m_current_color_buffer);
}

void sad::Sprite2D::setTextureCoordinates(const sad::Rect2D & texturecoordinates)
{
	m_texture_coordinates = texturecoordinates;
	normalizeTextureCoordinates();
}

void sad::Sprite2D::setTextureCoordinate(int index, const sad::Point2D & point)
{
	m_texture_coordinates[index] = point;
	// Try  to immediately convert point to normalized if needed
	if (m_texture)
	{
		sad::Point2D relativepoint(point.x() / m_texture->Width, point.y() / m_texture->Height );
		int newindex = 3 - index;

		// Take care of horizontal flip
		if (m_flipx)
		{
			switch(newindex) 
			{
				case 0 : { newindex = 1; break; }
				case 1 : { newindex = 0; break; }
				case 2 : { newindex = 3; break; }
				case 3 : { newindex = 2; break; }
			};
		}

		// Take care of vertical flip
		if (m_flipy)
		{
			switch(newindex) 
			{
				case 0 : { newindex = 3; break; }
				case 1 : { newindex = 2; break; }
				case 2 : { newindex = 1; break; }
				case 3 : { newindex = 0; break; }
			};
		}

		m_normalized_texture_coordinates[newindex] = relativepoint;
	}
}

void sad::Sprite2D::setTextureCoordinate(int index, double x ,double y)
{
	this->setTextureCoordinate(index, sad::Point2D(x, y));
}

const sad::Rect2D & sad::Sprite2D::textureCoordinates() const
{
	return m_texture_coordinates;
}

void sad::Sprite2D::setRenderableArea(const sad::Rect2D & rect)
{
	initFromRectangle(rect);
	buildRenderableArea();
}

sad::Rect2D sad::Sprite2D::area() const
{
	sad::Rect2D result(
		m_middle + sad::Point2D(m_size.Width / -2,  m_size.Height / -2),
		m_middle + sad::Point2D(m_size.Width /  2,  m_size.Height / -2),
		m_middle + sad::Point2D(m_size.Width /  2,  m_size.Height / 2),
		m_middle + sad::Point2D(m_size.Width / -2,  m_size.Height / 2)
	);
	return result;
}

const sad::Rect2D & sad::Sprite2D::renderableArea() const
{
	return m_renderable_area;
}

const sad::Point2D & sad::Sprite2D::point(int n) const
{
	return m_renderable_area[n];
}

const sad::Point2D & sad::Sprite2D::middle() const
{
	return m_middle;
}

void sad::Sprite2D::setMiddle(const sad::Point2D & p)
{
	m_middle = p;
	buildRenderableArea();
}

const sad::Size2D &  sad::Sprite2D::size() const
{
	return m_size;	
}

void sad::Sprite2D::setSize(const sad::Size2D & size)
{
	m_size = size;
	buildRenderableArea();
}

void sad::Sprite2D::moveBy(const sad::Point2D & dist)
{
	m_middle += dist;
	for (int i=0;i<4;i++)
	{
		m_renderable_area[i] += dist;
	}
}

void sad::Sprite2D::moveTo(const sad::Point2D & p)
{
	sad::Point2D  dist = p;
	dist -= middle();
	moveBy(dist);
}

void sad::Sprite2D::rotate(double angle)
{
	m_angle += angle;
	buildRenderableArea();
}

void sad::Sprite2D::setAngle(double angle)
{
	m_angle += angle;
	buildRenderableArea();
}

double sad::Sprite2D::angle() const
{
	return m_angle;
}

void sad::Sprite2D::setColor(const sad::AColor & clr) 
{
	m_color = clr;
}

const sad::AColor & sad::Sprite2D::color() const
{
	return m_color;
}

void sad::Sprite2D::setFlipX(bool flipx)
{
	bool changed = m_flipx != flipx;
	m_flipx = flipx;
	if (changed)
	{
		normalizeTextureCoordinates();
	}
}

void sad::Sprite2D::setFlipY(bool flipy)
{
	bool changed = m_flipy != flipy;
	m_flipy = flipy;
	if (changed)
	{
		normalizeTextureCoordinates();
	}
}

bool sad::Sprite2D::flipX() const
{
	return m_flipx;
}

bool sad::Sprite2D::flipY() const
{
	return m_flipy;
}

void sad::Sprite2D::setTexture(sad::Texture * texture)
{
	m_texture = texture;
	normalizeTextureCoordinates();
}

sad::Texture * sad::Sprite2D::texture() const
{
	return m_texture;
}

void sad::Sprite2D::setTexureName(const sad::String & name)
{
	m_texture_name = name;
	reloadTexture();
}

const sad::String& sad::Sprite2D::textureName()
{
	return m_texture_name;
}

void sad::Sprite2D::setScene(sad::Scene * scene)
{
	this->sad::SceneNode::setScene(scene);
	if (m_texture_name.length() != 0)
	{
		reloadTexture();
	}
}

void sad::Sprite2D::set(const sad::Sprite2D::Options & o)
{

	sad::Texture * tex = NULL;
	if (o.TextureContainer.exists())
	{
		sad::TextureContainer * c =  
			sad::TextureManager::ref()->getContainer(o.TextureContainer.value());
		tex = c->get(o.Texture);
	}
	else
	{
		tex  = sad::TextureManager::ref()->get(o.Texture);
	}
	this->setTexture(tex);
	this->setTextureCoordinates(o.TextureRectangle);
	this->setRenderableArea(o.Rectangle);
}


void sad::Sprite2D::makeSpanBetweenPoints(const sad::Rect2D & r, const sad::Point2D & p1, const sad::Point2D & p2)
{
	sad::Rect2D kr(r);
	for(int i = 0; i < 4; i++)
		kr[i].setX(0);

	double distance = p1.distance(p2) / 2;
	kr[0].setX(kr[0].x() - distance );
	kr[2].setX(kr[2].x() + distance );
	kr[1].setX(kr[1].x() + distance );
	kr[3].setX(kr[3].x() - distance );

	this->rotate(this->angle() * -1);
	this->setRenderableArea(kr);
	this->moveTo( (p1 + p2) / 2);
	double angle = atan2(p2.y() - p1.y(), p2.x() - p1.x());
	this->rotate(angle);
}

void sad::Sprite2D::initFromRectangleFast(const sad::Rect2D& rect)
{
	m_angle = 0;

	m_middle = rect[0];
	m_middle += rect[2];
	m_middle /= 2.0;

	m_size.Width = rect[0].distance(rect[1]);
	m_size.Height = rect[0].distance(rect[3]);

	m_renderable_area = rect;
}

void sad::Sprite2D::initFromRectangle(const sad::Rect2D& rect)
{
	bool error = false;
	sad::Rect2D baserect;
	sad::getBaseRect(rect, baserect, m_angle, &error);
	assert(!error);
	
	m_middle = baserect[0];
	m_middle += baserect[2];
	m_middle /= 2.0;

	m_size.Width = baserect[0].distance(baserect[1]);
	m_size.Height = baserect[0].distance(baserect[3]);
}

void sad::Sprite2D::buildRenderableArea()
{
	m_renderable_area = this->area();
	sad::rotate(m_renderable_area, m_angle);
}

void sad::Sprite2D::reloadTexture()
{
	if (m_texture_name.length() && scene())
	{
		if (scene()->renderer())
		{
			sad::Texture * tex = scene()->renderer()->textures()->get(m_texture_name);
			m_texture = tex;
			normalizeTextureCoordinates();
		}
	}	
}

void sad::Sprite2D::normalizeTextureCoordinates()
{
	if (m_texture)
	{
		for(int i = 0; i < 4; i++)
		{
			const sad::Point2D & point = m_texture_coordinates[i];
			sad::Point2D relativepoint(point.x() / m_texture->Width, point.y() / m_texture->Height );
			m_normalized_texture_coordinates[3 - i] = relativepoint;
		}
		if (m_flipx)
		{
			std::swap(m_normalized_texture_coordinates[0], m_normalized_texture_coordinates[1]);
			std::swap(m_normalized_texture_coordinates[2], m_normalized_texture_coordinates[3]);
		}
		if (m_flipy)
		{
			std::swap(m_normalized_texture_coordinates[0], m_normalized_texture_coordinates[3]);
			std::swap(m_normalized_texture_coordinates[1], m_normalized_texture_coordinates[2]);
		}
	}
}
