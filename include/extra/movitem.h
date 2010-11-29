#include "types.h"
#include "renderer.h"
#include <math.h>
#include "texture.h"

class MovingItem: public BasicNode {
public:
	//угол в градусах, ибо удобнее для нормального человека.
	MovingItem();
	MovingItem(float angle, float x, float y, float speed, sad::Texture *tex, float lx, float ly, float hx, float hy);
	void construct(float angle, float x, float y, float speed, sad::Texture *tex, float lx, float ly, float hx, float hy);
	bool calcNewPos();//истина если позиция внутри экрана, ложь если вылет за пределы
	void setTexture(sad::Texture *tex, float lx, float ly, float hx, float hy);
	void render();
protected:
	float m_r;
	sad::Texture *m_tex;
	float m_cos,m_cos0,m_cos1,m_cos2,m_cos3;
	float m_sin,m_sin0,m_sin1,m_sin2,m_sin3;
	float m_hx, m_hy, m_lx, m_ly;
	float m_x, m_y;
	float m_speed;
	float m_time;
	inline float x()  {return m_x; }
	inline float y()  {return m_y; }
	inline float lx() {return m_lx;}
	inline float ly() {return m_ly;}
	inline float hx() {return m_hx;}
	inline float hy() {return m_hy;}
	virtual void setId();
	void setRadius();
};

class PlayerBullet: public MovingItem {
public:
	static const int m_s_id = 25;
	inline int id() {return m_id;}
	PlayerBullet(float angle, float x, float y, float speed, sad::Texture *tex, float lx, float ly, float hx, float hy);
protected:
	virtual void setId();
	int m_id;
};

class EnemyBullet: public MovingItem {
public:
	static const int m_s_id = 26;
	inline int id() {return m_id;}
	EnemyBullet(float angle, float x, float y, float speed, sad::Texture *tex, float lx, float ly, float hx, float hy);
protected:
	virtual void setId();
	int m_id;
};

class Bonus: public MovingItem {
public:
	static const int m_s_id = 28;
	inline int id() {return m_id;}
	Bonus(float angle, float x, float y, float speed, sad::Texture *tex, float lx, float ly, float hx, float hy);
protected:
	virtual void setId();
	int m_id;
};