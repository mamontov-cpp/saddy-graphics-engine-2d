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

DECLARE_SOBJ_INHERITANCE(nodes::Background, sad::SceneNode);

// ========================================== PUBLIC METHODS ==========================================

nodes::Background::Background() : m_t(0)
{
    m_base_label = new sad::Sprite2D();
    m_rainbow_label1 = new sad::Sprite2D();
    m_rainbow_label2 = new sad::Sprite2D();
    m_outline_label = new sad::Sprite2D();

    m_base_label->addRef();
    m_rainbow_label1->addRef();
    m_rainbow_label2->addRef();
    m_outline_label->addRef();
}

nodes::Background::~Background()
{
    m_base_label->delRef();
    m_rainbow_label1->delRef();
    m_rainbow_label2->delRef();
    m_outline_label->delRef();
}


void nodes::Background::render()
{
    // Base label is used as mask
    m_base_label->render();

    // Now this is tricky, the rainbow transition effect is modelled 
    // as two sprites with varying widths
    glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);

    m_t += 1.0 / m_base_label->scene()->renderer()->fps();
    while (m_t > PERIOD)
    {
        m_t -= PERIOD;
    }

    double percents = m_t / PERIOD;

    double x = POSITION_X;
    double y = POSITION_Y;
    double width1 = static_cast<double>(static_cast<int>(WIDTH *  percents));

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
}

void nodes::Background::rendererChanged()
{
    m_base_label->rendererChanged();
    m_rainbow_label1->rendererChanged();
    m_rainbow_label2->rendererChanged();
    m_outline_label->rendererChanged();

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
    m_outline_label->setArea(sad::Rect2D(x, y, x + WIDTH, y + HEIGHT - 1));
}

void nodes::Background::setScene(sad::Scene* scene)
{
    m_base_label->setScene(scene);
    m_rainbow_label1->setScene(scene);
    m_rainbow_label2->setScene(scene);
    m_outline_label->setScene(scene);
}

// ========================================== PRIVATE METHODS ==========================================

nodes::Background::Background(const nodes::Background& b) : m_base_label(NULL), m_rainbow_label1(NULL), m_rainbow_label2(NULL)
{
    
}

nodes::Background& nodes::Background::operator=(const nodes::Background& b)
{
    return *this;
}