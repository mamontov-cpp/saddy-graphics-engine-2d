/*! \file    texture.h
    \author  Myself;Yourself
	\brief   Все, что относится к текстуре расположено здесь
*/
#include "primitives/hcolor.h"
#include "templates/hstring.h"
#include "templates/hlvector.hpp"
#pragma once

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
     \param[in] filename имя файлы
 */
 bool load(const hst::string & filename);
 /*! Загрузка из файла BMP
     \param[in] filename имя файлы
 */
 bool loadBMP(const hst::string & filename);
 /*! Загрузка из файла TGA
     \param[in] filename имя файлы
 */
 bool loadTGA(const hst::string & filename);
 /*! Загрузка из файла PNG
     \param[in] filename имя файлы
 */
 bool loadPNG(const hst::string & filename);
};

