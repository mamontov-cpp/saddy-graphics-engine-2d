/*! \file   lightmodel.h
	\author HiddenSeeker
	Contains specific light functions, that provided to simply wrap OpenGL
*/
#pragma once

#ifdef   _WIN32
    #include <windows.h>
    #include <gl/gl.h>														
    #include <gl/glu.h>
#else
	#include <GL/gl.h>														
    #include <GL/glu.h>
#endif

#ifndef GL_LIGHT_MODEL_COLOR_CONTROL
	#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
	#define GL_SINGLE_COLOR				 0x81F9
	#define GL_SEPARATE_SPECULAR_COLOR  0x81FA
#endif

namespace sad
{
	/*! Material face
	*/
	enum MaterialFace
	{
		Front=GL_FRONT,
		Back=GL_BACK,
		Both=GL_FRONT_AND_BACK
	};
	/*! Defines a material mode
	*/
	enum MaterialMode
	{
		Emission=GL_EMISSION,
		Ambient=GL_AMBIENT,
		Diffuse=GL_DIFFUSE,
		Specular=GL_SPECULAR,
		DiffuseAmbient=GL_AMBIENT_AND_DIFFUSE,
		Shininess=GL_SHININESS,
		ColorIndexes=GL_COLOR_INDEXES,
	};
	/*! \class LightModel
	     Class, that wraps color materials and common light settings
    */
	class LightModel
	{
	 public:
		     /*! Common light model parameters
			     \sa http://www.opengl.org/sdks/docs/man/xhtml/glLightModel.xml
			 */
			 enum CommonModel
			 { 
				 Ambient=GL_LIGHT_MODEL_AMBIENT,
				 LocalViewer=GL_LIGHT_MODEL_LOCAL_VIEWER,
				 ColorControl=GL_LIGHT_MODEL_COLOR_CONTROL,
				 TwoSide=GL_LIGHT_MODEL_TWO_SIDE
			 };
			 /*! Constants for color control
			 */
			 enum Control
			 {
				 Single=GL_SINGLE_COLOR,
				 Separate=GL_SEPARATE_SPECULAR_COLOR
			 };

		     static inline void enable();  //!< Enables lighting
			 static inline void disable(); //!< Disables lighting
			 /*! Sets an ambient model by a four float parameters
			     \param[in] a1 first parameter
				 \param[in] a2 second parameter
				 \param[in] a3 third parameter
				 \param[in] a4 fourth parameter
			 */
			 static inline void setAmbient(
										   float a1,
										   float a2,
									       float a3,
									       float a4
				                          );
			 /*! Sets a color control light model
			     \param[in] en  parameter
			 */
			 static inline void setControl(sad::LightModel::Control en);
			 /*! Sets a local viewer light model
				 \param[in] f floating-point value, that specifies how reflection angles are computed
			 */
			 static inline void setViewer(float f);
			 /*! Sets a two side light model
			     \param[in] f whether one- or two-sided lighting calc-s are done
			 */
			 static inline void setSides(float f); 
	};
	/*! \class ColorMaterial
	    Specifies various color material settings
	*/
	class ColorMaterial
	{
	 public:
		static inline bool isEnabled(); //!< Returns, whether it's enabled
		static inline void enable();  //!< Enables. By default it's enabled
		static inline void disable(); //!< Disables
		/*! Defines a color material mode
		*/
		enum Mode
		{
			Emission=GL_EMISSION,
			Ambient=GL_AMBIENT,
			Diffuse=GL_DIFFUSE,
			Specular=GL_SPECULAR,
			DiffuseAmbient=GL_AMBIENT_AND_DIFFUSE
		};
		/*! Sets color material. Call before ::enable()
			\param[in] f face
			\param[in] m mode
		*/
		static inline void set(sad::MaterialFace f, sad::ColorMaterial::Mode m);
	};

	/*! \class Material
		\sad http://www.opengl.org/sdk/docs/man/xhtml/glMaterial.xml
		Defines a material, specific for objects. Wraps glMaterial
	*/
	class Material
	{
	 private:
		     float m_adse[4];     //!< Ambient or diffuse or specular or emission or color indexes or shininess parameters
			 MaterialFace m_face; //!< Face
			 MaterialMode m_mode; //!< Mode

			 /*! Creates a material
				 \param[in] f face
				 \param[in] m mode
				 \param[in] a1 parameter
				 \param[in] a2 parameter
				 \param[in] a3 parameter
				 \param[in] a4 parameter
			 */
			 Material(
				      MaterialFace f,
				      MaterialMode m, 
					  float a1=0.0f,
					  float a2=0.0f,
					  float a3=0.0f,
					  float a4=0.0f
					 );
	 public:
		     /*! Ambient material with properties set to 0
			 */
			 Material();
		     /*! Copies a material
			     \param[in] o other material
			 */
		     Material(const Material & o);
		     /*! Applies a material
			     \returns false on error
			 */
			 bool apply();
			 ~Material(); //!< Destructor

			 /*! Creates an ambient material
				 \param[in] f face
				 \param[in] a1 parameter
				 \param[in] a2 parameter
				 \param[in] a3 parameter
				 \param[in] a4 parameter
			 */
			 static Material createAmbient(
				                           sad::MaterialFace f, 
				                           float a1,
										   float a2,
										   float a3,
										   float a4
										  );
			 /*! Creates an diffuse material
				 \param[in] f face
				 \param[in] a1 parameter
				 \param[in] a2 parameter
				 \param[in] a3 parameter
				 \param[in] a4 parameter
			 */
			 static Material createDiffuse(
				                           sad::MaterialFace f, 
				                           float a1,
										   float a2,
										   float a3,
										   float a4
										  );
			 /*! Creates an specular material
				 \param[in] f face
				 \param[in] a1 parameter
				 \param[in] a2 parameter
				 \param[in] a3 parameter
				 \param[in] a4 parameter
			 */
			 static Material createSpecular(
				                           sad::MaterialFace f, 
				                           float a1,
										   float a2,
										   float a3,
										   float a4
										  );
			 /*! Creates an emission material
				 \param[in] f face
				 \param[in] a1 parameter
				 \param[in] a2 parameter
				 \param[in] a3 parameter
				 \param[in] a4 parameter
			 */
			 static Material createEmission(
				                           sad::MaterialFace f, 
				                           float a1,
										   float a2,
										   float a3,
										   float a4
										  );
			 /*! Creates an ambient and diffusion material
				 \param[in] f face
				 \param[in] a1 parameter
				 \param[in] a2 parameter
				 \param[in] a3 parameter
				 \param[in] a4 parameter
			 */
			 static Material createAD     (
				                           sad::MaterialFace f, 
				                           float a1,
										   float a2,
										   float a3,
										   float a4
										  );
			 /*! Creates a  color indexes
				 \param[in] f face
				 \param[in] a1 parameter
				 \param[in] a2 parameter
				 \param[in] a3 parameter
			 */
			 static Material createColorIndexes(
				                                sad::MaterialFace f, 
				                                float a1,
										        float a2,
										        float a3
										        );
			 /*! Creates an shiny material
				 \param[in] f face
				 \param[in] a1 parameter
			 */
			 static Material createShiny(
				                           sad::MaterialFace f, 
				                           float a1
										  );
	};
	/*! \class Light
	    Contains a class of light. Wraps glLight
		\sa http://www.opengl.org/sdks/docs/man/xhtml/glLight.html
	*/
	class Light
	{
	 private:
		      static float m_param[8][4]; //!< Parameters for each light
			  static float m_pos[8][4];   //!< Positions of lights
			  static float m_spot[8][3];  //!< Position of spot direction
	 public:
		      /*! Light modes
			  */
			  enum LightMode
			  {
				  Ambient=GL_AMBIENT,
				  Diffuse=GL_DIFFUSE,
				  Specular=GL_SPECULAR
			  };
		      /*! Light enable (0-7)
			      \param[in] light  to enable
			  */
			  static inline void enable(int light);
			  /*! Light disable
				  \param[in] light to be disabled
			  */
			  static inline void disable(int light);
			  /*! Sets a light
				  \param[in] light light
				  \param[in] type type of light
				  \param[in] a1 parameter
				  \param[in] a2 parameter
				  \param[in] a3 parameter
				  \param[in] a4 parameter
			  */
			  static inline void set(
				                     int light,
									 sad::Light::LightMode en, 
									 float a1,
									 float a2,
									 float a3,
									 float a4
									);
			  /*! Sets a light position in homogenous coordinates
				  \param[in] light light
				  \param[in] a1 x
				  \param[in] a2 y
				  \param[in] a3 z
				  \param[in] a4 w (0,1)
			  */
			  static inline void locate(
				                        int light,
									    float a1,
									    float a2,
									    float a3,
									    float a4
									   );
			  /*! Sets a spot direction
				  \param[in] light light
				  \param[in] a1 x
				  \param[in] a2 y
				  \param[in] a3 z
			  */
			  static inline void spot(
				                        int light,
									    float a1,
									    float a2,
									    float a3
									 );
			  /*! Sets a spot exponent
			      \param[in] light light
				  \param[in] e exponent
			  */
			  static inline void spotExponent(int light,float e);
			  /*! Sets a spot cutoff
			      \param[in] light light
				  \param[in] e exponent
			  */
			  static inline void spotCutoff(int light,float e);
			  /*! Sets a constant attentuation
			      \param[in] light light
				  \param[in] f factor
			  */
			  static inline void AConstant(int light,float f);
			  /*! Sets a linear attentuation
			      \param[in] light light
				  \param[in] f factor
			  */
			  static inline void ALinear(int light,float f);
			  /*! Sets a quadratic attentuation
			      \param[in] light light
				  \param[in] f factor
			  */
			  static inline void AQuadratic(int light,float f);
	};

}


//========================Code goes here=========================
inline void sad::LightModel::enable()  { glEnable(GL_LIGHTING);  }
inline void sad::LightModel::disable() { glDisable(GL_LIGHTING); }
inline void sad::LightModel::setAmbient(float a1, float a2, float a3, float a4)
{ 
	float temp[4]={a1,a2,a3,a4};
	glLightModelfv((GLenum)sad::LightModel::Ambient,(GLfloat*)&(temp[0]));
}

inline void sad::LightModel::setControl(sad::LightModel::Control en)
{
	glLightModeli( (GLenum)sad::LightModel::ColorControl,(int)en );		            
}
inline void sad::LightModel::setViewer(float f)
{
	glLightModelf( (GLenum)sad::LightModel::LocalViewer,f );
}
inline void sad::LightModel::setSides(float f)
{
	glLightModelf( (GLenum)sad::LightModel::TwoSide, f );
}
inline bool sad::ColorMaterial::isEnabled()
{
	return glIsEnabled(GL_COLOR_MATERIAL)==GL_TRUE;
}
inline void sad::ColorMaterial::enable()
{
	glEnable(GL_COLOR_MATERIAL);
}
inline void sad::ColorMaterial::disable()
{
	glDisable(GL_COLOR_MATERIAL);
}
inline void sad::ColorMaterial::set(sad::MaterialFace f, sad::ColorMaterial::Mode m)
{
	glColorMaterial((GLenum)f,(GLenum)m);
}

inline void sad::Light::enable(int light)
{
	glEnable(GL_LIGHT0+light);
}

inline void sad::Light::disable(int light)
{
	glDisable(GL_LIGHT0+light);
}

inline void sad::Light::set(
				            int light,
						    sad::Light::LightMode en, 
							float a1,
							float a2,
							float a3,
							float a4
						   )
{
	m_param[light][0]=a1;
	m_param[light][1]=a2;
	m_param[light][2]=a3;
	m_param[light][3]=a4;
	glLightfv(GL_LIGHT0+light,(GLenum)en,(GLfloat *)&(m_param[light][0]));
}

inline void sad::Light::locate(
							   int light, 
							   float a1,
							   float a2,
							   float a3,
							   float a4
						      )
{
	m_pos[light][0]=a1;
	m_pos[light][1]=a2;
	m_pos[light][2]=a3;
	m_pos[light][3]=a4;
	glLightfv(GL_LIGHT0+light,GL_POSITION,(GLfloat *)&(m_pos[light][0]));
}
inline void sad::Light::spot(
				             int light,
							 float a1,
							 float a2,
							 float a3
							)
{
    m_spot[light][0]=a1;
	m_spot[light][1]=a2;
	m_spot[light][2]=a3;
	glLightfv(GL_LIGHT0+light,GL_SPOT_DIRECTION,(GLfloat *)&(m_spot[light][0]));
}
inline void sad::Light::spotExponent(int light, float e)
{
	glLightf(GL_LIGHT0+light,GL_SPOT_EXPONENT,e);
}
inline void sad::Light::spotCutoff(int light, float e)
{
	glLightf(GL_LIGHT0+light,GL_SPOT_CUTOFF,e);
}
inline void sad::Light::AConstant(int light, float e)
{
	glLightf(GL_LIGHT0+light,GL_CONSTANT_ATTENUATION,e);
}
inline void sad::Light::ALinear(int light, float e)
{
	glLightf(GL_LIGHT0+light,GL_LINEAR_ATTENUATION,e);
}
inline void sad::Light::AQuadratic(int light, float e)
{
	glLightf(GL_LIGHT0+light,GL_QUADRATIC_ATTENUATION,e);
}
