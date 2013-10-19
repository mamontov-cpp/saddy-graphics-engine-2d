#include "texture.h"
#include <vector>

void sad::Texture::makePOT()
{
	if (m_width==m_height && ((m_width & (m_width-1)) ==0 )) return;
    if (m_width==0) return;

	int m_pottest=(m_width+m_height)/2;
	int m_inc=( (m_pottest & (m_pottest-1) )==0)?0:1;

	int m_power=31;
	bool flag=false;
	while (!flag)
	{
		int test=m_pottest & (1 << m_power);
		if (test) 
			flag=true;
		if (!flag)
		{
		 --m_power;
		 if (m_power<0) return;
		}
	}

	int size=1 << m_power;
    size= size << m_inc;
	scaleFast(size,size);
}
void sad::Texture::scaleFast(int width, int height)
{
	std::vector<sad::uchar> datax;
	datax.resize(width*height*m_bpp/8);
    int b8=m_bpp/8;
    for (int i=0;i<height;i++)
	{
		for (int j=0;j<width;j++)
		{
			int pos_dest=(i*width+j)*b8;
			int pos_src_y=(int)((float)i/(float)height*m_height);
			int pos_src_x=(int)((float)j/(float)width*m_width);
			int pos_src=(pos_src_y*m_width+pos_src_x)*b8;

			datax[pos_dest]=m_data[pos_src];
			datax[pos_dest+1]=m_data[pos_src+1];
			datax[pos_dest+2]=m_data[pos_src+2];
			datax[pos_dest+3]=m_data[pos_src+3];
		}
	}

	int tsize=width*height*b8;
	for (int i=m_width*m_height*b8;i<tsize;i++)
	{
		m_data.add(255);
	}
	memcpy(&(m_data[0]),&(datax[0]),(size_t)tsize);
	m_width=width;
	m_height=height;
}

inline sad::uchar mid(sad::uchar xx,sad::uchar a1,sad::uchar a2,sad::uchar a3, sad::uchar a4)
{
	//return (sad::uchar)( ( xx+(int)a1/10+(int)a2/10+(int)a3/10+(int)a4/10 )/5 );
	return xx/5+a1/5+a2/5+a3/5+a4/5;
}
void sad::Texture::scale(int width, int height)
{
	std::vector<sad::uchar> datax;
	datax.resize(width*height*m_bpp/8);
    int b8=m_bpp/8;
	int pos_src_xm; int pos_src_xp;
	int pos_src_ym; int pos_src_yp;
    
	
	for (int i=0;i<height;i++)
	{
		for (int j=0;j<width;j++)
		{
			int pos_dest=(i*width+j)*b8;
			int pos_src_y=(int)((float)i/(float)height*m_height);
			int pos_src_x=(int)((float)j/(float)width*m_width);
			int pos_src=(pos_src_y*m_width+pos_src_x)*b8;
			
			pos_src_xm=(pos_src_x==0)?pos_src_x:pos_src_x-1;
			pos_src_xp=(pos_src_x==m_width-1)?pos_src_x:pos_src_x+1;
			pos_src_ym=(pos_src_y==0)?pos_src_y:pos_src_y-1;
			pos_src_yp=(pos_src_y==m_height-1)?pos_src_y:pos_src_y+1;
	
			sad::uchar * xx=&(m_data[pos_src]);
			sad::uchar * xmym=pixel(pos_src_ym,pos_src_xm);
			sad::uchar * xmyp=pixel(pos_src_ym,pos_src_xp);
			sad::uchar * xpym=pixel(pos_src_yp,pos_src_xm);
			sad::uchar * xpyp=pixel(pos_src_yp,pos_src_xp);

			datax[pos_dest]=mid(xx[0],xmym[0],xmyp[0],xpym[0],xpyp[0]);
			datax[pos_dest+1]=mid(xx[1],xmym[1],xmyp[1],xpym[1],xpyp[1]);
			datax[pos_dest+2]=mid(xx[2],xmym[2],xmyp[2],xpym[2],xpyp[2]);
			datax[pos_dest+3]=mid(xx[3],xmym[3],xmyp[3],xpym[3],xpyp[3]);

		}
	}

	int tsize=width*height*b8;
	for (int i=m_width*m_height*b8;i<tsize;i++)
	{
		m_data.add(255);
	}
	memcpy(&(m_data[0]),&(datax[0]),(size_t)tsize);
	m_width=width;
	m_height=height;
}
