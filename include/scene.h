/*! \file   scene.h
\author HiddenSeeker
\brief  Здесь расположены файлы сцены
*/
#include "templates/hlvector.hpp"
#include "templates/hhash.hpp"
#include <assert.h>
#pragma once
/*! Класс базового объекта сцены
*/
class BasicNode
{
	float m_x;    //!< Позиция
	float m_y;    //!< Позиция
	int   m_type; //!< Тип вершины (динамический)
public:
	static int Type; //!< Статический тип
	/*! Тип
	*/
	int type() const;
	/*! Процедура, которая будет выполняться при рендеринге объекта.
	*/
	virtual void render();
	/*! Деструктор
	*/
	virtual ~BasicNode();
};

/*! Класс сцены
*/
class Scene
{
private:
	hst::vector<BasicNode *>   m_layers;              //!< Слои, первый элемент и им же рендерится
	hst::hash<hst::string,unsigned long>  m_nodehash; //!< Вершина по ноде
public:
	Scene();    //!< Пустая сцена
	~Scene();   //!< Деструктор

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

//=================Исходный код находится здесь====================
template<typename T> T * Scene::get(const hst::string & name)
{
	if (!m_nodehash.contains(name)) return NULL;

	BasicNode *  ind=m_layers[m_nodehash[name]];
	assert(ind->type()==T::Type && "Inconsistent types!");
	return ind;
}
