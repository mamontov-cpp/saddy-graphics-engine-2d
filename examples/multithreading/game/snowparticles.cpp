#include "snowparticles.h"
#include "../game.h"

#include <db/dbdatabase.h>

DECLARE_SOBJ(game::SnowParticles);

game::SnowParticles::SnowParticles() 
: m_enabled(false), 
  m_source_scene(NULL), 
  m_game(NULL),
  m_falling_speed(0.0),
  m_speedup_coefficient(0.0),
  m_spawn_particles_delay(0.0),
  m_spawn_particles_delay_span(0.0),
  m_min_amount_of_particles_spawned(0),
  m_max_amount_of_particles_spawned(0)
{
    
}

game::SnowParticles::~SnowParticles()
{
    
}

void game::SnowParticles::start()
{
    m_enabled = true;
    this->spawnParticlesDelayed();

}

void game::SnowParticles::stop()
{
    m_enabled = false;
}

void game::SnowParticles::process()
{
    if (m_game)
    {
        if (!m_game->isPaused() && m_game->isNowPlaying())
        {
            double h_speed = m_game->windSpeed() * this->speedupCoefficient();
            double v_speed = this->fallingSpeed();

            double t = 1.0 / m_game->rendererForMainThread()->fps();

            sad::Point2D dp(h_speed * t, v_speed * t * (-1));
            for(size_t i = 0 ; i < m_particles.size(); i++)
            {
                m_particles[i]->moveBy(dp);
                if (m_particles[i]->area()[2].y() < 0)
                {
                    m_source_scene->removeNode(m_particles[i]);
                    m_particles.removeAt(i);
                    --i;
                }
            }
        }
    }
}

void game::SnowParticles::setGame(Game* game)
{
    m_game = game;
    sad::db::Database* db = m_game->rendererForMainThread()->database("gamescreen");
    m_source_scene = db->objectByName<sad::Scene>("particles");
}

void game::SnowParticles::clearParticleList()
{
    m_particles.clear();
}

void game::SnowParticles::setFallingSpeed(double speed)
{
    m_falling_speed = speed;
}

double game::SnowParticles::fallingSpeed() const
{
    return m_falling_speed;
}

void game::SnowParticles::setSpeedupCoefficient(double speedup)
{
    m_speedup_coefficient = speedup;
}

double game::SnowParticles::speedupCoefficient() const
{
    return m_speedup_coefficient;
}

void game::SnowParticles::setSpawnParticleDelay(double delay)
{
    m_spawn_particles_delay = delay;
}

double game::SnowParticles::spawnParticleDelay() const
{
    return m_spawn_particles_delay;
}

void game::SnowParticles::setSpawnParticleDelaySpan(double delay)
{
    m_spawn_particles_delay_span = delay;
}

double game::SnowParticles::spawnParticleDelaySpan() const
{
    return m_spawn_particles_delay_span;
}

void game::SnowParticles::setMinAmountOfParticlesSpawned(int amount)
{
    m_min_amount_of_particles_spawned = amount;

    this->correctParticleAmounts();
}

int game::SnowParticles::minAmountOfParticlesSpawned() const
{
    return m_min_amount_of_particles_spawned;
}

void game::SnowParticles::setMaxAmountOfParticlesSpawned(int amount)
{
    m_min_amount_of_particles_spawned = amount;

    this->correctParticleAmounts();
}

int game::SnowParticles::maxAmountOfParticlesSpawned() const
{
    return m_min_amount_of_particles_spawned;
}

void game::SnowParticles::setParticleName(const sad::String& particle_name)
{
    m_particle_name = particle_name;
}

const sad::String& game::SnowParticles::particleName() const
{
    return m_particle_name;
}

void game::SnowParticles::spawnParticlesDelayed()
{
    if (m_game)
    {
        if (m_game->isNowPlaying() && !m_game->isPaused() && m_enabled)
        {
            double delay = this->spawnParticleDelay();
            double d = (static_cast<double>(rand()) / RAND_MAX);
            delay += (d - 0.5) * this->spawnParticleDelaySpan();
            m_game->addDelayedTask(delay, [=]() {
                if (m_enabled) 
                {
                    this->spawnParticles();
                    this->spawnParticlesDelayed();
                }
            });
        }
    }
}

void game::SnowParticles::spawnParticles()
{
    if (m_game)
    {
        if (m_game->isNowPlaying() && !m_game->isPaused() && m_enabled)
        {
            const sad::Settings settings = m_game->rendererForMainThread()->settings();

            double y = settings.height();
            double x = settings.width();

            int amount = static_cast<int>(ceil(static_cast<double>(rand()) / RAND_MAX * (m_max_amount_of_particles_spawned - m_min_amount_of_particles_spawned)) + m_min_amount_of_particles_spawned);
            for (int i = 0; i < amount; i++)
            {
                sad::Sprite2D* sprite = new sad::Sprite2D();
                sprite->setScene(m_source_scene);
                sprite->setTreeName(m_source_scene->renderer(), "");
                sprite->set(this->particleName());
                double lx = x;
                double ly = y;
                if ((rand() > RAND_MAX / 2) || sad::is_fuzzy_zero(m_game->windSpeed())) 
                {
                    lx = static_cast<double>(rand()) / RAND_MAX * x;
                }
                else
                {
                    if (m_game->windSpeed() < 0)
                    {
                        ly = static_cast<double>(rand()) / RAND_MAX * y;
                        lx = x * (0.75 + static_cast<double>(rand()) / RAND_MAX * 0.25);
                    }
                    else
                    {
                        ly = static_cast<double>(rand()) / RAND_MAX * y;
                        lx = x * (0.00 + static_cast<double>(rand()) / RAND_MAX * 0.25);
                    }
                }
                sprite->moveTo(sad::Point2D(lx, ly));
                m_source_scene->addNode(sprite);
                m_particles << sprite;
            }
        }
    }
}

void game::SnowParticles::correctParticleAmounts()
{
    if (m_min_amount_of_particles_spawned > m_max_amount_of_particles_spawned)
    {
        std::swap(m_min_amount_of_particles_spawned, m_max_amount_of_particles_spawned);
    }
}



void game::exposeSnowParticles(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* snow_particle_binding = new sad::dukpp03::ClassBinding();

    snow_particle_binding->addMethod("setFallingSpeed", sad::dukpp03::bind_method::from(&game::SnowParticles::setFallingSpeed));
    snow_particle_binding->addMethod("fallingSpeed", sad::dukpp03::bind_method::from(&game::SnowParticles::fallingSpeed));
    snow_particle_binding->addMethod("setSpeedupCoefficient", sad::dukpp03::bind_method::from(&game::SnowParticles::setSpeedupCoefficient));
    snow_particle_binding->addMethod("speedupCoefficient", sad::dukpp03::bind_method::from(&game::SnowParticles::speedupCoefficient));
    snow_particle_binding->addMethod("setSpawnParticleDelay", sad::dukpp03::bind_method::from(&game::SnowParticles::setSpawnParticleDelay));
    snow_particle_binding->addMethod("spawnParticleDelay", sad::dukpp03::bind_method::from(&game::SnowParticles::spawnParticleDelay));
    snow_particle_binding->addMethod("setSpawnParticleDelaySpan", sad::dukpp03::bind_method::from(&game::SnowParticles::setSpawnParticleDelaySpan));
    snow_particle_binding->addMethod("spawnParticleDelaySpan", sad::dukpp03::bind_method::from(&game::SnowParticles::spawnParticleDelaySpan));
    snow_particle_binding->addMethod("setMinAmountOfParticlesSpawned", sad::dukpp03::bind_method::from(&game::SnowParticles::setMinAmountOfParticlesSpawned));
    snow_particle_binding->addMethod("minAmountOfParticlesSpawned", sad::dukpp03::bind_method::from(&game::SnowParticles::minAmountOfParticlesSpawned));
    snow_particle_binding->addMethod("setMaxAmountOfParticlesSpawned", sad::dukpp03::bind_method::from(&game::SnowParticles::setMaxAmountOfParticlesSpawned));
    snow_particle_binding->addMethod("maxAmountOfParticlesSpawned", sad::dukpp03::bind_method::from(&game::SnowParticles::maxAmountOfParticlesSpawned));
    snow_particle_binding->addMethod("setParticleName", sad::dukpp03::bind_method::from(&game::SnowParticles::setParticleName));
    snow_particle_binding->addMethod("particleName", sad::dukpp03::bind_method::from(&game::SnowParticles::particleName));
    snow_particle_binding->addMethod("start", sad::dukpp03::bind_method::from(&game::SnowParticles::start));
    snow_particle_binding->addMethod("stop", sad::dukpp03::bind_method::from(&game::SnowParticles::stop));

    ctx->addClassBinding("game::SnowParticles", snow_particle_binding);
}