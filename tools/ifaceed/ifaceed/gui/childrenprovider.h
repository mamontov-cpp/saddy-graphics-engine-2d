/*! \file childrenprovider.h

    Defines a global children provider, that provides children, linked to editor
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
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
    ChildrenProvider(MainPanelProxy* proxy = nullptr);
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
    virtual QString name(sad::SceneNode* node) override;
    /*! Returns a list of possible children
        \return list of children
     */
    virtual QVector<QPair<QString, unsigned long long> > possibleChildren() override;
    /*! Same as possible children - just a list of nodes
        \return list of nodes
     */
    QVector<QPair<QString, unsigned long long> > nodeList();
    /*! Sets inner flag to not remove children of layout grids
     */
    void doNotRemoveGridChildren();
    /*! Makes provider exclude major id
        \param[in] id major id
     */ 
    void excludeMajorId(unsigned long long id);
protected:
    /*! Whether we should erase children of other nodes from provider
     */
    bool m_scan_for_children;
    /*! A proxy for returning all objects
     */
    MainPanelProxy* m_proxy;
    /*! Excluded node from selection
     */ 
    unsigned long long m_excluded_major_id;
};

}
