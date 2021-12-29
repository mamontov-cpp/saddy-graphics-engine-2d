#include "cameramovement.h"

#include "game.h"

#include <animations/animationsblinking.h>

DECLARE_SOBJ(CameraMovement)

// ReSharper disable once CppPossiblyUninitializedMember
CameraMovement::CameraMovement(Game* game)
: m_move_left_boundary(350), m_move_right_boundary(500), m_max_shift_time(20000), m_game(game), m_backgrounds{nullptr, nullptr, nullptr}, m_bg_tiles{ nullptr, nullptr, nullptr },
  m_fg_tiles{ nullptr, nullptr, nullptr },
  m_arrow_in_scene(false),
  m_scene(nullptr),
  m_locked(false)// NOLINT(cppcoreguidelines-pro-type-member-init)
{
    m_arrow_sprite = new sad::Sprite2D();
    m_arrow_sprite->addRef();

    m_blinking = new sad::animations::Blinking();
    m_blinking->addRef();
    m_blinking->setTime(2000);
    m_blinking->setFrequency(12);
    m_blinking->setLooped(false);
    
}

CameraMovement::~CameraMovement()
{
    m_arrow_sprite->delRef();
    m_blinking->delRef();
}

void CameraMovement::setMoveLeftBoundary(double l)
{
    m_move_left_boundary = l;
}

double CameraMovement::moveLeftBoundary() const
{
    return m_move_left_boundary;
}

void CameraMovement::setMoveRightBoundary(double l)
{
    m_move_right_boundary = l;
}

double CameraMovement::moveRightBoundary() const
{
    return m_move_right_boundary;
}

void CameraMovement::setMaxShiftTime(double time)
{
    m_max_shift_time = time;
}

double CameraMovement::maxShiftTime() const
{
    return m_max_shift_time;
}

void CameraMovement::setArrowPosition(const sad::Point2D& p)
{
    m_arrow_position = p;
    m_arrow_sprite->moveTo(m_arrow_position);
}

const sad::Point2D& CameraMovement::arrowPosition() const
{
    return m_arrow_position;
}

static sad::String background_list[3] = {{"Background"}, {"Background2"}, {"Background3"} };
static sad::String bg_list[3] = { {"BgTile"}, {"BgTile2"}, {"BgTile3"} };
static sad::String fg_list[3] = { {"FgTile"}, {"FgTile2"}, {"FgTile3"} };

void CameraMovement::init()
{
    sad::db::Database* db = m_game->rendererForMainThread()->database("gamescreen");

    m_scene = db->objectByName<sad::Scene>("particles");
    m_arrow_sprite->setScene(m_scene);
    m_arrow_sprite->setTreeName(m_game->rendererForMainThread(), "");
    m_arrow_sprite->set("enemies_list/signArrowng");
    m_arrow_sprite->moveTo(m_arrow_position);

    m_arrow_in_scene = false;
    m_locked = false;

    std::function<void(sad::String[3], sad::Sprite2D*[3])> try_fill_vector = [db](sad::String a[3],  sad::Sprite2D* v[3]) {
        for (size_t i = 0; i < 3; i++)
        {
            sad::Sprite2D* s = db->objectByName<sad::Sprite2D>(a[i]);
            if (s)
            {
                v[i] =  s;
            }
            else
            {
                v[i] = 0;
            }
        }
    };
    try_fill_vector(background_list, m_backgrounds);
    try_fill_vector(bg_list, m_bg_tiles);
    try_fill_vector(fg_list, m_fg_tiles);
}

void CameraMovement::process()
{
    if (m_game)
    {
        if (!m_game->isPaused() && m_game->isNowPlaying() && !m_game->isWinning())
        {
            if (!m_locked)
            { 
                correctOffsetAndBackgrounds();
            }

            if (m_arrow_in_scene)
            {
                sad::Renderer* mr = m_game->rendererForMainThread();
                m_arrow_sprite->moveTo(m_arrow_position - mr->globalTranslationOffset());
            }
        }
    }
}

void CameraMovement::showArrow()
{
    if (m_game && !m_arrow_in_scene)
    {
        if (!m_game->isPaused() && m_game->isNowPlaying() && !m_game->isWinning())
        {
            sad::Renderer* r = m_game->rendererForMainThread();
            m_arrow_in_scene  = true;
            m_arrow_sprite->moveTo(m_arrow_position - r->globalTranslationOffset());
            m_scene->addNode(m_arrow_sprite);

            m_game->playSound("next_part");

            sad::animations::Instance* i = new sad::animations::Instance();
            i->setAnimation(m_blinking);
            i->setObject(m_arrow_sprite);
            i->end([=]() {
                this->m_arrow_in_scene = false;
                this->m_scene->removeNode(m_arrow_sprite);
            });
            r->animations()->add(i);
        }
    }
}

void CameraMovement::lock()
{
    m_locked = true;
}

void CameraMovement::unlock()
{
    m_locked = false;
}

bool CameraMovement::isLocked() const
{
    return m_locked;
}


void CameraMovement::moveCameraLeft(double offset, bool change_global_offset)
{
    sad::Renderer* r = m_game->rendererForMainThread();
    double w = static_cast<double>(r->settings().width());
    double ox = r->globalTranslationOffset().x();

    if (change_global_offset)
    {
        double oy = r->globalTranslationOffset().y();
        r->setGlobalTranslationOffset(sad::Point2D(ox + offset, oy));
    }

    sad::Point2D  dp(offset, 0);
    for (auto& fg_tile : m_fg_tiles)
    {
        fg_tile->moveBy(dp);
    }

    // Swap positions of tiles if need to
    sad::Sprite2D* s = m_backgrounds[2];
    ox = r->globalTranslationOffset().x();
    dp.setX(w * (-2.0));
    if (s->area().p0().x() + ox > w)
    {
        s->moveBy(dp);
#define LSHIFT(ARR)  { sad::Sprite2D* m = (ARR)[2]; (ARR)[2] = (ARR)[1]; (ARR)[1] = (ARR)[0]; (ARR)[0] = m;  }
        LSHIFT(m_backgrounds);

        s = m_bg_tiles[2];
        s->moveBy(dp);
        LSHIFT(m_bg_tiles);
    }
    for (int i = 2; i > -1; i--)
    {
        s = m_fg_tiles[i];
        double local_x = s->area().p0().x();
        if (local_x + ox > w)
        {
            double min_x = m_fg_tiles[0]->area().p0().x() - w;
            s->moveBy(sad::Point2D(min_x - local_x, 0));
            LSHIFT(m_fg_tiles);
        }
    }
}

void CameraMovement::moveCameraRight(double offset, bool change_global_offset)
{
    sad::Renderer* r = m_game->rendererForMainThread();
    double w = static_cast<double>(r->settings().width());
    double ox = r->globalTranslationOffset().x();

    if (change_global_offset)
    {
        double oy = r->globalTranslationOffset().y();
        r->setGlobalTranslationOffset(sad::Point2D(ox - offset, oy));
    }

    sad::Point2D dp = sad::Point2D(offset, 0);
    for (auto& fg_tile : m_fg_tiles)
    {
        fg_tile->moveBy(dp);
    }

    // Swap positions of tiles if need to
    sad::Sprite2D* s = m_backgrounds[0];
    ox = r->globalTranslationOffset().x();
    dp.setX(w * 2.0);

    if (s->area().p2().x() + ox < 0)
    {
#define RSHIFT(ARR)  { sad::Sprite2D* m = (ARR)[0]; (ARR)[0] = (ARR)[1]; (ARR)[1] = (ARR)[2]; (ARR)[2] = m;  }

        s->moveBy(dp);
        RSHIFT(m_backgrounds);

        s = m_bg_tiles[0];
        s->moveBy(dp);
        RSHIFT(m_bg_tiles);

    }

    for (int i = 0; i < 3; i++) // NOLINT
    {
        s = m_fg_tiles[i];
        double local_x = s->area().p2().x();
        if (local_x + ox < 0)
        {
            double max_x = m_fg_tiles[2]->area().p2().x() + w;
            s->moveBy(sad::Point2D(max_x - local_x, 0));
            RSHIFT(m_fg_tiles);
        }
    }
}


void CameraMovement::correctOffsetAndBackgrounds()
{
    sad::Renderer* r = m_game->rendererForMainThread();
    double ox = r->globalTranslationOffset().x();
    double oy = r->globalTranslationOffset().y();
    double px = m_game->player()->middle().x();
    double w = static_cast<double>(r->settings().width());
    double dv = w / 2.0 / (m_max_shift_time / 1000.0) * (1.0 / r->fps());
    if (px + ox < m_move_left_boundary)
    {
        // nox + px = m_move_left_boundary
        double nox = m_move_left_boundary - px;
        if (nox <= 0)
        {
            double dx = nox - ox;
            // Set global offset
            if (fabs(nox - ox) > dv)
            {
                dx = dv;
                r->setGlobalTranslationOffset(sad::Point2D(ox + dv, oy));
            }
            else
            {
                r->setGlobalTranslationOffset(sad::Point2D(nox, oy));
            }

            // Move foreground tiles
            moveCameraLeft(dx * 0.125, false);
        }
    }
    if (px + ox > m_move_right_boundary)
    {
        // nox + px = m_move_right_boundary
        double nox = m_move_right_boundary - px;
        if (nox >= -1 * (m_game->maxLevelX() - r->settings().width()))
        {
            double dx = nox - ox;
            // Set global offset
            if (fabs(nox - ox) > dv)
            {
                dx = -dv;
                r->setGlobalTranslationOffset(sad::Point2D(ox - dv, oy));
            }
            else
            {
                r->setGlobalTranslationOffset(sad::Point2D(nox, oy));
            }

            moveCameraRight(dx * 0.125, false);
        }
    }
}

void exposeCameraMovement(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* camera_movement_binding = new sad::dukpp03::ClassBinding();

    camera_movement_binding->addMethod("setMoveLeftBoundary", sad::dukpp03::bind_method::from(&CameraMovement::setMoveLeftBoundary));
    camera_movement_binding->addMethod("moveLeftBoundary", sad::dukpp03::bind_method::from(&CameraMovement::moveLeftBoundary));
    camera_movement_binding->addMethod("setMoveRightBoundary", sad::dukpp03::bind_method::from(&CameraMovement::setMoveRightBoundary));
    camera_movement_binding->addMethod("moveRightBoundary", sad::dukpp03::bind_method::from(&CameraMovement::moveRightBoundary));
    camera_movement_binding->addMethod("setMaxShiftTime", sad::dukpp03::bind_method::from(&CameraMovement::setMaxShiftTime));
    camera_movement_binding->addMethod("maxShiftTime", sad::dukpp03::bind_method::from(&CameraMovement::maxShiftTime));
    camera_movement_binding->addMethod("setArrowPosition", sad::dukpp03::bind_method::from(&CameraMovement::setArrowPosition));
    camera_movement_binding->addMethod("arrowPosition", sad::dukpp03::bind_method::from(&CameraMovement::arrowPosition));
    camera_movement_binding->addMethod("showArrow", sad::dukpp03::bind_method::from(&CameraMovement::showArrow));

    ctx->addClassBinding("CameraMovement", camera_movement_binding);
}