/*! \file   scene.h
\author HiddenSeeker
\brief  Здесь расположены файлы сцены
*/
#include "templates/hlvector.hpp"
#include "templates/hhash.hpp"
#include <assert.h>
#pragma once

namespace sad
{

/*! Класс базового объекта сцены
*/
class BasicNode
{
	float m_x;    //!< Позиция
	float m_y;    //!< Позиция
protected:
	int   m_type; //!< Тип вершины (динамический)
    
	BasicNode();
public:
	static int Type; //!< Статический тип
	/*! Тип
	*/
	int type() const;
	/*! Процедура, которая будет выполняться при рендеринге объекта.
	*/
	virtual void render()=0;
	/*! Деструктор
	*/
	virtual ~BasicNode();
};

/*! Класс камеры
*/
class Camera
{
 private:
	     float m_curX;
		 float m_curY;
		 float m_curZ;

		 float m_rotX;
		 float m_rotY;
		 float m_rotZ;
		 
		 float m_angle;
 public:
	     /*! Default constructed camera
		 */
	     Camera();
		 /*! Custom camera
		     \param x    x coord
			 \param y    y coord
			 \param z    zoom
			 \param angle angle
			 \param rx    rotation of x
			 \param ry    rotation of y
			 \param rz    rotation of z
		 */
         Camera(float x, float y, float z,float angle=0, float rx=0, float ry=0, float rz=0);

		 /*! Applies a camera
		 */
		 void apply();
		 
		 inline float & x() { return m_curX;}
         inline float & y() { return m_curY;}
         inline float & z() { return m_curZ;}

		 inline float & rotationX() { return m_rotX;}
         inline float & rotationY() { return m_rotY;}
         inline float & rotationZ() { return m_rotZ;}
         inline float & angle() { return m_angle;}

		 /*! Destructor
		 */
		 ~Camera();
};
/*! Класс сцены
*/
class Scene
{
private:
	hst::vector<BasicNode *>   m_layers;                //!< Слои, первый элемент и им же рендерится
	hst::hash<hst::string,unsigned long>  m_nodehash;   //!< Вершина по ноде
	hst::vector<BasicNode *>   m_marked;                //!< Помеченные для удаления вершины
	hst::vector< hst::triplet<BasicNode *,hst::string,unsigned long > >   m_toadd;                 //!< Помеченные для добавления вершины 
	sad::Camera              m_camera;                  //!< Текущая камера
public:
	sad::Camera   & camera();  //!< Текущая камера

	Scene();    //!< Пустая сцена
	~Scene();   //!< Деструктор

	/*! Удаляет все из сцены
	*/
	void clear();
	/*! Отмечает вершину для удаления в следующем цикле
	    \param[in] what что надо будет удалить
	*/
	void markForDeletion(BasicNode * what);
	/*! Отмечает вершину для добавление в следующем цикле
	    \param[in] node  объект 
      	\param[in] name  имя
	    \param[in] lay   слой
	*/
	void markForAddition(BasicNode * node, 
		                 const hst::string & name=hst::string(),
		                 unsigned long lay=(unsigned long)-1);

	/*! Добавляет объект в сцену
	\param[in] node  объект 
	\param[in] name  имя
	\param[in] lay   слой
	*/
	void add(
		BasicNode * node, 
		const hst::string & name=hst::string(),
		unsigned long lay=(unsigned long)-1
		);
	/*! Удаляет объект из сцены
	\param[in] name имя объекта
	*/
	void remove(const hst::string & name);
	/*! Рендерит сцену
	*/
	void render();
	/*! Получение объекта по имени. Если не найдено, то вернет NULL
	\param[in] name имя
	*/
	template<typename T> T * get(const hst::string & name); 
};

}

typedef sad::BasicNode BasicNode;
typedef sad::Scene     Scene;

//=================Исходный код находится здесь====================
template<typename T> T * sad::Scene::get(const hst::string & name)
{
	if (!m_nodehash.contains(name)) return NULL;

	BasicNode *  ind=m_layers[m_nodehash[name]];
	assert(ind->type()==T::Type && "Inconsistent types!");
	return static_cast<T *>(ind);
}
