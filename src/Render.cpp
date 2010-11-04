/*! \file   Render.cpp
    \author FreakyBlast

	\brief  Definition of Render.
 
	In this file OpenGL function has been used obviously.
	Комментарии пока на русском, дабы лучше разобрались в работе рендера.
*/

#include <windows.h>													
#include "Render.h"

#include <gl/gl.h>														
#include <gl/glu.h>													

//Подключаем библиотеки, через ж... конечно, но зато не надо шариться в настройках проекта
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )


SadRender MainRender;

SadRender::SadRender(void)
{
}

SadRender::~SadRender(void)
{
}

//Устанавливаем OpenGL
BOOL SadRender::SetupOpenGL(GL_Window *window)
{
	PIXELFORMATDESCRIPTOR pfd =											// pfd указывает Windows как устанавливать OpenGL
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Размер этого Pixel Format Descriptor'а
		1,																// Номер версии
		PFD_DRAW_TO_WINDOW |											// Формат должен поддерживать отрисовку в окно
		PFD_SUPPORT_OPENGL |											// Формат должен поддерживать OpenGL
		PFD_DOUBLEBUFFER,												// Формат должен поддерживать Double Buffering
		PFD_TYPE_RGBA,													// Формат должен поддерживать RGBA режим
		window->init.bitsPerPixel,										// Выбираем глубину цвета
		0, 0, 0, 0, 0, 0,												// Игнорируем биты цвета
		0,																// Нет альфа буфера (пока нету)
		0,																// Игнорируем бит сдвига
		0,																// Нету Accumulation Buffer'а
		0, 0, 0, 0,														// Accumulation биты игнорируем
		MainSettings.DEPTH,												// Глубина Z-Buffer'а (Depth Buffer)  
		0,																// Нет Stencil Buffer'а (пока нету)
		0,																// Нет Auxiliary Buffer'а
		PFD_MAIN_PLANE,													// Главный слой для отрисовки
		0,																// Зарезирвировано
		0, 0, 0															// Игнорируем маску слоёв
	};

	GLuint PixelFormat;													// Содержит выбранный формат

	PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// Ищем совместимый Pixel Format
	if (PixelFormat == 0)												// Мы нашли?
	{
		// Неудача
		ReleaseDC (window->hWnd, window->hDC);							// Освобождаем контекст устройсва
		window->hDC = 0;												// Обнуляем его
		DestroyWindow (window->hWnd);									// Убиваем окно
		window->hWnd = 0;												// Обнуляем хэндл окна
		return FALSE;													// Возвращаем False (Ложь)
	}

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// Пытаемся установить этот формат
	{
		// Неудачно
		ReleaseDC (window->hWnd, window->hDC);							// Освобождаем контекст устройсва
		window->hDC = 0;												// Обнуляем его
		DestroyWindow (window->hWnd);									// Убиваем окно
		window->hWnd = 0;												// Обнуляем хэндл окна
		return FALSE;													// Возвращаем False (Ложь)
	}

	window->hRC = wglCreateContext (window->hDC);						// Пробуем получить контекст рендера
	if (window->hRC == 0)												// Мы его получили?
	{
		// Неудачно
		ReleaseDC (window->hWnd, window->hDC);							// Освобождаем контекст устройсва
		window->hDC = 0;												// Обнуляем его
		DestroyWindow (window->hWnd);									// Убиваем окно
		window->hWnd = 0;												// Обнуляем хэндл окна
		return FALSE;													// Возвращаем False (Ложь)
	}

	// Делаем этот контект рендера текущим
	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
	{
		// Неудачно
		wglDeleteContext (window->hRC);									// Удаляем контекст рендера
		window->hRC = 0;												// Обнуляем его
		ReleaseDC (window->hWnd, window->hDC);							// Освобождаем контекст устройсва
		window->hDC = 0;												// Обнуляем его
		DestroyWindow (window->hWnd);									// Убиваем окно
		window->hWnd = 0;												// Обнуляем хэндл окна
		return FALSE;													// Возвращаем False (Ложь)
	}

	return TRUE;
}

void SadRender::Reshape(int width, int height)
{
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// Переустанавливаем ViewPort (область видимости)
	glMatrixMode (GL_PROJECTION);										// Выбираем матрицу проекции
	glLoadIdentity ();													// Сбрасываем её на единичную
	gluPerspective (MainSettings.FOV*(GLfloat)(height)/(GLfloat)(width), (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
		MainSettings.ZNEAR, MainSettings.ZFAR);		
	glMatrixMode (GL_MODELVIEW);										// Выбираем видовую матрицу
	glLoadIdentity ();													// Сбрасываем её на единичную


	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Действительно красивое вычисление перспективы
}


