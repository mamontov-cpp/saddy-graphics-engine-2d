#include "initphysics.h"

#include <p2d/multisamplingcollisiondetector.h>

#include <sprite2d.h>

// A precision error for designer's editor when designing level
#define DESIGNER_PRECISION_ERROR  (2.0)

void initPhysicsWorld(sad::p2d::World* world)
{
    world->setDetector(new sad::p2d::MultisamplingCollisionDetector(2.0));
    world->addRef();
    world->addGroup("player");
    world->addGroup("enemies");
    world->addGroup("platforms");
    world->addGroup("walls");
    world->addGroup("coins");
    world->addGroup("player_bullets");
    world->addGroup("enemy_bullets");
}


void initPhysicsPlatforms(sad::p2d::World* world, sad::Scene* main_scene, game::MovingPlatformRegistry* registry)
{
    const sad::Vector<sad::SceneNode*>&  nodes = main_scene->objects();
    // Filter grouped and ungrouped platforms
    sad::Vector<sad::Sprite2D*> platform_sprites;
    sad::Vector<sad::Sprite2D*> ungrouped_platform_sprites;
    for (size_t i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->isInstanceOf("sad::Sprite2D"))
        {
            sad::String name = nodes[i]->objectName();
            name.toUpper();
            if ((name.getOccurence("FLOOR") != -1) || (name.getOccurence("PLATFORM") != -1))
            {
                sad::Sprite2D* node = dynamic_cast<sad::Sprite2D*>(nodes[i]);
                if (node)
                {
                    if (name.getOccurence("MOVING") != -1)
                    {
                        ungrouped_platform_sprites << node;
                    }
                    else
                    {
                        platform_sprites << node;
                    }
                }
            }
        }
    }
    // Add unfiltered platforms to group
    for (size_t i = 0; i < ungrouped_platform_sprites.size(); i++)
    {
        sad::p2d::Body* body = new sad::p2d::Body();
        body->setCurrentAngularVelocity(0);
        body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
        body->attachObject(ungrouped_platform_sprites[i]);
        sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
        sad::Rect2D rct = ungrouped_platform_sprites[i]->area();
        // Slight increase of area, due to paddings
        rct = sad::Rect2D(rct[0].x(), rct[0].y(), rct[2].x() + 1, rct[2].y());
        rect->setRect(rct);
        body->setShape(rect);
        body->initPosition(ungrouped_platform_sprites[i]->middle());
        registry->addPlatform(ungrouped_platform_sprites[i]->objectName(), body);

        world->addBodyToGroup("platforms", body);
    }
    // Sort sprites according from left to right
    std::sort(platform_sprites.begin(), platform_sprites.end(), [](sad::Sprite2D* a, sad::Sprite2D* b) {
        return a->area()[0].x() < b->area()[0].x();
    });
    // Group some platforms horizontally
    for (size_t i = 0; i < platform_sprites.size(); i++)
    {
        sad::Vector<sad::Sprite2D*> sprites_in_group;
        sprites_in_group << platform_sprites[i];
        sad::Rect2D common_rectangle = platform_sprites[i]->area();
        for (size_t j = i + 1; j < platform_sprites.size(); j++)
        {
            sad::Rect2D candidate_rectangle = platform_sprites[j]->area();
            double common_ymin = std::min(common_rectangle.p0().y(), common_rectangle.p2().y());
            double common_ymax = std::max(common_rectangle.p0().y(), common_rectangle.p2().y());

            double candidate_ymin = std::min(candidate_rectangle.p0().y(), candidate_rectangle.p2().y());
            double candidate_ymax = std::max(candidate_rectangle.p0().y(), candidate_rectangle.p2().y());

            double common_xmin = std::min(common_rectangle.p0().x(), common_rectangle.p2().x());
            double common_xmax = std::max(common_rectangle.p0().x(), common_rectangle.p2().x());

            double candidate_xmin = std::min(candidate_rectangle.p0().x(), candidate_rectangle.p2().x());
            double candidate_xmax = std::max(candidate_rectangle.p0().x(), candidate_rectangle.p2().x());

            if (sad::is_fuzzy_equal(common_ymin, candidate_ymin, DESIGNER_PRECISION_ERROR)
                && sad::is_fuzzy_equal(common_ymax, candidate_ymax, DESIGNER_PRECISION_ERROR)
                && sad::is_fuzzy_equal(common_xmax, candidate_xmin, DESIGNER_PRECISION_ERROR))
            {
                sprites_in_group << platform_sprites[j];
                common_rectangle = sad::Rect2D(common_xmin, common_ymin, candidate_xmax, common_ymax);
            }
        }
        if (sprites_in_group.size() > 1)
        {
            sad::p2d::Body* body = new sad::p2d::Body();
            body->setCurrentAngularVelocity(0);
            body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
            sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
            // Slight increase of area, due to paddings
            common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y(), common_rectangle[2].x() + 1, common_rectangle[2].y());
            // Small fix for tiny platforms, to ensure their bounding box will be some times smaller than one
            if (common_rectangle.height() < 40) {
                common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y() + 3, common_rectangle[2].x() - 1, common_rectangle[2].y());
            }
            rect->setRect(common_rectangle);
            body->setShape(rect);
            body->attachObjects(sprites_in_group);
            body->initPosition((common_rectangle[0] + common_rectangle[2]) / 2.0);

            world->addBodyToGroup("platforms", body);
            for (size_t k = 0; k < sprites_in_group.size(); k++)
            {
                platform_sprites.removeFirst(sprites_in_group[k]);
            }
            // Decrement, so we can iterate through other platforms
            --i;
        }
    }
    // We don't group platforms vertically except it's columns, because it could be solved by bounce solver easily
    std::sort(platform_sprites.begin(), platform_sprites.end(), [](sad::Sprite2D* a, sad::Sprite2D* b) {
        return a->area()[0].y() < b->area()[0].y();
    });
    // Group some platforms vertically
    for (size_t i = 0; i < platform_sprites.size(); i++)
    {
        sad::Vector<sad::Sprite2D*> sprites_in_group;
        sprites_in_group << platform_sprites[i];
        sad::Rect2D common_rectangle = platform_sprites[i]->area();
        for (size_t j = i + 1; j < platform_sprites.size(); j++)
        {
            sad::Rect2D candidate_rectangle = platform_sprites[j]->area();
            double common_ymin = std::min(common_rectangle.p0().y(), common_rectangle.p2().y());
            double common_ymax = std::max(common_rectangle.p0().y(), common_rectangle.p2().y());

            double candidate_ymin = std::min(candidate_rectangle.p0().y(), candidate_rectangle.p2().y());
            double candidate_ymax = std::max(candidate_rectangle.p0().y(), candidate_rectangle.p2().y());

            double common_xmin = std::min(common_rectangle.p0().x(), common_rectangle.p2().x());
            double common_xmax = std::max(common_rectangle.p0().x(), common_rectangle.p2().x());

            double candidate_xmin = std::min(candidate_rectangle.p0().x(), candidate_rectangle.p2().x());
            double candidate_xmax = std::max(candidate_rectangle.p0().x(), candidate_rectangle.p2().x());

            if (sad::is_fuzzy_equal(common_xmin, candidate_xmin, DESIGNER_PRECISION_ERROR)
                && sad::is_fuzzy_equal(common_xmax, candidate_xmax, DESIGNER_PRECISION_ERROR)
                && sad::is_fuzzy_equal(common_ymax, candidate_ymin, DESIGNER_PRECISION_ERROR))
            {
                sprites_in_group << platform_sprites[j];
                common_rectangle = sad::Rect2D(common_xmin, common_ymin, common_xmax, candidate_ymax);
            }
        }
        if (sprites_in_group.size() > 1)
        {
            sad::p2d::Body* body = new sad::p2d::Body();
            body->setCurrentAngularVelocity(0);
            body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
            sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
            // Slight increase of area, due to paddings
            common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y(), common_rectangle[2].x() + 1, common_rectangle[2].y());
            rect->setRect(common_rectangle);
            body->setShape(rect);
            body->attachObjects(sprites_in_group);
            body->initPosition((common_rectangle[0] + common_rectangle[2]) / 2.0);


            world->addBodyToGroup("platforms", body);
            for (size_t k = 0; k < sprites_in_group.size(); k++)
            {
                platform_sprites.removeFirst(sprites_in_group[k]);
            }
            // Decrement, so we can iterate through other platforms
            --i;
        }
    }
    // Add remaining platforms to world
    for (size_t i = 0; i < platform_sprites.size(); i++)
    {
        sad::p2d::Body* body = new sad::p2d::Body();
        body->setCurrentAngularVelocity(0);
        body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
        body->attachObject(platform_sprites[i]);
        sad::Rect2D common_rectangle = platform_sprites[i]->area();
        sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
        // Slight increase of area, due to paddings
        common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y(), common_rectangle[2].x() + 1, common_rectangle[2].y());
        // Small fix for tiny platforms, to ensure their bounding box will be some times smaller than one
        if (common_rectangle.height() < 40) {
            common_rectangle = sad::Rect2D(common_rectangle[0].x(), common_rectangle[0].y() + 3, common_rectangle[2].x() - 1, common_rectangle[2].y());
        }
        rect->setRect(common_rectangle);
        body->setShape(rect);
        body->initPosition(platform_sprites[i]->middle());

        world->addBodyToGroup("platforms", body);
    }
}

void initCoins(sad::p2d::World* world, sad::db::Database* db, sad::Renderer* r, game::UnanimatedCoins* coins)
{
    sad::Vector<sad::Sprite2D*> coin_sprites = game::UnanimatedCoins::fetchCoinSprites(db);
    coins->init(coin_sprites, db, r);
    for (size_t i = 0; i < coin_sprites.size(); i++)
    {
        sad::p2d::Body* body = new sad::p2d::Body();
        body->setCurrentAngularVelocity(0);
        body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
        body->attachObject(coin_sprites[i]);

        sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
        rect->setRect(coin_sprites[i]->area());
        body->setShape(rect);
        body->initPosition(coin_sprites[i]->middle());

        world->addBodyToGroup("coins", body);
    }
}
