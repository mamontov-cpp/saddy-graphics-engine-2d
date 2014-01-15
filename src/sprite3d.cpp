#include "../include/sprite3d.h"
#include <math.h>
#include <renderer.h>
#include <texturemanager.h>
#include "os/glheaders.h"

DECLARE_SOBJ_INHERITANCE(sad::Sprite3D,sad::SceneNode)

sad::Sprite3D::Sprite3D()
: 
m_flipx(false),
m_flipy(false),
m_alpha(0),
m_theta(0),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(0,0,0,0),
m_area(sad::Point3D(0, 0), sad::Point3D(0, 0)),
m_texture(NULL),
m_color(sad::AColor(255,255,255,0))
{

}

sad::Sprite3D::Sprite3D(		
	sad::Texture * texture,
	const sad::Rect2D & texturecoordinates,
	sad::Rect<sad::Point3D> & area	
)
: 
m_flipx(false),
m_flipy(false),
m_alpha(0),
m_theta(0),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(texturecoordinates),
m_area(area),
m_texture(texture),
m_color(sad::AColor(255,255,255,0))
{
	normalizeTextureCoordinates();
	buildRenderableArea();
}

sad::Sprite3D::Sprite3D(
		const sad::String& texture,
		const sad::Rect2D& texturecoordinates,
		sad::Rect<sad::Point3D>& area		
	)
: 
m_texture_name(texture),
m_flipx(false),
m_flipy(false),
m_alpha(0),
m_theta(0),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(texturecoordinates),
m_area(area),
m_texture(NULL),
m_color(sad::AColor(255,255,255,0))
{
	normalizeTextureCoordinates();
}

sad::Sprite3D::~Sprite3D()
{

}

void sad::Sprite3D::render()
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
	  glVertex3f(m_renderable_area[i].x(),m_renderable_area[i].y(),m_renderable_area[i].z());
   }  
   glEnd();
   glColor4iv(m_current_color_buffer);

}

void sad::Sprite3D::setTextureCoordinates(const sad::Rect2D & texturecoordinates)
{
	m_texture_coordinates = texturecoordinates;
	normalizeTextureCoordinates();
}

void sad::Sprite3D::setTextureCoordinate(int index, const sad::Point2D & point)
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

void sad::Sprite3D::setTextureCoordinate(int index, double x ,double y)
{
	this->setTextureCoordinate(index, sad::Point2D(x, y));
}


const sad::Rect2D & sad::Sprite3D::textureCoordinates() const
{
	return m_texture_coordinates;
}

void sad::Sprite3D::setArea(const sad::Rect<sad::Point3D> & rect)
{
	m_area = rect;
	buildRenderableArea();
}

const sad::Rect<sad::Point3D> & sad::Sprite3D::area() const
{
	return m_area;
}

const sad::Rect<sad::Point3D> & sad::Sprite3D::renderableArea() const
{
	return m_renderable_area;
}

const sad::Point3D & sad::Sprite3D::point(int n) const
{
	return m_area[n];
}

void sad::Sprite3D::setPoint(int index, const sad::Point3D & p)
{
	m_area[index] = p;
	sad::Point3D  pivot = middle();
	sad::Point3D  dist = p - pivot;
	sad::Point3D  result= dist;
  
	result.setX(dist.x()*cos(m_alpha)-dist.y()*sin(m_alpha));
	result.setY(dist.x()*sin(m_alpha)*cos(m_theta)
			   +dist.y()*cos(m_alpha)*cos(m_theta)
			   -dist.z()*sin(m_theta)
			   );
	result.setZ(dist.x()*sin(m_alpha)*sin(m_theta)
			   +dist.y()*cos(m_alpha)*sin(m_theta)
			   +dist.z()*cos(m_theta)
			   );
    
  m_renderable_area[index] = result + pivot;
}

sad::Point3D sad::Sprite3D::middle() const
{
	sad::Point3D result = m_area[0];
	result += m_area[1];
	result += m_area[2];
	result += m_area[3];
	result /= 4.0;
	return result;
}

void sad::Sprite3D::moveBy(const sad::Point3D & dist)
{
	for (int i=0;i<4;i++)
	{
		m_area[i] += dist;
		m_renderable_area[i] += dist;
	}
}

void sad::Sprite3D::moveTo(const sad::Point3D & p)
{
	sad::Point3D  dist = p;
	dist -= middle();
	moveBy(dist);
}

void sad::Sprite3D::rotate(double alpha, double theta)
{
	m_alpha += alpha;
	m_theta += theta;
	buildRenderableArea();
}

void sad::Sprite3D::setAlpha(double alpha)
{
	m_alpha = alpha;
	buildRenderableArea();
}

void sad::Sprite3D::setTheta(double theta)
{
	m_theta = theta;
	buildRenderableArea();
}

double sad::Sprite3D::alpha() const
{
	return m_alpha;
}

double sad::Sprite3D::theta() const
{
	return m_theta;
}

void sad::Sprite3D::setColor(const sad::AColor & clr) 
{
	m_color = clr;
}

const sad::AColor & sad::Sprite3D::color() const
{
	return m_color;
}

void sad::Sprite3D::setFlipX(bool flipx)
{
	bool changed = m_flipx != flipx;
	m_flipx = flipx;
	if (changed)
	{
		normalizeTextureCoordinates();
	}
}

void sad::Sprite3D::setFlipY(bool flipy)
{
	bool changed = m_flipy != flipy;
	m_flipy = flipy;
	if (changed)
	{
		normalizeTextureCoordinates();
	}
}

bool sad::Sprite3D::flipX() const
{
	return m_flipx;
}

bool sad::Sprite3D::flipY() const
{
	return m_flipy;
}

void sad::Sprite3D::setTexture(sad::Texture * texture)
{
	m_texture = texture;
	normalizeTextureCoordinates();
}

sad::Texture * sad::Sprite3D::texture() const
{
	return m_texture;
}

void sad::Sprite3D::setTexureName(const sad::String & name)
{
	m_texture_name = name;
	reloadTexture();
}

const sad::String& sad::Sprite3D::textureName()
{
	return m_texture_name;
}

void sad::Sprite3D::setScene(sad::Scene * scene)
{
	this->sad::SceneNode::setScene(scene);
	if (m_texture_name.length() != 0)
	{
		reloadTexture();
	}
}

void sad::Sprite3D::buildRenderableArea()
{
	sad::Point3D pivot = middle();
	for(int i = 0; i < 4; i++)
	{
		sad::Point3D dist = m_area[i] - pivot;
		sad::Point3D result=dist;
  
		result.setX(dist.x() * cos(m_alpha) - dist.y() * sin(m_alpha));
		result.setY(dist.x() * sin(m_alpha) * cos(m_theta)
				   +dist.y() * cos(m_alpha) * cos(m_theta)
				   -dist.z() * sin(m_theta));
		result.setZ(dist.x() * sin(m_alpha) * sin(m_theta)
				   +dist.y() * cos(m_alpha) * sin(m_theta)
				   +dist.z() * cos(m_theta));
    
		m_renderable_area[i] = result + pivot;
	}
}

void sad::Sprite3D::reloadTexture()
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

void sad::Sprite3D::normalizeTextureCoordinates()
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
