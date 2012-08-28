/*! \file spritegameobject.h
	\author HiddenSeeker

	An sprite-based game object, based from sprite
 */
#include "abstractgameobject.h"
#include <config/sprite2dconfig.h>
#include <config/sprite2dobserver.h>


/*! \class SpriteGameObject
	
	A sprite-based game object
 */
class SpriteGameObject: public AbstractGameObject
{
 private:
		 Sprite2DConfigObserver * m_observer; //!< A config observer
 public:
		 /*! Constructs a new observer. It does not create sprite
			\param[in] spritegroup name of sprite group
			\param[in] index   index of sprites
			\param[in] config  reference to config
			\param[in] policy  policy, how observer should work with setting size
		  */
		 SpriteGameObject(const hst::string & spritegroup, int index, 
						  Sprite2DConfig * config,
						  Sprite2DConfigObserverPolicy policy = COP_CHANGESIZE
						 );
		 /*! Creates a sprite
			 \param[in] p center of sprite
		  */
		 bool createSprite(const hPointF & p);
		 /*! Returns a sprite of object
			 \return sprite of object
		  */
		 inline Sprite2DController * sprite() { return m_observer->sprite(); }
		 /*! Adds an object to scene. Adds it to container to scene.
			 \param[in] scene scene
		 */
		 //virtual void addToScene(InterlockedScene * scene);
		 /*! Removes an object from scene
		  */
		 //virtual void removeFromScene();
		 /*! Destructs an object
		  */
		 virtual ~SpriteGameObject();
};


/*! Constructs a new observer
    \param[in] spritegroup name of sprite group
	\param[in] index   index of sprites
	\param[in] p       center of point
	\param[in] policy  policy, how observer should work with setting size
	\return NULL if can't create
*/
SpriteGameObject * createSpriteGameObject(const hst::string & spritegroup, int index, 
										  const hPointF & p,
										  Sprite2DConfigObserverPolicy policy = COP_CHANGESIZE
										 );



/*!
 * A sprite config
 */
extern Sprite2DConfig   * GlobalSpritesConfig;
