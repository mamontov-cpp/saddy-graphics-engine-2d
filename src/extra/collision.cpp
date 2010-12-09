#include "stdafx.h"
#include <math.h>
#include "collision.h"
//Function which return length of collision of two spaces on one axle
//@in x11,x12 - coordinates of first cutter
//@in x21,x22 - coordinates of second cutter
//@out float - length of first collision, 0 if no collision detected.
//length is a direction on which should be moved 1 line, opposite the direction of axle
float get1DCollision(float x11,float x12, float x21,float x22)
{
  float result=0;
  float tmp1,tmp2; //tmp values for comparing;
  if (x11<x21)
  {
	  //Case
	  //               x21----------------x22
	  //x11------------------x12
	  //x11------------------------------------x12
	  if (x12>=x21)
	  {
         result=x12-x21;
	  }
  }
  else
  {
	  //Case
	  //x21------------------x22
	  //         x11-----x12
	  //         x11--------------x12
	  if (x11<=x22)
	  {
	    tmp1=x12-x21;
		tmp2=x22-x11;
		result=(tmp1<tmp2)?tmp1:-1*tmp2;
	  }
  }
  return result;
}
float get1DCollision(const h1DLine  & l1, const h1DLine & l2)
{
	return get1DCollision(l1.p1(),l1.p2(),l2.p1(),l2.p2()); //This is just an overhead
}
//Function which returns a rectangle of common space of rectangles r1 and r2
//@in r1 - first rectangle
//@in r2 - second rectangle
//@out rectangle of collision space, or point (0,0) if collision is not detected.
//Width and height of this rectangle, shows a length of moving rectangle r1 left and up
//to make this two with no collisions.
hst::xyrect getCollisionRect(const hst::xyrect & r1, const hst::xyrect & r2)
{
	hst::xyrect result(hPointF(0,0),0,0); //Init some empty result;
	float colwidth=0;                  //Width of collision
	float colheight=0;                 //Height of collision
	colwidth =get1DCollision((float)(r1[0].x()),(float)(r1[1].x()),(float)(r2[0].x()),(float)(r2[1].x()));
	colheight=get1DCollision((float)(r1[0].y()),(float)(r1[3].y()),(float)(r2[0].y()),(float)(r2[3].y()));
	if (fabs(colwidth)>0.0001 && fabs(colheight)>0.0001) //They are colliding
	{
		result.setHeight(colheight);
		result.setWidth(colwidth);
	}
	return result;   //Return result
}
//Function, which detects a collision of two rectangles, also detects a collision
//Of r1, related to r2
//@in r1 - first rectangle
//@in r2 - second rectangle
//@out bool - true if collides, false if not.
bool collides(const hst::xyrect & r1,const hst::xyrect & r2)
{
 float colwidth=0;                  //Width of collision
 float colheight=0;                 //Height of collision
 //CFIX_LOG(L"%d %d %d %d\n",(int)(r1[0].x),(int)(r1[0].y),(int)(r1.width()),(int)(r1.height()));
 //CFIX_LOG(L"%d %d %d %d\n",(int)(r2[0].x),(int)(r2[0].y),(int)(r2.width()),(int)(r2.height()));

 colwidth =get1DCollision((float)(r1[0].x()),(float)(r1[1].x()),(float)(r2[0].x()),(float)(r2[1].x()));
 colheight=get1DCollision((float)(r1[0].y()),(float)(r1[3].y()),(float)(r2[0].y()),(float)(r2[3].y()));
 //CFIX_LOG(L"Width:%d Height:%d\n",abs(colwidth)>0.001,abs(colheight)>0.001);
 if (fabs(colwidth)>0.00001 && fabs(colheight)>0.00001) //If they are collides
	 return true;
 else
	 return false;
 
}
inline bool iseqf(float f1,float f2) {return fabs(f1-f2)<0.1;}
bool     collides(const hLine & li1, const  hLine & li2,axle_t ax)
{
   h1DLine l1=li1.proect(ax),l2=li2.proect(ax); //Use projection functions
   if (iseqf(l1.p1(),l1.p2()))
   {
	   if (iseqf(l1.p1(),l2.p1()) )
		   return true;
	   if (iseqf(l1.p1(),l2.p2()) )
		   return true;
   }
   if (iseqf(l2.p1(),l2.p2()))
   {
	   if (iseqf(l2.p1(),l1.p1()) )
		   return true;
	   if (iseqf(l2.p1(),l1.p2()) )
		   return true;
   }
   return fabs(get1DCollision(l1,l2))>0.0001;   
}
bool     collides(const hLine & li1,const  hLine & li2)
{
   axle_t ax1=li1.getAxle(),ax2=li1.getPerAxle(),ax3=li2.getAxle(),ax4=li2.getPerAxle();
   bool res=false;
   //Poly collides, for four dividing functions
   bool t1=collides(li1,li2,ax1);
   bool t2=collides(li1,li2,ax2);
   bool t3=collides(li1,li2,ax3);
   bool t4=collides(li1,li2,ax4); 
   res=t1 && t2 && t3 && t4;
   return res;
}
bool     isWithin(const hPointF & p,const hst::xyrect & r)
{
	double dx=p.x()-r.p().x();
	double dy=p.y()-r.p().y();
	return (dx>0 && dx<r.width()) && (dy>0 && dy<r.height()); //Simply compare 
}
bool     collides(const hLine & l,const hst::xyrect & r)
{
	h1DLine horrect((float)(r.p().x()),(float)(r.p().x()+r.width()));
	h1DLine vorrect((float)(r.p().y()),(float)(r.p().y()+r.height()));
    h1DLine hline=l.proect(0);
	h1DLine vline=l.proect(M_PI/2);
	bool  vcollides=fabs(get1DCollision(vorrect,vline))>0.001;
	bool  hcollides=fabs(get1DCollision(horrect,hline))>0.001;
	return vcollides && hcollides;
}
bool     collides(const hPointF & p1,const hPointF & p2,const hst::xyrect & r)
{
	return isWithin(p1,r) || isWithin(p2,r);
}
//return true if collides
bool    getCollisionPoint(const hLine & l1, const hLine & l2,hPointF & res)
{
   if (collides(l1,l2))
   {
     //Analize lines, to determine their descriptor
	 //This method works with straight line, so don't use it for finite lines
     hsline s1=l1.analyze(),s2=l2.analyze();
	 /*! This code just provides solving system equations alike
	    k11*x+k12*y=b1
		k21*x+k21*y=b1
		By the Kramer method
	 */
	 float D=s1.k1*s2.k2-s1.k2*s2.k1;
	 if (fabs(D)>0.001)               //If lines not fully colliding
	 {
       float D1=s1.b*s2.k2-s2.b*s1.k2;
 	   float D2=s2.b*s1.k1-s1.b*s2.k1;
	   res=hPointF(D1/D,D2/D);
	 }
	 else
	 {
       res=(l1.point1()+l1.point2()+l2.point1()+l2.point2())*0.25;
	 }
	 return true;
   }
   else 
	   return false;
}

bool detectDiagonalCollision(const hst::xyrect & r1f,
			                 const hst::xyrect & r1s,
			                 const hst::xyrect & r2f,
			                 const hst::xyrect & r2s
			                )
{
	//Calculate first orthogonal axle (first box)

	h1DLine pr1,pr2;
	axle_t ax1=0.0f, ax2=0.0f;
	hPointF pp1_n,pp2_n,pp1_p,pp2_p;  //Horizontal and vertical
	if (r1f.p().x()<r1s.p().x())
	{
		if (r1f.p().y()<r1s.p().y())
		{
			pp1_n=r1f[0]; pp2_n=r1s[2];
			pp1_p=r1f[1]; pp2_p=r1f[3];
		}
		else
		{
			pp1_n=r1f[3]; pp2_n=r1s[1];
            pp1_p=r1f[0]; pp2_p=r1f[2];
		}
	}
	else
	{
        if (r1f.p().y()<r1s.p().y())
		{
			pp1_n=r1f[1]; pp2_n=r1s[3];
			pp1_p=r1f[0]; pp2_p=r1f[2];
		}
		else
		{
			pp1_n=r1f[2]; pp2_n=r1s[0];
            pp1_p=r1f[1]; pp2_p=r1f[3];
		}
	}
	hLine test1(pp1_n,pp2_n);
	ax1=test1.getAxle();
	if (ax1>0) ax2=ax1-1.57; else ax2=ax1+1.57;
	pr1=test1.proect(ax1);
	pr2=hLine(pp1_p,pp2_p).proect(ax2);

	hLine test21(r2f[0],r2s[1]), test22(r2f[3],r2s[1]);
	hLine test23(r2f[0],r2f[2]), test24(r2f[1],r2f[3]);

    float precision=0.0001;  //Precision of detection
	h1DLine t21=test21.proect(ax1);
	t21.addLine(test22.proect(ax1));
	t21.addLine(test23.proect(ax1));
	t21.addLine(test24.proect(ax1));
	
	h1DLine t22=test21.proect(ax2);
	t22.addLine(test22.proect(ax2));
	t22.addLine(test23.proect(ax2));
	t22.addLine(test24.proect(ax2));
	
	float tf1=fabs(get1DCollision(pr1,t21));
	float tf2=fabs(get1DCollision(pr2,t22));

	return tf1>precision && tf2>precision;
}
bool collides(const hst::xyrect & r1f,
			  const hst::xyrect & r1s,
			  const hst::xyrect & r2f,
			  const hst::xyrect & r2s
			  )
{
    if (!detectDiagonalCollision(r1f,r1s,r2f,r2s)) 
		return false;
	if (!detectDiagonalCollision(r2f,r2s,r1f,r1s)) 
		return false;

	h1DLine det1, det2;
	if (r1f.p().x()<r1s.p().x()) { det1=h1DLine(r1f.p().x(),r1s.p().x()+r1s.width());}
	else                         { det1=h1DLine(r1s.p().x(),r1f.p().x()+r1f.width());}    
	if (r2f.p().x()<r2s.p().x()) { det2=h1DLine(r2f.p().x(),r2s.p().x()+r2s.width());}
	else                         { det2=h1DLine(r2s.p().x(),r2f.p().x()+r2f.width());}    

	float precision=0.0001; 
	if (fabs(get1DCollision(det1,det2))<precision) return false;
	

	if (r1f.p().y()<r1s.p().y()) { det1=h1DLine(r1f.p().y(),r1s.p().y()+r1s.height());}
	else                         { det1=h1DLine(r1s.p().y(),r1f.p().y()+r1f.height());}    
	if (r2f.p().y()<r2s.p().y()) { det2=h1DLine(r2f.p().y(),r2s.p().y()+r2s.height());}
	else                         { det2=h1DLine(r2s.p().y(),r2f.p().y()+r2f.height());}    

	 
	if (fabs(get1DCollision(det1,det2))<precision) return false;
	
	return true;
}

bool optimizedcollides(const hst::xyrect & r1f,
					   const hst::xyrect & r1s,
					   const hst::xyrect & r2f,
					   const hst::xyrect & r2s
			          )
{
	float maxx=(r1f.width()<r2f.width())?r2f.width():r1f.width();
	float maxy=(r1f.height()<r2f.height())?r2f.height():r1f.height();

	if (fabs(r1f.p().x()-r1s.p().x())<maxx &&
        fabs(r2f.p().x()-r2s.p().x())<maxx &&
        fabs(r1f.p().y()-r1s.p().y())<maxy &&
        fabs(r2f.p().y()-r2s.p().y())<maxy )
		return collides(r1s,r2s);

	return collides(r1f,r1s,r2f,r2s); 
}
