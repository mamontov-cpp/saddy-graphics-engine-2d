#include "formattedlabel.h"
#include "renderer.h"

DECLARE_SOBJ_INHERITANCE(sad::FormattedLabel,sad::Label)

sad::formattedlabel::Arg::~Arg()
{

}

sad::FormattedLabel::FormattedLabel() 
: sad::Label(nullptr, sad::Point2D(), "")
{
    m_update_interval = 200;
    m_created = true;
    m_timer.start();
}

void sad::FormattedLabel::update()
{
    fmt::TempFormatter<> stream = fmt::Format(m_format_string);
    std::vector<fmt::BasicFormatter<char>::Arg *> args;
    for(unsigned int i = 0; i < m_args.size(); i++)
    {
        fmt::BasicFormatter<char>::Arg * a = m_args[i]->substitute();
        args.push_back(a);
        stream.append(a);
    }
    stream.completeFormatting();
    this->setString(str(stream));
    for(size_t i = 0; i < args.size(); i++)
    {
        delete args[i];
    }
}

void sad::FormattedLabel::render()
{
    m_timer.stop();
    if (m_created || m_timer.elapsed() >= m_update_interval)	
    {
        m_timer.start();
        m_created = false;
        this->update();
    }
    this->sad::Label::render();
}

sad::FormattedLabel::~FormattedLabel()
{
    for(unsigned int i = 0; i < m_args.size(); i++)
    {
        delete m_args[i];
    }
}


sad::FormattedLabel * sad::FormattedLabel::argFPS(sad::Renderer * r)
{
    if (!r)
        r = sad::Renderer::ref();
    return this->castedConstArg<int>(r, &sad::Renderer::fps);
}
