#include "lightmodel.h"

using namespace sad;

Material::Material()
{
	m_face=Both;
	m_mode=Ambient;
	m_adse[0]=m_adse[1]=m_adse[2]=m_adse[3]=0.0f;
}
Material::Material(const Material & o)
{
	m_face=o.m_face;
	m_mode=o.m_mode;
	m_adse[0]=o.m_adse[0];
	m_adse[1]=o.m_adse[1];
	m_adse[2]=o.m_adse[2];
	m_adse[3]=o.m_adse[3];
}
Material::~Material()
{

}
Material::Material(
				      MaterialFace f,
				      MaterialMode m, 
					  float a1,
					  float a2,
					  float a3,
					  float a4
				   )
{
	m_face=f;
	m_mode=m;
	m_adse[0]=a1;
	m_adse[1]=a2;
	m_adse[2]=a3;
	m_adse[3]=a4;
}

bool Material::apply()
{
  glMaterialfv((GLenum)m_face,(GLenum)m_mode,(GLfloat*)&(m_adse[0]));
  return glGetError()==0;
}

Material Material::createAmbient(
								 sad::MaterialFace f, 
								 float a1,
								 float a2,
								 float a3,
								 float a4
								)
{
	return Material(f,Ambient,a1,a2,a3,a4);
}

Material Material::createDiffuse(
								 sad::MaterialFace f, 
								 float a1,
								 float a2,
								 float a3,
								 float a4
								)
{
	return Material(f,Diffuse,a1,a2,a3,a4);
}

Material Material::createSpecular(
								 sad::MaterialFace f, 
								 float a1,
								 float a2,
								 float a3,
								 float a4
								)
{
	return Material(f,Specular,a1,a2,a3,a4);
}
Material Material::createEmission(
								 sad::MaterialFace f, 
								 float a1,
								 float a2,
								 float a3,
								 float a4
								)
{
	return Material(f,Emission,a1,a2,a3,a4);
}

Material Material::createAD     (
								 sad::MaterialFace f, 
								 float a1,
								 float a2,
								 float a3,
								 float a4
								)
{
	return Material(f,DiffuseAmbient,a1,a2,a3,a4);
}
Material Material::createColorIndexes(
									  sad::MaterialFace f, 
									  float a1,
									  float a2,
									  float a3
								     )
{
	return Material(f,ColorIndexes,a1,a2,a3);
}
Material Material::createShiny(sad::MaterialFace f, float a1)
{
	return Material(f,Shininess,a1);
}

float Light::m_param[8][4];
float Light::m_pos[8][4];
float Light::m_spot[8][3];