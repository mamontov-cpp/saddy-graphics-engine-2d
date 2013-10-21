#include <freetype/font.h>

#include <log/log.h>
#include <sadmutex.h>
#include <sadptrhash.h>
#include <sadrect.h>

#include <cassert>
#include <algorithm>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#ifdef WIN32
#define NOMINMAX
#include<windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

// Taken from SDL_TTF sources for ceiling data
#define FT_CEIL(X)  (((X + 63) & -64) / 64)

namespace sad
{

namespace freetype
{

/*! An OpenGL calllist, which can render font of specified width
 */
class FontCallList
{
public:
	/*! Creates new call list for specified face
		\param[in] current face for call list
		\param[in] height height of current font
	 */
	FontCallList(FT_Face face, unsigned int height);
	/*! Renders a string s, at specified point p
		\param[in] s rendered string
		\param[in] a position, where list is rendered
	 */
	void render(const sad::String & s, const sad::Point2D & p);
	/*! Returns a size of label
		\param[in] s rendered string
		\return size of rendered label
	 */
	sad::Size2D size(const sad::String & s);
	/*! Frees all resources from GPU
	 */
	~FontCallList();
	/*! A total size of font
	 */
	inline float totalSize()
	{
		return m_height + m_descender;
	}
protected:
	GLuint *     m_textures;	     //!< Textures
	GLuint       m_base;			 //!< First id
	sad::Size2D  m_sizes[256];		 //!< Widths
	float        m_height;			 //!< Height of font
	float        m_descender;        //!< A maximal descender for a font
	float        m_requested_height; //!< A requested height on creation
	float        m_linegap;          //!< A gap between the lines
	/*! Whether call list was successfully built and is on GPU
	 */
	bool         m_on_gpu;  
	/*! Converts char to wide char
		\param[in] c character to be converted
		\return wide character
	 */
	static wchar_t toWideChar(unsigned char c);
	/*! Creates a texture for specified bitmap, uploading it to GPU
		\param[in, out] bitmap a bitmap data
		\param[in] index offset of data in textures
		\return size of created bitmap
	 */
	sad::Size2D createTextureForBitmap(
		FT_Bitmap & bitmap, 
		unsigned int index
	);
	/*! Taken from http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
		Computes next power of two
		\param[in] v integer
		\return integer with next power of two
	*/
	static unsigned int nextPowerOfTwo(unsigned int v);
};

class FontImpl: public sad::Font
{
public:
	/*! Creates new font implementation
	 */
	FontImpl();
	/*! Loads a font
	    \param[in] filename a path to TTF or other files, to be loaded via freetype
	 */
	virtual bool load(const sad::String & filename);
	/*! Renders a string on screen
	    \param[in] str string
		\param[in] p   upper-left point in viewport coordinates
	 */
	virtual void render(const sad::String & str,const sad::Point2D & p); 
	/*! Returns a estimated size of label, rendered with specified size
	    \param[in] str string
		\return size of label
	 */
	virtual sad::Size2D size(const sad::String & str);	
	/*! Frees all resources from implementation
	 */
	~FontImpl();
protected:
	/*! Creates and returns current size for data
		\param[in] size a new size for data
		\return call list for rendering
	 */
	sad::freetype::FontCallList * callList(unsigned int size);
	/*! A freetype library descriptor
	 */
	FT_Library m_library;
	/*! A used freetype face
	 */
	FT_Face    m_face;
	/*! A call lists as map from type to data 
	 */
	typedef sad::PtrHash<unsigned int, sad::freetype::FontCallList> CallListMap;
	/*! A cache for lists, which renders a string
	 */
	CallListMap m_height_cache;
	/*! Cleans all resources, freeing all data
	 */
	void cleanup();
	/*! A cached size
	 */
	unsigned int m_cached_size;
	/*! A cached list
	 */
	sad::freetype::FontCallList * m_cached_list;
	/*! A first creating time, when font is created
	 */
	bool m_first_time_create_calllist;
	/*! A ratio for creating a call list
	 */
	double m_ratio;
	/*! Creates new call list of specified size
		If first time call list is being created, ratio is stored inside and value
		is recalculated
		\param[in] face a face for list creation
		\param[in] size  
	 */
	sad::freetype::FontCallList * makeCallList(FT_Face face, unsigned int size);
};

}

}


sad::freetype::Font::Font()
: m_dptr(new sad::freetype::FontImpl())
{

}

bool sad::freetype::Font::load(const sad::String & filename)
{
	return m_dptr->load(filename);
}

void sad::freetype::Font::render(const sad::String & str,const sad::Point2D & p)
{
	m_dptr->setSize(m_size);
	m_dptr->setColor(m_color);
	m_dptr->render(str, p);
}

sad::Size2D sad::freetype::Font::size(const sad::String & str)
{
	m_dptr->setSize(m_size);
	return m_dptr->size(str);
}

sad::freetype::Font::~Font()
{
	delete m_dptr;
}

// Uncommend to enable glyph rendering debug
#define FREETYPE_GLYPH_DEBUG

#ifdef FREETYPE_GLYPH_DEBUG

static unsigned const int render_rect_indices_count = 4;

static unsigned int render_rect_indices[render_rect_indices_count][2] = 
{
	{0, 1},
	{1, 2},
	{2, 3},
	{3, 0}
};
/*! Renders rectangle
	\param[in] x1 X coordinate of first point
	\param[in] y1 Y coordinate of first point
	\param[in] x2 X coordinate of second point
	\param[in] y2 Y coordinate of second point
	\param[in] c  ciolor
 */
void debug_render_rect(double x1, double y1, double x2, double y2, const sad::Color & c)
{
	sad::Rect2D r(x1, y1, x2, y2);

	glDisable(GL_TEXTURE_2D);
		
	glBegin(GL_LINES);
	glColor3ub(c.r(), c.g(),  c.b());

	for(int i = 0; i < render_rect_indices_count; i++)
	{
		const sad::Point2D & p1 = r[render_rect_indices[i][0]];
		const sad::Point2D & p2 = r[render_rect_indices[i][1]];

		glVertex2f((float)(p1.x()), (float)(p1.y()));
		glVertex2f((float)(p2.x()), (float)(p2.y()));
	}

	glEnd();

	glEnable(GL_TEXTURE_2D);
}

#endif
sad::freetype::FontCallList::FontCallList(FT_Face face, unsigned int height)
: m_on_gpu(false), m_textures(NULL), m_height(0), m_base(NULL),
m_descender(0), m_requested_height((float)height), m_linegap(0)
{
	// if face is null, than there is nothing we can do
	if (face == NULL)
		return;
	
	m_textures = new GLuint[256];
	m_base = glGenLists(256);
	glGenTextures( 256, m_textures );
	bool result = true;
	FT_Set_Char_Size( face, height << 6, height << 6, 96, 96);

	m_linegap = (float)(FT_CEIL(FT_MulFix(face->height, face->size->metrics.y_scale)));
	double commonfontsize = 0;
	for(unsigned int currentcharindex = 0; currentcharindex < 256; currentcharindex++)
	{
		// Avoid 255 max value overflow and convert to wide char
		wchar_t character = toWideChar((unsigned char)currentcharindex);
		bool characterresult = false;
		if (!FT_Load_Glyph( face, FT_Get_Char_Index( face, character ), FT_LOAD_DEFAULT))
		{
			FT_Glyph glyph = NULL;
			if(!FT_Get_Glyph(face->glyph, &glyph))
			{
				FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
				FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
				FT_Bitmap & bitmap = bitmap_glyph->bitmap;
				
				sad::Size2D size = this->createTextureForBitmap(bitmap, currentcharindex);
				m_sizes[currentcharindex] = size;
			
				// Create new call list
				glNewList(m_base + currentcharindex,GL_COMPILE);	
				glBindTexture(GL_TEXTURE_2D, m_textures[currentcharindex]);	
				glPushMatrix();

				glTranslatef((float)(bitmap_glyph->left),0.0f,0.0f);
				glTranslatef(0.0f,(float)(bitmap_glyph->top - bitmap.rows),0.0f);
				float	x=(float)(bitmap.width) / (float)(size.Width);
				float   y=(float)(bitmap.rows)  / (float)(size.Height);
				glBegin(GL_QUADS);

				glTexCoord2d(0.0f,0.0f); 
				glVertex2f(0.0f,(float)(bitmap.rows));
				glTexCoord2d(0.0f,y); 
				glVertex2f(0.0f,0.0f);
				glTexCoord2d(x,y); 
				glVertex2f((float)(bitmap.width),0);
				glTexCoord2d((float)x,0.0f); 
				glVertex2f((float)(bitmap.width),(float)(bitmap.rows));
				glEnd();

#ifdef FREETYPE_GLYPH_DEBUG
				debug_render_rect(0, 0, bitmap.width, bitmap.rows, sad::Color(255, 255, 0));
#endif

				glPopMatrix();
				glTranslatef((float)(face->glyph->advance.x >> 6) ,0,0);
	
				// Set size computing props
				m_sizes[currentcharindex].Width = (float)(face->glyph->advance.x >> 6);
				m_sizes[currentcharindex].Height = (float)(face->glyph->metrics.vertAdvance >> 6);

				glEndList();

				commonfontsize = std::max(commonfontsize, (double)(bitmap.rows));
				m_descender = std::max(m_descender, (float)(bitmap.rows - bitmap_glyph->top));
				m_height = std::max(m_height, (float)(bitmap_glyph->top));
				
				characterresult = true;
			}
		}
		// Mark this id is invalid, so we can not destroy it on shutdown
		if (characterresult == false)
		{
			m_textures[currentcharindex] = -1;
		}
		result = result && characterresult;
	}

	m_linegap *= this->totalSize() / (float)commonfontsize;

	m_on_gpu = result;
}

/*! Buggy intel gpu guard lock
 */
static sad::Mutex sad_freetype_font_lock;

void sad::freetype::FontCallList::render(const sad::String & s, const sad::Point2D & p)
{
	if (!m_on_gpu)
		return;
   
	float h = m_height; 

	sad::String string = s;
	string.removeAllOccurences("\r");
	sad::StringList lines=string.split('\n');
   
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	glListBase(m_base);
 
	float modelview_matrix[16]={};	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	sad_freetype_font_lock.lock();
	float lineheight = m_height + m_descender;
	for(unsigned int i = 0; i < lines.count(); i++) 
	{
		float offset = (float)(p.y()) - lineheight * i;
		if (i > 0)
		{
			offset -= m_linegap * i;
		}
		// We must decrement offset, since rendering starts at
		// baseline, so we compute baseline by decremnenting line height
		offset += - lineheight + m_descender;

		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(modelview_matrix);
		glTranslatef((float)p.x(), offset, 0);
		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].data());
		glPopMatrix();
	}
	sad_freetype_font_lock.unlock();
   
	glPopAttrib();		
}

sad::Size2D sad::freetype::FontCallList::size(const sad::String & s)
{
	sad::String string = s;
	string.removeAllOccurences("\r");
	sad::StringList lines = string.split('\n');
  	

	sad::Size2D result;
	result.Height  = lines.size() * (m_height + m_descender);
	if (lines.size() > 1)
	{
		result.Height += m_linegap * (lines.size() - 1);
	}
	result.Width = 0.0;

	double ratio =  1.0 /*m_requested_height / m_height*/;
	for(unsigned int i = 0; i < lines.count(); i++)
	{
		double width = 0;	  
		for(unsigned int j = 0; j < lines[i].length(); j++)
		{
			unsigned char c = *reinterpret_cast<unsigned char*>(&(lines[i][j]));
			width += m_sizes[c].Width * ratio;
		}
		result.Width = std::max(width, result.Width); 
	}
	

	return result;
}

sad::freetype::FontCallList::~FontCallList()
{
	glDeleteLists(m_base,256);	
	if (m_textures) {
		for(int i = 0; i < 256; i++)
		{
			if (m_textures >= 0)
			{
				glDeleteTextures(1, m_textures + i);
			}
		}
	}
	delete [] m_textures;
}


wchar_t sad::freetype::FontCallList::toWideChar(unsigned char c)
{
	unsigned char mb[2] = {c, 0};
    wchar_t wc[2] = {c, 0};
	if (c > 127)  
	{ 
		mbtowc(wc,(const char *)mb,2); 
	}
	wchar_t result = wc[0]; 
    // Convert russian locale letters
	if (c == '¸')              
	{ 
		result = 0x0451; 
	}
	if (c == '¨')              
	{ 
		result = 0x0401; 
	}
	if (c >= 'À' && c <= 'ß')   
	{ 
		result = 0x0410 + (c - 'À'); 
	}
	if (c >= 'à' && c <= 'ÿ')   
	{ 
		result = 0x0430 + (c - 'à'); 
	}
	return result;
}

sad::Size2D sad::freetype::FontCallList::createTextureForBitmap(
	FT_Bitmap & bitmap, 
	unsigned int index
)
{
	//Allocate a texture
	unsigned int width = nextPowerOfTwo( bitmap.width );
	unsigned int height = nextPowerOfTwo( bitmap.rows );
	GLubyte* expanded_data = new GLubyte[ 2 * width * height];

	//Fill the data
	for(unsigned int j = 0; j <height;j++) 
	{
		for(unsigned int i = 0; i < width; i++)
		{
			expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = 
				((int)i>=bitmap.width || (int)j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}

	//Create texture	
	glBindTexture( GL_TEXTURE_2D, m_textures[index]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA2, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
	delete [] expanded_data;

	sad::Size2D result;
	result.Width = width;
	result.Height = height;
	return result;
}

unsigned int sad::freetype::FontCallList::nextPowerOfTwo(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

sad::freetype::FontImpl::FontImpl() 
: sad::Font(), m_library(0), m_face(0), m_cached_size(-1), m_cached_list(NULL),
m_first_time_create_calllist(true), m_ratio(0)
{
	// If freetype failed, than do nothing
	if (FT_Init_FreeType(&m_library))  
	{
		m_library = 0;
	}
}

bool sad::freetype::FontImpl::load(const sad::String & filename)
{
	cleanup();
	if (m_library == 0)
		return false;
	// Exit if face creation failed
	if (FT_New_Face( m_library, filename.data(), 0, &m_face )) 
	{
		m_face = 0;
		return false;
	}
	return true;
}


void sad::freetype::FontImpl::render(const sad::String & str,const sad::Point2D & p)
{
	this->setCurrentColor();

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	
	this->callList(m_size)->render(str, p);

	glPopAttrib();
	this->restoreColor();
}

sad::Size2D sad::freetype::FontImpl::size(const sad::String & s)
{
	return this->callList(m_size)->size(s);
}

sad::freetype::FontImpl::~FontImpl()
{
	cleanup();
	if (m_library != 0)
		FT_Done_FreeType(m_library);
}


sad::freetype::FontCallList * sad::freetype::FontImpl::callList(unsigned int size)
{
	if (m_cached_list != NULL && size == m_cached_size)
	{
		return m_cached_list;
	}

	if (m_height_cache.contains(size))
	{
		m_cached_size = size;
		m_cached_list = m_height_cache[size];
		return m_cached_list;
	}

	sad::freetype::FontCallList * list = makeCallList(m_face, size);
	m_cached_size = size;
	m_cached_list = list;
	m_height_cache.insert(size, list);

	return list;
}


sad::freetype::FontCallList * sad::freetype::FontImpl::makeCallList(FT_Face face, unsigned int size)
{
	unsigned int pointsize = (unsigned int)((float)size / 3 * 4);
	return new sad::freetype::FontCallList(m_face, (unsigned int)pointsize);
}

void sad::freetype::FontImpl::cleanup()
{
	if (m_face != 0)
	{
		FT_Done_Face(m_face);
	}
	for(CallListMap::iterator it = m_height_cache.begin(); it != m_height_cache.end(); it++)
	{
		delete it.value();
	}
	m_height_cache.clear();
	m_cached_list = NULL;
}

