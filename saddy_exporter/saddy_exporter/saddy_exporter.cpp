// saddy_exporter.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "../pngimage.h"
#include "../templates/hlist.hpp"
#include <stdio.h>


using namespace hst;

int main(int argc, char ** argv)
{
	if (argc<7)
	{
		printf("Usage: exporter [font] [size] [colorr] [colorg] [colorb] [output]\n");
		return 11;
	}
	int size=0;int colorr=0,colorg=0,colorb=0;
	sscanf(argv[2],"%d",&size);
	sscanf(argv[3],"%d",&colorr);
	sscanf(argv[4],"%d",&colorg);
	sscanf(argv[5],"%d",&colorb);

	hst::font fnt(size,argv[1],1,hst::acolor(colorr,colorg,colorb,255));
	hst::string txt="a";
	
	hst::list<hst::xyrect> fr;

	pngimage test("file.png",32,32);

	int maxy=0,maxx=0;
	
	for (unsigned char i=33;i<128;i++)
	{
		printf("%d. %c \n",i,i);
		txt[0]=i;
		MarkedupText tmp;
		tmp.add(hpair<hst::font,hString>(fnt,txt));
		hst::xyrect tt=pngimage::sizeOf(tmp);
		if (maxx<tt.width())
		{
			maxx=tt.width();
		}
		if (maxy<tt.height())
		{
			maxy=tt.height();
		}
		fr<<tt;
	}
	int sizex=10*maxx;
	int sizey=10*maxy;

	pngimage result(hst::string(argv[6])+hst::string(".PNG"),sizex,sizey);
	int curx=0;
	int cury=0;

	FILE * file=fopen((hst::string(argv[6])+hst::string(".CFG")).data(),"wt");
	if (!file) return 121;
	fprintf(file,"%d\n",128-33);
	for (int i=0;i<fr.count();i++)
	{
      txt[0]=i+33;
	  int cx=curx+(maxx-fr[i].width())/2;
	  int cy=cury-fr[i].height();
	  result.drawText(pointf(curx,cury),MarkedupText()<<hst::pair<hst::font,hst::string>(fnt,txt));
	  fprintf(file,"%c %f %f %f %f\n",txt[0],
		                              (float)curx/(float)sizex,
									  (float)cury/(float)sizey,
									  (float)(curx+fr[i].width())/(float)sizex,
									  (float)(cury+fr[i].height())/(float)sizey
									  );
	  if ((i+1)%10==0)
	  {
		  curx=0;
		  cury+=maxy;
	  }
	  else
	  {
		  curx+=maxx;
	  }
	}
	fclose(file);
	result.save();
	return 0;
}

