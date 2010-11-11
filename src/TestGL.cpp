
#include "..\include\Engine.h"
#include "gl\gl.h"
#include "gl\glu.h"

//Угол на который вращается кубик
float angle=0;
//Данные для кубика
const GLfloat boxVertexes[] =
{
	/*x,y,z*/
	-1, -1, -1,
	1, -1, -1,
	1,  1, -1,
	-1,  1, -1,
	-1, -1,  1,
	1, -1,  1,
	1,  1,  1,
	-1,  1,  1,
};

const float boxUVs[] =
{
	/*u,v одинаковые для всех вершин каждого полигона */
	0, 0,
	1, 0,
	1, 1,
	0, 1
};
//Нормали для каждой грани
const float boxNormals[] =
{
	0,  1,  0,
	0, -1,  0,
	1,  0,  0,
	-1,  0,  0,
	0,  0,  1,
	0,  0, -1
};
const int boxQuads[] =
{
	/* индексы вершин, по 4 на сторону кубика */
	1, 0, 3, 2,
	4, 5, 6, 7,
	1, 2, 6, 5,
	0, 4, 7, 3,
	1, 5, 4, 0,
	3, 7, 6, 2
};
//Инициализация настроек
void InitSettings()
{
	//Ставим режим 640х480х32
	MainSettings.SCREEN_WIDTH	=	640;							
	MainSettings.m_screenheight	=	480;
	MainSettings.BPP			=	32;
	//Глубина z-буффера 24 бита
	MainSettings.DEPTH			=	24;
	//Запускаем в оконном режиме, по клавише F1 можно переключиться в полноэкран
	MainSettings.m_isfullscreen		=	FALSE;
	//Угол обзора (Quake вида)
	MainSettings.FOV			=	45.0f;
	//Ближние и дальние проскости отсечения
	MainSettings.ZNEAR			=	0.1f;
	MainSettings.ZFAR			=	100.0f;
	//Заголовок окна
	MainSettings.TITLE			=	"Тестовый пример Saddy.Нажмите 'F1' для полноэкранного режима, 'Esc' для выхода.";

}
//Инициализация данных сцены
BOOL Initialize (sadWindow* window, Keys* keys)	// Инициализация
{
	//Угол обнуляем
	angle = 0;

	return TRUE;
}
//Деинициализация
void Deinitialize (void)							// Деинициализация
{
	//Пока тут пусто
}
//Запускается перед каждым кадром для изменения положения объектов и т.д и т.п.
void Update (__int64 DeltaTime)					// Расчет сцены в зависимости от прошедшего времени
{
	//DeltaTime - время прошедшее с предыдущего запуска
	angle+=100*DeltaTime/1000;

}

//Рисуем
void Draw (void)									// Отрисовка всей сцены
{
	//Очищаем буффер цвета и глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Делаем единичной видовую мартицу
	glLoadIdentity();

	//Отодвигаемся от кубика чтобы он был виден
	glTranslatef(0,0,-6);
	//Вращаем его
	glRotatef(angle,1,2,3);
	//Рисуем 6-ть плоскостей
	glBegin(GL_QUADS);
	for(int i = 0; i < 6; i++ )
	{
		for (int j=0;j<4;j++)
		{
			//Устанавливаем цвет
			glColor3fv(&boxVertexes[boxQuads[i*4+j]*3]);
			//Устанавливаем координаты
			glVertex3fv(&boxVertexes[boxQuads[i*4+j]*3]);
		}
	}
	glEnd();

	//Эта комманда говорит видеокарточке что вроде всё нарисовано
	//и пусть она начинает рисовать, а мы пока кубик еще повернем.
	//Есть еще glFinish(), но она ждет пока всё нарисуется, а у нас времени в обрез
	glFlush();

}
