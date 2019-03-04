/*! \file cameramovement.h
 *  
 *  Defines a camera movement logic
 */
#pragma once
#include <object.h>
#include <sprite2d.h>
#include <animations/animationsblinking.h>

class Game;

/*! Defines a camera movement for multithreading
 */
class CameraMovement : public sad::Object
{
SAD_OBJECT
public:
    /*! Makes new camera movement for specified game
     */
    CameraMovement(Game* game);
    /*! A camera movement data
     */
    ~CameraMovement();
    /*! Sets move left boundary, which when crossed by player makes camera shift left
     *  \param[in] l left boundary
     */
    void setMoveLeftBoundary(double l);
    /*! Returns move left boundary, which when crossed by player makes camera shift left
     *  \return left bounary
     */
    double moveLeftBoundary() const;
    /*! Sets move right boundary, which when crossed by player makes camera shift right
     *  \param[in] l right boundary
     */
    void setMoveRightBoundary(double l);
    /*! Returns move right boundary, which when crossed by player makes camera shift right
     *  \return right bounary
     */
    double moveRightBoundary() const;
    /*! Sets maximum shifting time for camera movement
     *  \param[in] time a time
     */
    void setMaxShiftTime(double time);
    /*! Returns maximum shifting time for camera movement
     *  \return maximum shiftin time 
     */
    double maxShiftTime() const;
    /*! Sets arrow position for point
     *  \param[in] p point
     */
    void setArrowPosition(const sad::Point2D& p);
    /*! Returns arrow position
     *  \return arrow position
     */
    const sad::Point2D& arrowPosition() const;
    /*! Inits camera movement
     */
    void init();
    /*! Processes camera movement in game
     */
    void process();
    /*! Shows an arrow for camera movement
     */
    void showArrow();
    /*! Locks a movement
     */
    void lock();
    /*! Unlocks a movement
     */
    void unlock();
    /*! Returns is movement is locked
     */
    bool isLocked() const;
    /*! Moves camera left
     *  \param[in] offset an offset for camera to change
     *  \param[in] change_global_offset whether we should change global offset for camera
     */
    void moveCameraLeft(double offset, bool change_global_offset);
    /*! Moves camera right
     *  \param[in] offset an offset for camera to change
     *  \param[in] change_global_offset whether we should change global offset for camera
     */
    void moveCameraRight(double offset, bool change_global_offset);
private:
    /*! Correct global offset and backgrounds
     */
    void correctOffsetAndBackgrounds();
    /*! A boundary for moving left for player actor, when camera should shift left
     */
    double m_move_left_boundary;
    /*! A boundary for moving right for player actor, when camera should shift right
     */
    double m_move_right_boundary;
    /*! Maximal time (in milliseconds) for camera to set correctly
     */
    double m_max_shift_time;
    /*! A linked game, where all data is stored
     */
    Game* m_game;
    /*! Backgrounds
     */
    sad::Sprite2D* m_backgrounds[3];
    /*! Background tiles
     */
    sad::Sprite2D* m_bg_tiles[3];
    /*! Foreground tiles
     */
    sad::Sprite2D* m_fg_tiles[3];
    /*! An arrow sprite data
     */
    sad::Sprite2D* m_arrow_sprite;
    /*! An arrow position for camera movement
     */
    sad::Point2D m_arrow_position;
    /*! An animation for blinking for arrow position
     */
    sad::animations::Blinking* m_blinking;
    /*! A flag, determining whether arrow is in scene
     */
    bool m_arrow_in_scene;
    /*! A scene for adding arrow
     */
    sad::Scene* m_scene;
    /*! Whether camera movement is locked
     */
    bool m_locked;
};

/*! Exposes camera movement to specified context
 *  \param[in] c context
 */
void exposeCameraMovement(void* c);

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(CameraMovement);