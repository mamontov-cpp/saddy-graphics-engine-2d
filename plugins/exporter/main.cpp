#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QImage>
#include <QHash>
#include <QApplication>
#include <QTextCodec>
#include <stdio.h>
#include <math.h>


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
	if (argc<3)
	{
		printf("Usage: exporter <font> <font size> <output file name>\n");
		return 1;
	}
	QTextCodec * codec = QTextCodec::codecForLocale();
	int size=0;
	sscanf(argv[2],"%d",&size);
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
	char string[2] = "A";
	QString renderedstring;
	int linespacing = metrics.lineSpacing();
	int totalwidth = 0;
	int rowwidth = 0;
	int totalrowwidth = 0;
	for(unsigned int c =  0; c < 256; c++)
	{
		unsigned char realCharacter = (unsigned char)c;
		string[0] = realCharacter;
		renderedstring = codec->toUnicode(string);
		int width = metrics.width(string[0]);
		int leftbearing = metrics.leftBearing(string[0]);
		int rightbearing = metrics.rightBearing(string[0]);
		totalwidth = width + abs(leftbearing) + abs(rightbearing);
		rowwidth += totalwidth;
		if ((c + 1) % 16 == 0)
		{
			totalrowwidth = std::max(totalrowwidth, rowwidth);
			rowwidth = 0;
		}
		
	}
	// 6. Compute image size
	int image_width = totalrowwidth;
	int image_height = 16 * linespacing;
	// 7. Make image size a square with power of two
	int max_image_size = std::max(image_width, image_height);
	int real_image_size = next_power_of_two(max_image_size);
	// 8. Create image
	QImage image(QSize(real_image_size, real_image_size), QImage::Format_ARGB32);
	// The image must be transparent,otherwise we wll have problems with it's loading
	image.fill(QColor(255,255,255,0).rgba());
	QString image_mapping_file = QString(argv[3]) + ".png";
	QString config_mapping_file = QString(argv[3]) + ".cfg";
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
	painter.setPen(QColor(255, 255, 255));
	painter.setFont(font);
	int current_x_pos = 0;
	int current_y_pos = 0;
	int character_in_row = 0;
	int length = 256;
	fprintf(file, "%d\n", linespacing);

	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setRenderHint(QPainter::TextAntialiasing, false);

	int characters_in_row = 16;
	/*		
	for(unsigned int character =  0; character < 256; character++)
	{
		unsigned char c = (unsigned char)character;
		QRect bbox = bounds[c];
		unsigned int relative_x_pos = current_x_pos;
		unsigned int relative_y_pos = current_y_pos;
		unsigned int relative_x_end_pos = relative_x_pos + bbox.width();
		unsigned int relative_y_end_pos = relative_y_pos + bbox.height();
		
		bbox.moveTopLeft(QPoint(current_x_pos, current_y_pos));
		string[0] = c;
		QString renderedstring = codec->toUnicode(string);
		painter.drawText(bbox, Qt::AlignLeft | Qt::AlignVCenter, renderedstring);

		int w = metrics.width(string);
		fprintf(file,"%d %u %u %u %u %d\n", character, relative_x_pos, relative_y_pos,
										   relative_x_end_pos, relative_y_end_pos, w);
		++character_in_row;
		if (character_in_row == characters_in_row)
		{
			character_in_row = 0;
			current_x_pos = 0;
			current_y_pos += linespacing;
		}
		else
		{
			current_x_pos += linespacing;
		}
	}
	int spacing = metrics.lineSpacing();
	int pheight = metrics.height();
	int ascent = metrics.ascent();
	painter.end();
	image.save(image_mapping_file);
	// We don't construct kerning table, since
	// Qt uses a logical width, which allows to translate glyphs, aligning them
	// kerning-alike
	fclose(file);
	*/
	return 0;
}

