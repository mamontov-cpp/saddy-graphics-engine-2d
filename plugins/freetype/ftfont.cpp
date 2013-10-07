#include "ftfont.h"

#include <log/log.h>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>


#include <GL/gl.h>
#include <GL/glu.h>

#include <assert.h>

/** Font information about a freetype
 */
struct FTFontInfo
{
	FT_Library library;  //!< A freetype library descriptor
	FT_Face    face;     //!< A used freetype face 
};


FTFont::FTFont()
{
 m_info = new FTFontInfo();
 m_info->library=0; // Init all records as all
 m_info->face=0;    // Init face as null
 if (FT_Init_FreeType( &(m_info->library) ))  //!<  Freetype failed
 {
	 m_info->library = NULL;
 }
 m_renderheight = 0;
 m_rendercolor = hst::acolor(0,0,0,0);
}

FTFont::FTHeightFont * FTFont::newFTHeightFont(unsigned int height) 
{
	FTFont::FTHeightFont * result = new FTFont::FTHeightFont();
	result->m_base = 0;
	result->m_texs = NULL;
	for (int i=0;i<256;i++)
		result->m_w[i]=0;
	result->m_height = 0; 
	return result;
}
void FTFont::deleteFTHeightFont(FTFont::FTHeightFont *fnt) 
{
	if (fnt->m_texs) {
		glDeleteLists(fnt->m_base,256);
		glDeleteTextures(256,fnt->m_texs);
		delete [] fnt->m_texs;
	}
	delete  fnt;
}
hRectF FTFont::sizeOfFont(FTFont::FTHeightFont * fnt, unsigned int height, const sad::String & str)
{
  float maxx=0.0f,cury=(float)height,curx=0.0f;

  sad::StringList lines=str.split('\n');
  // 1.3 - is a space of data
  cury = (lines.count()) * fnt->m_height*1.3f;
  for(unsigned int i=0;i<lines.count();i++)
  {
	  curx = 0.0f;
	  for(unsigned int j=0;j<lines[i].length();j++)
	  {
		  curx+=fnt->m_w[(unsigned char)(lines[i][j])];
	  }
	  if (curx>maxx)
	  {
		  maxx = curx;
	  }
  }
  
  return hRectF(hPointF(0,0),hPointF(maxx,cury));
}
hRectF FTFont::size(const sad::String & str)
{
  if (m_lists_cache.contains(this->m_renderheight) == false) {
	return hRectF(hPointF(0,0),hPointF(0,0));
  }
  return this->sizeOfFont(m_lists_cache[m_renderheight],m_renderheight,str);
}

// BUG: on old Intel GMA drivers CallLists can segfault while
// mulithreading. Have fun with that.
os::mutex m_render_ft_font_mutex;

void FTFont::renderWithHeight(FTFont::FTHeightFont * fnt, 
							  unsigned int height, 
							  const sad::String & str, 
							  float x, 
						      float y
						     )
{
   if (fnt->m_texs == NULL)
		return;
   //Get color
   GLint   clr[4]={};
   glGetIntegerv(GL_CURRENT_COLOR,clr);
   glColor4ub(m_rendercolor.r(),m_rendercolor.g(),m_rendercolor.b(),255-m_rendercolor.a());
   
   GLuint font=fnt->m_base;
   float h=(float)height/0.63f;						//We make the height about 1.5* that of

   sad::StringList lines=str.split('\n');
   
   glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
   glMatrixMode(GL_MODELVIEW);
   glDisable(GL_LIGHTING);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

   glListBase(font);

   float modelview_matrix[16]={};	
   glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

   m_render_ft_font_mutex.lock();
   for(unsigned int i=0;i<lines.count();i++) 
   {
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(modelview_matrix);
		glTranslatef(x,y-(fnt->m_height)*i*1.3f-fnt->m_height ,0);
		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].data());
		glPopMatrix();
   }
   m_render_ft_font_mutex.unlock();
   
   glPopAttrib();		
   //Restore color
   glColor4iv(clr);
   //coord_system::restore();
}
void FTFont::render(const sad::String & str, float x, float y)
{
   if (m_lists_cache.contains(this->m_renderheight) == false) {
	return;
  }
  return this->renderWithHeight(m_lists_cache[m_renderheight],m_renderheight,str,x,y);
}


void FTFont::shutdownFTFace()
{
 if (m_info->face) {
	 FT_Done_Face(m_info->face);
 }
 m_info->face = NULL;
}

void FTFont::cleanupHeightContainer()
{
 for(FTFont::HeightContainer::iterator it= m_lists_cache.begin(); it!=m_lists_cache.end(); it++)
 {
	 FTFont::FTHeightFont * fnt = it.value();
	 this->deleteFTHeightFont(fnt);
 }
 this->m_lists_cache.clear();
}

FTFont::~FTFont() 
{
 this->cleanupHeightContainer();
 this->shutdownFTFace();
 if (m_info->library) {
	 FT_Done_FreeType(m_info->library);
 }
 delete m_info;
}

/*! Calculates next power 2
    \param[in] a number
*/
inline int next2 ( int a )
{
	int p=1;
	while(p<a) 
		p=p<<1;
	return p;
}

/*! Creates a list
*/
static bool create_list(FT_Face face, unsigned  char ch, GLuint base, GLuint * tbase, float & w, float * _pheight )
{ 
    unsigned char mb[89]={ch,0};
    wchar_t wc[2]={ch,0};
	if (ch>127)  { mbtowc(wc,(const char *)mb,2); }
 
    if (ch==(unsigned char)'¸')              { wc[0]=0x0451; }
	if (ch==(unsigned char)'¨')              { wc[0]=0x0401; }
	if (ch>=(unsigned char)'À' && ch<=(unsigned char)'ß')   { wc[0]=0x0410+((char)ch-'À'); }
	if ((char)ch>='à' && (char)ch<='ÿ')   { wc[0]=0x0430+((char)ch-'à'); }
 
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, wc[0] ), FT_LOAD_DEFAULT ))
		return false;  // Load glyph failed
	
	FT_Glyph glyph=NULL;
    if(FT_Get_Glyph( face->glyph, &glyph ))
		return false; // Get glyph failed
    
	
	//Converting glyph to a bitmap
	FT_Error err = FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	//Allocate a texture
	int width = next2( bitmap.width );
	int height = next2( bitmap.rows );
	GLubyte* expanded_data = new GLubyte[ 2 * width * height];

	//Fill the data
	for(int j=0; j <height;j++) 
	{
		for(int i=0; i < width; i++)
		{
			expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = 
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}

	//Create texture	
	glBindTexture( GL_TEXTURE_2D, tbase[ch]);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA2, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
	
	//assert(glGetError() == GL_NO_ERROR);
	delete [] expanded_data; // Free the memory, because texture is already created	
	glNewList(base+ch,GL_COMPILE);	
	glBindTexture(GL_TEXTURE_2D,tbase[ch]);	
	glPushMatrix();

	glTranslatef((float)(bitmap_glyph->left),0.0f,0.0f);
	glTranslatef(0.0f,(float)(bitmap_glyph->top-bitmap.rows),0.0f);
	float	x=(float)bitmap.width / (float)width,y=(float)bitmap.rows / (float)height;
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f,0.0f); glVertex2f(0.0f,(float)(bitmap.rows));
	glTexCoord2d(0.0f,y); glVertex2f(0.0f,0.0f);
	glTexCoord2d(x,y); glVertex2f((float)(bitmap.width),0);
	glTexCoord2d((float)x,0.0f); glVertex2f((float)(bitmap.width),(float)(bitmap.rows));
	glEnd();
	
	glPopMatrix();
	glTranslatef((float)(face->glyph->advance.x >> 6) ,0,0);
	
	// Set size computing props
	w=(float)(face->glyph->advance.x >> 6);
	
	float glyph_height =  (float)(bitmap_glyph->top);
	*_pheight = (glyph_height > *_pheight)? glyph_height : *_pheight;
	glEndList();
	
	

	return true;
}

void FTFont::setColor(const hst::acolor & cl)
{
	m_rendercolor=cl;
}

// BUG: on old Intel GMA cards two concurrent calls
// of glTexImage2D can cause texture corruption
static os::mutex m_ftfont_setheight_lock;
bool FTFont::setHeight(unsigned int height)
{
	m_ftfont_setheight_lock.lock();
	bool result = true;
	if (m_lists_cache.contains(height) == false) 
	{
		result = this->buildHeightFont(height);
	}
	m_renderheight = height;
	m_ftfont_setheight_lock.unlock();
	return result;
}

bool FTFont::buildHeightFont(unsigned int height)
{
	FTFont::FTHeightFont * font = this->newFTHeightFont(height);
	bool result = true;
	if (font!=NULL) 
	{
		// Actual build stuff
		font->m_texs = new GLuint[256];
		if (font->m_texs == NULL) {
			delete font;
			result  = false;
		}
		else 
		{
		   font->m_base=glGenLists(256);
		   glGenTextures( 256, font->m_texs );
		   bool flag=true;
		   FT_Set_Char_Size( m_info->face, height << 6, height << 6, 96, 96);
		   for(unsigned char i=0;i<255;i++)
		   {
			   flag=flag && create_list(m_info->face,i,font->m_base,font->m_texs,*(font->m_w+i),&(font->m_height));
		   }
		   if (!flag)
		   {
		    delete font->m_texs; font->m_texs=NULL;
			delete font;
		   }
		   result = flag;
		}
		if (font)
			this->m_lists_cache.insert(height,font);
	}
	else 
	{
	  result = false;
	}
	return result;
}



bool FTFont::load(const char * fnt_file, unsigned int height, const hst::acolor & cl)
{
   // If library does not exists
   if (m_info->library == 0) 
   {
	   return false;
   }
   FT_Face face=0;
   // Exit if face creation failed
   if (FT_New_Face( m_info->library, fnt_file, 0, &face )) 
   {
	   return false;
   }
   cleanupHeightContainer();
   shutdownFTFace();
   m_info->face = face;
   setColor(cl);
   return setHeight(height);
}


void FTFont::render(const sad::String & str,const pointf & p)
{
  this->render(str,(float)(p.x()),float(p.y()));
}

