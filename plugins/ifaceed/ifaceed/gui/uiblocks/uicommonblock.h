/* \file uicommonblock.h

    Contains definition of UI group for Common group of widgets
 */

class QWidget;
class QLineEdit;
class QTabWidget;
class QTableWidget;
class QPushButton;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for Common group of widgets
 */
class UICommonBlock
{
public:
    /*! Constructs new UI group
     */
    UICommonBlock();
    /*! Inits block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UICommonBlock();

    QLineEdit* txtMousePosX;
    QLineEdit* txtMousePosY;
    QTabWidget* tabTypes;
    QTabWidget* tabObjects;
    QTableWidget* twDatabaseProperties;
    QPushButton* btnFixTextureCoordinates;
    QPushButton* btnClearObjectSelection;
};

}

}
