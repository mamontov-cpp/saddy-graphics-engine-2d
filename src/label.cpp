#include "label.h"
#include "geometry2d.h"
#include "renderer.h"
#include "sadmutex.h"
#include "sadscopedlock.h"
#include "renderer.h"
#include "glcontext.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbmethodpair.h"

#include "os/glfontgeometries.h"
#include "fontshaderfunction.h"

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "opticksupport.h"

DECLARE_SOBJ_INHERITANCE(sad::Label,sad::SceneNode)


//#define DEBUG_FORMATTED_RENDERING 1

sad::Label::Label() :
m_string(""),
m_maximal_line_width(0), 
m_point(0, 0), m_angle(0), 
m_size(20), m_linespacing_ratio(1.0),
m_color(0, 0, 0, 0),
m_overflow_strategy(sad::Label::OverflowStrategy::LOS_VISIBLE),
m_break_text(sad::Label::BreakText::LBT_NORMAL),
m_text_ellipsis_position(sad::Label::TextEllipsisPosition::LTEP_MIDDLE),
m_maximum_lines(0),
m_rendered_chars(0),
m_overflow_strategy_for_lines(sad::Label::OverflowStrategy::LOS_VISIBLE),
m_text_ellipsis_position_for_lines(sad::Label::TextEllipsisPosition::LTEP_MIDDLE),
m_formatted(false),
m_computed_rendering_string(false),
m_computed_rendering_point(false),
m_geometries(new sad::os::GLFontGeometries()),
m_geometries_dirty(true),
m_line_shader_function(nullptr)
{
    PROFILER_EVENT;
    m_geometries->addRef();
}

sad::Label::Label(
    sad::Font *  font,
    const sad::Point2D  & point,
    const sad::String & string
) : 
m_string(string), 
m_maximal_line_width(0), m_point(point),  m_angle(0), 
m_size(20), m_linespacing_ratio(1.0),
m_color(0, 0, 0, 0),
m_overflow_strategy(sad::Label::OverflowStrategy::LOS_VISIBLE),
m_break_text(sad::Label::BreakText::LBT_NORMAL),
m_text_ellipsis_position(sad::Label::TextEllipsisPosition::LTEP_MIDDLE),
m_maximum_lines(0),
m_rendered_chars(0),
m_overflow_strategy_for_lines(sad::Label::OverflowStrategy::LOS_VISIBLE),
m_text_ellipsis_position_for_lines(sad::Label::TextEllipsisPosition::LTEP_MIDDLE),
m_formatted(false),
m_computed_rendering_string(false),
m_computed_rendering_point(false),
m_geometries(new sad::os::GLFontGeometries()),
m_geometries_dirty(true),
m_line_shader_function(nullptr)
{
    PROFILER_EVENT;
    m_geometries->addRef();
    m_font.attach(font);
    recomputeRenderedString();
}

sad::Label::Label(
    const sad::String &  font,
    const sad::Point2D  & point,
    const sad::String & string,
    const sad::String & tree
) :
m_string(string), 
m_maximal_line_width(0), m_point(point),  m_angle(0), 
m_size(20), m_linespacing_ratio(1.0),
m_color(0, 0, 0, 0),
m_overflow_strategy(sad::Label::OverflowStrategy::LOS_VISIBLE),
m_break_text(sad::Label::BreakText::LBT_NORMAL),
m_text_ellipsis_position(sad::Label::TextEllipsisPosition::LTEP_MIDDLE),
m_maximum_lines(0),
m_rendered_chars(0),
m_overflow_strategy_for_lines(sad::Label::OverflowStrategy::LOS_VISIBLE),
m_text_ellipsis_position_for_lines(sad::Label::TextEllipsisPosition::LTEP_MIDDLE),
m_formatted(false),
m_computed_rendering_string(false),
m_computed_rendering_point(false),
m_geometries(new sad::os::GLFontGeometries()),
m_geometries_dirty(true),
m_line_shader_function(nullptr)
{
    PROFILER_EVENT;
    m_geometries->addRef();
    m_font.setTree(nullptr, tree);
    m_font.setPath(font);
    recomputeRenderedString();
}

void sad::Label::setTreeName(sad::Renderer* r, const sad::String& tree_name)
{
    PROFILER_EVENT;
    m_font.setTree(r, tree_name);
    m_font_cache.setTree(r, tree_name);
    m_geometries_dirty = true;
}

void sad::Label::regions(sad::Vector<sad::Rect2D> & r)
{
    PROFILER_EVENT;
    r << m_cached_region;
}

static sad::db::schema::Schema* LabelBasicSchema = nullptr;

static sad::Mutex LabelBasicSchemaInit;

sad::db::schema::Schema* sad::Label::basicSchema()
{
    PROFILER_EVENT;
    if (LabelBasicSchema == nullptr)
    {
        LabelBasicSchemaInit.lock();
        if (LabelBasicSchema == nullptr)
        {
            LabelBasicSchema = new sad::db::schema::Schema();
            LabelBasicSchema->addParent(sad::SceneNode::basicSchema());
            LabelBasicSchema->add(
                "font", 
                new sad::db::MethodPair<sad::Label, sad::String>(
                    &sad::Label::fontName,
                    &sad::Label::setFontName
                )
            );
            LabelBasicSchema->add(
                "fontsize", 
                new sad::db::MethodPair<sad::Label, unsigned int>(
                    &sad::Label::size,
                    &sad::Label::setSize
                )
            );
            LabelBasicSchema->add(
                "linespacing", 
                new sad::db::MethodPair<sad::Label, float>(
                    &sad::Label::lineSpacingRatio,
                    &sad::Label::setLineSpacingRatio
                )
            );
            LabelBasicSchema->add(
                "angle", 
                new sad::db::MethodPair<sad::Label, double>(
                    &sad::Label::angle,
                    &sad::Label::setAngle
                )
            );
            LabelBasicSchema->add(
                "area", 
                new sad::db::MethodPair<sad::Label, sad::Rect2D>(
                    &sad::Label::area,
                    &sad::Label::setArea
                )
            );
            LabelBasicSchema->add(
                "text", 
                new sad::db::MethodPair<sad::Label, sad::String>(
                    &sad::Label::string,
                    &sad::Label::setString
                )
            );
            void (sad::Label::*p)(const sad::AColor&)= &sad::Label::setColor;
            LabelBasicSchema->add(
                "color", 
                new sad::db::MethodPair<sad::Label, sad::AColor>(
                    &sad::Label::color,
                    p
                )
            );

            sad::db::Property* mlw_property = new sad::db::MethodPair<sad::Label, unsigned int>(
                &sad::Label::maximalLineWidth,
                &sad::Label::setMaximalLineWidth
            );
            mlw_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
            LabelBasicSchema->add("maximallinewidth", mlw_property);

            sad::db::Property* os_property = new sad::db::MethodPair<sad::Label, unsigned int>(
                &sad::Label::overflowStrategyAsIndex,
                &sad::Label::setOverflowStrategyFromIndex
            );
            os_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
            LabelBasicSchema->add("overflowstrategy", os_property);

             sad::db::Property* bt_property = new sad::db::MethodPair<sad::Label, unsigned int>(
                &sad::Label::breakTextAsIndex,
                &sad::Label::setBreakTextFromIndex
            );
            bt_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
            LabelBasicSchema->add("breaktext", bt_property);

            sad::db::Property* tep_property = new sad::db::MethodPair<sad::Label, unsigned int>(
                &sad::Label::textEllipsisAsIndex,
                &sad::Label::setTextEllipsisPositionAsIndex
            );
            tep_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
            LabelBasicSchema->add("textellipsisposition", tep_property);


            sad::db::Property* mlines_property = new sad::db::MethodPair<sad::Label, unsigned int>(
                &sad::Label::maximalLinesCount,
                &sad::Label::setMaximalLinesCount
            );
            mlines_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
            LabelBasicSchema->add("maximallinescount", mlines_property);

            sad::db::Property* oslines_property = new sad::db::MethodPair<sad::Label, unsigned int>(
                &sad::Label::overflowStrategyForLinesAsIndex,
                &sad::Label::setOverflowStrategyForLinesFromIndex
            );
            oslines_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
            LabelBasicSchema->add("overflowstrategyforlines", oslines_property);

            sad::db::Property* teplines_property = new sad::db::MethodPair<sad::Label, unsigned int>(
                &sad::Label::textEllipsisForLinesAsIndex,
                &sad::Label::setTextEllipsisPositionForLinesAsIndex
            );
            teplines_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
            LabelBasicSchema->add("textellipsispositionforlines", teplines_property);

            sad::db::Property* hasformatting_property = new sad::db::MethodPair<sad::Label, bool>(
                &sad::Label::hasFormatting,
                &sad::Label::setHasFormatting
                );
            hasformatting_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(false));
            LabelBasicSchema->add("hasformatting", hasformatting_property);


            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(LabelBasicSchema);
        }
        LabelBasicSchemaInit.unlock();
    }
    return LabelBasicSchema;
}

sad::db::schema::Schema* sad::Label::schema() const
{
    PROFILER_EVENT;
    return sad::Label::basicSchema();
}

void sad::Label::render()
{
    PROFILER_EVENT;
    sad::ScopedLock recompute_string_lock(&m_recompute_string_lock);
    sad::ScopedLock recompute_point_lock(&m_recompute_point_lock);

    sad::Font * font = m_font.get();
    if (!font)
        return;

    font->setSize(m_size);
    font->setColor(m_color);
    font->setLineSpacingRatio(m_linespacing_ratio);

    if (!m_computed_rendering_string)
    {
        recomputeRenderedString(false);
    }
    if (!m_computed_rendering_point)
    {
        recomputeRenderingPoint(false);
    }

    if (!m_computed_rendering_string || !m_computed_rendering_point)
    {
        return;
    }

    sad::Renderer* renderer = this->renderer();
    if (!renderer  || (m_size < 0))
    {
        return;
    }

    if (renderer->context()->isOpenGL3compatible())
    {
       if (m_geometries_dirty)
       {
           m_rendered_lines.clear();
           m_geometries->startRebuilding();
           if (m_formatted)
           {
               buildGeometriesWithFormatting(font);
           }
           else
           {
               buildGeometriesWithoutFormatting(font);
           }
           m_geometries_dirty = false;
       }
        // Render geometries
       sad::FontShaderFunction* font_function = static_cast<sad::FontShaderFunction*>(m_shader_function);
       if (!font_function)
       {
           font_function = renderer->defaultFontShaderFunction();
       }
       m_geometries->draw(this, m_color, m_center, m_angle, font_function);
       // Render lines
       sad::FontShaderFunction* line_function = m_line_shader_function;
       if (!line_function)
       {
           line_function = renderer->defaultFontLineShaderFunction();
       }
       line_function->apply(this, &m_color, m_center, m_angle);
       for (size_t i  = 0; i  < m_rendered_lines.size(); ++i)
       {
           if (m_rendered_lines[i].Color.exists())
           {
               line_function->setColor(m_rendered_lines[i].Color.value());
           }
           else
           {
               line_function->setColor(m_color);
           }
           renderer->render()->line(this->scene(), m_rendered_lines[i].P1, m_rendered_lines[i].P2, m_color, line_function, true);
       }
       line_function->disable();
    }
    else
    {
        m_geometries_dirty = false;
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(static_cast<GLfloat>(m_center.x()), static_cast<GLfloat>(m_center.y()), 0.0f);
        glRotatef(static_cast<GLfloat>(m_angle / M_PI * 180.0f), 0.0f, 0.0f, 1.0f);

        if (m_formatted)
        {
            renderWithFormatting(font);
        }
        else
        {
            renderWithoutFormatting(font);
        }
        glPopMatrix();
    }
}


void sad::Label::rendererChanged()
{
    PROFILER_EVENT;
    sad::Renderer* new_renderer = this->renderer();
    if (m_font.dependsOnRenderer())
    {
        m_font.setRenderer(new_renderer);
        m_font_cache.setRenderer(new_renderer);
    }
    if (m_geometries->renderer())
    {
        m_geometries->renderer()->removeFontGeometries(m_geometries);
    }
    if (new_renderer)
    {
        new_renderer->addFontGeometries(m_geometries);
    }
    m_geometries_dirty = true;
}

void sad::Label::setArea(const sad::Rect2D & r)
{
    PROFILER_EVENT;
    m_point = r[0];
    recomputeRenderingPoint();
}

sad::Rect2D sad::Label::area() const
{
    PROFILER_EVENT;
    // Preserve linkage to a renderer
    if (m_computed_rendering_point == false)
    {
        const_cast<sad::Label *>(this)->recomputeRenderingPoint();
    }
    sad::Size2D  size(m_half_padding.x() * (-2), m_half_padding.y() * 2);
    if (m_computed_rendering_point == false)
    {
        size.Width = 0;
        size.Height = 0;
    }

    sad::Rect2D  result(m_point.x(), 
                        m_point.y(), 
                        m_point.x() + size.Width,
                        m_point.y() - size.Height);
    return result;
}

sad::Rect2D sad::Label::region() const
{
    PROFILER_EVENT;
    sad::Rect2D result = this->area();
    sad::rotate(result, static_cast<float>(m_angle));
    return result;
}



sad::Label::~Label()
{
    PROFILER_EVENT;
    clearFontsCache();
    m_geometries->delRef();
}

void sad::Label::setScene(sad::Scene * scene)
{
    PROFILER_EVENT;
    this->sad::SceneNode::setScene(scene);
    if (m_font.dependsOnRenderer() && scene)
    {
        m_font.setRenderer(scene->renderer());
        reloadFont();
    }
    if (m_geometries->renderer())
    {
        m_geometries->renderer()->removeFontGeometries(m_geometries);
    }
    if (scene)
    {
        if (scene->renderer())
        {
            scene->renderer()->addFontGeometries(m_geometries);
        }
    }
    m_geometries_dirty = true;
}

bool sad::Label::canBeRotated() const
{
    PROFILER_EVENT;
    return true;
}

void sad::Label::rotate(double angle)
{ 
    this->setAngle(this->angle() + angle);
}

void sad::Label::setPoint(const sad::Point2D & point)
{
    PROFILER_EVENT;
    m_point = point;
    recomputeRenderingPoint();
}

void sad::Label::setAngle(double angle)
{
    PROFILER_EVENT;
    m_angle = angle;
    m_cached_region = this->region();
}

void sad::Label::setFontName(const sad::String & name)
{
    PROFILER_EVENT;
    m_font.setPath(name);
    reloadFont();
    m_geometries_dirty = true;
}

void sad::Label::setFont(sad::Font * font)
{
    PROFILER_EVENT;
    m_font.attach(font);
    recomputeRenderedString();
    m_geometries_dirty = true;
}

void sad::Label::setFont(const sad::String & name, sad::Renderer * r, const sad::String & tree)
{
    PROFILER_EVENT;
    if (!r)
        r = sad::Renderer::ref();
    m_font.setPath(name);
    m_font.setTree(r, tree);
    m_geometries_dirty = true;
    recomputeRenderedString();
}

void sad::Label::setString(const sad::String & string)
{
    PROFILER_EVENT;
    m_string = string;
    m_geometries_dirty = true;
    recomputeRenderedString();
}

void sad::Label::setSize(unsigned int size)
{
    PROFILER_EVENT;
    m_size = size;
    m_geometries_dirty = true;
    recomputeRenderedString();
}

float sad::Label::builtinLineSpacing() const
{
    PROFILER_EVENT;
    // 0.1f is placed to avoid division by zero
    sad::Font * font = m_font.get();
    if (!font)
        return 0.1f;
    font->setSize(m_size);
    return font->builtinLineSpacing();
}

void sad::Label::setLineSpacing(float spacing)
{
    PROFILER_EVENT;
    setLineSpacingRatio(spacing / this->builtinLineSpacing());
}

void sad::Label::setLineSpacingRatio(float ratio)
{
    PROFILER_EVENT;
    m_linespacing_ratio = ratio;
    m_geometries_dirty = true;
    recomputeRenderedString();
}

void sad::Label::setTreeName(const sad::String & tree_name)
{
    PROFILER_EVENT;
    m_font.setTree(m_font.renderer(), tree_name);
    m_font_cache.setTree(m_font.renderer(), tree_name);
    m_geometries_dirty = true;
    recomputeRenderedString();
}

void sad::Label::setMaximalLineWidth(unsigned int width)
{
    PROFILER_EVENT;
    m_maximal_line_width = width;
    m_geometries_dirty = true;
    recomputeRenderedString();
}

unsigned int sad::Label::maximalLineWidth() const
{
    PROFILER_EVENT;
    return m_maximal_line_width;
}

void sad::Label::setOverflowStrategy(sad::Label::OverflowStrategy s)
{
    PROFILER_EVENT;
    m_overflow_strategy = s;
    m_geometries_dirty = true;
    recomputeRenderedString();
}

void sad::Label::setOverflowStrategyFromIndex(unsigned int s)
{
    PROFILER_EVENT;
	if (s > static_cast<unsigned int>(sad::Label::OverflowStrategy::LOS_ELLIPSIS))
    {
        s = static_cast<unsigned int>(sad::Label::OverflowStrategy::LOS_ELLIPSIS);
    }
    setOverflowStrategy(static_cast<sad::Label::OverflowStrategy>(s));
}

sad::Label::OverflowStrategy sad::Label::overflowStrategy() const
{
    PROFILER_EVENT;
    return m_overflow_strategy;
}

unsigned int sad::Label::overflowStrategyAsIndex() const
{
    PROFILER_EVENT;
    return static_cast<unsigned int>(overflowStrategy());
}

void sad::Label::setBreakText(sad::Label::BreakText value)
{
    PROFILER_EVENT;
    m_break_text = value;
    m_geometries_dirty = true;
    recomputeRenderedString();
}


void sad::Label::setBreakTextFromIndex(unsigned int value)
{
    PROFILER_EVENT;
    if (value > static_cast<unsigned int>(sad::Label::BreakText::LBT_BREAK_WORD))
    {
        value = static_cast<unsigned int>(sad::Label::BreakText::LBT_BREAK_WORD);
    }
    setBreakText(static_cast<sad::Label::BreakText>(value));
}

sad::Label::BreakText sad::Label::breakText() const
{
    PROFILER_EVENT;
    return m_break_text;
}

unsigned int sad::Label::breakTextAsIndex() const
{
    PROFILER_EVENT;
    return static_cast<unsigned int>(breakText());
}

void sad::Label::setTextEllipsisPosition(sad::Label::TextEllipsisPosition value)
{
    PROFILER_EVENT;
    m_text_ellipsis_position = value;
    m_geometries_dirty = true;
    recomputeRenderedString();
}

void sad::Label::setTextEllipsisPositionAsIndex(unsigned int value)
{
    PROFILER_EVENT;
    if (value > static_cast<unsigned int>(sad::Label::TextEllipsisPosition::LTEP_END))
    {
        value = static_cast<unsigned int>(sad::Label::TextEllipsisPosition::LTEP_END);
    }
    m_geometries_dirty = true;
    setTextEllipsisPosition(static_cast<sad::Label::TextEllipsisPosition>(value));
}

sad::Label::TextEllipsisPosition sad::Label::textEllipsis() const
{
    PROFILER_EVENT;
    return m_text_ellipsis_position;
}

unsigned int sad::Label::textEllipsisAsIndex() const
{
    PROFILER_EVENT;
    return static_cast<unsigned int>(textEllipsis());
}

void sad::Label::setMaximalLinesCount(unsigned int line_count)
{
    PROFILER_EVENT;
    m_maximum_lines = line_count;
    m_geometries_dirty = true;
    recomputeRenderedString();
}

unsigned int sad::Label::maximalLinesCount() const
{
    PROFILER_EVENT;
    return m_maximum_lines;
}

void sad::Label::setOverflowStrategyForLines(sad::Label::OverflowStrategy s)
{
    PROFILER_EVENT;
    m_overflow_strategy_for_lines = s;
    m_geometries_dirty = true;
    recomputeRenderedString();
}

void sad::Label::setOverflowStrategyForLinesFromIndex(unsigned int s)
{
    PROFILER_EVENT;
    if (s > static_cast<unsigned int>(sad::Label::OverflowStrategy::LOS_ELLIPSIS))
    {
        s = static_cast<unsigned int>(sad::Label::OverflowStrategy::LOS_ELLIPSIS);
    }
    m_geometries_dirty = true;
    setOverflowStrategyForLines(static_cast<sad::Label::OverflowStrategy>(s));    
}

sad::Label::OverflowStrategy sad::Label::overflowStrategyForLines() const
{
    PROFILER_EVENT;
    return m_overflow_strategy_for_lines;
}

unsigned int sad::Label::overflowStrategyForLinesAsIndex() const
{
    PROFILER_EVENT;
    return static_cast<unsigned int>(overflowStrategyForLines());
}

void sad::Label::setTextEllipsisPositionForLines(sad::Label::TextEllipsisPosition value)
{
    PROFILER_EVENT;
    m_text_ellipsis_position_for_lines = value;
    m_geometries_dirty = true;
    recomputeRenderedString();
}

void sad::Label::setTextEllipsisPositionForLinesAsIndex(unsigned int value)
{
    PROFILER_EVENT;
    if (value > static_cast<unsigned int>(sad::Label::TextEllipsisPosition::LTEP_END))
    {
        value = static_cast<unsigned int>(sad::Label::TextEllipsisPosition::LTEP_END);
    }
    m_geometries_dirty = true;
    setTextEllipsisPositionForLines(static_cast<sad::Label::TextEllipsisPosition>(value));
}

sad::Label::TextEllipsisPosition sad::Label::textEllipsisForLines() const
{
    PROFILER_EVENT;
    return m_text_ellipsis_position_for_lines;
}

unsigned int sad::Label::textEllipsisForLinesAsIndex() const
{
    PROFILER_EVENT;
    return static_cast<unsigned int>(textEllipsisForLines());
}

bool sad::Label::hasFormatting() const
{
    PROFILER_EVENT;
    return m_formatted;
}

void sad::Label::setHasFormatting(bool value)
{
    PROFILER_EVENT;
    m_formatted = value;
    m_geometries_dirty = true;
    recomputeRenderedString();
}

void sad::Label::makeFormatted()
{
    PROFILER_EVENT;
    setHasFormatting(true);
}

void sad::Label::disableFormatting()
{
    PROFILER_EVENT;
    setHasFormatting(false);
}

void sad::Label::makeSpanBetweenPoints(
    const sad::Point2D & p1, 
    const sad::Point2D & p2
)
{
    PROFILER_EVENT;
    sad::Font * font = m_font.get();
    if (font) {
        sad::Size2D  size = font->size(m_rendered_string);
        double dy2 = p2.y() - p1.y();
        double dx2 = p2.x() - p1.x();
        double angle2 = 0;
        if (sad::is_fuzzy_zero(dx2) == false || sad::is_fuzzy_zero(dy2) == false)
        {
            angle2 = atan2(dy2, dx2);
            if (angle2 < 0)
            {
                angle2 += 2 * M_PI;
            }
        }
        setPoint((p1.x() + p2.x() - size.Width) / 2, (p1.y() + p2.y() + size.Height) / 2);
        setAngle(angle2);
    }
}

sad::String sad::Label::makeRenderingString(
    const sad::String& string,   
    unsigned int maximal_line_width,
    sad::Label::OverflowStrategy s,
    sad::Label::BreakText bt,
    sad::Label::TextEllipsisPosition tep,
    unsigned int maximum_lines,
    sad::Label::OverflowStrategy overflow_for_lines,
    sad::Label::TextEllipsisPosition text_ellipsis_for_lines
)
{
    PROFILER_EVENT;
    sad::String result;
    if (maximal_line_width == 0 && maximum_lines == 0)
    {
        result = string;
    } 
    else
    {
        bool has_n =  string.getLastOccurrence("\n") != -1;
        bool has_r =  string.getLastOccurrence("\r") != -1;
        sad::String tmp = string;
        if (has_n && has_r)
        {
            // Windows string preprocessing
            tmp.replaceAllOccurrences("\r", "");
        } 
        else
        {
            // MacOSX 
            if (!has_n && has_r)
            {
                tmp.replaceAllOccurrences("\r", "\n");
            }
        }
        bool last_line_changed = false;
        sad::StringList lines = string.split("\n", sad::String::SplitBehaviour::KEEP_EMPTY_PARTS);
        sad::StringList new_lines;
        if (maximal_line_width == 0)
        {
            new_lines = lines;
            last_line_changed = true;
        }
        for(size_t i = 0; i < lines.size() && (maximal_line_width != 0); i++)
        {
            if (bt == sad::Label::BreakText::LBT_BREAK_WORD)
            {
                sad::StringList words = lines[i].split(' ');
                new_lines << "";
                sad::String* current_word = &(new_lines[new_lines.size() - 1]);         
                for(size_t j = 0; j <  words.size(); j++)
                {
                    // In case that word  is larger than line width - we can do nothing to prevent it from 
                    // becoming larger
                    sad::String candidate_word = words[j];
                    candidate_word.replaceAllOccurrences("\n", "");
                    if (candidate_word.length() > maximal_line_width && current_word->length() == 0)
                    {
                        *current_word = sad::Label::formatTextLine(
                            candidate_word,
                            maximal_line_width,
                            s,
                            tep
                        );
                        new_lines << "";
                        current_word = &(new_lines[new_lines.size() - 1]);
                        last_line_changed = false;
                    }
                    else
                    {
                        int offset = (current_word->length() == 0) ? 0 : 1; 
                        if (current_word->length() + offset + candidate_word.length() > maximal_line_width)
                        {
                            new_lines << "";
                            current_word = &(new_lines[new_lines.size() - 1]);                              
                            --j;
                            last_line_changed = false;
                        }
                        else
                        {
                            if (current_word->length())
                            {
                                *current_word += " ";
                            }
                            *current_word += candidate_word;
                            last_line_changed = true;
                        }
                    }
                }
            }
            if (bt == sad::Label::BreakText::LBT_NORMAL)
            {
                sad::String line = lines[i];
                line.replaceAllOccurrences("\n", "");
                new_lines << sad::Label::formatTextLine(
                    line,
                    maximal_line_width,
                    s,
                    tep
                );                
            }
        }
        if (bt == sad::Label::BreakText::LBT_BREAK_WORD && new_lines.size() != 0 && !last_line_changed)
        {
            new_lines.removeAt(new_lines.size() - 1);
        }
        if (maximum_lines != 0 
            && new_lines.size() > maximum_lines 
            && overflow_for_lines != sad::Label::OverflowStrategy::LOS_VISIBLE)
        {
            if (overflow_for_lines == sad::Label::OverflowStrategy::LOS_HIDDEN)
            {
                new_lines.removeRange(maximum_lines, new_lines.size() - 1);
            }
            if (overflow_for_lines == sad::Label::OverflowStrategy::LOS_ELLIPSIS)
            {
                if (text_ellipsis_for_lines == sad::Label::TextEllipsisPosition::LTEP_BEGIN)
                {
                    new_lines.removeRange(0, new_lines.size() - maximum_lines - 1);                    
                    new_lines[0] = "...";
                }
                if (text_ellipsis_for_lines == sad::Label::TextEllipsisPosition::LTEP_END)
                {
                    new_lines.removeRange(maximum_lines, new_lines.size() - 1);                    
                    new_lines[new_lines.size() - 1] = "...";
                }
                if (text_ellipsis_for_lines == sad::Label::TextEllipsisPosition::LTEP_MIDDLE)
                {
                    long left_length = (maximum_lines - 1) / 2 + (maximum_lines - 1) % 2;
                    long right_length = (maximum_lines - 1) / 2;
                    if (new_lines.size() == 1)
                    {
                        new_lines[0] = "...";
                    }
                    else
                    {
                        if (new_lines.size() == 2)
                        {
                            new_lines[1] = "...";
                        }
                        else
                        {
                            new_lines.removeRange(left_length + 1, new_lines.size() - right_length - 1);
                            new_lines[left_length] = "...";
                        }
                    }
                }
            }
        }
        result = sad::join(new_lines, "\n");
    }
    return result;
}




sad::util::Markup::Document sad::Label::makeRenderingString(
    const sad::util::Markup::Document& string,
    unsigned int maximal_line_width,
    sad::Label::OverflowStrategy s,
    sad::Label::BreakText bt,
    sad::Label::TextEllipsisPosition tep,
    unsigned int maximum_lines,
    sad::Label::OverflowStrategy overflow_for_lines,
    sad::Label::TextEllipsisPosition text_ellipsis_for_lines
)
{
    PROFILER_EVENT;
    if (maximal_line_width == 0 && maximum_lines == 0)
    {
        return string;
    }
    else
    {
        sad::util::Markup::Document lines = string;
        bool last_line_changed = false;
        sad::util::Markup::Document new_lines;
        if (maximal_line_width == 0)
        {
            new_lines = lines;
            last_line_changed = true;
        }
        for (size_t i = 0; i < lines.size() && (maximal_line_width != 0); i++)
        {
            if (bt == sad::Label::BreakText::LBT_BREAK_WORD)
            {
                sad::Vector<sad::util::Markup::Command> words = sad::Label::breakIntoWords(lines[i]);
                new_lines << sad::Vector<sad::util::Markup::Command>();
                sad::Vector<sad::util::Markup::Command>* current_word = &(new_lines[new_lines.size() - 1]);
                for (size_t j = 0; j < words.size(); j++)
                {
                    // In case that word  is larger than line width - we can do nothing to prevent it from 
                    // becoming larger
                    const sad::util::Markup::Command& candidate_word = words[j];
                    size_t cur_word_length = sad::Label::length(*current_word);
                    if (candidate_word.Content.length() > maximal_line_width && cur_word_length == 0)
                    {
                        sad::util::Markup::Command candidate_word_copy = candidate_word;
                        candidate_word_copy.Content = sad::Label::formatTextLine(
                            candidate_word.Content,
                            maximal_line_width,
                            s,
                            tep
                        );
                        current_word->push_back(candidate_word_copy);
                        new_lines << sad::Vector<sad::util::Markup::Command>();
                        current_word = &(new_lines[new_lines.size() - 1]);
                        last_line_changed = false;
                    }
                    else
                    {
                        if (cur_word_length + candidate_word.Content.length() > maximal_line_width)
                        {
                            new_lines << sad::Vector<sad::util::Markup::Command>();
                            current_word = &(new_lines[new_lines.size() - 1]);
                            --j;
                            last_line_changed = false;
                        }
                        else
                        {
                            current_word->push_back(candidate_word);
                            last_line_changed = true;
                        }
                    }
                }
            }
            if (bt == sad::Label::BreakText::LBT_NORMAL)
            {
                const sad::Vector<sad::util::Markup::Command>& line = lines[i];
                new_lines << sad::Label::formatTextLine(
                    line,
                    maximal_line_width,
                    s,
                    tep
                );
            }
        }
        if (bt == sad::Label::BreakText::LBT_BREAK_WORD && new_lines.size() != 0 && !last_line_changed)
        {
            new_lines.removeAt(new_lines.size() - 1);
        }
        if (maximum_lines != 0
            && new_lines.size() > maximum_lines
            && overflow_for_lines != sad::Label::OverflowStrategy::LOS_VISIBLE)
        {
            if (overflow_for_lines == sad::Label::OverflowStrategy::LOS_HIDDEN)
            {
                new_lines.removeRange(maximum_lines, new_lines.size() - 1);
            }
            if (overflow_for_lines == sad::Label::OverflowStrategy::LOS_ELLIPSIS)
            {
                if (text_ellipsis_for_lines == sad::Label::TextEllipsisPosition::LTEP_BEGIN)
                {
                    sad::util::Markup::Command c = sad::Label::firstCommand(new_lines);
                    c.Content = "...";
                    new_lines.removeRange(0, new_lines.size() - maximum_lines - 1);
                    new_lines[0].clear(); 
                    new_lines[0].push_back(c);
                }
                if (text_ellipsis_for_lines == sad::Label::TextEllipsisPosition::LTEP_END)
                {
                    sad::util::Markup::Command c = sad::Label::lastCommand(new_lines);
                    c.Content = "...";
                    new_lines.removeRange(maximum_lines, new_lines.size() - 1);
                    new_lines[new_lines.size() - 1].clear(); 
                    new_lines[new_lines.size() - 1].push_back(c);
                }
                if (text_ellipsis_for_lines == sad::Label::TextEllipsisPosition::LTEP_MIDDLE)
                {
                    long left_length = (maximum_lines - 1) / 2 + (maximum_lines - 1) % 2;
                    long right_length = (maximum_lines - 1) / 2;
                    if (new_lines.size() == 1)
                    {
                        sad::util::Markup::Command c = sad::Label::firstCommand(new_lines);
                        c.Content = "...";
                        new_lines[0].clear();
                        new_lines[0].push_back(c);
                    }
                    else
                    {
                        if (new_lines.size() == 2)
                        {
                            sad::util::Markup::Command c = sad::Label::lastCommand(new_lines);
                            c.Content = "...";
                            new_lines[1].clear();
                            new_lines[1].push_back(c);
                        }
                        else
                        {
                            new_lines.removeRange(left_length + 1, new_lines.size() - right_length - 1);
                            sad::util::Markup::Command c = sad::Label::lastCommand(new_lines);
                            c.Content = "...";
                            new_lines[left_length].clear();
                            new_lines[left_length].push_back(c);
                        }
                    }
                }
            }
        }
        return new_lines;
    }
}

size_t sad::Label::length(const sad::Vector<sad::util::Markup::Command>& c)
{
    PROFILER_EVENT;
    size_t length = 0;
    for (size_t i = 0; i < c.size(); i++)
    {
        length += c[i].Content.length();
    }
    return length;
}

sad::Vector<sad::util::Markup::Command> sad::Label::breakIntoWords(const sad::Vector<sad::util::Markup::Command>& v)
{
    PROFILER_EVENT;
    sad::Vector<sad::util::Markup::Command> result;
    for (size_t i = 0; i < v.size(); i++)
    {
        sad::StringList lst = v[i].Content.split(' ');
        bool last_has_space = false;
        if (v[i].Content.size())
        {
            last_has_space = (v[i].Content[v[i].Content.length() - 1] == ' ');
        }
        if ((lst.size()  == 0) && last_has_space)
        {
            result << v[i];
        }
        else
        {
            for (size_t j = 0; j < lst.size(); j++)
            {
                sad::util::Markup::Command c = v[i];
                c.Content = lst[j];
                if (j == lst.size() - 1)
                {
                    if (last_has_space)
                        c.Content += ' ';
                }
                else 
                {
                    c.Content += ' ';
                }
                result << c;
            }
        }
    }
    return result;
}

sad::String sad::Label::formatTextLine(
    const sad::String& string,
    unsigned int maximal_line_width,
    sad::Label::OverflowStrategy s,
    sad::Label::TextEllipsisPosition tep
)
{
    PROFILER_EVENT;
    sad::String result = string;
    if (string.length() > maximal_line_width && s != sad::Label::OverflowStrategy::LOS_VISIBLE && maximal_line_width != 0)
    {
        if (s == sad::Label::OverflowStrategy::LOS_HIDDEN)
        {
            result = string.subString(0, maximal_line_width);
        }

        if (s == sad::Label::OverflowStrategy::LOS_ELLIPSIS)
        {
            if (maximal_line_width <= 3)
            {
                switch(maximal_line_width)
                {
                 case 1: { result = "."; break;}
                 case 2: { result = ".."; break;}
                 case 3: { result = "..."; break;}
                }
            }
            else
            {
                if (string.consistsOfWhitespaceCharacters())
                {
                    result = "";
                }
                else
                {
                    if (tep == sad::Label::TextEllipsisPosition::LTEP_BEGIN)
                    {
                        result = "...";                        
                        sad::String part = string.substr(string.length() - maximal_line_width + 3, maximal_line_width - 3);
                        part.trim();
                        // Clear string, that consists from spaces
                        if (part.length())
                        {
                            result += part;
                        }
                    }
                    // In case of string with length 4 we should use this strategy too to simplify computation
                    if (tep == sad::Label::TextEllipsisPosition::LTEP_END || (maximal_line_width == 4 && tep == sad::Label::TextEllipsisPosition::LTEP_MIDDLE))
                    {
                        result = string.substr(0, maximal_line_width - 3);
                        result.trim();
                        result += "...";  
                    }

                    if (tep == sad::Label::TextEllipsisPosition::LTEP_MIDDLE && maximal_line_width > 4)
                    {
                        int halfwidth = (maximal_line_width - 3) / 2;
                        int halfmod = (maximal_line_width - 3) % 2;
                        sad::String left = string.substr(0, halfwidth);
                        sad::String right = string.getRightPart(halfwidth + halfmod);
                        left.trimRight();
                        right.trimLeft();
                        if (left.length() == 0 && right.length() == 0)
                        {
                            result = "...";
                        }
                        else
                        {
                            result = left;
                            result += "...";
                            result += right;
                        }
                    }
                }
            }
        }
    }
    return result;
}

sad::Vector<sad::util::Markup::Command> sad::Label::formatTextLine(
    const sad::Vector<sad::util::Markup::Command>& string,
    unsigned int maximal_line_width,
    sad::Label::OverflowStrategy s,
    sad::Label::TextEllipsisPosition tep
)
{
    PROFILER_EVENT;
    sad::Vector<sad::util::Markup::Command> result = string;
    size_t length = sad::Label::length(string);
    if (length > maximal_line_width && s != sad::Label::OverflowStrategy::LOS_VISIBLE && maximal_line_width != 0)
    {
        if (s == sad::Label::OverflowStrategy::LOS_HIDDEN)
        {
             result = sad::Label::subString(string, 0, maximal_line_width);
        }

        if (s == sad::Label::OverflowStrategy::LOS_ELLIPSIS)
        {
            if (maximal_line_width <= 3)
            {
                sad::util::Markup::Command cmd;
                if (string.size())
                {
                    cmd = string[0];
                }
                result.clear();
                switch(maximal_line_width)
                {
                 case 1: { cmd.Content = "."; break;}
                 case 2: { cmd.Content = ".."; break;}
                 case 3: { cmd.Content = "..."; break;}
                }
                result.push_back(cmd);
            }
            else
            {
                if (sad::Label::consistsOfWhitespaceCharacters(string))
                {
                    sad::util::Markup::Command cmd;
                    if (string.size())
                    {
                        cmd = string[0];
                    }
                    result.clear();
                    cmd.Content.clear();
                    result.push_back(cmd);
                }
                else
                {
                    if (tep == sad::Label::TextEllipsisPosition::LTEP_BEGIN)
                    {
                        // A first command part
                        result.clear();
                        sad::util::Markup::Command cmd;
                        if (string.size())
                        {
                            cmd = string[0];
                        }
                        cmd.Content = "...";
                        result.push_back(cmd);
                        
                        sad::Vector<sad::util::Markup::Command> part = sad::Label::subString(string, length - maximal_line_width + 3, maximal_line_width - 3);
                        sad::Label::trim(part);
                        // Clear string, that consists from spaces
                        if (sad::Label::length(part))
                        {
                            result << part;
                        }
                    }
                    // In case of string with length 4 we should use this strategy too to simplify computation
                    if (tep == sad::Label::TextEllipsisPosition::LTEP_END || (maximal_line_width == 4 && tep == sad::Label::TextEllipsisPosition::LTEP_MIDDLE))
                    {
                        result =  sad::Label::subString(string, 0, maximal_line_width - 3);
                        sad::Label::trim(result);
                        if (result.size())
                        {
                            result[result.size() - 1].Content += "...";  
                        }
                        else
                        {
                            sad::util::Markup::Command cmd;
                            cmd.Content = "...";
                            result.push_back(cmd);
                        }
                    }
                    if (tep == sad::Label::TextEllipsisPosition::LTEP_MIDDLE && maximal_line_width > 4)
                    {
                        int halfwidth = (maximal_line_width - 3) / 2;
                        int halfmod = (maximal_line_width - 3) % 2;
                        int length = sad::Label::length(string);
                        int rightpart = halfwidth + halfmod;
                        sad::Vector<sad::util::Markup::Command> left = sad::Label::subString(string, 0, halfwidth);
                        sad::Vector<sad::util::Markup::Command> right = sad::Label::subString(string, length - rightpart, rightpart);
                        sad::Label::trim(left, false, true);
                        sad::Label::trim(right, true, false);
                        result.clear();
                        if (sad::Label::length(left) == 0 && sad::Label::length(right) == 0)
                        {
                            sad::util::Markup::Command cmd;
                            if (string.size())
                            {
                                cmd = string[0];
                            }
                            cmd.Content = "...";
                            result.push_back(cmd);
                        }
                        else
                        {
                            result = left;
                            if (result.size())
                            {
                                result[result.size() - 1].Content += "...";
                            }
                            else
                            {
                                if (right.size())
                                {
                                    right[0].Content = "..." + right[0].Content;
                                }
                            }
                            result << right;
                        }
                    }
                }
            }
        }
    }
    return result;
}

sad::util::Markup::Command sad::Label::firstCommand(const sad::util::Markup::Document& doc)
{
    PROFILER_EVENT;
    for(size_t i = 0; i < doc.size(); i++)
    {
        if (doc[i].size())
        {
            return doc[i][0];
        }
    }
    return sad::util::Markup::Command();
}

sad::util::Markup::Command sad::Label::lastCommand(const sad::util::Markup::Document& doc)
{
    PROFILER_EVENT;
    for(int i = doc.size() - 1; i > -1; i--)
    {
        if (doc[i].size())
        {
            return doc[i][0];
        }
    }
    return sad::util::Markup::Command();
}

bool sad::Label::consistsOfWhitespaceCharacters(const sad::Vector<sad::util::Markup::Command>& row)
{
    PROFILER_EVENT;
    bool result = false;
    for(size_t i = 0; i < row.size(); i++)
    {
        result = result && row[i].Content.consistsOfWhitespaceCharacters();   
    }
    return result;
}

void sad::Label::trim(sad::Vector<sad::util::Markup::Command>& row, bool left, bool right)
{
    PROFILER_EVENT;
    if (row.size())
    {
        if (left) 
        {
            row[0].Content.trimLeft();
        }
        if (right) 
        {
            row[row.size() - 1].Content.trimRight();
        }
    }
}

sad::Vector<sad::util::Markup::Command> sad::Label::subString(const sad::Vector<sad::util::Markup::Command>& row, int begin, int length)
{
    PROFILER_EVENT;
    sad::Vector<sad::util::Markup::Command> result;
    int index = 0;
    int first_index = begin;
    int last_index = begin + length - 1;
    for(size_t i = 0; i < row.size(); i++)
    {
        int new_last_index = index + row[i].Content.length() - 1;
        if (index < first_index)
        {
            if (new_last_index >= first_index)
            {
                int string_position_index = first_index - index;
                sad::util::Markup::Command cmd = row[i];
                if (new_last_index <= last_index)
                {
                    cmd.Content = cmd.Content.subString(string_position_index, cmd.Content.length() - string_position_index);
                }
                else
                {
                    cmd.Content = cmd.Content.subString(string_position_index, last_index - first_index + 1);
                }
                result.push_back(cmd);
            }
        }
        else
        {
            if ((index <= last_index))
            {
                sad::util::Markup::Command cmd = row[i];
                if (new_last_index > last_index)
                {
                    cmd.Content = cmd.Content.subString(0, last_index - index + 1);
                }
                result.push_back(cmd);
            }
        }
        index += row[i].Content.length();
    }
    return result;
}

void sad::Label::moveBy(const sad::Point2D& p)
{
    PROFILER_EVENT;
    setPoint(point() + p);
}

unsigned int sad::Label::renderedStringLength() const
{
    PROFILER_EVENT;
    return m_rendered_chars;
}


void sad::Label::setRenderingStringLimit(unsigned int limit)
{
    PROFILER_EVENT;
    m_geometries_dirty = true;
    m_rendering_character_limit.setValue(limit);
}

void sad::Label::clearRenderingStringLimit()
{
    PROFILER_EVENT;
    m_rendering_character_limit.clear();
}

void sad::Label::setRenderingStringLimitAsRatioToLength(double limit)
{
    PROFILER_EVENT;
    this->setRenderingStringLimit(static_cast<unsigned int>(limit * this->renderedStringLength()));
}

void sad::Label::onRemovedFromScene()
{
    PROFILER_EVENT;
    sad::Renderer* r = m_geometries->renderer();
    if (!r)  r = this->renderer();
    if (r)
    {
        r->removeFontGeometries(m_geometries);
    }
    m_geometries->setRenderer(nullptr);
    m_geometries_dirty = true;
}

void sad::Label::setShaderFunction(sad::ShaderFunction* fun)
{
    PROFILER_EVENT;
    if (fun)
    {
        assert(fun->canBeUsedForFonts());
    }
    this->sad::SceneNode::setShaderFunction(fun);
}

void sad::Label::setLineShaderFunction(sad::FontShaderFunction* fun)
{
    PROFILER_EVENT;
    if (m_line_shader_function)
    {
        m_line_shader_function->delRef();
    }
    m_line_shader_function = fun;
    if (m_line_shader_function)
    {
        m_line_shader_function->addRef();
    }
}

sad::FontShaderFunction* sad::Label::lineShaderFunction() const
{
    PROFILER_EVENT;
    return m_line_shader_function;
}

void sad::Label::reloadFont()
{
    PROFILER_EVENT;
    sad::Font * font = m_font.get();
    if (font)
    {
        recomputeRenderedString();
    }
}


void sad::Label::recomputeRenderingPoint(bool lock)
{
    PROFILER_EVENT;
    if (lock)
    {
        m_recompute_point_lock.lock();
    }
    m_computed_rendering_point = false;

    sad::Font * font = m_font.get();
    if (font)
    {
        m_center.setX(m_point.x() + m_label_size.Width / 2);
        m_center.setY(m_point.y() - m_label_size.Height / 2);
        m_half_padding.setX(m_label_size.Width / -2.0);
        m_half_padding.setY(m_label_size.Height / 2.0);

        m_computed_rendering_point = true;

        m_cached_region = this->region();
    }

    if (lock)
    {
        m_recompute_point_lock.unlock();
    }
}

sad::Size2D sad::Label::getSizeWithoutFormatting(sad::Font* font)
{
    PROFILER_EVENT;
    return font->size(m_rendered_string);   
}

sad::Size2D sad::Label::getSizeWithFormatting(sad::Font* font)
{
    PROFILER_EVENT;
    sad::Size2D result(0, 0);
    m_document_metrics.clear();
    sad::Vector<float> rowmaxs;
    for (size_t row = 0; row < m_document.size(); row++)
    {
        float maxascender = 0;
        float maxlinespacingvalue = 0;
        float width = 0;
        float ymaxx = 0;
        bool last_line = (row == m_document.size() - 1);
        for (size_t i = 0; i < m_document[row].size(); i++)
        {
            sad::util::Markup::Command& c = m_document[row][i];
            sad::Pair<sad::Font*, int> pair = this->applyFontCommand(font, c);
            sad::Font* fnt = pair.p1();
            int flags = pair.p2();
            // Last line
            if (last_line)
            {
                fnt->setLineSpacingRatio(1);
            }
            sad::Size2D sz = fnt->size(c.Content, flags);
            c.Ascender = fnt->ascent();
            c.LineSpacingValue = fnt->lineSpacing();
            c.Width = sz.Width;
            float localymax = result.Height + fnt->builtinLineSpacing();
            float ascentmax = result.Height + fnt->ascent();
            if (c.Underlined)
            {
                ascentmax += 2;
            }
            ymaxx = (ymaxx > localymax) ? ymaxx : localymax;
            ymaxx = (ymaxx > ascentmax) ? ymaxx : ascentmax;
            maxascender = (maxascender > c.Ascender) ? maxascender : c.Ascender; 
            maxlinespacingvalue = (maxlinespacingvalue > c.LineSpacingValue) ? maxlinespacingvalue : c.LineSpacingValue;
            width += c.Width;
        }
        rowmaxs << ymaxx;
        sad::Label::FormattedRowMetrics metrics;
        metrics.Ascender = maxascender;
        metrics.LineSpacingValue = maxlinespacingvalue;
        metrics.Width = width;
        m_document_metrics << metrics;

        result.Height += metrics.LineSpacingValue;
        result.Width = (result.Width > metrics.Width) ? result.Width : metrics.Width;
    }

    rowmaxs << result.Height;
    result.Height = *(std::max_element(rowmaxs.begin(), rowmaxs.end()));
    
    return result;
}


void sad::Label::recomputeRenderedString(bool lock)
{
    PROFILER_EVENT;
    if (lock)
    {
        m_recompute_string_lock.lock();
    }

    m_computed_rendering_string = false;

    if (m_formatted)
    {
        this->recomputeRenderingStringWithFormatting();
    }
    else
    {
        this->recomputeRenderingStringWithoutFormatting();
    }

    sad::Font* font = m_font.get();
    if (font)
    {
        font->setSize(m_size);
        font->setLineSpacingRatio(m_linespacing_ratio);

        if (m_formatted)
        {
            m_label_size = this->getSizeWithFormatting(font);
            assert(m_document.size() == m_document_metrics.size());
        }
        else
        {
            m_label_size = this->getSizeWithoutFormatting(font);
        }
    }

    // Recompute rendering point
    recomputeRenderingPoint(lock);

    m_geometries_dirty = true;

    if (m_computed_rendering_point)
        m_computed_rendering_string = true;

    if (lock)
    {
        m_recompute_string_lock.unlock();
    }
}

void sad::Label::recomputeRenderingStringWithoutFormatting()
{
    PROFILER_EVENT;
    m_rendered_string = sad::Label::makeRenderingString(
        m_string,
        m_maximal_line_width,
        m_overflow_strategy,
        m_break_text,
        m_text_ellipsis_position,
        m_maximum_lines,
        m_overflow_strategy_for_lines,
        m_text_ellipsis_position_for_lines
    );
    // Recompute rendered char count
    m_rendered_chars = 0;
    for (size_t i = 0; i < m_rendered_string.length(); i++)
    {
        if (m_rendered_string[i] != '\r' && m_rendered_string[i] != '\n')
        {
            m_rendered_chars++;
        }
    }	
}

void sad::Label::recomputeRenderingStringWithFormatting()
{
    PROFILER_EVENT;
    m_document = sad::util::Markup::parseDocument(m_string, sad::util::Markup::Command());
    m_document = sad::Label::makeRenderingString(
        m_document,
        m_maximal_line_width,
        m_overflow_strategy,
        m_break_text,
        m_text_ellipsis_position,
        m_maximum_lines,
        m_overflow_strategy_for_lines,
        m_text_ellipsis_position_for_lines
    );
    // Recompute rendered char count
    m_rendered_chars = 0;
    for (size_t i = 0; i < m_document.size(); i++)
    {
        for (size_t j = 0; j < m_document[i].size(); j++)
        {
            m_rendered_chars += m_document[i][j].Content.length();
        }
    }
}

void sad::Label::clearFontsCache()
{
    PROFILER_EVENT;
    m_font_cache.clear();
}

sad::Pair<sad::Font*, int> sad::Label::applyFontCommand(sad::Font* font, const sad::util::Markup::Command& c)
{
    PROFILER_EVENT;
    sad::Font* fnt = font;
    int flags = 0;
    // Apply font
    if (c.Font.exists())
    {
        fnt = this->getFontForDocument(c.Font.value());
    }
    // Apply color
    if (c.Color.exists())
    {
        fnt->setColor(c.Color.value());
    }
    else
    {
        fnt->setColor(m_color);
    }
    // Apply line spacing
    if (c.Linespacing.exists())
    {
        switch (c.Linespacing.value().Type)
        {
            case sad::util::Markup::LineSpacingSizeType::MLST_PERCENTS: fnt->setLineSpacingRatio(c.Linespacing.value().Size / 100.0); break;
            case sad::util::Markup::LineSpacingSizeType::MLST_PIXELS: fnt->setLineSpacing(c.Linespacing.value().Size); break;
        };
    }
    else
    {
        fnt->setLineSpacingRatio(m_linespacing_ratio);
    }
    // Apply size
    if (c.Size.exists())
    {
        switch (c.Size.value().Type)
        {
        case sad::util::Markup::FontSizeType::MFZST_PIXELS: fnt->setSize(c.Size.value().Size); break;
        case sad::util::Markup::FontSizeType::MFZST_POINTS: fnt->setSizeInPoints(c.Size.value().Size); break;
        };
    }
    else
    {
        fnt->setSize(m_size);
    }
    if (c.Bold)
    {
        flags = flags | sad::Font::RenderFlags::FRF_Bold;
    }
    if (c.Italic)
    {
        flags = flags | sad::Font::RenderFlags::FRF_Italic;
    }
    return sad::Pair<sad::Font*, int>(fnt,flags);
}

sad::Font* sad::Label::getFontForDocument(const sad::String& s)
{
    PROFILER_EVENT;
    return m_font_cache.get(s, m_font);
}

void sad::Label::renderWithoutFormatting(sad::Font* font)
{
    PROFILER_EVENT;
    if (m_rendering_character_limit.exists())
    {
        int length = 0;
        int limitlength = 0;
        bool can_add = true;
        for (size_t i = 0; (i < m_rendered_string.size()) && can_add; i++)
        {
            if (limitlength >= m_rendering_character_limit.value())
            {
                can_add = false;
            }
            else
            {
                if (m_rendered_string[i] != '\n' && m_rendered_string[i] != '\r')
                {
                    limitlength += 1;
                }
                length += 1;
            }
        }
        sad::String result = m_rendered_string.subString(0, length);
        font->render(result, m_half_padding);
    }
    else
    {
        font->render(m_rendered_string, m_half_padding);
    }
}

void sad::Label::buildGeometriesWithoutFormatting(sad::Font* font)
{
    PROFILER_EVENT;
    sad::Font::GeometryRenderData geometry_render_data;
    geometry_render_data.Renderer = this->renderer();
    geometry_render_data.Color = m_color;
    geometry_render_data.OwnColor = false;

    if (m_rendering_character_limit.exists())
    {
        int length = 0;
        int limitlength = 0;
        bool can_add = true;
        for (size_t i = 0; (i < m_rendered_string.size()) && can_add; i++)
        {
            if (limitlength >= m_rendering_character_limit.value())
            {
                can_add = false;
            }
            else
            {
                if (m_rendered_string[i] != '\n' && m_rendered_string[i] != '\r')
                {
                    limitlength += 1;
                }
                length += 1;
            }
        }
        sad::String result = m_rendered_string.subString(0, length);
        font->fillGeometries(geometry_render_data, *m_geometries, result, m_half_padding);
    }
    else
    {
        font->fillGeometries(geometry_render_data, *m_geometries, m_rendered_string, m_half_padding);
    }
}

void sad::Label::renderWithFormatting(sad::Font* font)
{
    PROFILER_EVENT;
    assert(m_document.size() == m_document_metrics.size());
    sad::Point2D point = m_half_padding;
    sad::Renderer* renderer = this->renderer();
    int rendered_count = 0;
    bool isOpenGL3supported = false;
    if (renderer)
    {
        isOpenGL3supported = renderer->context()->isOpenGL3compatible();
    }
    for (size_t row = 0; row < m_document.size(); row++)
    {
        double x = point.x();
        bool last_line = (row == m_document.size() - 1);
        for (size_t i = 0; i < m_document[row].size(); i++)
        {
            sad::util::Markup::Command& c = m_document[row][i];
            sad::Pair<sad::Font*, int> pair = this->applyFontCommand(font, c);
            sad::Font* fnt = pair.p1();
            int flags = pair.p2();
            // Last line
            if (last_line)
            {
                fnt->setLineSpacingRatio(1);
            }
            double y = point.y() - m_document_metrics[row].Ascender + c.Ascender;
            double part_width = c.Width;

            if (m_rendering_character_limit.exists())
            {
                // Support for boundied character limit
                if (rendered_count >= m_rendering_character_limit.value())
                {
                    // If we already rendered everything, return it
                    return;
                }
                else
                {
                    int total_count = rendered_count + c.Content.length();
                    if (total_count > rendered_count)
                    {
                        // Render part of document
                        sad::String string = c.Content.subString(0, m_rendering_character_limit.value() - rendered_count);
                        part_width = fnt->size(string, flags).Width;
                        fnt->render(string, sad::Point2D(x, y), flags);
                        rendered_count += string.length();
                    }
                    else
                    {
                        // Render whole part of document
                        fnt->render(c.Content, sad::Point2D(x, y), flags);
                        // Move cursor to next
                        rendered_count += c.Content.length();
                    }
                }
            }
            else
            {
                fnt->render(c.Content, sad::Point2D(x, y), flags);
            }
            if (renderer)
            {
                if (c.Strikethrough)
                {
                    // We assume that capital letters are bigger than 
                    // lowercase in no more than two times, so dividing ascender by 4 would give us
                    // a position in middle of lower case
                    double ky = point.y() - m_document_metrics[row].Ascender + c.Ascender / 4;
                    sad::Point2D p1(x, ky), p2(x + part_width, ky);
                    sad::AColor clr = fnt->color();
                    //clr.setA(255 - clr.a());
                    renderer->render()->line(this->scene(), p1, p2, clr);
                }
                if (c.Underlined)
                {
                    double ky = point.y() - m_document_metrics[row].Ascender - 2;
                    sad::Point2D p1(x, ky), p2(x + part_width, ky);
                    sad::AColor clr = fnt->color();
                    //clr.setA(255 - clr.a());
                    renderer->render()->line(this->scene(), p1, p2, clr);
                }
            }
            x += part_width;
        }

        point.setY(point.y() -  m_document_metrics[row].LineSpacingValue);
    }

#ifdef DEBUG_FORMATTED_RENDERING
    if (renderer)
    {
        renderer->render()->rectangle(sad::Rect2D(m_half_padding, m_half_padding * (-1)), sad::AColor(255, 0, 0, 255));
    }
#endif
}


void sad::Label::buildGeometriesWithFormatting(sad::Font* font)
{
    PROFILER_EVENT;
    assert(m_document.size() == m_document_metrics.size());

    sad::Point2D point = m_half_padding;
    sad::Renderer* renderer = this->renderer();
    int rendered_count = 0;
    for (size_t row = 0; row < m_document.size(); row++)
    {
        double x = point.x();
        bool last_line = (row == m_document.size() - 1);
        for (size_t i = 0; i < m_document[row].size(); i++)
        {
            sad::util::Markup::Command& c = m_document[row][i];
            sad::Pair<sad::Font*, int> pair = this->applyFontCommand(font, c);
            sad::Font* fnt = pair.p1();
            int flags = pair.p2();
            // Last line
            if (last_line)
            {
                fnt->setLineSpacingRatio(1);
            }
            double y = point.y() - m_document_metrics[row].Ascender + c.Ascender;
            double part_width = c.Width;

            sad::Font::GeometryRenderData geometry_render_data;
            geometry_render_data.Renderer = renderer;
            geometry_render_data.Color = m_color;
            if (c.Color.exists())
            {
                geometry_render_data.Color = c.Color.value();
                geometry_render_data.OwnColor = true;
            }
            else
            {
                geometry_render_data.OwnColor = false;
            }

            if (m_rendering_character_limit.exists())
            {
                // Support for boundied character limit
                if (rendered_count >= m_rendering_character_limit.value())
                {
                    // If we already rendered everything, return it
                    return;
                }
                else
                {
                    int total_count = rendered_count + c.Content.length();
                    if (total_count > rendered_count)
                    {
                        // Render part of document
                        sad::String string = c.Content.subString(0, m_rendering_character_limit.value() - rendered_count);
                        part_width = fnt->size(string, flags).Width;
                        fnt->fillGeometries(geometry_render_data, *m_geometries, string, sad::Point2D(x, y), flags);
                        rendered_count += string.length();
                    }
                    else
                    {
                        // Render whole part of document
                        fnt->fillGeometries(geometry_render_data, *m_geometries, c.Content, sad::Point2D(x, y), flags);
                        // Move cursor to next
                        rendered_count += c.Content.length();
                    }
                }
            }
            else
            {
                fnt->fillGeometries(geometry_render_data, *m_geometries, c.Content, sad::Point2D(x, y), flags);
            }
            if (renderer)
            {
                if (c.Strikethrough)
                {
                    // We assume that capital letters are bigger than 
                    // lowercase in no more than two times, so dividing ascender by 4 would give us
                    // a position in middle of lower case
                    double ky = point.y() - m_document_metrics[row].Ascender + c.Ascender / 4;
                    sad::Point2D p1(x, ky), p2(x + part_width, ky);
                    sad::Label::LineRenderedData data;
                    if (c.Color.exists())
                    {
                        data.Color.setValue(c.Color.value());
                    }
                    data.P1 = p1;
                    data.P2 = p2;
                    m_rendered_lines.push_back(data);
                }
                if (c.Underlined)
                {
                    double ky = point.y() - m_document_metrics[row].Ascender - 2;
                    sad::Point2D p1(x, ky), p2(x + part_width, ky);
                    sad::Label::LineRenderedData data;
                    if (c.Color.exists())
                    {
                        data.Color.setValue(c.Color.value());
                    }
                    data.P1 = p1;
                    data.P2 = p2;
                    m_rendered_lines.push_back(data);
                }
            }
            x += part_width;
        }

        point.setY(point.y() - m_document_metrics[row].LineSpacingValue);
    }
}
