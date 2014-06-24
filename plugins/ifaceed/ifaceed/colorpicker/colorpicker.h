/*! \file colorpicker\colorpicker.h
	\author HiddenSeeker

	Describes a widget, which performs color picking
 */
#include <QTableWidget>

class ColorPicker: public QWidget
{
	Q_OBJECT
public: 
	/*! Constructs new resource tree widget 
	 */
	ColorPicker(QWidget * parent = NULL);
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
protected:
	/*! Default row count in color picker
	 */
	static int DefaultRowCount;
	/*! Default column count in color picker
	 */
	static int DefaultColumnCount;
	/*! Default size of cell
	 */
	static int CellSize;
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
	/*! Describes a row to be set in color picker
	 */
	int m_row_to_be_set;
	/*! Describes a column to be set in color picker
	 */
	int m_col_to_be_set;
	/*! A widget for palette colors
	 */ 
	QTableWidget  * m_palette;
};
