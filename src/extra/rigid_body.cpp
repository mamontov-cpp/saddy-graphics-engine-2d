#include "rigid_body.h"
#include "../../include/p2d/vector.h"
#include "../../include/p2d/collides1d.h"
#include <utility>
RigidBody::~RigidBody() {}

vector32 ortho(const vector32 & v)
{
	if (v.y()<=1.0E-6 && v.y()>=-1.0E-6 ) return vector32(0,1,0);
	//x*v.x()+y*v.y()=0  -> y*v.y=-v.x - > y=-v.x/v.y 
	float x=1;
	float y=0.0f-v.x()/v.y();
	return vector32(x,y,0);
}


struct collide_info
{
	const ::s3d::point * px[2];
	const ::s3d::point * py[2];
};
struct boxes
{
	const BoundingBox * b[2];
};
/*! Sets test points for further testing
*/
template<int ax1,int ax2,int ax3,int ax4>
void set_test_points(collide_info & i,boxes & bx )
{
	i.px[0]=&( bx.b[0]->operator[](ax1) );
	i.px[1]=&( bx.b[1]->operator[](ax2) );
	i.py[0]=&( bx.b[0]->operator[](ax3) );
	i.py[1]=&( bx.b[1]->operator[](ax4) );
}

typedef void (*point_testers)(collide_info & i,boxes & bx);

point_testers testers[2][2]={   
{ set_test_points<2,0,3,1>, set_test_points<1,3,0,2> },
{ set_test_points<3,1,2,0>, set_test_points<0,2,1,3> }
};

bool collides(const BoundingBox & old_1,const BoundingBox & new_1,
			  const BoundingBox & old_2,const BoundingBox & new_2
			  )
{
	return false;
}

bool collides(const BoundingBox & old, const BoundingBox & nwp,
			  const ::s3d::point & p1, const ::s3d::point & p2 )
{
 bool result=true;
 boxes b; b.b[0]=&old; b.b[1]=&nwp;
 collide_info ci;

 point_testers 
 tester=testers[(nwp[0].x()>old[0].x())?1:0][(nwp[0].y()>old[0].y())?1:0];
 tester(ci,b);

 //Test horizontal and vertical axis
 bool collides_x=p2d::collides1D(ci.px[0]->x(),ci.px[1]->x(),p1.x(),p2.x());
 bool collides_y=p2d::collides1D(ci.px[0]->y(),ci.px[1]->y(),p1.y(),p2.y());
 result=collides_x && collides_y;
 if (result)
 {
   //Calculate projections on axis of first body
		::s3d::point axe_11=p2-p1;
		normalize(axe_11);
		::s3d::point axe_12=ortho(axe_11);
		//Proect on first
		float min=scalar(*(ci.px[0]),axe_11),max=scalar(*(ci.px[1]),axe_11);
		if (min>max) std::swap(min,max);

#define IF_SET(I2,AXE)  tmp=scalar(*(ci.py[I2]),AXE);       \
				 if (tmp<min) min=tmp; if (tmp>max) max=tmp;       
		float tmp=0.0f; 
		IF_SET(0,axe_11);
		IF_SET(1,axe_11);
		bool collides11=p2d::collides1D(scalar(p1,axe_11),scalar(p2,axe_11),min,max);
		min=scalar(*(ci.px[0]),axe_12),max=scalar(*(ci.px[1]),axe_12);
		if (min>max) std::swap(min,max);
		IF_SET(0,axe_12);
		IF_SET(1,axe_12);
		bool collides12=p2d::collides1D(scalar(p1,axe_12),scalar(p2,axe_12),min,max);
		result=collides11 && collides12;
#undef IF_SET
		return result;
 }
 else
	 return result;
}

bool collides(const RigidBody & b,const ::s3d::point & p1, const ::s3d::point & p2)
{
	return collides(b.oldPoint(),b.newPoint(),p1,p2);
}

#define PRECISION 0.00001
float collision_time(const RigidBody & b1, const RigidBody & b2, BoundingBox & pos)
{
  float begin=0;
  float end=b1.interval();
  BoundingBox pos1=b1.oldPoint();
  BoundingBox pos2=b1.newPoint();
  while(end-begin>PRECISION)
  {
	float time=(begin+end)/2.0f;
	BoundingBox tmp=b1.oldPoint();
	move(tmp,b1.v()*time);
	if (collides(pos1,tmp,b2.oldPoint(),b2.newPoint()))
	{ pos2=tmp; end=time; }
	else
	{ pos1=tmp; begin=time; }
  }
  pos=pos2;
  return end;
}
float collision_time2(const RigidBody & b1, const RigidBody & b2, BoundingBox & pos1,BoundingBox & pos2)
{
  float begin=0;
  float end=b1.interval();
  BoundingBox pos11=b1.oldPoint();
  BoundingBox pos12=b1.newPoint();
  BoundingBox pos21=b2.oldPoint();
  BoundingBox pos22=b2.newPoint();
  while(end-begin>PRECISION)
  {
	float time=(begin+end)/2.0f;
	BoundingBox tmp1=b1.oldPoint();
	move(tmp1,b1.v()*time);
	BoundingBox tmp2=b1.oldPoint();
	move(tmp2,b2.v()*time);
	if (collides(pos11,tmp1,pos12,tmp2))
	{ pos12=tmp1; pos22=tmp2; end=time; }
	else
	{ pos11=tmp2; pos21=tmp2; begin=time; }
  }
  pos1=pos12;
  pos2=pos22;
  return end;
}

DECLARE_SOBJ_INHERITANCE(Collidable,Sprite)

Collidable::~Collidable() {}

bool Collidable::collides(Collidable * o)
{
	return ::collides(oldPoint(),newPoint(),o->oldPoint(),o->newPoint());
}

bool testCollidables(void * b1,void * b2)
{
	return reinterpret_cast<Collidable *>(b1)->collides(reinterpret_cast<Collidable*>(b2));
}
