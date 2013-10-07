#include "label.h"
#include "renderer.h"

DECLARE_SOBJ_INHERITANCE(Label,sad::BasicNode)
DECLARE_SOBJ_INHERITANCE(FormattedLabel,Label)

void Label::operator()(const sad::ResizeEvent & o)
{
	float old_width=(float)(m_renderer->settings().width());
	float old_height=(float)(m_renderer->settings().height());
	m_rend_point.setX(m_p.x() * o.new_width/old_width);
	m_rend_point.setY(m_p.y() * o.new_height/old_height);
}
Label::Label(sad::Renderer * renderer)
{
	m_renderer = renderer;
}
Label::Label(
		      LabelFont fnt,
		      const sad::String & str,
			  const pointf      & p,
			  sad::Renderer * renderer
			)
{
	m_renderer = renderer;
    m_fnt=fnt;
	m_str=str;
	m_p=p;
	m_rend_point=p;
}

Label::~Label()
{
}

void Label::render()
{
	m_fnt->render(m_str,m_rend_point);
}

FormattedLabel::Arg::~Arg()
{

}

FormattedLabel::FormattedLabel(sad::Renderer * renderer) : Label(renderer)
{
	m_update_interval = 200;
	m_created = true;
	m_timer.start();
}

void FormattedLabel::setFont(const sad::String & fnt, sad::Renderer * r)
{
	sad::BasicFont * font = r->fonts()->get(fnt);
	if (font)
		setFont(font);
}


void FormattedLabel::update()
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
	this->string() = str(stream);
	for(int i = 0; i < args.size(); i++)
	{
		delete args[i];
	}
}

void FormattedLabel::render()
{
	m_timer.stop();
	if (m_created || m_timer.elapsed() >= m_update_interval)	
	{
		m_timer.start();
		m_created = false;
		this->update();
	}
	this->Label::render();
}

FormattedLabel::~FormattedLabel()
{
	for(unsigned int i = 0; i < m_args.size(); i++)
	{
		delete m_args[i];
	}
}


FormattedLabel * FormattedLabel::argFPS(sad::Renderer * r)
{
	return this->castedConstArg<int>(r, &sad::Renderer::fps);
}
