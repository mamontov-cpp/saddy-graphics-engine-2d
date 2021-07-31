#include "sceneactions.h"

#include <scene.h>

#include <sadpair.h>

#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>


#include "../../qstdstring.h"
#include "../../blockedclosuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uisceneblock.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"

#include "../../core/editor.h"

#include "../../history/scenes/scenesadd.h"
#include "../../history/scenes/scenesremove.h"
#include "../../history/scenes/sceneschangename.h"
#include "../../history/scenes/sceneslayerswap.h"
#include "../../history/scenes/scenesclear.h"

#include <QLineEdit>

Q_DECLARE_METATYPE(sad::Scene*) //-V566
Q_DECLARE_METATYPE(sad::SceneNode*) //-V566
Q_DECLARE_METATYPE(sad::db::Object*) //-V566


// ============================= PUBLIC METHODS =============================

gui::actions::SceneActions::SceneActions(QObject* parent) : QObject(parent), gui::actions::AbstractActions()
{

}

gui::actions::SceneActions::~SceneActions()
{

}

QString gui::actions::SceneActions::nameForScene(sad::Scene* scene)
{
    return this->viewableObjectName(scene);
}

void gui::actions::SceneActions::addSceneToSceneList(sad::Scene* s)
{
    QString name = this->viewableObjectName(s);
    QListWidgetItem* i =  new QListWidgetItem();
    i->setText(name);
    
    QVariant v;
    v.setValue(s);
    i->setData(Qt::UserRole, v);
    m_editor->uiBlocks()->uiSceneBlock()->lstScenes->addItem(i);
    
    QVariant vk;
    vk.setValue(static_cast<sad::db::Object*>(s));
    m_editor->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject->addItem(name, vk);
}

void  gui::actions::SceneActions::removeLastSceneFromSceneList()
{
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes;
    if (lstScenes->count())
    {
        QListWidgetItem* i = lstScenes->takeItem(lstScenes->count() - 1);
        QComboBox* cmbAnimationInstanceObject = m_editor->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject;
        sad::Scene* s  = i->data(Qt::UserRole).value<sad::Scene*>();
        delete i;

        int pos = this->findInComboBox<sad::db::Object*>(cmbAnimationInstanceObject, s);
        if (pos > -1)
        {
            cmbAnimationInstanceObject->removeItem(pos);
        }
    }
}


void gui::actions::SceneActions::insertSceneToSceneList(sad::Scene* s, int position)
{
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes;
    QString name = this->viewableObjectName(s);
    QListWidgetItem* i =  new QListWidgetItem();
    i->setText(name);
    
    QVariant v;
    v.setValue(s);
    i->setData(Qt::UserRole, v);
    lstScenes->insertItem(position, i);
}

void gui::actions::SceneActions::removeSceneFromSceneList(int position)
{
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes;
    QListWidgetItem* i =  lstScenes->takeItem(position);
    delete i;
}

sad::Scene* gui::actions::SceneActions::currentScene()
{
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes;    
    QListWidgetItem* item = lstScenes->currentItem();
    sad::Scene* scene = nullptr;
    if (item)
    {
        scene = item->data(Qt::UserRole).value<sad::Scene*>();
    }
    return scene;
}

void gui::actions::SceneActions::updateSceneName(sad::Scene* s, bool update_ui)
{
    int row = this->findSceneInList(s);
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes;    
    if (row != -1)
    {
        lstScenes->item(row)->setText(this->viewableObjectName(s));
    }
    if ((s == currentScene()) && update_ui)
    {
        QLineEdit* txtSceneName = m_editor->uiBlocks()->uiSceneBlock()->txtSceneName;
        int pos = txtSceneName->cursorPosition();
        bool b = txtSceneName->blockSignals(true);
        txtSceneName->setText(STD2QSTRING(s->objectName().c_str()));
        txtSceneName->blockSignals(b);
        txtSceneName->setCursorPosition(pos);
    }
}

int gui::actions::SceneActions::findSceneInList(sad::Scene* s)
{
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes;   
    return this->findInList(lstScenes, s);
}

void gui::actions::SceneActions::setScenesInList(sad::Scene* s1, sad::Scene* s2, int pos1, int pos2)
{
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes;   
    sad::Scene* s = this->currentScene();
    lstScenes->item(pos1)->setText(this->viewableObjectName(s1));
    QVariant v1;
    v1.setValue(s1);
    lstScenes->item(pos1)->setData(Qt::UserRole, v1);

    lstScenes->item(pos2)->setText(this->viewableObjectName(s2));
    QVariant v2;
    v2.setValue(s2);
    lstScenes->item(pos2)->setData(Qt::UserRole, v2);

    if (s == s1 || s == s2)
    {
        this->currentSceneChanged(lstScenes->currentRow());
    }
}


unsigned long long  gui::actions::SceneActions::addSceneWithName(const QString& name, bool from_editor)
{
    sad::Scene* s  = new sad::Scene();
    QLineEdit* txtSceneName = m_editor->uiBlocks()->uiSceneBlock()->txtSceneName; 
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes; 
    if (from_editor)
    {
        QString kname = txtSceneName->text();
        if (kname.length())
        {
            s->setObjectName(Q2STDSTRING(kname));
        }
    }
    else
    {
        s->setObjectName(Q2STDSTRING(name));
    }

    sad::Renderer::ref()->add(s);
    sad::Renderer::ref()->database("")->table("scenes")->add(s);

    history::Command* c = new history::scenes::Add(s);
    c->commit(m_editor);
    if (from_editor)
    {
        m_editor->history()->add(c);
    }
    else
    {
        m_editor->currentBatchCommand()->add(c);
    }
    invoke_blocked<QListWidget, void (QListWidget::*)(int), int>(lstScenes, &QListWidget::setCurrentRow, lstScenes->count() - 1);
    
    return s->MajorId;
}


void gui::actions::SceneActions::scriptableRemoveScene(sad::Scene* scene, bool from_editor)
{
    if (m_editor->isInEditingState())
    {
        return;
    }
    if (from_editor)
    {
        scene = currentScene();
    }
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes; 
    QComboBox* cmbAnimationInstanceObject = m_editor->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject;
    if (scene)
    {
        if (m_editor->machine()->isInState("selected"))
        {
            m_editor->machine()->enterState("idle");
            m_editor->shared()->setSelectedObject(nullptr);
        }

        int row; 
        if (from_editor)
        {	
            row = lstScenes->currentRow();
        }
        else
        {
            row = this->findSceneInList(scene);
        }

        int positioninanimationcombo = this->findInComboBox<sad::db::Object*>(cmbAnimationInstanceObject, scene);
        sad::Vector< sad::Pair<sad::SceneNode*, int> > positions;
        sad::Vector<sad::SceneNode*>  nodes = scene->objects();
        sad::Vector<unsigned long long> nodeids;
        for(size_t i = 0; i < nodes.size(); i++)
        {
            nodeids << nodes[i]->MajorId;
            int position = this->findInComboBox<sad::db::Object*>(cmbAnimationInstanceObject, nodes[i]);
            if (position > -1)
            {
                bool found = false;
                int foundpos = -1;
                for(size_t j = 0; j < positions.size(); j++)
                {
                    if (positions[j].p2() > position)
                    {
                        found = true;
                        foundpos = positions[j].p2();
                    }
                }
                if (found)
                {
                    positions.insert(sad::Pair<sad::SceneNode*, int>(nodes[i], position), foundpos);
                }
                else
                {
                    positions << sad::Pair<sad::SceneNode*, int>(nodes[i], position);
                }
            }
        }

        sad::Vector<sad::db::Object*> animationinstances;
        sad::Vector<sad::animations::Instance*> dependentinstances;
        sad::Vector< sad::Pair<sad::animations::Instance*, unsigned long long> > dependentonnodes;
        sad::Renderer::ref()->database("")->table("animationinstances")->objects(animationinstances);
        for(size_t  i = 0; i < animationinstances.size(); i++)
        {
            sad::db::Object* object = animationinstances[i];
            if (object->isInstanceOf("sad::animations::Instance") || object->isInstanceOf("sad::animations::WayInstance"))
            {
                sad::animations::Instance* ainstance = static_cast<sad::animations::Instance*>(object);
                if (ainstance->objectId() == scene->MajorId)
                {
                    dependentinstances << ainstance;
                }
                if (std::find(nodeids.begin(), nodeids.end(), ainstance->objectId()) != nodeids.end())
                {
                    dependentonnodes << sad::Pair<sad::animations::Instance*, unsigned long long>(ainstance, ainstance->objectId());
                }
            }
        }

        history::scenes::Remove* c = new history::scenes::Remove(scene, row);
        c->set(positioninanimationcombo, positions, dependentinstances, dependentonnodes);
        
        sad::Vector< sad::Pair<sad::SceneNode*, gui::GridPosition> > affected_grids;
        m_editor->actions()->gridActions()->findParentGrids(nodes, affected_grids);
        c->setAffectedGrids(affected_grids);

        this->m_editor->history()->add(c);
        c->commit(m_editor);
    }
}


// ============================= PUBLIC SLOTS =============================


void gui::actions::SceneActions::addScene()
{
    addSceneWithName("", true);
}

void gui::actions::SceneActions::currentSceneChanged(int index)
{
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes; 
    QLineEdit* txtSceneName = m_editor->uiBlocks()->uiSceneBlock()->txtSceneName; 
    QListWidget* lstSceneObjects = m_editor->uiBlocks()->uiSceneBlock()->lstSceneObjects;
    if (m_editor->machine()->isInState("adding")
        || (m_editor->machine()->isInState("selected") 
            && m_editor->machine()->currentState() != "selected"))
    {
        if (this->currentScene())
        {
            bool b = lstScenes->blockSignals(true);
            int row = this->findSceneInList(this->currentScene());
            if (row != -1)
            {
                lstScenes->setCurrentRow(row);
            }
            lstScenes->blockSignals(b);
            return;
        }
    }
    if (index != -1) 
    {
        QListWidgetItem* item = lstScenes->item(index);
        sad::Scene* s = item->data(Qt::UserRole).value<sad::Scene*>();
        bool b = txtSceneName->blockSignals(true);
        txtSceneName->setText(STD2QSTRING(s->objectName().c_str()));
        txtSceneName->blockSignals(b);

        lstSceneObjects->clear();
        const sad::Vector<sad::SceneNode*>& nodes = s->objects();
        for(size_t i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->active())
            {
                QListWidgetItem* ki = new QListWidgetItem();
                ki->setText(this->viewableObjectName(nodes[i]));
                
                QVariant v;
                v.setValue(nodes[i]);
                ki->setData(Qt::UserRole, v);
                lstSceneObjects->addItem(ki);
            }
        }
    }
}

void gui::actions::SceneActions::sceneNameChanged(const QString&)
{
    sad::Scene* scene = currentScene();
    QLineEdit* txtSceneName = m_editor->uiBlocks()->uiSceneBlock()->txtSceneName; 
    if (scene)
    {
        sad::String oldname = scene->objectName();
        sad::String newname = Q2STDSTRING(txtSceneName->text());
        
        history::Command* c = new history::scenes::ChangeName(scene, oldname, newname);
        this->m_editor->history()->add(c);
        c->commitWithoutUpdatingUI(m_editor);
    }
}


void gui::actions::SceneActions::removeScene()
{
    /*! Run it from editor
     */
    scriptableRemoveScene(nullptr, true);
}

void gui::actions::SceneActions::sceneMoveBack()
{
    sad::Scene* scene = currentScene();
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes; 
    if (scene)
    {
        int row = lstScenes->currentRow();
        if (row != 0)
        {
            sad::Scene* previousscene = lstScenes->item(row -1)->data(Qt::UserRole).value<sad::Scene*>();

            history::Command* c = new history::scenes::LayerSwap(scene, previousscene, row, row - 1);
            this->m_editor->history()->add(c);
            c->commit(m_editor);
        }
    }
}

void  gui::actions::SceneActions::sceneMoveFront()
{
    sad::Scene* scene = currentScene();
    QListWidget* lstScenes = m_editor->uiBlocks()->uiSceneBlock()->lstScenes; 
    if (scene)
    {
        int row = lstScenes->currentRow();
        if (row < lstScenes->count() - 1)
        {
            sad::Scene* nextscene = lstScenes->item(row + 1)->data(Qt::UserRole).value<sad::Scene*>();

            history::Command* c = new history::scenes::LayerSwap(scene, nextscene, row, row + 1);
            this->m_editor->history()->add(c);
            c->commit(m_editor);
        }
    }
}

void gui::actions::SceneActions::clearScene()
{
    sad::Scene* scene = currentScene();
    QComboBox* cmbAnimationInstanceObject = m_editor->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject;
    if (scene)
    {

        sad::Vector<sad::SceneNode*> nodes = scene->objects();
        sad::Vector< sad::Pair<sad::SceneNode*, int> > positions;
        sad::Vector<unsigned long long> nodeids;
        for(size_t i = 0; i < nodes.size(); i++)
        {
            nodeids << nodes[i]->MajorId;
            int pos = this->findInComboBox<sad::db::Object*>(cmbAnimationInstanceObject, nodes[i]);
            if (pos > -1)
            {
                positions << sad::Pair<sad::SceneNode*, int>(nodes[i], pos);
            }
        }

        sad::Vector<sad::db::Object*> animationinstances;
        sad::Vector< sad::Pair<sad::animations::Instance*, unsigned long long> > dependentinstances;
        sad::Renderer::ref()->database("")->table("animationinstances")->objects(animationinstances);
        for(size_t  i = 0; i < animationinstances.size(); i++)
        {
            sad::db::Object* object = animationinstances[i];
            if (object->isInstanceOf("sad::animations::Instance") || object->isInstanceOf("sad::animations::WayInstance"))
            {
                sad::animations::Instance* ainstance = static_cast<sad::animations::Instance*>(object);
                if (std::find(nodeids.begin(), nodeids.end(), ainstance->objectId()) != nodeids.end())
                {
                    dependentinstances << sad::Pair<sad::animations::Instance*, unsigned long long>(ainstance, ainstance->objectId());
                }
            }
        }




        history::scenes::Clear* c = new history::scenes::Clear(scene);
        c->set(positions, dependentinstances);
        sad::Vector< sad::Pair<sad::SceneNode*, gui::GridPosition> > affected_grids;
        m_editor->actions()->gridActions()->findParentGrids(nodes, affected_grids);
        c->setAffectedGrids(affected_grids);
        this->m_editor->history()->add(c);
        c->commit(m_editor);
    }
}
