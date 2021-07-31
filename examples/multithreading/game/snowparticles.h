/*! \file snowparticles.h
 *   
 *   A simple snow particles that fall from top of screen to bottom
 */
#pragma once
#include <sprite2d.h>
#include <sadstring.h>

class Game;

namespace game
{
/*! A list of snow particles to make them fall from top of screen to bottom
 */
class SnowParticles: public sad::Object
{
SAD_OBJECT
public:
    /*! Constructs default snow particles
     */
    SnowParticles();
    /*! Does nothing, since scene should destroy sprites
     */
    ~SnowParticles();
    /*! Starts spawning new particles
     */
    void start();
    /*! Stops spawning new particles
     */
    void stop();
    /*! Performs all actions, related to handling and spawning specific particles
     */
    void process();
    /*! Sets game for particles
     */
    void setGame(Game* game);
    /*! Clears particle list
     */
    void clearParticleList();
    /*! Sets falling speed for particles
     *  \param[in] speed  a speed for particles
     */
    void setFallingSpeed(double speed);
    /*! Returns falling speed for particles
     *  \return falling speed
     */
    double fallingSpeed() const;
    /*! Sets speed up coefficient for particles
     *  \param[in] speedup a coefficient that multiplies a wind speed to get speed of particles
     */
    void setSpeedupCoefficient(double speedup);
    /*! Return speed up coefficient for particles
     *  \return speed up coefficient
     */
    double speedupCoefficient() const;
    /*! Sets spawn particle delay
     *  \param[in] delay for spawned particles
     */
    void setSpawnParticleDelay(double delay);
    /*! Returns spawn particle delay
     *  \return spawn particle delay
     */
    double spawnParticleDelay() const;
    /*! Sets spawn particle delay span
     *  \param[in] delay for spawned particles
     */
    void setSpawnParticleDelaySpan(double delay);
    /*! Returns spawn particle delay span
     *  \return spawn particle delay span
     */
    double spawnParticleDelaySpan() const;
    /*! Sets minimal amount of particles spawned
     *  \param[in] amount of particles
     */
    void setMinAmountOfParticlesSpawned(int amount);
    /*! Returns minimal amount of particles spawned
     *  \return minimal amount of particles spawned
     */
    int minAmountOfParticlesSpawned() const;
    /*! Sets maximal amount of particles spawned
     *  \param[in] amount of particles
     */
    void setMaxAmountOfParticlesSpawned(int amount);
    /*! Returns maximal amount of particles spawned
     *  \return maximal amount of particles spawned
     */
    int maxAmountOfParticlesSpawned() const;
    /*! Sets particle name
     *  \param[in] particle_name a name for particle
     */
    void setParticleName(const sad::String& particle_name);
    /*! Returns particle name for sprites
     *  \return particle name a name particle
     */
    const sad::String& particleName() const;
    /*! Spawn particles delayed
     */
    void spawnParticlesDelayed();
private:
    /*! Spawns particles
     */
    void spawnParticles();
    /*! Corrects particle amounts, swapping them if necessary
     */
    void correctParticleAmounts();
    /*! Whether particles are enabled
     */
    bool m_enabled;
    /*! A list of particles, that should fall
     */
    sad::Vector<sad::Sprite2D*> m_particles;
    /*! A scene, where particles should be
     */
    sad::Scene* m_source_scene;
    /*! A source game, where wind speed should be
     */
    Game* m_game;
    /*! A falling speed for particles
     */
    double m_falling_speed;
    /*! A speedup coefficient
     */
    double m_speedup_coefficient;
    /*! A delay between spawned particles
     */
    double m_spawn_particles_delay;
    /*! A span  for random delay between delayed particles
     */
    double m_spawn_particles_delay_span;
    /*! Min amount of particles spawned
     */
    size_t m_min_amount_of_particles_spawned;
    /*! Max amount of particles spawned
     */
    size_t m_max_amount_of_particles_spawned;
    /*! A particle options name
     */
    sad::String m_particle_name;
};

/*! Exposes snow particles to context
 *  \param[in] c context
 */
void exposeSnowParticles(void* c);

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(game::SnowParticles);