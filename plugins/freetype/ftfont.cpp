#include "ftfont.h"
#include <assert.h>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>


FTFont::FTFont()
{
 m_height=0;
 m_texs=NULL;
 m_base=0;
 m_cl=hst::acolor(0,0,0,0); 
 for (int i=0;i<256;i++)
      m_w[i]=0;
}

hRectF FTFont::size(const hst::string & str)
{
  float maxx=0.0f,cury=m_height,curx=0.0f;
  for (int i=0;i<str.length();i++)
  {
    if (str[i]!='\n') { curx+=m_w[i]; }
	else              { if (curx>maxx) maxx=curx; curx=0.0f; cury+=m_height; }
  }
  if (str[str.length()-1]=='\n') cury-=m_height;
  if (curx>maxx) maxx=curx;
  return hRectF(hPointF(0,0),hPointF(maxx,cury));
}

FTFont::~FTFont() 
{
  if (m_texs)
  {
	glDeleteLists(m_base,256);
	glDeleteTextures(256,m_texs);
	delete [] m_texs;
  }
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
static bool create_list(FT_Face face, unsigned  char ch, GLuint base, GLuint * tbase, float & w )
{ 
    unsigned char mb[2]={ch,0};
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
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
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
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
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
	
	
	float v[4]={};
	glGetFloatv(GL_CURRENT_RASTER_POSITION ,v);
	w=(float)(bitmap.width);
	glEndList();
	
	return true;
}
bool FTFont::load(const char * fnt_file, unsigned int height, const hst::acolor & cl)
{
   m_cl=cl;
   m_texs = new GLuint[256];
   this->m_height=(float)height;
   
   FT_Library library=0;
   if (FT_Init_FreeType( &library ))  //!<  Freetype failed
   {   
		delete m_texs; m_texs=NULL;
		return false;
   }
   FT_Face face=0;
   if (FT_New_Face( library, fnt_file, 0, &face )) //!< Face failed
   {   
        delete m_texs; m_texs=NULL; FT_Done_FreeType(library); return false;
   }
   
   FT_Set_Char_Size( face, height << 6, height << 6, 96, 96);
  
   //Allocated resources
   m_base=glGenLists(256);
   glGenTextures( 256, m_texs );
   bool flag=true;
   for(unsigned char i=0;i<255;i++)
   {
	  flag=flag &&	create_list(face,i,m_base,m_texs,*(m_w+i));
   }
   FT_Done_Face(face);
   FT_Done_FreeType(library);
   if (!flag)
   {
     delete m_texs; m_texs=NULL;
   }
   return flag;
}

void coord_system::switchToWCS()
{
    glPushAttrib(GL_TRANSFORM_BIT);
	GLint	viewport[4]={};
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
	glPopAttrib();
}

void coord_system::restore()
{
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void FTFont::render(const hst::string & str, float x, float y)
{
   if (!m_texs) return;
   coord_system::switchToWCS();
   //Get color
   GLint   clr[4]={};
   glGetIntegerv(GL_CURRENT_COLOR,clr);
   glColor4ub(m_cl.r(),m_cl.g(),m_cl.b(),255-m_cl.a());
   
   GLuint font=m_base;
   float h=m_height/0.63f;						//We make the height about 1.5* that of

   hst::stringlist lines=str.split('\n');
   
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

   for(int i=0;i<lines.length();i++) 
   {
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x,y-h*i,0);
		glMultMatrixf(modelview_matrix);
		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].data());
		glPopMatrix();
   }
   
   
   glPopAttrib();		
   //Restore color
   glColor4iv(clr);
   coord_system::restore();
}

void FTFont::render(const hst::string & str,const pointf & p)
{
  this->render(str,(float)(p.x()),float(p.y()));
}

