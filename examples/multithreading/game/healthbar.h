/*! \file healthbar.h
 *
 *  A simple health bar for showing a health of player
 */
#pragma once
#include <sprite2d.h>
#include <label.h>

class Game;

namespace game
{
/*! A simple health bar data
 */
class HealthBar: public sad::SceneNode
{
SAD_OBJECT
public:
    /*! Top left angle for bar
     */
    static const sad::Point2D TopLeftAngle;
    /*! Heart size
     */
    static const sad::Point2D HeartSize;
    /*! Padding between bars
     */
    static const double Padding;
    /*! Constructs new health bar
     *  \param[in] game a game
     */
    HealthBar(Game* game);
    /*! Can be inherited
     */
    virtual ~HealthBar() override;
    /*! Sets tree name for health bar
     *  \param[in] renderer a renderer
     *  \param[in] tree_name a tree name
     */
    virtual void setTreeName(sad::Renderer* renderer, const sad::String& tree_name = "") override;
    /*! Sets scene for health bar
     */
    virtual void setScene(sad::Scene* scene) override;
    /*! Triggers changed renderer for bar
     */
    virtual void rendererChanged() override;
    /*! Renders health bar, setting proper object flags
     */
    virtual void render() override;
protected:
    /*! Makes new game
     */
    Game* m_game;
    /*! A heart sprite data
     */
    sad::Sprite2D* m_heart;
    /*! A multiplier label
     */
    sad::Label* m_multiplier_label;
    /*! A life label data
     */
    sad::Label* m_life_label;
    /*! A gui scene;
     */
    sad::Scene* m_scene;
    /*! Multiplier label point
     */
    sad::Point2D m_multiplier_label_point;
    /*! A life label point
     */
    sad::Point2D m_life_label_point;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(game::HealthBar);
