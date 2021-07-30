#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QImage>
#include <QHash>
#include <QApplication>
#include <QTextCodec>
#include <cstdio>
#include <cmath>
#include <cassert>
#include <algorithm>

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

#ifdef WIN32

#if  (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    
    inline QString __std2qstring(const std::string& s)
    {
        if (s.empty())
            return QString();
        return QString::fromLocal8Bit(s.c_str());
    }
    #define Q2STDSTRING(STR)    (std::string((STR).toLocal8Bit()))
    #define STD2QSTRING(STR)    (::__std2qstring(STR))
#else
    #define Q2STDSTRING(STR)    ((STR).toStdString())
    #define STD2QSTRING(STR)    (QString(std::string(STR).c_str()))
#endif

#else
    #define Q2STDSTRING(STR)    ((STR).toStdString())
    #define STD2QSTRING(STR)    (QString(std::string(STR).c_str()))
#endif


int main(int argc, char *argv[])
{
    // We miust construct it in order to work with fonts
#ifdef WIN32
    setlocale(LC_CTYPE, ".1251");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
#endif
    QApplication a(argc, argv);
    // 1. Parse command arguments
    if (argc<3)
    {
        printf("Usage:                  exporter <font> <font size> <output file name>\n"
               "Export string as image: exporter <font> <font size> <text> <output file name>\n"
        );
        return 1;
    }
    QTextCodec * codec = QTextCodec::codecForLocale();
    int size=0;
    sscanf(argv[2],"%d",&size);
    // 2. Get font
    QFont font(argv[1], size);
    font.setPixelSize(size);
    if (!font.exactMatch())
    {
        printf("Font \"%s\" not found\n", argv[1]);
        return 2;
    }
    // 3. create metrics
    QFontMetrics metrics(font);
    // 4. Compute bounding boxes
    // 4.1. if we should render string, render it
    if (argc == 5)
    {
        QRect sizes = metrics.boundingRect(argv[3]);

        // Make image
        QImage image(sizes.width(), sizes.height(), QImage::Format_ARGB32);
        // The image must be transparent,otherwise we wll have problems with it's loading
        image.fill(QColor(255,255,255,0).rgba());

        QPainter painter;
        painter.begin(&image);
        //QPen textpen(QColor(0, 0, 0));
        QPen textpen(QColor(255, 255, 255));
        //QPen debugpen(QColor(255, 0, 0));
        painter.setPen(textpen);
        painter.setFont(font);

        painter.drawText(0, 0, sizes.width(), sizes.height(), Qt::AlignHCenter | Qt::AlignVCenter, argv[3]);

        painter.end();
        QString file_name = argv[4];
        file_name += ".png";
        if (!image.save(file_name))
        {
            printf("Unable to save file");
        }
        return 0;
    }
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
        int width = metrics.width(renderedstring[0]);
        int leftbearing = metrics.leftBearing(renderedstring[0]);
        int rightbearing = metrics.rightBearing(renderedstring[0]);
        int delta = (leftbearing < 0 ) ? -leftbearing :  0; 
        totalwidth = width + delta;
        totalwidth += (rightbearing > 0) ? rightbearing : 0;
        rowwidth += totalwidth;
        if ((c + 1) % 16 == 0)
        {
            totalrowwidth = std::max(totalrowwidth, rowwidth + 16);
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
    FILE * file = fopen(Q2STDSTRING(config_mapping_file).c_str(), "wt");
    if (file == nullptr)
    {
        printf("Cannot open mapping file\n");
        return 3;
    }
    // Draw image and write the mapping file
    QPainter painter;
    painter.begin(&image);
    //QPen textpen(QColor(0, 0, 0));
    QPen textpen(QColor(255, 255, 255));
    //QPen debugpen(QColor(255, 0, 0));
    painter.setPen(textpen);
    painter.setFont(font);
    int current_x_pos = 0;
    int current_y_pos = 0;
    int character_in_row = 0;
    
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setRenderHint(QPainter::TextAntialiasing, false);

    metrics = painter.fontMetrics();

    fprintf(file, "%d %d\n", linespacing, metrics.ascent());

    int characters_in_row = 16;
    for(unsigned int character =  0; character < 256; character++)
    {
        unsigned char c = (unsigned char)character;

        string[0] = c;
        QString renderedstring = codec->toUnicode(string);

        int width = metrics.width(renderedstring[0]);
        int leftbearing = metrics.leftBearing(renderedstring[0]);
        int rightbearing = metrics.rightBearing(renderedstring[0]);
        totalwidth = width;
        
        int delta = (leftbearing < 0 ) ? -leftbearing :  0; 
        totalwidth += delta;
        totalwidth += (rightbearing < 0) ? -rightbearing : 0;


        unsigned int relative_x_pos = current_x_pos + delta;
        unsigned int relative_y_pos = current_y_pos;
        unsigned int relative_x_end_pos = relative_x_pos + totalwidth;
        unsigned int relative_y_end_pos = relative_y_pos + linespacing;
        

        painter.setPen(textpen);
        painter.drawText(relative_x_pos, 
                         relative_y_pos + metrics.ascent(), 
                         renderedstring);
        /*
        painter.setPen(debugpen);
        painter.drawLine(relative_x_end_pos, 
                         relative_y_pos,
                         relative_x_end_pos,
                         relative_y_end_pos
                        );
        */
        fprintf(file,
                "%u %u %u %u %d %d\n", 
                relative_x_pos, 
                relative_y_pos,
                relative_x_end_pos, 
                relative_y_end_pos, 
                (leftbearing < 0) ? leftbearing : 0,
                (rightbearing < 0) ? rightbearing : 0
               );

        ++character_in_row;
        if (character_in_row == characters_in_row)
        {
            character_in_row = 0;
            current_x_pos = 0;
            current_y_pos += linespacing;
            /*
            painter.setPen(debugpen);
            painter.drawLine(0, 
                             current_y_pos,
                             512,
                             current_y_pos
                            );
            */
        }
        else
        {
            current_x_pos += totalwidth + 1;
        }
    }
    painter.end();
    image.save(image_mapping_file);
    // We don't construct kerning table, since
    // Qt uses a logical width, which allows to translate glyphs, aligning them
    // kerning-alike
    fclose(file);
    return 0;
}

