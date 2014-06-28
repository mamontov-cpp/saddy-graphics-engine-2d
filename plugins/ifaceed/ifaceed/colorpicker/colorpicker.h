/*! \file colorpicker\colorpicker.h
	\author HiddenSeeker

	Describes a widget, which performs color picking
 */
#include <QTableWidget>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QImage>
#include <QHash>

class ColorPicker: public QWidget
{
	Q_OBJECT
public: 
	/*! Constructs new resource tree widget 
	 */
	ColorPicker(QWidget * parent = NULL);
	/*! Selected color data
	 */
	QColor selectedColor();
	/*! Destroys data from a widget
	 */
	~ColorPicker();
protected slots:
	/*! Activated, when selected item in palette
		\param[in] current new item
		\param[in] previous previous item
	 */
	void  paletteItemChanged(QTableWidgetItem * current, QTableWidgetItem * previous);
	/*! Resets selection for picker
	 */
	void  setPaletteSelection();
	/*! Emitted, when red spinbox's value is changed
		\param[in] v value
	 */
	void  redChanged(int v);
	/*! Emitted, when green spinbox's value is changed
		\param[in] v value
	 */
	void  greenChanged(int v);
	/*! Emitted, when blue spinbox's value is changed
		\param[in] v value
	 */
	void  blueChanged(int v);
	/*! Emitted, when alpha spinbox's value is changed
		\param[in] v value
	 */
	void  alphaChanged(int v);
protected:
	/*! Default row count in color picker
	 */
	static int PaletteDefaultRowCount;
	/*! Default column count in color picker
	 */
	static int PaletteDefaultColumnCount;
	/*! Default size of cell
	 */
	static int PaletteCellSize;
	/*! A padding between palette and preview and between color wheel and numeric color data
	 */
	static int VerticalPadding;
	/* A padding between palette, preview group and between wheel text-data
	 */
	static int HorizontalPadding;
	/*! A row count for color preview 
	 */
	static int PreviewRowCount;
	/*! A column count for color preview 
	 */
	static int PreviewColumnCount;
	/*! Width and height of markers in color picker
	 */
	static int MarkerSize;
	/*! A width for lightness gradient
	 */
	static int LightnessGradientWidth;
	/*! A height for alpha gradient
	 */
	static int AlphaGradientHeight;
	/*! Paints color picker's palette and parts near palette
		\param[in] e event
	 */
	virtual void paintEvent(QPaintEvent * e);
	/*! Handles resize, resizing elements
		\param[in] e event
	 */
	virtual void resizeEvent(QResizeEvent * e);
	/*! Handles movement, moving elements
		\param[in] e event
	 */
	virtual void moveEvent(QMoveEvent * e);
	/*! Resizes widgets, making them fit to tree
		\param[in] r a rectangle
	 */
	void resizeWidgets(const QRect & r);
	/*! Creates a preview cells for color picker
	 */
	void createPreviewCells();
	/*! Inits palette in picker and makes cells non-editable
	 */
	void initRandomPaletteAndMakeCellsNonEditable();
	/*! Fills row with items
		\param[in] row row inde
	 */
	void fillRow(int row);
	/*! Shifts rows down and fills first with white cells
	 */
	void shiftRows();
	/*! Fills columns with items
		\param[in] column column index
	 */
	void fillColumn(int column);
	/*! Shifts columns right and fills first with white cells
	 */
	void shiftColumns();
	/*! Updates full preview table, basing on row and column
		\param[in] row a row cell from palette
		\param[in] column a column cell from palette
	 */
	void updateFullPreviewTable(int row, int column);
	/*! Updates color parts, based on row and column color
		\param[in] row a row cell from palette
		\param[in] column a column cell from palette
	 */
	void updateSilentlyColorParts(int row, int column);
	/*! Updates colors in all places, when working with spinboxes
		\param[in] c new color
	 */
	void updateColorsInPalettePreviewColorWheel(const QColor & c);
	/*! Regenerate images for painting
	 */
	void regenerateImages();
	/*! Regenerates lightness image for
		\param[in] height specified height of  image
	 */
	void regenerateLightnessImage(int height);
	/*! Regenerates alpha image for
		\param[in] width specified width of image
	 */
	void regenerateAlphaImage(int width);
	/*! Generates color wheel
		\param[in] lightnes a lightness for data
		\param[in] alpha alpha for wheel 
		\param[in] size a side part
	 */
	void generateColorWheel(int lightness, int alpha, int side);
	/*! Describes a row to be set in color picker
	 */
	int m_row_to_be_set;
	/*! Describes a column to be set in color picker
	 */
	int m_col_to_be_set;

	/*! A labels for colors, as following R,G,B, A
	 */
	QLabel*      m_color_labels[4];
	/*! A spinboxes for colors, as following R,G,B, A
	 */
	QSpinBox*    m_colors_data[4];
	/*! A grid container widget for layout
	 */
	QWidget*     m_colors_grid_container;
	/*! A layout for colors in color picker
	 */
	QGridLayout*  m_colors_layout;
	/*! A widget for palette colors
	 */ 
	QTableWidget* m_palette;
	/*! A widget for showing color and adjacent preview colors
	 */
	QTableWidget* m_preview;
	/*! A hash, where key is pair (lightness, alpha) and color wheel is value
	 */
	QHash<QPair<int, int>, QImage> m_color_wheels;
	/*! Lightness image
	 */
	QImage* m_lightness_image;
	/*! Alpha image data
	 */
	QImage* m_alpha_image;
	/*! A size of color wheel for color picker
	 */
	int m_wheel_size;
};
