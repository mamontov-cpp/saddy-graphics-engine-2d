#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QImage>
#include <QHash>
#include <QApplication>
#include <stdio.h>
#include <math.h>

#define MIN_CHARACTER 32
#define MAX_CHARACTER 128

/*! Taken from http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
	Computes next power of two
	\param[in] v integer
	\return integer with next power of two
 */
unsigned int next_power_of_two(unsigned int v1)
{
	unsigned int v = v1; 
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

int main(int argc, char *argv[])
{
	// We miust construct it in order to work with fonts
	QApplication a(argc, argv);
	// 1. Parse command arguments
	if (argc<7)
	{
		printf("Usage: exporter <font> <font size> <red color component> <green color component> <blue color component> <output file name>\n");
		return 1;
	}
	int size=0;int colorr=0,colorg=0,colorb=0;
	sscanf(argv[2],"%d",&size);
	sscanf(argv[3],"%d",&colorr);
	sscanf(argv[4],"%d",&colorg);
	sscanf(argv[5],"%d",&colorb);
	// 2. Get font
	QFont font(argv[1], size);
	QString family = font.family();
	if (font.exactMatch() == false)
	{
		printf("Font \"%s\" not found\n", argv[1]);
		return 2;
	}
	// 3. create metrics
	QFontMetrics metrics(font);
	// 4. Compute bounding boxes
	QHash<char, QRect> bounds;
	for(unsigned char c =  MIN_CHARACTER; c < MAX_CHARACTER; c++)
	{
		unsigned char realCharacter = c;
		// If we does not replace space, the bounding rect
		// will be empty, which can lead to no space between
		// letters in data
		if (c == ' ') 
			realCharacter = 'A';
		// We must take bounding rect of all character space, with advances
		bounds.insert(c, metrics.boundingRect(QString(QChar(c))));
	}
	// 5. Compute maximal bounding rect
	QRect max(0,0,0,0);
	for(QHash<char, QRect>::iterator it = bounds.begin(); it != bounds.end(); it++)
	{
		if (it.value().width() > max.width()) max.setWidth(it.value().width());
		if (it.value().height() > max.height()) max.setHeight(it.value().height());
	}
	// 6. Compute image size
	int total_characters = MAX_CHARACTER-MIN_CHARACTER+1;
	int characters_in_row = (int)sqrt((float)(total_characters));
	if (characters_in_row*characters_in_row != total_characters)
	{
		++characters_in_row;
	}
	int rows = total_characters / characters_in_row;
	if (total_characters % characters_in_row != 0)
	{
		++rows;
	}
	int image_width = characters_in_row * max.width();
	int image_height = rows * max.height();
	// 7. Make image size a square with power of two
	int max_image_size = std::max(image_width, image_height);
	int real_image_size = next_power_of_two(max_image_size);
	// 8. Create image
	QImage image(QSize(real_image_size, real_image_size), QImage::Format_ARGB32);
	// The image must be transparent,otherwise we wll have problems with it's loading
	image.fill(QColor(255,255,255,0).rgba());
	QString image_mapping_file = QString(argv[6]) + ".PNG";
	QString config_mapping_file = QString(argv[6]) + ".CFG";
	//Open a file
	FILE * file = fopen(config_mapping_file.toStdString().c_str(), "wt");
	if (file == NULL)
	{
		printf("Cannot open mapping file\n");
		return 3;
	}
	// Draw image and write the mapping file
	QPainter painter;
	painter.begin(&image);
	painter.setPen(QColor(colorr, colorg, colorb));
	painter.setFont(font);
	int current_x_pos = 0;
	int current_y_pos = 0;
	int character_in_row = 0;
	for(unsigned char c =  MIN_CHARACTER; c < MAX_CHARACTER; c++)
	{
		QRect bbox = bounds[c];
		float relative_x_pos = ((float)current_x_pos)/real_image_size;
		float relative_y_pos = ((float)current_y_pos)/real_image_size;
		float relative_x_end_pos = relative_x_pos + ((float)bbox.width())/real_image_size;
		float relative_y_end_pos = relative_y_pos + ((float)bbox.height())/real_image_size;
		
		bbox.moveTopLeft(QPoint(current_x_pos, current_y_pos));
		painter.drawText(bbox, Qt::AlignCenter, QString(QChar(c)));

		fprintf(file,"%c %f %f %f %f\n",c, relative_x_pos, relative_y_pos,
										   relative_x_end_pos, relative_y_end_pos);
		++character_in_row;
		if (character_in_row == characters_in_row)
		{
			character_in_row = 0;
			current_x_pos = 0;
			current_y_pos += max.height();
		}
		else
		{
			current_x_pos += max.width();
		}
	}
	painter.end();
	image.save(image_mapping_file);
	fclose(file);
	return 0;
}

