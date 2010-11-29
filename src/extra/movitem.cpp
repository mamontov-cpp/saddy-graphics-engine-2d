#include "movitem.h"

MovingItem::MovingItem() {
}
MovingItem::MovingItem(float angle, float x, float y, float speed, sad::Texture *tex, float lx, float ly, float hx, float hy) {
	construct(angle, x, y, speed, tex, lx, ly, hx, hy);
}
void MovingItem::construct(float angle, float x, float y, float speed, sad::Texture *tex, float lx, float ly, float hx, float hy) {
	setTexture(tex, lx, ly, hx, hy);
	angle = angle*3.14/180;
	m_cos = cos(angle);
	m_sin = sin(angle);
	m_cos0 = cos(angle+0.785f);
	m_sin0 = sin(angle+0.785f);
	m_cos1 = cos(angle+2.355f);
	m_sin1 = sin(angle+2.355f);
	m_cos2 = cos(angle+3.925f);
	m_sin2 = sin(angle+3.925f);
	m_cos3 = cos(angle+5.495f);
	m_sin3 = sin(angle+5.495f);
	m_x = x;
	m_y = y;
	m_speed = speed;
	m_time = 0;
	setId();
	setRadius();
}
bool MovingItem::calcNewPos() {
	float time, delta_time;
	time = (float)sad::Renderer::instance().elapsedInMSeconds();
	if (time>m_time) {
		delta_time = time - m_time;
	} else {
		delta_time = time + 1000 - m_time;
	}
	m_time = time;
	m_x = m_x+m_speed*m_cos*delta_time*0.0000001;
	m_y = m_y+m_speed*m_sin*delta_time*0.0000001;
	bool out;
	if (m_x<-0.3f || m_x>0.3f || m_y<-0.3f || m_y>0.3f) {
		out = true;
	} else {
		out = false;
	}
	if (out) {
		sad::Renderer::instance().getCurrentScene()->markForDeletion((sad::BasicNode*)this);
	}
	return !out;
}
void MovingItem::setTexture(sad::Texture *tex,float lx, float ly, float hx, float hy) {
	m_tex = tex;
	m_lx = lx;
	m_ly = ly;
	m_hx = hx;
	m_hy = hy;
}
void MovingItem::render() {
	if (calcNewPos()) {
		m_tex->enable();
		glBegin(GL_QUADS);

	
		glTexCoord2f(m_lx, m_ly);	
		glVertex3f(m_x+m_r*m_sin0,m_y+m_r*m_cos0,0.5);
		glTexCoord2f(m_lx, m_hy);
		glVertex3f(m_x+m_r*m_sin1,m_y+m_r*m_cos1,0.5);
		glTexCoord2f(m_hx, m_hy);
		glVertex3f(m_x+m_r*m_sin2,m_y+m_r*m_cos2,0.5);
		glTexCoord2f(m_hx, m_ly);
		glVertex3f(m_x+m_r*m_sin3,m_y+m_r*m_cos3,0.5);
    
		glEnd();
	}
}
void MovingItem::setRadius() {
	m_r = 0.01f;
}
void MovingItem::setId() {
}
void PlayerBullet::setId() {
	m_id = 25;
}
PlayerBullet::PlayerBullet(float angle, float x, float y, float speed, sad::Texture *tex, float lx, float ly, float hx, float hy) {
	construct(angle, x, y, speed, tex, lx, ly, hx, hy);
}
void EnemyBullet::setId() {
	m_id = 26;
}
EnemyBullet::EnemyBullet(float angle, float x, float y, float speed, sad::Texture *tex, float lx, float ly, float hx, float hy) {
	construct(angle, x, y, speed, tex, lx, ly, hx, hy);
}
void Bonus::setId() {
	m_id = 28;
}
Bonus::Bonus(float angle, float x, float y, float speed, sad::Texture *tex, float lx, float ly, float hx, float hy) {
	construct(angle, x, y, speed, tex, lx, ly, hx, hy);
}