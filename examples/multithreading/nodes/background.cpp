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

DECLARE_SOBJ_INHERITANCE(nodes::Background, sad::SceneNode);

// ========================================== PUBLIC METHODS ==========================================

nodes::Background::Background()
{
    m_base_label = new sad::Sprite2D();
    m_rainbow_label1 = new sad::Sprite2D();
    m_rainbow_label2 = new sad::Sprite2D();

    m_base_label->addRef();
    m_rainbow_label1->addRef();
    m_rainbow_label2->addRef();
}

nodes::Background::~Background()
{
    m_base_label->delRef();
    m_rainbow_label1->delRef();
    m_rainbow_label2->delRef();
}


void nodes::Background::render()
{

    //glColorMask(false, false, false, true);
    //glBlendFunc(GL_ZERO, GL_SRC_ALPHA);

    m_base_label->render();


    //glColorMask(true, true, true, true);
    glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);

    m_rainbow_label1->render();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void nodes::Background::rendererChanged()
{
    m_base_label->rendererChanged();
    m_rainbow_label1->rendererChanged();
    m_rainbow_label2->rendererChanged();

    m_base_label->setTreeName("");
    m_base_label->set("label_base");

    double x = POSITION_X;
    double y = POSITION_Y;
    m_base_label->setArea(sad::Rect2D(x, y, x + WIDTH, y + HEIGHT));

    m_rainbow_label1->setTreeName("");
    m_rainbow_label1->set("label_rainbow");
    m_rainbow_label1->setArea(sad::Rect2D(x, y, x + WIDTH, y + HEIGHT));

    m_main_texture_rect = m_rainbow_label1->textureCoordinates();
}

void nodes::Background::setScene(sad::Scene * scene)
{
    m_base_label->setScene(scene);
    m_rainbow_label1->setScene(scene);
    m_rainbow_label2->setScene(scene);
}

// ========================================== PRIVATE METHODS ==========================================

nodes::Background::Background(const nodes::Background& b) : m_base_label(NULL), m_rainbow_label1(NULL), m_rainbow_label2(NULL)
{
    
}

nodes::Background& nodes::Background::operator=(const nodes::Background& b)
{
    return *this;
}