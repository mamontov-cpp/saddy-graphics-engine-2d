/*! \file    texture.h
\author  Myself;Yourself
\brief   Все, что относится к текстуре расположено здесь
*/
#include "primitives/hcolor.h"
#include "templates/hstring.h"
#include "templates/hwstring.h"
#include "templates/hlvector.hpp"
#ifdef WIN32
#include <windows.h>
#else
#endif
#include <gl/gl.h>
#include <gl/glu.h>
#pragma once


namespace sad
{
	static unsigned int globalID=0;

	/*! Класс текстуры
	*/
	class Texture
	{
	public:
		/*! Описывает режим текстурирования
		*/
		enum Mode
		{
			BORDER_REPEAT,
			BORDER_CLAMP,
			COMBINE_REPLACE,
			COMBINE_ADD,
			COMBINE_MODULATE
		};

	private:

		hst::vector<Uint8> m_data;   //!< Биты текстуры
		Uint8              m_filter; //!< Способ фильтрации
		Uint8              m_bpp;    //!< Число битов на пиксел
		unsigned int       m_width;  //!< Ширина
		unsigned int       m_height; //!< Высота
		unsigned int       m_id;     //!< ID текстуры в видеопамяти
		Mode               m_mode;   //!< Режим границ

	public:

		/*! Создает пустую текстуру
		*/
		Texture();
		/*! Деструктор
		*/
		~Texture();
		/*! Строит мипмапы
		*/
		void buildMipMaps();
		/*! Загрузка из файла, в зависимости от расширения
		\param[in] filename имя файла
		*/
		bool load(const hst::string & filename);
		/*! Загрузка из файла BMP
		\param[in] filename имя файла
		*/
		bool loadBMP(const hst::string & filename);
		/*! Загрузка из файла TGA
		\param[in] filename имя файла
		*/
		bool loadTGA(const hst::string & filename);
		/*! Загрузка из файла TGA
		\param[in] filename имя файла
		*/
		bool loadTGA(const hst::wstring & filename);
		/*! Загрузка из файла PNG
		\param[in] filename имя файла
		*/
		bool loadPNG(const hst::string & filename);
		/*! Загрузка из файла PNG
		\param[in] filename имя файла
		*/
		bool loadPNG(const hst::wstring & filename);
		/*! Загрузка прекомпилированной текстуры формата TGA
		*/
		void loadDefaultTGATexture();
		/*! Выгружает текстуру
		*/
		void disable();
		/*! Включает текстуру
		*/
		void enable();
		/*! Устанавливает альфа-канал
		    \param[in] a альфа-канал
		*/
		void setAlpha(Uint8 a);
		/*! Устанавливает альфа-канал для цвета
		    \param[in] a    альфа-канал
			\param[in] clr  цвет, для которого устанавливается альфа
			\param[in] prec погрешность
		*/
		void setAlpha(Uint8 a, const hst::color & clr, Uint8 prec=0);
		/*! Устанавливает режим
		    \param[in] mode режим
		*/
		void setMode(Texture::Mode mode);
	};
}