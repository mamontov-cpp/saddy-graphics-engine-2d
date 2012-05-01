/*! \file   sprite2dconfigloader.h
	\author HiddenSeeker
	
	An abstract loader for config for 2d sprite
 */
#include "../templates/hhash.hpp"
#include "sprite2dtemplate.h"
#pragma once

/*! Inner container of sprites in config 
	(string - is unique id of sprite group, int - an index of sprite in group)
 */
typedef hst::hash<hst::string, 
				  hst::hash<int,Sprite2DTemplate> 
				 >  Sprite2DTemplateContainer;

/*! \class Sprite2DConfigLoader
	
	A loader to load 2d config
 */
class Sprite2DConfigLoader
{
 protected:
			/*! Inserts new template into container
				 \param[in]  group      sprite group
				 \param[in]  index      sprite index in group
				 \param[in]  stemplate  sprite template
				 \param[out] container  container where it should be inserted
			 */ 
		    void insertTemplate(const hst::string & group, 
						    	int index,
								const Sprite2DTemplate & stemplate,
								Sprite2DTemplateContainer & container);
 public:
		/*! A function for loading a config. For loading a config, you must fill a container
			with appropriate data and return true. If some errors occured, function must return false
			\param[out] container container to be filled
			\return     whether loading was correct
		 */
	    virtual bool load(Sprite2DTemplateContainer & container)=0;
		/*! Destructor to fill it with data
		 */
		virtual ~Sprite2DConfigLoader();
};
