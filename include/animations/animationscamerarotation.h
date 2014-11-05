/*! \file animationscamerarotation.h
	\author HiddenSeeker

	Describes an animation for editing a camera rotation
 */
#pragma once
#include "animationsanimation.h"
#include "animationsanimationfastcall.h"

#include "../sadpoint.h"
#include "../scene.h"


namespace sad
{
	
namespace animations
{

/*! An animation of rotating the camera
 */
class CameraRotation: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! An abstract fast call for rotating camera
	 */
	class AbstractFastCall: public sad::animations::AnimationFastCall
	{
	public: 
		/*! Applies a rotation to camera
			\param[in] angle angle for rotation
		 */
		virtual void call(double angle) = 0;
		/*! Must be inherited
		 */
		virtual ~AbstractFastCall();
	};

	/*! A dummy fast call. Does nothing
	 */
	class DummyFastCall: public sad::animations::CameraRotation::AbstractFastCall
	{
	public: 
		/*! Does nothing
			\param[in] angle angle for rotation
		 */
		virtual void call(double angle);
		/*! Must be inherited
		 */
		virtual ~DummyFastCall();
	};

	/*! A real fast call for scene
	 */
	class FastCall: public  sad::animations::CameraRotation::AbstractFastCall
	{
	public: 
		/*! Sets a scene and pivot for translation
			\param[in] s scene
			\param[in] p a pivot offset for rotation
		 */
		FastCall(sad::Scene* s, const sad::Point3D& p);
		/*! Applies rotation and translation
			\param[in] angle angle for rotation
		 */
		virtual void call(double angle);
		/*! Must be inherited
		 */
		virtual ~FastCall();
	protected:
		/*! A pivot for translation
		 */
		sad::Point3D m_pivot;
		/*! A sceme for rotating a camera
		 */
		sad::Scene* m_scene;
	};



	/*! Creates new empty animation
	 */
	CameraRotation();
	/*! Can be inherited
	 */
	virtual ~CameraRotation();
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
	/*! Set minimal angle for camera rotation
		\param[in] angle an angle for rotation
	 */
	void setMinAngle(double angle);
	/*! Returns minimal angle
		\return minimal angle for rotation
	 */
	double minAngle() const;
	/*! Set maximal angle for camera rotation
		\param[in] angle an angle for rotation
	 */
	void setMaxAngle(double angle);
	/*! Returns maximal angle
		\return maximal angle for rotation
	 */
	double maxAngle() const;
	/*! Sets a pivot point for rotation
		\param[in] p pivot
	 */
	void setPivot(const sad::Point3D& p);
	/*! Returns a pivot point for rotation
		\return pivot point
	 */
	const sad::Point3D& pivot() const;
	/*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time);
    /*! Saves states of object in animation instance
        \param[in] i an animation instance
        \return whether we can work further with this object in instance
     */
    virtual bool saveState(sad::animations::Instance* i);
    /*! Resets state of object in animation instance, when animation ended
        \param[in] i an animation instance
     */
    virtual void resetState(sad::animations::Instance* i);
protected:
	/*! A minimal angle for editing
	 */
	double m_min_angle;
	/*! A maximal angle for editing
	 */
	double m_max_angle;
	/*! A pivot point for rotation
	 */
	sad::Point3D m_pivot;
};

}

}

