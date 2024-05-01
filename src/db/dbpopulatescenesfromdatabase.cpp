#include "db/dbpopulatescenesfromdatabase.h"
#include "db/dbdatabase.h"

#include "renderer.h"

#include <algorithm>
#include "opticksupport.h"

struct scene_comparator_t
{
    bool operator()(sad::db::Object* a, sad::db::Object* b) 
    {
        PROFILER_EVENT;
        sad::Scene * aa = static_cast<sad::Scene*>(a);
        sad::Scene * bb = static_cast<sad::Scene*>(b);

        return aa->sceneLayer() < bb->sceneLayer();
    }
};

struct scenenodes_comparator_t
{
    bool operator()(sad::SceneNode* a, sad::SceneNode* b) 
    {
        return a->cachedLayer() < b->cachedLayer();
    }
};

void sad::db::populateScenesFromDatabase(sad::Renderer * r, sad::db::Database * db, bool clear)
{
    PROFILER_EVENT;
    sad::db::Table* scenes = db->table("scenes");
    sad::db::Table* scenenodes = db->table("scenenodes");
    if (scenes && scenenodes)
    {
        if (clear)
        {
            r->clear();
        }
        // A vector of scenes
        sad::Vector<sad::db::Object *> scenesdata;
        scenes->objects(scenesdata);
        for(size_t i = 0; i < scenesdata.size(); i++)
        {
            if (scenesdata[i]->isInstanceOf("sad::Scene") == false)
            {
                scenesdata.removeAt(i);
                --i;
            }
        }
        scene_comparator_t scene_comparator;
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
            if (scenenodesdata[i]->isInstanceOf("sad::SceneNode"))
            {
                sad::SceneNode* node = static_cast<sad::SceneNode*>(scenenodesdata[i]);
                unsigned long long nodesceneid = node->sceneId();
                if (minorids_to_objects.contains(nodesceneid) == false)
                {
                    minorids_to_objects.insert(nodesceneid, sad::Vector<sad::SceneNode*>());				
                }
                minorids_to_objects[nodesceneid] << node;
            }
        }

        // Fill scenes
        scenenodes_comparator_t scenenodes_comparator;

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
