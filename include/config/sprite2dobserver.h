/*! \file   sprite2dobserver.h
	\author HiddenSeeker
	
	An observer, which can work with config for 2d sprites is placed here
 */
#pragma once
#include "sprite2dtemplate.h"

#include "../sprite2dcontroller.h"
#include "../templates/hstring.h"


namespace sad
{
/* A config for sprites 
 */
class Sprite2DConfig;

/*! A policy for observer config, what it should do, 
	when size of sprite in config is changed
 */
enum Sprite2DConfigObserverPolicy
{
	COP_CHANGESIZE,           //!< Observer should change size of sprite to config size
	COP_CHANGEIFNOTCHANGED,   //!< Observer should change size, only if his size is not changed, otherwise keep
	                          //   it to custom size
    COP_KEEPSIZE              //!< Keep size of sprite anyway
};

/*! \class Sprite2DConfigObserver
	A config observer, which has a sprite, attached to it
 */
class Sprite2DConfigObserver
{
 private:
		 Sprite2DConfig *             m_config;    //!< Attached sprite config
		 Sprite2DConfigObserverPolicy m_policy;    //!< How observer should act
		 hst::string       m_spritegroup; //!< Sprite Group index in the config
		 int               m_index;     //!< Sprite index in config
		 
		 Sprite2DController * m_sprite;  //!< Sprite, which we are working on 
 public:
		 /*! Constructs a new observer. It does not creates a sprite, so you must create it
			 manually, using createSprite. But observer registers in a config. as observer at
			 creation.
			\param[in] spritegroup name of sprite group
			\param[in] index   index of sprites
			\param[in] config  reference to config
			\param[in] policy  policy, how observer should work with setting size
		  */
		 Sprite2DConfigObserver(const hst::string & spritegroup, int index, 
								Sprite2DConfig * config,
								Sprite2DConfigObserverPolicy policy = COP_CHANGESIZE
							   );
		 /*! Creates a new sprite, with center in specified position
			 The new sprite will not be flipped on any of axis and has a rotation angle of 0
			 \param[in] center sprite center point.
			 \return true if succeeded, false if data in config is not found
		  */
		 bool createSprite(const hPointF & center);
		 /*! Checks, whether sprite can be updated from container. It can update if sprite group and index
			 are presented in container
			 \param[in] container container, from which we can find an object
			 \return true, if can
		  */
		 bool canBeUpdatedFrom(const Sprite2DTemplateContainer & container);
		 /*! Updates a sprite from config, using policy
		  */
		 void notifyConfigChanged();
		 /*! Returns current associated sprite
			 \return sprite
		  */ 
		 inline Sprite2DController * sprite() const {return m_sprite; }
		 /*! Destructor. Removes a sprite if have one and unregisters self from config.
		  */
		 ~Sprite2DConfigObserver();
		 /*! Notifies observer, that config is gone
          */
		 inline void notifyConfigGone() { m_config = NULL; }
};

}
