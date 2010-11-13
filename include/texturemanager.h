/*!  \file    texturemanager.h
\author  Unknown
\brief   Управление текстурами
*/
#include "texture.h"
#include "templates/hhash.hpp"
#pragma once

namespace sad
{
	/*! Управление текстурами сосредоточено здесь.
	В том числе память из-под текстур освобождается здесь же.
	*/
	class TextureManager
	{
	private:
		hst::hash<hst::string,Texture *> m_data; //!< Данные о текстурах

		static TextureManager * m_instance;      //!< Инстанс

		TextureManager();
		TextureManager(const TextureManager &);
		TextureManager & operator=(const TextureManager &);
	public:
		/*! Возвращает текущий инстанс менеджера
		*/
		static TextureManager * instance();
		/*! Деструктор
		*/
		~TextureManager();
		/*! Возвращает текущую текстуру по имени, если есть, иначе NULL
		\param[in] name имя
		*/
		Texture *  get(const hst::string & name);
		/*! Добавляет текстуру в список
		\param[in] name имя
		\param[in] tex  текстура
		*/
		void load(const hst::string & name, Texture * tex);
		/*! Выгружает текстуру
		\param[in] name имя
		*/
		void unload(const hst::string & name);
	};

}