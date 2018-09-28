#include <p2d/bouncesolver.h>
#include <p2d/world.h>

#include <log/log.h>


void sad::p2d::BounceSolver::SolverTask::destroy()
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


DECLARE_SOBJ(sad::p2d::BounceSolver);

sad::p2d::BounceSolver::BounceSolver()
: m_toi(0),
  m_inelastic_collisions(false),
  m_inelastic_collision_type(sad::p2d::BounceSolver::ICT_NO_INELASTIC_COLLISION),
  m_resilience{1.0, 1.0},
  m_rotationfriction{0.0, 0.0},
  m_shouldperformrotationfriction(true),
  m_recursion_limit(100),
  m_recursion_counter(0),
  m_collision_precision_step(0.1),
  m_max_solver_iterations(100)
{
    m_find = new sad::p2d::FindContactPoints();
    m_first = NULL;
    m_second = NULL;
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

        double time = 0;
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

bool sad::p2d::BounceSolver::inelasticBounceWithFixedSecondBody(sad::p2d::Body* b1, sad::p2d::Body* b2)
{
    m_first = b1;
    m_second = b2;

    double world_step = m_first->world()->timeStep();

    sad::Maybe<sad::p2d::BounceSolver::SolverTask> maybe_task = this->findBasicTaskForInelasticBounce();
    if (maybe_task.exists())
    {
        sad::p2d::BounceSolver::SolverTask task = maybe_task.value();
        if (sad::is_fuzzy_zero(task.PivotTime) && (task.PivotTime >= 0  && task.PivotTime <= world_step))
        {
            // Further compute TOI and perform inelastic bounce
            sad::p2d::SetOfPointsPair pairs;
            this->solveTOIFCP(task.First, task.FirstVelocity, task.Second, task.SecondVelocity, pairs);
            m_toi += task.PivotTime;

            if (!pairs.empty()
                &&  (m_toi > 0 || sad::is_fuzzy_zero(m_toi, COLLISION_PRECISION * 1000))
                && ((m_toi < world_step)  || sad::is_fuzzy_equal(m_toi, world_step)))
            {
                // TODO: compute collision position, insert new collisions into list, recompute other collisions
                sad::p2d::Body::CollisionData new_data = this->fixLocalInelasticCollision(task);
                task.destroy();
                size_t insert_position = this->insertDataIntoCollisionList(new_data);

                // TODO: insert collisions into list,
                // TODO: compute end position and fetch recompute list
                // TODO: recompute other collisions, if any, because we changed our speed.

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


size_t sad::p2d::BounceSolver::insertDataIntoCollisionList(const sad::p2d::Body::CollisionData& data)
{
    size_t result = 0;
    sad::p2d::Body::CollisionData& first_collision = m_first->collisions()[0];
    if (sad::is_fuzzy_equal(first_collision.TOI, data.TOI))
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
            first_collision.OwnSpeed = data.OwnSpeed;  // Oops, still should do something here, where the max formula?
            return result;
        }
    }
    for (size_t i = 1; i < m_first->collisions().size(); i++)
    {
         sad::p2d::Body::CollisionData& prev_collision = m_first->collisions()[i - 1];
         sad::p2d::Body::CollisionData& next_collision = m_first->collisions()[i];
         if (sad::is_fuzzy_equal(next_collision.TOI, data.TOI))
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

                 next_collision.PlatformSpeed = maxOrSum(next_collision.PlatformSpeed, data.PlatformSpeed);
                 next_collision.OwnSpeed = data.OwnSpeed;  // Oops, still should do something here, where the max formula?
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

sad::p2d::Body::CollisionData  sad::p2d::BounceSolver::fixLocalInelasticCollision(const sad::p2d::BounceSolver::SolverTask& task)
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
    sad::p2d::CollisionTest test;
    double iteration = 0;
    bool colliding = true;
    sad::p2d::Vector result_dposition(0.0, 0.0);
    do
    {
        sad::p2d::Vector dpos = speed_sum * m_collision_precision_step;
        first_toi_collision_shape->move(dpos);
        toi_collision_position += dpos;
        result_dposition += dpos;
        colliding = test.invoke(first_toi_collision_shape, second_toi_collision_shape);
        ++iteration;
    } while((iteration < m_max_solver_iterations) && colliding);

    // Compute new speed for first body

    sad::p2d::Vector new_speed = task.FirstVelocity +  normal * sad::p2d::scalar(task.FirstVelocity, normal);

    first_toi_collision_shape->deleteBlock(first_toi_collision_shape);
    second_toi_collision_shape->deleteBlock(second_toi_collision_shape);

    return { {m_second},  m_toi, effective_platform_speed, new_speed, toi_collision_position, result_dposition};
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

sad::Maybe<sad::p2d::BounceSolver::SolverTask> sad::p2d::BounceSolver::findBasicTaskForInelasticBounce()
{
    sad::p2d::CollisionShape* s1  = m_first->currentShape()->clone();
    sad::p2d::Vector cp1 = m_first->position();

    // Determine specific velocities and times, when bodies had collided
    double world_step = m_first->world()->timeStep();
    if (m_first->collisions().empty())
    {
        sad::p2d::Vector v = (m_first->positionAt(world_step) - cp1) / world_step;
        //sad::p2d::Vector pos = m_first->()
        m_first->collisions().add({
            {}, 0,  sad::p2d::Vector(0,0), v, cp1, sad::p2d::Vector(0,0)
        });
        m_first->collisions().add({
            {}, world_step,  sad::p2d::Vector(0,0), v, m_first->positionAt(world_step), sad::p2d::Vector(0,0)
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
)
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
)
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
)
{
    sad::p2d::CollisionTest test;

    sad::p2d::Vector first_velocity = data.PlatformSpeed + data.OwnSpeed;

    sad::Vector<sad::p2d::Body::CollisionData>& list = m_second->collisions();
    double toi = data.TOI;
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

bool sad::p2d::BounceSolver::bounceNormal(sad::p2d::Body* b1, sad::p2d::Body* b2)
{
    m_first = b1;
    m_second = b2;
    p2d::SetOfPointsPair pairs;
    this->solveTOIFCP(pairs);
    if (!pairs.empty() &&  (m_toi > 0 || sad::is_fuzzy_zero(m_toi, COLLISION_PRECISION * 1000)))
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

    double m1 = m_first->weight().value();
    double m2 = m_second->weight().value();

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

    this->resolveNormalSpeed(m_first, normalPart1, m_second, cachedNormal2, 0);
    if (!m_inelastic_collisions) {
        m_first->correctTangentialVelocity(normalPart1 + tangentialPart1);
    }
    m_first->correctPosition(m_av1 * m_toi);
    this->tryResolveFriction(m_first, tangentialPart1, normalPart1 - cachedNormal1, 0, pivot1);
    this->resolveNormalSpeed(m_second, normalPart2, m_first, cachedNormal1, 1);
    if (!m_inelastic_collisions) {
        m_second->correctTangentialVelocity(normalPart2 + tangentialPart2);
    }
    m_second->correctPosition(m_av2 * m_toi);
    this->tryResolveFriction(m_second, tangentialPart2, normalPart2 - cachedNormal2, 1, pivot2);
}


std::string sad::p2d::BounceSolver::dump()
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

sad::p2d::BounceSolver::InelasticCollisionType sad::p2d::BounceSolver::inelasticCollisionType() const
{
    return m_inelastic_collision_type;
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
        
    sad::p2d::Point center1 = m_first->currentShape()->center();

    sad::p2d::Point center2 = m_second->currentShape()->center();

    SL_INTERNAL(fmt::Format(tpl) << m_first->currentShape()->dump()
                                 << m_av1.x() << m_av1.y()
                                 << m_second->currentShape()->dump()
                                 << m_av2.x() << m_av2.y()
                                 << m_toi << m
               );
    }
}


static int boundspeed_solving_branches[3][3] =
{
    {0, 2, 2},
    {1, 0, 2},
    {1, 1, 0}
};

static int bound_solver_get_branch_index(sad::p2d::Body * b)
{
    int index = 0;
    if (b->fixed())
    {
        index = 2;
    }
    else
    {
        if (b->weight().isInfinite()) 
            index = 1;
    }
    return index;

}
void sad::p2d::BounceSolver::resolveNormalSpeed(
    sad::p2d::Body * b1, 
    sad::p2d::Vector & n1, 
    sad::p2d::Body * b2, 
    const sad::p2d::Vector & n2,
    int index
)
{
    sad::p2d::Vector vn1 = n1;
    n1 *= -1;
    int index1 = bound_solver_get_branch_index(b1);
    int index2 = bound_solver_get_branch_index(b2);

    // Get outcome from table
    int branch = boundspeed_solving_branches[index1][index2];

    if ( branch == 0 )
    {
        double m1 = b1->weight().value();
        double m2 = b2->weight().value();
        // If both object has infinite weight, we resolve them as 
        // equal weight objects
        if (b1->weight().isInfinite())
        {
            m1 = 1;
            m2 = 1;
        }
        n1 += ((vn1 * m1 + n2 * m2) / (m1 + m2)) * 2 ;
        n1 *= m_resilience[index];
        return;
    }
    // If only first object has infinite weight
    // his speed won't change
    if (branch == 1)
    {
        n1 *= -(m_resilience[index]);
        return;
    }
    if (branch == 2)
    {
        n1 =  (n2 * 2 - vn1);
        n1 *= m_resilience[index];
        return;
    }
    
}


void sad::p2d::BounceSolver::resetCoefficients()
{
    m_resilience[0] = 1.0;
    m_resilience[1] = 1.0;
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
            double w = wR / forcemomentlength;

            b->sheduleAngularVelocity(w);
        }
    }
}
