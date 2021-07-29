#include <p2d/bouncesolver.h>
#include <p2d/world.h>

#include <log/log.h>


//  =============================== sad::p2d::BounceSolver::SolverTask ================

void sad::p2d::BounceSolver::SolverTask::destroy() const
{
    if (First)
    {
        First->deleteBlock(First);
    }
    if (Second)
    {
        Second->deleteBlock(Second);
    }
}


// ================================ PUBLIC METHODS ================================

DECLARE_SOBJ(sad::p2d::BounceSolver);

sad::p2d::BounceSolver::BounceSolver()
: m_toi(0),
  m_restitution_coefficient(1.0),
  m_rotationfriction{0.0, 0.0},
  m_shouldperformrotationfriction(true),
  m_inelastic_collisions(false),
  m_inelastic_collision_type(sad::p2d::BounceSolver::ICT_NO_INELASTIC_COLLISION),
  m_recursion_limit(100),
  m_recursion_counter(0),
  m_collision_precision_step(0.001),
  m_max_solver_iterations(100),
  m_ignore_no_contact_points(false),
  m_ignore_negative_toi(false)
{
    m_find = new sad::p2d::FindContactPoints();
    m_first = nullptr;
    m_second = nullptr;
    m_debug = false;
    this->resetCoefficients();
}

sad::p2d::BounceSolver::~BounceSolver()
{
    delete m_find;
}

bool sad::p2d::BounceSolver::bounce(sad::p2d::Body* b1, sad::p2d::Body* b2)
{
    ++m_recursion_counter;
    if (m_recursion_counter > m_recursion_limit)
    {
        this->logFCPError("Recursion limit reached");
        return false;
    }
    bool result;
    if (m_inelastic_collision_type == sad::p2d::BounceSolver::ICT_NO_INELASTIC_COLLISION)
    {
         result = this->bounceNormal(b1, b2);
    }
    else
    {
        if (m_inelastic_collision_type == sad::p2d::BounceSolver::ICT_FIRST)
        {
            result = this->inelasticBounceWithFixedSecondBody(b1, b2);
        }
        else
        {
            result = this->inelasticBounceWithFixedSecondBody(b2, b1);
        }
    }
    --m_recursion_counter;
    return result;
}


void sad::p2d::BounceSolver::logFCPError(const char * m)
{
    if (m_debug)
    {
    sad::String tpl = "Cannot find a contact points. Performing an object dump\n";
    tpl <<  "1st body: \n{0}\n";
    tpl <<  "velocity ({1},{2})\n";
    tpl <<  "2nd body: \n{3}\n";
    tpl <<  "velocity ({4},{5})\nTOI: {6} Reason: {7} \n";

    SL_INTERNAL(fmt::Format(tpl) << m_first->currentShape()->dump()
                                 << m_av1.x() << m_av1.y()
                                 << m_second->currentShape()->dump()
                                 << m_av2.x() << m_av2.y()
                                 << m_toi << m
               );
    }
}

std::string sad::p2d::BounceSolver::dump() const
{
    std::string contacts = "not found!\n";
    if (m_contact.exists())
    {
        contacts = str(fmt::Format("({0}, {1}), ({2}, {3})") 
                   << m_contact.value().p1().x()
                   << m_contact.value().p1().y()
                   << m_contact.value().p2().x()
                   << m_contact.value().p2().y());
    }
    std::string result =  "1st body: {0} and  velocity ({1},{2})\n";
    result += "2nd body: {3} and velocity  ({4},{5})\n";
    result += "TOI: {6}\n";
    result += "Contact points: {7}\n";
    result += "1st body scheduled position at {8}, {9} velocity {10}, {11}\n";
    result += "2nd body scheduled position at {12}, {13} velocity {14}, {15}\n";
    result = str(fmt::Format(result) << m_first->currentShape()->dump()
                       << m_av1.x() << m_av1.y()
                       << m_second->currentShape()->dump()
                       << m_av2.x() << m_av2.y()
                       << m_toi << contacts
                       << m_first->nextPosition().x() << m_first->nextPosition().y()
                       << m_first->nextTangentialVelocity().x() <<m_first->nextTangentialVelocity().y()
                       << m_second->nextPosition().x() << m_second->nextPosition().y()
                       << m_second->nextTangentialVelocity().x() <<m_second->nextTangentialVelocity().y()					
                );
    return result;
}

void sad::p2d::BounceSolver::toggleInelasticCollisions(bool b)
{
    m_inelastic_collisions = b;
}

bool sad::p2d::BounceSolver::isEnabledInelasticCollisions() const
{
    return m_inelastic_collisions;
}

void sad::p2d::BounceSolver::setInelasticCollisionType(sad::p2d::BounceSolver::InelasticCollisionType type)
{
    m_inelastic_collision_type = type;
    m_inelastic_collisions = (type != sad::p2d::BounceSolver::ICT_NO_INELASTIC_COLLISION);
}

void sad::p2d::BounceSolver::setInelasticCollisionTypeAsUnsignedInt(unsigned int type)
{
    if (type > sad::p2d::BounceSolver::ICT_SECOND)
    {
        type = sad::p2d::BounceSolver::ICT_SECOND;
    }
    this->setInelasticCollisionType(static_cast<sad::p2d::BounceSolver::InelasticCollisionType>(type));
}

unsigned int sad::p2d::BounceSolver::inelasticCollisionTypeAsUnsignedInt() const
{
    return this->inelasticCollisionType();
}


sad::p2d::BounceSolver::InelasticCollisionType sad::p2d::BounceSolver::inelasticCollisionType() const
{
    return m_inelastic_collision_type;
}


void sad::p2d::BounceSolver::setRecursionLimit(size_t limit)
{
    m_recursion_limit = limit;
}


size_t sad::p2d::BounceSolver::recursionLimit() const
{
    return m_recursion_limit;
}

void sad::p2d::BounceSolver::setCollisionPrecisionStep(double step)
{
    m_collision_precision_step = step;
}


double sad::p2d::BounceSolver::collisionPrecisionStep() const
{
    return m_collision_precision_step;
}

void sad::p2d::BounceSolver::setMaxSolverIterations(size_t value)
{
    m_max_solver_iterations = value;
}

size_t sad::p2d::BounceSolver::maxSolverIterations() const
{
    return m_max_solver_iterations;
}

void sad::p2d::BounceSolver::toggleIgnoreContactPoints(bool value)
{
    m_ignore_no_contact_points =  value;
}

bool sad::p2d::BounceSolver::getIgnoreContactPointsFlag() const
{
    return m_ignore_no_contact_points;
}

void sad::p2d::BounceSolver::toggleIgnoreNegativeTOI(bool value)
{
    m_ignore_negative_toi = value;
}

bool sad::p2d::BounceSolver::getIgnoreNegativeTOIFlag() const
{
    return m_ignore_negative_toi;
}

// =========================================== PRIVATE METHODS =============================

bool sad::p2d::BounceSolver::inelasticBounceWithFixedSecondBody(sad::p2d::Body* b1, sad::p2d::Body* b2)
{
    m_first = b1;
    m_second = b2;

    double world_step = m_first->world()->timeStep();

    sad::Maybe<sad::p2d::BounceSolver::SolverTask> maybe_task = this->findBasicTaskForInelasticBounce();
    if (maybe_task.exists())
    {
        sad::p2d::BounceSolver::SolverTask task = maybe_task.value();
        if (sad::is_fuzzy_zero(task.PivotTime) || (task.PivotTime >= 0  && task.PivotTime <= world_step) || sad::is_fuzzy_equal(task.PivotTime, world_step))
        {
            // Further compute TOI and perform inelastic bounce
            sad::p2d::SetOfPointsPair pairs;
            this->solveTOIFCP(task.First, task.FirstVelocity, task.Second, task.SecondVelocity, pairs);
            m_toi += task.PivotTime;

            bool has_pairs_or_flag_is_set = !pairs.empty()  || m_ignore_no_contact_points;

            if (has_pairs_or_flag_is_set
                &&  ((m_toi > 0) || m_ignore_negative_toi || sad::is_fuzzy_zero(m_toi, COLLISION_PRECISION * 1000))
                && ((m_toi < world_step)  || sad::is_fuzzy_equal(m_toi, world_step)))
            {
                // Compute new collision position and speed
                sad::p2d::Body::CollisionData new_data = this->fixLocalInelasticCollision(task);
                task.destroy();
                // Insert new collisions into list,
                size_t insert_position = this->insertDataIntoCollisionList(new_data);
                // Fetch bodies to recompute position with list
                sad::Vector<sad::p2d::Body*> bodies_to_recompute = this->getBodiesToRecomputeCollisionsWith(insert_position);
                // Recompute last position for first body
                this->resheduleFistBodyPositionAfterInElasticBounce(insert_position);
                // Exclude already handled collisions, otherwise try recompute collisions with other bodies
                for(size_t i = 0; i < bodies_to_recompute.size(); i++)
                {
                    if (!sad::p2d::BounceSolver::isAlreadyHandledInelasticCollisionBetween(b1, bodies_to_recompute[i]))
                    {
                        this->bounce(b1, bodies_to_recompute[i]);
                    }
                }
                return true;
            }
            else
            {
                const char* reason = "Can't find pairs";
                if (!pairs.empty())
                    reason = "TOI is negative";
                logFCPError(reason);
                task.destroy();
            }
        }
    }

    return false;
}

sad::Vector<sad::p2d::Body*> sad::p2d::BounceSolver::getBodiesToRecomputeCollisionsWith(size_t position) const
{
    sad::Vector<sad::p2d::Body*> result;
    for(size_t i = position + 1; i < m_first->collisions().size(); i++)
    {
        result << m_first->collisions()[i].Bodies;
    }
    result.removeAll(m_second);
    return result;
}

void sad::p2d::BounceSolver::resheduleFistBodyPositionAfterInElasticBounce(size_t insert_position) const
{

    if (insert_position != m_first->collisions().size() - 1)
    {
        double world_step = m_first->world()->timeStep();

        m_first->collisions().resize(insert_position + 1); // Erase all collisions after current
        // Compute data at world step time and shedule new position
        sad::p2d::Body::CollisionData& data = m_first->collisions()[insert_position];
        sad::p2d::Vector speed = data.OwnSpeed + data.PlatformSpeed;
        sad::p2d::Vector new_position = data.Position + speed * (world_step - data.TOI);

        sad::p2d::Body::CollisionData result_data;
        result_data.DOwnSpeed = sad::p2d::Vector(0, 0);
        result_data.DPosition = sad::p2d::Vector(0, 0);
        result_data.OwnSpeed = data.OwnSpeed;
        result_data.PlatformSpeed = data.PlatformSpeed;
        result_data.Position = new_position;
        result_data.TOI = world_step;

        m_first->collisions() << result_data;
        m_first->shedulePosition(new_position);
    }
    else
    {
        m_first->shedulePosition(m_first->collisions()[m_first->collisions().size() - 1].Position);
    }
}

/*! Returns max of two values if they have same sign, otherwise sum
    \param[in] a value
    \param[in] b value
 */
inline double maxOrSum(double a, double b)
{
    if (a < 0) {
        return (b < 0) ? std::min(a, b) : (a + b);
    } else {
        return (b > 0) ? std::max(a, b) : (a + b);
    }
}

/*! Returns max of two values if they have same sign, otherwise sum
    \param[in] a value
    \param[in] b value
 */
inline sad::p2d::Vector maxOrSum(const sad::p2d::Vector& a, const sad::p2d::Vector&  b)
{
    return {maxOrSum(a.x(), b.x()), maxOrSum(a.y(), b.y())};
}


size_t sad::p2d::BounceSolver::insertDataIntoCollisionList(const sad::p2d::Body::CollisionData& data) const
{
    size_t result = 0;
    sad::p2d::Body::CollisionData& first_collision = m_first->collisions()[0];
    if (sad::is_fuzzy_equal(first_collision.TOI, data.TOI, COLLISION_PRECISION))
    {
        if (first_collision.Bodies.empty())
        {
            first_collision = data;
            return result;
        }
        else
        {
            // Recompute platform velocity
            first_collision.Bodies << data.Bodies[0];

            // Recompute base position with position according to current data
            sad::p2d::Vector base_position = data.Position - data.DPosition;
            sad::p2d::Vector result_dposition = maxOrSum(first_collision.DPosition, data.DPosition);
            first_collision.Position = base_position + result_dposition;
            first_collision.DPosition = result_dposition;

            first_collision.PlatformSpeed = maxOrSum(first_collision.PlatformSpeed, data.PlatformSpeed);

            sad::p2d::Vector base_own_speed = first_collision.OwnSpeed - first_collision.DOwnSpeed;
            sad::p2d::Vector result_d_own_speed = data.DOwnSpeed + first_collision.DOwnSpeed;
            first_collision.OwnSpeed = base_own_speed + result_d_own_speed;
            first_collision.DOwnSpeed = result_d_own_speed;
            return result;
        }
    }

    // Resolve tiny negative TOI
    if (data.TOI < first_collision.TOI)
    {
        m_first->collisions().insert(data, 0);
        return 0;
    }
    for (size_t i = 1; i < m_first->collisions().size(); i++)
    {
         sad::p2d::Body::CollisionData& prev_collision = m_first->collisions()[i - 1];
         sad::p2d::Body::CollisionData& next_collision = m_first->collisions()[i];
         if (sad::is_fuzzy_equal(next_collision.TOI, data.TOI, COLLISION_PRECISION))
         {
             if (next_collision.Bodies.empty())
             {
                 next_collision = data;
                 return i;
             }
             else
             {
                 // Recompute platform velocity
                 next_collision.Bodies << data.Bodies[0];

                 // Recompute base position with position according to current data
                 sad::p2d::Vector base_position = data.Position - data.DPosition;
                 sad::p2d::Vector result_dposition = maxOrSum(next_collision.DPosition, data.DPosition);
                 next_collision.Position = base_position + result_dposition;
                 next_collision.DPosition = result_dposition;

                 sad::p2d::Vector base_own_speed = next_collision.OwnSpeed - next_collision.DOwnSpeed;
                 sad::p2d::Vector result_d_own_speed = data.DOwnSpeed + next_collision.DOwnSpeed;
                 next_collision.OwnSpeed = base_own_speed + result_d_own_speed;
                 next_collision.DOwnSpeed = result_d_own_speed;
                 return i;
             }
         }
         else
         {
            if ((prev_collision.TOI <= data.TOI) && (data.TOI <= next_collision.TOI))
            {
                m_first->collisions().insert(data, i);
                return i;
            }
         }
    }

    return result;
}

sad::p2d::Body::CollisionData  sad::p2d::BounceSolver::fixLocalInelasticCollision(const sad::p2d::BounceSolver::SolverTask& task) const
{
    sad::p2d::Vector dposition = m_first->position() - m_first->currentShape()->center();
    sad::p2d::Vector collision_position = task.First->center() + dposition;
    double local_toi = m_toi - task.PivotTime;
    // Compute correct collision position at time of collision
    sad::p2d::Vector toi_collision_position = collision_position + task.FirstVelocity * local_toi;
    // A fist collision shape for toi
    sad::p2d::CollisionShape* first_toi_collision_shape = m_first->currentShape()->clone();
    first_toi_collision_shape->move(toi_collision_position - m_first->position());

    // Get second collision shape in time of toi
    sad::p2d::CollisionShape* second_toi_collision_shape = task.Second->clone();
    second_toi_collision_shape->move(task.SecondVelocity * local_toi);

    // Now compute normal from center of first shape to second
    sad::p2d::Vector normal;
    second_toi_collision_shape->normalToPointOnSurface(first_toi_collision_shape->center(), normal);

    // We need normal part of platform velocity
    sad::p2d::Vector effective_platform_speed = normal * sad::p2d::scalar(task.SecondVelocity, normal);

    sad::p2d::Vector reverse_own_speed = task.FirstVelocity * - 1;

    sad::p2d::Vector speed_sum  = effective_platform_speed + reverse_own_speed;
    sad::p2d::Vector result_dposition(0.0, 0.0);

    if (sad::is_fuzzy_zero(sad::p2d::modulo(speed_sum)))
    {
        second_toi_collision_shape->normalToPointOnSurface(first_toi_collision_shape->center(), speed_sum);
        if (sad::is_fuzzy_zero(sad::p2d::modulo(speed_sum)))
        {
            speed_sum.setX(1.0);
        }
        sad::p2d::Cutter1D second_cutter = second_toi_collision_shape->project(speed_sum);
        sad::p2d::Cutter1D first_cutter = first_toi_collision_shape->project(speed_sum);
        speed_sum *= std::max(second_cutter.p1(), second_cutter.p2()) - std::min(first_cutter.p1(), first_cutter.p2());

        first_toi_collision_shape->move(speed_sum);
        toi_collision_position += speed_sum;
        result_dposition += speed_sum;
    }
    
    sad::p2d::CollisionTest test;
    double iteration = 0;
    bool colliding;
    if (test.invoke(first_toi_collision_shape, second_toi_collision_shape))
    { 
        do
        {
            sad::p2d::Vector dpos = speed_sum * m_collision_precision_step;
            first_toi_collision_shape->move(dpos);
            toi_collision_position += dpos;
            result_dposition += dpos;
            colliding = test.invoke(first_toi_collision_shape, second_toi_collision_shape);
            ++iteration;
        } while((iteration < m_max_solver_iterations) && colliding);
    }

    // Compute new speed for first body

    sad::p2d::Vector d_speed = normal * sad::p2d::scalar(task.FirstVelocity, normal) * (-1.0);
    sad::p2d::Vector new_speed = task.FirstVelocity +  d_speed;
    first_toi_collision_shape->deleteBlock(first_toi_collision_shape);
    second_toi_collision_shape->deleteBlock(second_toi_collision_shape);

    return { {m_second},  m_toi, effective_platform_speed, new_speed, d_speed, toi_collision_position, result_dposition};
}

sad::Maybe<sad::p2d::BounceSolver::SolverTask> sad::p2d::BounceSolver::findBasicTaskForInelasticBounce() const
{
    sad::p2d::CollisionShape* s1  = m_first->currentShape()->clone();
    sad::p2d::Vector cp1 = m_first->position();

    // Determine specific velocities and times, when bodies had collided
    double world_step = m_first->world()->timeStep();
    if (m_first->collisions().empty())
    {
        sad::p2d::Vector v = (m_first->positionAt(world_step) - cp1) / world_step;
        sad::p2d::Vector zero_dvelocity(0, 0), zero_platform_speed(0,0);
        //sad::p2d::Vector pos = m_first->()
        m_first->collisions().add({
            {}, 0,  zero_platform_speed, v, zero_dvelocity, cp1, sad::p2d::Vector(0,0)
        });
        m_first->collisions().add({
            {}, world_step,  zero_platform_speed, v, zero_dvelocity, m_first->positionAt(world_step), sad::p2d::Vector(0,0)
        });
    }
    sad::Maybe<sad::p2d::BounceSolver::SolverTask> solver_task;
    for(size_t i = 0 ; i < m_first->collisions().size(); i++)
    {
        sad::p2d::Body::CollisionData& data = m_first->collisions()[i];
        sad::p2d::CollisionShape* first_shape = s1->clone();
        first_shape->move(data.Position - cp1);

        this->tryFindBasicTaskForInelasticBounceForData(data, first_shape, solver_task);

        first_shape->deleteBlock(first_shape);
    }


    return solver_task;
}


void sad::p2d::BounceSolver::tryFindBasicTaskForInelasticBounceForData(
    sad::p2d::Body::CollisionData& data,
    sad::p2d::CollisionShape*  first_shape,
    sad::Maybe<sad::p2d::BounceSolver::SolverTask>& solver_task
) const
{
    sad::p2d::Vector first_velocity = data.PlatformSpeed + data.OwnSpeed;

    // Get second body position at this time
    if (m_second->collisions().empty())
    {
        sad::p2d::CollisionShape* second_shape  = m_second->currentShape()->clone();
        sad::p2d::Vector cp2 = m_second->position();

        sad::p2d::Vector position = m_second->positionAt(data.TOI);
        sad::p2d::Vector dposition = position - cp2;
        second_shape->move(dposition);

        sad::p2d::Vector av2 = dposition / data.TOI;
        if (sad::is_fuzzy_zero(data.TOI))
        {
            av2 = m_second->tangentialVelocity();
        }
        sad::p2d::CollisionTest test;
        if (test.invoke(first_shape, second_shape))
        {
            solver_task.setValue({first_shape->clone(1), first_velocity, second_shape, av2, data.TOI});
        }
        else
        {
            second_shape->deleteBlock(second_shape);
        }
    }
    else
    {
        // What if platform jumped too? We should scan it too.
        sad::p2d::Body::CollisionData& first_collision_data =  m_second->collisions()[0];
        sad::p2d::Body::CollisionData& last_collision_data =  m_second->collisions()[m_second->collisions().size() - 1];

        if (data.TOI < first_collision_data.TOI)
        {
            this->tryFindBasicTaskForInelasticBounceForData(data, first_shape, first_collision_data, solver_task);
        }
        else
        {
            if (data.TOI > last_collision_data.TOI)
            {
                this->tryFindBasicTaskForInelasticBounceForData(data, first_shape, last_collision_data, solver_task);
            }
            else
            {
                // Full scan in between for platform
                this->tryFindBasicTaskForInelasticBounceWithCollisions(data, first_shape, solver_task);
            }
        }
    }
}


void sad::p2d::BounceSolver::tryFindBasicTaskForInelasticBounceForData(
    sad::p2d::Body::CollisionData& data,
    sad::p2d::CollisionShape*  first_shape,

    sad::p2d::Body::CollisionData& second_data,
    sad::Maybe<sad::p2d::BounceSolver::SolverTask>& solver_task
) const
{
    sad::p2d::CollisionTest test;

    sad::p2d::CollisionShape* second_shape  = m_second->currentShape()->clone();
    sad::p2d::Vector cp2 = m_second->position();

    double dtime = data.TOI - second_data.TOI;
    sad::p2d::Vector speed = (second_data.PlatformSpeed + second_data.OwnSpeed);
    sad::p2d::Vector first_velocity = data.PlatformSpeed + data.OwnSpeed;
    sad::p2d::Vector npos = second_data.Position + speed * dtime;
    second_shape->move(npos - cp2);
    if (test.invoke(first_shape, second_shape))
    {
        solver_task.setValue({first_shape->clone(1), first_velocity, second_shape, speed, data.TOI});
    }
    else
    {
        second_shape->deleteBlock(second_shape);
    }
}

void sad::p2d::BounceSolver::tryFindBasicTaskForInelasticBounceWithCollisions(
    sad::p2d::Body::CollisionData& data,
    sad::p2d::CollisionShape*  first_shape,
    sad::Maybe<sad::p2d::BounceSolver::SolverTask>& solver_task
) const
{
    sad::p2d::CollisionTest test;

    sad::p2d::Vector first_velocity = data.PlatformSpeed + data.OwnSpeed;

    sad::Vector<sad::p2d::Body::CollisionData>& list = m_second->collisions();
    for(size_t i = 1; i < list.size(); i++)
    {
        if (sad::is_fuzzy_equal(data.TOI, list[i-1].TOI)
            || sad::is_fuzzy_equal(data.TOI, list[i].TOI)
            || (list[i-1].TOI <= data.TOI) && (data.TOI <= list[i].TOI))
        {
            sad::p2d::CollisionShape* second_shape  = m_second->currentShape()->clone();
            sad::p2d::Vector cp2 = m_second->position();
            sad::p2d::Vector speed = list[i-1].PlatformSpeed + list[i-1].OwnSpeed;
            sad::p2d::Vector dv = (list[i -1].Position  + speed * (data.TOI - list[i-1].TOI))  - cp2;
            second_shape->move(dv);
            if (test.invoke(first_shape, second_shape))
            {
                solver_task.setValue({first_shape->clone(1), first_velocity, second_shape, speed, data.TOI});
            }
            else
            {
                second_shape->deleteBlock(second_shape);
            }
            return;
        }
    }
}


bool sad::p2d::BounceSolver::isAlreadyHandledInelasticCollisionBetween(sad::p2d::Body* first, sad::p2d::Body* second)
{
    for(size_t i = 0; i < first->collisions().size(); i++)
    {
        const sad::Vector<sad::p2d::Body*>& bodies = first->collisions()[i].Bodies;
        if (std::find(bodies.begin(), bodies.end(), second) != bodies.end())
        {
            return true;
        }
    }
    return false;
}



bool sad::p2d::BounceSolver::bounceNormal(sad::p2d::Body* b1, sad::p2d::Body* b2)
{
    m_first = b1;
    m_second = b2;
    p2d::SetOfPointsPair pairs;
    this->solveTOIFCP(pairs);
    if (!pairs.empty() &&  ((m_toi > 0) || m_ignore_negative_toi || sad::is_fuzzy_zero(m_toi, COLLISION_PRECISION * 1000)))
    {
        this->performBouncing(pairs);
        this->resetCoefficients();
        return true;
    }
    else
    {
        const char* reason = "Can't find pairs";
        if (!pairs.empty())
            reason = "TOI is negative";
        logFCPError(reason);
    }
    return false;
}


/*! Implement solving for bouncing
    \param[in] pairs pairs of contact points
 */
void sad::p2d::BounceSolver::performBouncing(const sad::p2d::SetOfPointsPair & pairs)
{
    //TODO: Implement another types of friction

    sad::p2d::Point normal1;
    m_first->currentShape()->normalToPointOnSurface(pairs[0].p1(), normal1);
    double pivot1 = m_first->currentShape()->center().distance(pairs[0].p1());
    m_force_moment[0] = (pairs[0].p1() - m_first->currentShape()->center());

    sad::p2d::Point normal2;
    m_second->currentShape()->normalToPointOnSurface(pairs[0].p2(), normal2);
    double pivot2 = m_second->currentShape()->center().distance(pairs[0].p2());
    m_force_moment[1] = (pairs[0].p2() - m_second->currentShape()->center());

    sad::p2d::Vector v1 = m_first->tangentialVelocityAt(m_toi);
    double project1 = p2d::scalar(v1, normal1);
    sad::p2d::Vector normalPart1 = normal1;
    normalPart1 *= project1;
    sad::p2d::Vector cachedNormal1 = normalPart1;

    sad::p2d::Vector tangentialPart1 = v1;
    tangentialPart1 -= normalPart1;

    sad::p2d::Vector v2 = m_second->tangentialVelocityAt(m_toi);
    double project2 = p2d::scalar(v2, normal2);
    sad::p2d::Vector normalPart2 = normal2;
    normalPart2 *= project2;
    sad::p2d::Vector cachedNormal2 = normalPart2;

    sad::p2d::Vector tangentialPart2 = v2;
    tangentialPart2 -= normalPart2;

    this->resolveNormalSpeed(m_first, normalPart1, m_second, normalPart2, pairs);
    if (!m_inelastic_collisions) {
        m_first->correctTangentialVelocity(normalPart1 + tangentialPart1);
    }
    double ctoi = m_toi - this->collisionPrecisionStep();
    double after_toi_part = m_first->TimeStep - ctoi;
    m_first->correctPosition(m_av1 * ctoi + (normalPart1 + tangentialPart1) * after_toi_part);
    this->tryResolveFriction(m_first, tangentialPart1, normalPart1 - cachedNormal1, 0, pivot1);
    if (!m_inelastic_collisions) {
        m_second->correctTangentialVelocity(normalPart2 + tangentialPart2);
    }
    m_second->correctPosition(m_av2 * ctoi + (normalPart2 + tangentialPart2) * after_toi_part);
    this->tryResolveFriction(m_second, tangentialPart2, normalPart2 - cachedNormal2, 1, pivot2);
}



void  sad::p2d::BounceSolver::solveTOIFCP(
    sad::p2d::CollisionShape* first,
    const sad::p2d::Vector& av1,
    sad::p2d::CollisionShape* second,
    const sad::p2d::Vector& av2,
    sad::p2d::SetOfPointsPair& pairs
)
{
    m_av1 = av1;
    m_av2 = av2;


    m_contact.clear();

    pairs = m_find->invoke(first,   m_av1,
                           second,  m_av2
                           );
    if (pairs.size()  == 2)
    {
        pairs[0].set1((pairs[0].p1() + pairs[1].p1()) / 2.0);
        pairs[0].set2((pairs[0].p2() + pairs[1].p2()) / 2.0);
        pairs.removeAt(1);
        m_shouldperformrotationfriction = false;
    }

    if (!pairs.empty())
    {
        m_contact.setValue(pairs[0]);
        // Compute time of impact
        double x1 = pairs[0].p1().x();
        double y1 = pairs[0].p1().y();
        double x2 = pairs[0].p2().x();
        double y2 = pairs[0].p2().y();

        double avx1 = m_av1.x();
        double avy1 = m_av1.y();
        double avx2 = m_av2.x();
        double avy2 = m_av2.y();

        double time;
        if (sad::non_fuzzy_zero(avx2 - avx1))
        {
            time = (x1 - x2) / (avx2 - avx1);
        }
        else
        {
            time = (y1 - y2) / (avy2 - avy1);
        }

        time -= COLLISION_PRECISION;
        m_toi = time;
    }
}

void sad::p2d::BounceSolver::solveTOIFCP(sad::p2d::SetOfPointsPair& pairs)
{
    this->solveTOIFCP(
        m_first->currentShape(),
        m_first->averageChangeIndependentTangentialVelocity(),
        m_second->currentShape(),
        m_second->averageChangeIndependentTangentialVelocity(),
        pairs
    );
}

void sad::p2d::BounceSolver::resolveNormalSpeed(
    sad::p2d::Body* b1,
    sad::p2d::Vector& n1,
    sad::p2d::Body* b2,
    sad::p2d::Vector& n2,
    const sad::p2d::SetOfPointsPair& /*pairs*/
) const
{
    bool is_body_1_fixed = b1->weight().isInfinite() || b1->fixed();
    bool is_body_2_fixed = b2->weight().isInfinite() || b2->fixed();
    if (is_body_1_fixed && is_body_2_fixed)
    {
        is_body_1_fixed = b1->fixed();
        is_body_2_fixed = b2->fixed();
    }

    // Took from https://en.wikipedia.org/wiki/Coefficient_of_restitution
    // Formula looks as follows:
    //        m1u1 + m2u2 + m2k(u2-u1)
    //   v1 = ------------------------
    //                m1 + m2
    //
    //        m1u1 + m2u2 + m1k(u1-u2)
    //   v2 = ------------------------
    //                m1 + m2
    // where u1 = n1, u2 = n2
    // If m1 is infinite, v1 = u1, v2 = u1 + k(u1-u2)
    double k = m_restitution_coefficient;

    double v1x = 0 , v2x = 0, v1y = 0, v2y = 0, m1 = 1.0, m2 = 1.0, must_compute_full_collision = true;
    if (is_body_1_fixed)
    {
       if (is_body_2_fixed)
       {
           m1 = 1.0;
           m2 = 1.0;
       }
       else
       {
           // b1 is fixed, b2 is not, take speed from other one
           v1x = n1.x();
           v1y = n1.y();

           v2x = n1.x() + k * (n1.x() - n2.x());
           v2y = n1.y() + k * (n1.y() - n2.y());

           must_compute_full_collision = false;
       }
    }
    else
    {
        if (is_body_2_fixed)
        {
            // b2 is fixed, b1 is not, take speed from other one
            v2x = n2.x();
            v2y = n2.y();

            v1x = n2.x() - k * (n1.x() - n2.x());
            v1y = n2.y() - k * (n1.y() - n2.y());

            must_compute_full_collision = false;
        }
        else
        {
            m1 = (b1->weight().isInfinite()) ? 1.0 : (b1->weight().value());
            m2 = (b2->weight().isInfinite()) ? 1.0 : (b2->weight().value());
        }
    }
    if (must_compute_full_collision)
    {
        double msum = m1 + m2;
        // m1u1x+m2u2x
        double vxmsum = m1 * n1.x() + m2 * n2.x();
        // m1u1y+m2u2y
        double vymsum = m1 * n1.y() + m2 * n2.y();
        // k*(u2x -u1x)
        double kxdiff = k * (n2.x() - n1.x());
        // k*(u2y -u1y)
        double kydiff = k * (n2.y() - n1.y());

        v1x = (vxmsum + m2 * kxdiff) / msum;
        v1y = (vymsum + m2 * kydiff) / msum;

        v2x = (vxmsum - m1 * kxdiff) / msum;
        v2y = (vymsum - m1 * kydiff) / msum;
    }

    n1.setX(v1x);
    n1.setY(v1y);

    n2.setX(v2x);
    n2.setY(v2y);
}


void sad::p2d::BounceSolver::resetCoefficients()
{
    m_restitution_coefficient = 1.0;
    m_rotationfriction[0] = 0.0;
    m_rotationfriction[1] = 0.0;
    m_shouldperformrotationfriction = true;
}

void sad::p2d::BounceSolver::tryResolveFriction(
    sad::p2d::Body * b, 
    const sad::p2d::Vector & t, 
    const sad::p2d::Vector &  /*ni*/,
    int index,
    double  /*pivot*/
)
{
    if (sad::non_fuzzy_zero(m_rotationfriction[index]) 
        && !b->weight().isInfinite()
        && m_shouldperformrotationfriction
        && !m_inelastic_collisions)
    {
        double w = b->angularVelocityAt(m_toi);
        sad::p2d::Vector tangential = m_force_moment[index];
        sad::p2d::mutableUnit(tangential);
        sad::p2d::mutableNormalizedOrtho(tangential, sad::p2d::OVI_DEG_90);

        sad::p2d::Vector tangentialUnit = tangential;
        double forcemomentlength = sad::p2d::modulo(m_force_moment[index]);
        double rotation_tangential_modulo = w * forcemomentlength;
        tangential *= rotation_tangential_modulo;
        tangential += t;

        if (sad::non_fuzzy_zero(p2d::modulo(tangential)) && !b->weight().isInfinite())
        {
            tangential *= (1 - m_rotationfriction[index] / b->weight().value());
            // Compute new tangential velocity
            // What should we do here
            
            // Since we have no sliding friction force, a tangential part of velocity
            // won't change, so we must convert a tangential part to velocity. 
            tangential -= t;
            double wR = sad::p2d::scalar(tangential, tangentialUnit);
            double angular = wR / forcemomentlength;

            b->sheduleAngularVelocity(angular);
        }
    }
}
