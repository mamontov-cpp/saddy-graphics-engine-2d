/*! \file enemycounter.h
 *  
 *  Defines an enemy counter data
 */
#pragma once
#include <functional>
#include <object.h>
#include <sadvector.h>

class EnemyCounter: public sad::Object
{
SAD_OBJECT
public:
    /*! Constructs default enemy counter
     */
    EnemyCounter();
    /*! Resets enemy counter
     */
    virtual void reset();
    /*! Clears enemy counter
     */
    void clear();
    /*! Sets enemy counter 
     *  \param[in] result a resulting enemy counter
     */
    void set(int result);
    /*! Increments enemy counter
     */
    void increment();
    /*! Decrements enemy counter. If counter is zero, triggers callbacks
     */
    void decrement();
    /*! Places callback, which will be called, when counter is zero
     *  \param[in] f function
     */
    void onZeroEnemies(const std::function<void()>& f);
private:
    /*! An enemy counter data
     */
    int m_enemy_counter;

    /*! List of functions
     */
    sad::Vector<std::function<void()> > m_list;
};

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(EnemyCounter);