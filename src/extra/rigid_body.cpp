#include "rigid_body.h"
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
/*! Solves one-dimensional collision task
	\param[in] x11 first coordinate first body
	\param[in] x12 second coordinate first body
	\param[in] x21 first  coordinate second body
	\param[in] x22 second coordinate second body
*/
static inline bool collides1D(float x11, float x12, float x21, float x22)
{
	if (x11>x12) { float tmp=x11; x11=x12; x12=tmp; }
	if (x21>x22) { float tmp=x21; x21=x22; x22=tmp; }
	return (x21<=x12) && (x22>=x11);
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
	bool result=true;
	boxes b1; b1.b[0]=&old_1; b1.b[1]=&new_1;
	boxes b2; b2.b[0]=&old_2; b2.b[1]=&new_2;

	collide_info ci[2];

	point_testers 
	tester=testers[(new_1[0].x()>old_1[0].x())?1:0][(new_1[0].y()>old_1[0].y())?1:0];
	tester(ci[0],b1);
	tester=testers[(new_2[0].x()>old_2[0].x())?1:0][(new_2[0].y()>old_2[0].y())?1:0];
	tester(ci[1],b2);

	//Test horizontal and vertical axis
	bool collides_x=collides1D(ci[0].px[0]->x(),ci[0].px[1]->x(),ci[1].px[0]->x(),ci[1].px[1]->x());
	bool collides_y=collides1D(ci[0].px[0]->y(),ci[0].px[1]->y(),ci[1].px[0]->y(),ci[1].px[1]->y());
    result=collides_x &&  collides_y;
	if (result)
	{
		//Calculate projections on axis of first body
		::s3d::point axe_11=*(ci[0].px[1])-*(ci[0].px[0]);
		normalize(axe_11);
		::s3d::point axe_12=ortho(axe_11);
		//Proect on first
		float min=scalar(*(ci[1].px[0]),axe_11),max=scalar(*(ci[1].px[1]),axe_11);
		if (min>max) std::swap(min,max);

#define IF_SET(I1,I2,AXE)  tmp=scalar(*(ci[I1].py[I2]),AXE);       \
				 if (tmp<min) min=tmp; if (tmp>max) max=tmp;       
		float tmp=0.0f; 
		IF_SET(1,0,axe_11);
		IF_SET(1,1,axe_11);
		bool collides11=collides1D(scalar(*(ci[0].px[0]),axe_11),scalar(*(ci[0].px[1]),axe_11),min,max);
		min=scalar(*(ci[1].px[0]),axe_12),max=scalar(*(ci[1].px[1]),axe_12);
		if (min>max) std::swap(min,max);
		IF_SET(1,0,axe_12);
		IF_SET(1,1,axe_12);
		bool collides12=collides1D(scalar(*(ci[0].py[0]),axe_12),scalar(*(ci[0].py[1]),axe_12),min,max);
		result=collides11 && collides12;
		if (result)
		{
			::s3d::point axe_21=*(ci[1].px[1])-*(ci[1].px[0]);
			normalize(axe_21);
			::s3d::point axe_22=ortho(axe_21);
			min=scalar(*(ci[0].px[0]),axe_21),max=scalar(*(ci[0].px[1]),axe_21);
			if (min>max) std::swap(min,max);
			IF_SET(0,0,axe_21);
			IF_SET(0,1,axe_21);
			bool collides21=collides1D(scalar(*(ci[1].px[0]),axe_21),scalar(*(ci[1].px[1]),axe_21),min,max);
			min=scalar(*(ci[0].px[0]),axe_22),max=scalar(*(ci[0].px[1]),axe_22);
			if (min>max) std::swap(min,max);
			IF_SET(0,0,axe_22);
			IF_SET(0,1,axe_22);
			bool collides22=collides1D(scalar(*(ci[1].py[0]),axe_22),scalar(*(ci[1].py[1]),axe_22),min,max);
			result=collides21 && collides22;
		}
	}
#undef IF_SET
	return result;
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
 bool collides_x=collides1D(ci.px[0]->x(),ci.px[1]->x(),p1.x(),p2.x());
 bool collides_y=collides1D(ci.px[0]->y(),ci.px[1]->y(),p1.y(),p2.y());
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
		bool collides11=collides1D(scalar(p1,axe_11),scalar(p2,axe_11),min,max);
		min=scalar(*(ci.px[0]),axe_12),max=scalar(*(ci.px[1]),axe_12);
		if (min>max) std::swap(min,max);
		IF_SET(0,axe_12);
		IF_SET(1,axe_12);
		bool collides12=collides1D(scalar(p1,axe_12),scalar(p2,axe_12),min,max);
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

SAD_DECLARE(Collidable,Sprite)

Collidable::~Collidable() {}

bool Collidable::collides(Collidable * o)
{
	return ::collides(oldPoint(),newPoint(),o->oldPoint(),o->newPoint());
}

bool testCollidables(void * b1,void * b2)
{
	return reinterpret_cast<Collidable *>(b1)->collides(reinterpret_cast<Collidable*>(b2));
}
