/* \file uicustomobjectblock.h

    Contains definition of UI group for CustomObject group of widgets
 */
#pragma once
namespace gui
{

namespace resourcetreewidget
{

class ResourceTreeWidget;

}

}


class QWidget;
class QPushButton;
class QTabWidget;
class QTableWidget;
class QRadioButton;


namespace gui
{

namespace uiblocks
{

/* A definition of UI group for CustomObject group of widgets
 */
class UICustomObjectBlock
{
public:
    /*! Constructs new UI group
     */
    UICustomObjectBlock();
    /*! Initializes block with children of main widget
        param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UICustomObjectBlock();

    QPushButton* btnCustomObjectAdd;
    QTabWidget* tabCustomObjectsProperties;
    gui::resourcetreewidget::ResourceTreeWidget* rtwCustomObjectSchemas;
    QTableWidget* twCustomObjectProperties;
    QRadioButton* rbCustomObjectPlaceAndRotate;
    QRadioButton* rbCustomObjectTwoClicksPlaces;
};

}

}
