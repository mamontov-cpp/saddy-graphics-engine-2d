/*! \file platformblinking.h
 *
 *  Makes it possible for platform to disappear and appear again
 */
#pragma once
#include <sprite2d.h>
#include <p2d/body.h>
#include <functional>

class Game;

namespace game
{

/*! A platform blinking for platform
 */
class PlatformBlinking
{
public:
    /*! Constructs new process
     * \param[in] game a game
     * \param[in] platform an inner platform
     * \param[in] time a time for blinking
     */
    PlatformBlinking(Game* game, sad::Sprite2D* platform, double time);
    /*! Destroys blinking platform
     */
    ~PlatformBlinking();
    /*! Disables blinking, making platform re-appear
     */
    void disable();
    /*! Enables blinking
     */
    void enable();
    /*! Sets time before blinking a platform
     *  \param[in] time a time
     */
    void setTime(double time);
private:
    /*! Shows platform
     */
    void showPlatform() const;
    /*! Hides platform
     */
    void hidePlatform() const;
    /*! A game part
     */
    Game* m_game;
    /*! A time between transition shift
     */
    double m_time;
    /*! Whether platform is hidden
     */
    bool m_hidden;
    /*! Whether blinking is enabled
     */
    bool m_enabled;
    /*! A platform part
     */
    sad::Sprite2D* m_platform;
    /*! A body for platform
     */
    sad::p2d::Body* m_body;
    /*! An enabling callback
     */
    std::function<void()> m_hide_callback;
    /*! A disabling callback
     */
    std::function<void()> m_show_callback;
};


}
