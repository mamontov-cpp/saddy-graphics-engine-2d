#include "background.h"

#include <renderer.h>

#include <os/glheaders.h>

// X position for sprites
#define POSITION_X (400 - (384.0 / 2))
// Y position for sprites
#define POSITION_Y (600.0 - 300.0 + (124.0 / 2))
// A width
#define WIDTH 384
// A height
#define HEIGHT 124
// A period
#define PERIOD 3.0
// A second period for background
#define PERIOD2 8.0
// A third period for background
#define PERIOD3 14.0

DECLARE_SOBJ_INHERITANCE(nodes::Background, sad::SceneNode);

// ========================================== PUBLIC METHODS ==========================================

nodes::Background::Background(bool should_show) : m_t(0), m_t2(0), m_t3(0), m_show_label(should_show)
{
    m_base_label = new sad::Sprite2D();
    m_rainbow_label1 = new sad::Sprite2D();
    m_rainbow_label2 = new sad::Sprite2D();
    m_outline_label = new sad::Sprite2D();
    m_main_background = new sad::Sprite2D();
    m_hills_background = new sad::Sprite2D();
    m_inner_tile = new sad::Sprite2D();
    m_top_tile  = new sad::Sprite2D();

    m_base_label->addRef();
    m_rainbow_label1->addRef();
    m_rainbow_label2->addRef();
    m_outline_label->addRef();
    m_main_background->addRef();
    m_hills_background->addRef();
    m_inner_tile->addRef();
    m_top_tile->addRef();
}

nodes::Background::~Background()
{
    m_base_label->delRef();
    m_rainbow_label1->delRef();
    m_rainbow_label2->delRef();
    m_outline_label->delRef();
    m_main_background->delRef();
    m_hills_background->delRef();
    m_inner_tile->delRef();
    m_top_tile->delRef();
}


void nodes::Background::render()
{
    if (m_show_label) 
    {
        // Base label is used as mask
        m_base_label->render();
        // Now this is tricky, the rainbow transition effect is modelled 
        // as two sprites with varying widths
        glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    }

    double dt = 1.0 / m_base_label->scene()->renderer()->fps();
    m_t += dt;
    while (m_t > PERIOD)
    {
        m_t -= PERIOD;
    }
    m_t2 += dt;
    while (m_t2 > PERIOD2)
    {
        m_t2 -= PERIOD2;
    }
    m_t3 += dt;
    while (m_t3 > PERIOD3)
    {
        m_t3 -= PERIOD3;
    }

    double percents = m_t / PERIOD;

    double x = POSITION_X;
    double y = POSITION_Y;
    double width1 = static_cast<double>(static_cast<int>(WIDTH *  percents));

    if (m_show_label)
    {
        double tex_width = (1.0 - percents) * m_main_texture_rect.width();

        sad::Rect2D rect1(m_main_texture_rect.p0().x() + tex_width, m_main_texture_rect.p0().y(), m_main_texture_rect.p2().x(), m_main_texture_rect.p2().y());
        sad::Rect2D rect2(m_main_texture_rect.p0().x(), m_main_texture_rect.p0().y(), m_main_texture_rect.p0().x() + tex_width, m_main_texture_rect.p2().y());

        m_rainbow_label1->setTextureCoordinates(rect1);
        m_rainbow_label2->setTextureCoordinates(rect2);

        m_rainbow_label1->setArea(sad::Rect2D(x, y, x + width1, y + HEIGHT));
        m_rainbow_label2->setArea(sad::Rect2D(x + width1, y, x + WIDTH, y + HEIGHT));

        m_rainbow_label1->render();

        m_rainbow_label2->render();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_outline_label->render();

        glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
    }

    // Draw scrolling background
    double p0y = m_main_background->area().p0().y();
    double height = m_main_background->area().height();
    double width = m_main_background->area().width();
    double offset = - width * (m_t2 / PERIOD2);

    m_main_background->setArea(sad::Rect2D(offset, p0y, offset + width, p0y + height));
    m_main_background->render();

    m_main_background->setArea(sad::Rect2D(offset + width, p0y, offset + width * 2, p0y + height));
    m_main_background->render();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    offset = -width * m_t3 / PERIOD3;
    
    m_hills_background->setArea(sad::Rect2D(offset, p0y, offset + width, p0y + height));
    m_hills_background->render();

    m_hills_background->setArea(sad::Rect2D(offset + width, p0y, offset + width * 2, p0y + height));
    m_hills_background->render();


    // Do not offset more than three tiles, which we draw forward anyway
    while (offset < -120.0)
    {
        offset += 120.0;
    }

    m_inner_tile->setArea(sad::Rect2D(offset, 0, offset + 39, 39));
    m_top_tile->setArea(sad::Rect2D(offset, 40, offset + 39, 79));

    // Draw floor
    for(size_t i = 0; i < 24; i++) 
    {
        m_inner_tile->render();
        m_top_tile->render();

        m_top_tile->moveBy(sad::Point2D(40, 0));
        m_inner_tile->moveBy(sad::Point2D(40, 0));
    }
}

void nodes::Background::rendererChanged()
{
    m_base_label->rendererChanged();
    m_rainbow_label1->rendererChanged();
    m_rainbow_label2->rendererChanged();
    m_outline_label->rendererChanged();
    m_main_background->rendererChanged();
    m_hills_background->rendererChanged();
    m_inner_tile->rendererChanged();
    m_top_tile->rendererChanged();


    m_base_label->setTreeName("");
    m_base_label->set("label_base");

    double x = POSITION_X;
    double y = POSITION_Y;
    m_base_label->setArea(sad::Rect2D(x, y, x + WIDTH, y + HEIGHT));

    m_rainbow_label1->setTreeName("");
    m_rainbow_label1->set("label_rainbow");
    m_rainbow_label1->setArea(sad::Rect2D(x, y, x + WIDTH, y + HEIGHT));

    m_rainbow_label2->setTreeName("");
    m_rainbow_label2->set("label_rainbow");
    m_rainbow_label2->setArea(sad::Rect2D(x, y, x + WIDTH, y + HEIGHT));

    m_main_texture_rect = m_rainbow_label1->textureCoordinates();

    m_outline_label->setTreeName("");
    m_outline_label->set("label_outline");
    m_outline_label->setArea(sad::Rect2D(x, y, x + WIDTH, y + HEIGHT));

    m_main_background->setTreeName("");
    m_main_background->set("backgrounds_list/set3_backgroundng");
    m_main_background->setArea(sad::Rect2D(0, 0, 800, 600));

    m_hills_background->setTreeName("");
    m_hills_background->set("backgrounds_list/set3_hillsng");
    m_hills_background->setArea(sad::Rect2D(0, 0, 800, 600));

    m_inner_tile->setTreeName("");
    m_inner_tile->set("tiles_list/tileBrown_18ng");
    m_inner_tile->setArea(sad::Rect2D(0, 0, 32, 32));

    m_top_tile->setTreeName("");
    m_top_tile->set("tiles_list/tileBrown_02ng");
    m_top_tile->setArea(sad::Rect2D(0, 33, 32, 65));
}

void nodes::Background::setScene(sad::Scene* scene)
{
    m_base_label->setScene(scene);
    m_rainbow_label1->setScene(scene);
    m_rainbow_label2->setScene(scene);
    m_outline_label->setScene(scene);
    m_main_background->setScene(scene);
    m_hills_background->setScene(scene);
    m_inner_tile->setScene(scene);
    m_top_tile->setScene(scene);
}

// ========================================== PRIVATE METHODS ==========================================

nodes::Background::Background(const nodes::Background& b) : m_base_label(NULL), m_rainbow_label1(NULL), m_rainbow_label2(NULL), m_t(0), m_t2(0), m_t3(0), m_show_label(false)
{
    
}

nodes::Background& nodes::Background::operator=(const nodes::Background& b)
{
    return *this;
}