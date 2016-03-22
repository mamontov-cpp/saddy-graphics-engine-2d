/*! \file childrenprovider.h

    Defines a global children provider, that provides children, linked to editor
 */
#pragma once
#include <cstdlib>
#include "gui/layouts/layoutcelledit.h"


namespace gui
{

class MainPanelProxy;

class ChildrenProvider: public gui::layouts::LayoutCellEdit::ChildrenProvider
{
public:
    /*! Makes new children provider with specified proxy
        \param proxy a linked proxy
     */
    ChildrenProvider(MainPanelProxy* proxy = NULL);
    /*! Sets proxy for provider
        \param[in] p proxy
     */
    void setProxy(MainPanelProxy* p);
    /*! Could be inherited
     */
    virtual ~ChildrenProvider();
    /*! Returns a full name for a node
        \param[in] node a node
        \return name
     */
    virtual QString name(sad::SceneNode* node);
    /*! Returns a list of possible children
        \return list of children
     */
    virtual QVector<QPair<QString, unsigned long long> > possibleChildren();
protected:
    /*! A proxy for returning all objects
     */
    MainPanelProxy* m_proxy;
};

}
