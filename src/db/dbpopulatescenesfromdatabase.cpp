#include "db/dbpopulatescenesfromdatabase.h"
#include "db/dbdatabase.h"

#include "renderer.h"

#include <algorithm>

void sad::db::populateSceneFromDatabase(sad::Renderer * r, sad::db::Database * db)
{
	sad::db::Table* scenes = db->table("scenes");
	sad::db::Table* scenenodes = db->table("scenenodes");
	if (scenes && scenenodes)
	{
		r->clear();
		// A vector of scenes
		sad::Vector<sad::db::Object *> scenesdata;
		scenes->objects(scenesdata);
		struct
		{
			bool operator()(sad::db::Object* a, sad::db::Object* b) 
			{
				sad::Scene * aa = static_cast<sad::Scene*>(a);
				sad::Scene * bb = static_cast<sad::Scene*>(b);

				return aa->sceneLayer() < bb->sceneLayer();
			}	
		} scene_comparator;
		std::sort(scenesdata.begin(), scenesdata.end(), scene_comparator);
		// A sorted scenes
		for(size_t i = 0; i < scenesdata.size(); i++)
		{
			r->add(static_cast<sad::Scene*>(scenesdata[i]));
		}

		sad::Vector<sad::db::Object *> scenenodesdata;
		scenenodes->objects(scenenodesdata);
		// Fill hash of minor ids to objects
		sad::Hash<unsigned long long, sad::Vector<sad::SceneNode *> > minorids_to_objects;
		for(size_t i = 0; i < scenenodesdata.size(); i++)
		{
			sad::SceneNode* node = static_cast<sad::SceneNode*>(scenenodesdata[i]);
			unsigned long long nodesceneid = node->sceneId();
			if (minorids_to_objects.contains(nodesceneid) == false)
			{
				minorids_to_objects.insert(nodesceneid, sad::Vector<sad::SceneNode*>());				
			}
			minorids_to_objects[nodesceneid] << node;
		}

		// Fill scenes
		struct
		{
			bool operator()(sad::SceneNode* a, sad::SceneNode* b) 
			{				
				return a->cachedLayer() < b->cachedLayer();
			}	
		} scenenodes_comparator;

		for(sad::Hash<unsigned long long, sad::Vector<sad::SceneNode *> >::iterator it = minorids_to_objects.begin();
			it != minorids_to_objects.end();
			++it)
		{
			sad::db::Object * sceneobject = scenes->queryByMinorId(it.key());
			if (sceneobject)
			{
				sad::Vector<sad::SceneNode* > & nodes = it.value();
				std::sort(nodes.begin(), nodes.end(), scenenodes_comparator);
				sad::Scene* scene = static_cast<sad::Scene *>(sceneobject);
				for(size_t i = 0; i < nodes.size(); i++)
				{
					scene->add(nodes[i]);
				}
			}
		}

	}
}
