 // ReSharper disable once CppUnusedIncludeDirective
#include <QMessageBox>
#include <QCheckBox>
#include <QLineEdit>

#include <animations/animationsanimation.h>
#include <animations/animationsinstance.h>

#include <animations/easing/easingfunction.h>

#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <p2d/app/way.h>

#include "../../gui/animationprocess.h"
#include "../../gui/mainpanelproxy.h"

#include "../../gui/actions/animationactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

#include "../../gui/textedit/textedit.h"

#include "../../blockedclosuremethodcall.h"

#include "../../acolordialog.h"
#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../core/typeconverters/qcolortosadacolor.h"
#include "../../core/typeconverters/sadacolortoqcolor.h"
#include "../../core/typeconverters/qrectftosadrect2d.h"
#include "../../core/typeconverters/sadrect2dtoqrectf.h"


#include "../../history/animations/animationsnew.h"
#include "../../history/animations/animationsremove.h"
#include "../../history/animations/animationschangename.h"
#include "../../history/animations/animationschangetime.h"
#include "../../history/animations/animationschangelooped.h"
#include "../../history/animations/animationschangeblinkingfrequency.h"
#include "../../history/animations/animationschangecolorcolor.h"
#include "../../history/animations/animationschangeresizestartingsize.h"
#include "../../history/animations/animationschangeresizeendingsize.h"
#include "../../history/animations/animationschangerotateangle.h"
#include "../../history/animations/animationschangewaymovingway.h"
#include "../../history/animations/animationschangefontlistfonts.h"
#include "../../history/animations/animationschangefontsizesize.h"
#include "../../history/animations/animationschangerect.h"
#include "../../history/animations/animationschangecamerapivot.h"
#include "../../history/animations/animationschangecameraangle.h"
#include "../../history/animations/animationschangecameraoffset.h"
#include "../../history/animations/animationschangeshakingfrequency.h"
#include "../../history/animations/animationsaddtocomposite.h"
#include "../../history/animations/animationsremovefromcomposite.h"
#include "../../history/animations/animationsswapincomposite.h"
#include "../../history/animations/animationschangepropertyaspoint2displayedintwospinboxes.h"
#include "../../history/animations/animationschangeeasingfunctiontype.h"
#include "../../history/animations/animationschangeeasingovershootamplitude.h"
#include "../../history/animations/animationschangeeasingperiod.h"

Q_DECLARE_METATYPE(sad::animations::Animation*) //-V566
Q_DECLARE_METATYPE(sad::p2d::app::Way*) //-V566

// ===============================  PUBLIC METHODS ===============================

gui::actions::AnimationActions::AnimationActions(QObject* parent) : QObject(parent), gui::actions::AbstractActions()
{
    m_animation = new gui::AnimationProcess();
}

void gui::actions::AnimationActions::setEditor(core::Editor* e)
{
    this->gui::actions::AbstractActions::setEditor(e);
    m_animation->setEditor(e);
}

gui::actions::AnimationActions::~AnimationActions()
{
    delete m_animation;	
}

bool gui::actions::AnimationActions::producesLoop(sad::animations::Animation* first, sad::animations::Animation* second) const
{
    bool result = false;
    sad::Vector<sad::db::Object*> objects;
    sad::Renderer::ref()->database("")->table("animations")->objects(objects);
    sad::Vector<sad::animations::Composite*> animations;
    for(size_t i = 0; i < objects.size(); i++)
    {
        if (objects[i]->isInstanceOf("sad::animations::Sequential") || 
            objects[i]->isInstanceOf("sad::animations::Parallel"))
        {
            animations << static_cast<sad::animations::Composite*>(objects[i]);
        }
    }
    for(size_t i = 0; (i < animations.size()) && result == false; i++)
    {
        sad::Hash<unsigned long long, unsigned long long> tmp;
        result = result || producesLoop(animations[i], tmp, first, second);
    }
    return result;
}

bool gui::actions::AnimationActions::producesLoop(
    sad::animations::Composite* current,
    const sad::Hash<unsigned long long, unsigned long long>& visited,
    sad::animations::Animation* first,
    sad::animations::Animation* second
)
{
    // If already visited this node, then we must have returned here by loop
    if (visited.contains(current->MajorId))
    {
        return true;
    }
    sad::Hash<unsigned long long, unsigned long long> local = visited;	
    local.insert(current->MajorId, 1);
    if (current == first)
    {
        // If second node has been visited already, than we must been here
        if (local.contains(second->MajorId))
        {
            return true;
        }
        if (second->isInstanceOf("sad::animations::Parallel") || second->isInstanceOf("sad::animations::Sequential"))
        {
            sad::animations::Composite* c = static_cast<sad::animations::Composite*>(second);
            // Check if loop can be found just by jumping from second animation
            if (producesLoop(c, local, first, second))
            {
                return true;
            }
        }
    }

    bool result = false;
    for(size_t i = 0; i < current->size() && result == false; i++)
    {
        sad::animations::Animation* a  = current->animation(i);
        if (a)
        {
            if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
            {
                sad::animations::Composite* c = static_cast<sad::animations::Composite*>(a);
                result = result || producesLoop(c, local, first, second);
            }
        }
    }
    return result;
}


void gui::actions::AnimationActions::removeAnimationFromDatabase(
    sad::animations::Animation* a,
    bool from_editor
) const
{
    QListWidget* listWidget = m_editor->uiBlocks()->uiAnimationBlock()->lstAnimations;
    int posinmainlist = this->findInList<sad::animations::Animation*>(listWidget, a);
    QComboBox* combo = m_editor->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceAnimationFromDatabase;
    int posininstances = this->findInComboBox<sad::animations::Animation*>(combo, a);

    sad::Vector< sad::Pair<sad::animations::Composite*, sad::Vector<int> > > list;

    sad::Vector<sad::db::Object*> animations;
    sad::Renderer::ref()->database("")->table("animations")->objects(animations);
    for(size_t i = 0; i < animations.size(); i++)
    {
        sad::db::Object* object = animations[i];
        if (object->isInstanceOf("sad::animations::Parallel") || object->isInstanceOf("sad::animations::Sequential"))
        {
            sad::animations::Composite* c = static_cast<sad::animations::Composite*>(object);
            sad::Vector<unsigned long long> majorids = c->animationMajorIds();
            sad::Vector<int> positions;
            for(size_t j = 0; j < majorids.size(); j++)
            {
                if (majorids[j] == a->MajorId)
                {
                    positions << j;
                }
            }
            if (positions.size() != 0)
            {
                list << sad::Pair<sad::animations::Composite*, sad::Vector<int> >(c, positions);				
            }
        }
    }

    sad::Vector<sad::db::Object*> animationinstances;
    sad::Vector<sad::animations::Instance*> dependentinstances;
    sad::Renderer::ref()->database("")->table("animationinstances")->objects(animationinstances);
    for(size_t  i = 0; i < animationinstances.size(); i++)
    {
        sad::db::Object* object = animationinstances[i];
        if (object->isInstanceOf("sad::animations::Instance") || object->isInstanceOf("sad::animations::WayInstance"))
        {
            sad::animations::Instance* ainstance = static_cast<sad::animations::Instance*>(object);
            if (ainstance->animationMajorId() == a->MajorId)
            {
                dependentinstances << ainstance;
            }
        }
    }

    history::animations::Remove* command = new history::animations::Remove(a);
    command->set(posinmainlist, posininstances, list);
    command->set(dependentinstances);
    command->commit(m_editor);

    m_editor->addToHistory(command, from_editor);
}

bool gui::actions::AnimationActions::addAnimationToCompositeList(
    sad::animations::Composite* a,
    sad::animations::Animation* added_animation,
    bool from_editor
) const
{
    bool result = false;
    if (producesLoop(a, added_animation) == false)
    {
        sad::animations::Composite* co = static_cast<sad::animations::Composite*>(a);
        history::Command* c = new history::animations::AddToComposite(co, added_animation->MajorId);
        c->commit(m_editor);
        m_editor->addToHistory(c, from_editor);
        result = true;
    }
    return result;
}


bool gui::actions::AnimationActions::removeAnimationFromCompositeList(
        sad::animations::Composite* a,
        sad::animations::Animation* animation,
        bool from_editor,
        int row
) const
{
    unsigned long long majorid = animation->MajorId;
    if (row == -1)
    {
        for(size_t i = 0; i < a->animationMajorIds().size() && row == -1; i++)
        {
            if (a->animationMajorIds()[i] == majorid)
            {
                row =  i;
            }
        }
    }
    bool result = false;
    if (row != -1)
    {
        result = true;
        history::Command* c = new history::animations::RemoveFromComposite(a, majorid, row);
        c->commit(m_editor);
        m_editor->addToHistory(c, from_editor);
    }
    return result;
}

QString gui::actions::AnimationActions::nameForAnimation(sad::animations::Animation* a) const
{
    QString result = const_cast<gui::actions::AnimationActions*>(this)->viewableObjectName(a);
    if (a)
    {
        sad::String s = a->serializableName();
        s.removeAllOccurrences("sad::animations::");
        s.insert('[', 0);
        s << "] ";
        std::string ss = s.c_str(); 
        result = QString(STD2QSTRING(ss)) + result;
    }
    return result;
}


void gui::actions::AnimationActions::addAnimationToViewingLists(sad::animations::Animation* a) const
{
    QString name = this->nameForAnimation(a);

    QListWidget* lstAnimations = m_editor->uiBlocks()->uiAnimationBlock()->lstAnimations;
    QListWidget* lstCompositeCandidates = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeCandidates;
    QComboBox* cmbAnimationInstanceAnimationFromDatabase = m_editor->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceAnimationFromDatabase;

    QVariant v;
    v.setValue(a);

    QListWidget* listofanimations = lstAnimations;
    listofanimations->addItem(name);
    listofanimations->item(listofanimations->count() - 1)->setData(Qt::UserRole, v);

    QComboBox* listofanimationsforinstances = cmbAnimationInstanceAnimationFromDatabase;
    listofanimationsforinstances->addItem(name);
    listofanimationsforinstances->setItemData(listofanimationsforinstances->count() - 1, v, Qt::UserRole);

    QListWidget* listofanimationcandidates = lstCompositeCandidates;
    listofanimationcandidates->addItem(name);
    listofanimationcandidates->item(listofanimationcandidates->count() - 1)->setData(Qt::UserRole, v);
}


void gui::actions::AnimationActions::removeAnimationFromViewingLists(sad::animations::Animation* a) const
{
    QListWidget* lstAnimations = m_editor->uiBlocks()->uiAnimationBlock()->lstAnimations;
    QListWidget* lstCompositeList = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeList;
    QListWidget* lstCompositeCandidates = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeCandidates;	
    QComboBox* cmbAnimationInstanceAnimationFromDatabase = m_editor->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceAnimationFromDatabase;
    
    int pos = this->findInList(lstAnimations, a);

    if (a == m_editor->shared()->selectedAnimation())
    {
        lstCompositeList->clear();
    }

    if (pos > -1)
    {
        delete lstAnimations->takeItem(pos);
    }

    pos = this->findInComboBox(cmbAnimationInstanceAnimationFromDatabase, a);
    if (pos > - 1)
    {
        cmbAnimationInstanceAnimationFromDatabase->removeItem(pos);
    }

    pos = this->findInList(lstCompositeCandidates, a);
    if (pos > -1)
    {
        delete lstCompositeCandidates->takeItem(pos);
    }

}

void gui::actions::AnimationActions::updateAnimationName(sad::animations::Animation* a) const
{
    QListWidget* lstAnimations = m_editor->uiBlocks()->uiAnimationBlock()->lstAnimations;
    QListWidget* lstCompositeList = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeList;
    QListWidget* lstCompositeCandidates = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeCandidates;	
    QComboBox* cmbAnimationInstanceAnimationFromDatabase = m_editor->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceAnimationFromDatabase;

    QString name = this->nameForAnimation(a);

    int pos = this->findInList(lstAnimations, a);
    if (pos > -1)
    {
        lstAnimations->item(pos)->setText(name);
    }

    pos = this->findInComboBox(cmbAnimationInstanceAnimationFromDatabase, a);
    if (pos > - 1)
    {
        cmbAnimationInstanceAnimationFromDatabase->setItemText(pos, name);
    }

    pos = this->findInList(lstCompositeCandidates, a);
    if (pos > -1)
    {
        lstCompositeCandidates->item(pos)->setText(name);
    }

    pos = this->findInList(lstCompositeList, a);
    if (pos > -1)
    {
        lstCompositeList->item(pos)->setText(name);
    }
}

// ===============================  PUBLIC SLOTS METHODS ===============================

void gui::actions::AnimationActions::addAnimation() const
{
    gui::uiblocks::UIAnimationBlock* blk = m_editor->uiBlocks()->uiAnimationBlock(); 
    QComboBox * cmbtype = blk->cmbAddedAnimationType;
    if (cmbtype->currentIndex() != -1)
    {
        QString animationtypename = cmbtype->currentText();
        animationtypename = QString("sad::animations::") + animationtypename;

        sad::animations::Animation* a = m_editor->animationFactory()->create(Q2STDSTRING(animationtypename));

        if (a)
        {
            a->easing()->setFunctionTypeAsUnsignedInt(blk->cmbAnimationEasingType->currentIndex());
            a->easing()->setOvershootAmplitude(blk->dsbAnimationEasingOvershootAmplitude->value());
            a->easing()->setPeriod(blk->dsbAnimationEasingPeriod->value());

            a->setLooped(blk->cbAnimationLooped->checkState() == Qt::Checked);
            a->setTime(blk->dsbAnimationTime->value());
            a->setObjectName(Q2STDSTRING(blk->txtAnimationName->text()));
            if (a->isInstanceOf("sad::animations::Blinking"))
            {
                unsigned int frequency = static_cast<unsigned int>(blk->sbBlinkingFrequency->value());
                a->setProperty("frequency", frequency);
            }
            if (a->isInstanceOf("sad::animations::Color"))
            {
                QColor sourcemincolor = blk->cwColorStartingColor->backgroundColor();
                QColor sourcemaxcolor = blk->cwColorEndingColor->backgroundColor();

                sad::AColor mincolor;
                sad::AColor maxcolor;

                core::typeconverters::QColorToSadAColor::convert(sourcemincolor, mincolor);
                core::typeconverters::QColorToSadAColor::convert(sourcemaxcolor, maxcolor);

                a->setProperty("min_color", mincolor);
                a->setProperty("max_color", maxcolor);
            }

            if (a->isInstanceOf("sad::animations::Resize"))
            {
                sad::Point2D startsize(
                    blk->dabResizeStartingSizeX->value(),
                    blk->dabResizeStartingSizeY->value()
                );
                a->setProperty("start_size", startsize);

                sad::Point2D endsize(
                    blk->dabResizeEndingSizeX->value(),
                    blk->dabResizeEndingSizeY->value()
                );
                a->setProperty("end_size", endsize);
            }

            if (a->isInstanceOf("sad::animations::Rotate"))
            {
                double minangle = blk->dsbRotateStartingAngle->value();
                double maxangle = blk->dsbRotateEndingAngle->value();
                minangle = minangle / 180.0 * M_PI;
                maxangle = maxangle / 180.0 * M_PI;

                a->setProperty("min_angle", minangle);
                a->setProperty("max_angle", maxangle);
            }

            if (a->isInstanceOf("sad::animations::WayMoving"))
            {
                unsigned long long selectedid = 0;
                int index = blk->cmbWayAnimationWay->currentIndex();
                if (index >= 0)
                {
                    QVariant v = blk->cmbWayAnimationWay->itemData(index, Qt::UserRole);
                    sad::p2d::app::Way* w = v.value<sad::p2d::app::Way*>();
                    if (w)
                    {
                        selectedid = w->MajorId;
                    }
                }
                a->setProperty("way", selectedid);
            }

            if (a->isInstanceOf("sad::animations::FontList"))
            {
                QStringList list = blk->txtFontListList->toPlainText().split("\n", QString::SkipEmptyParts);
                sad::Vector<sad::String> nlist;
                for(size_t i = 0; i < list.size(); i++)
                {
                    QString tmp = list[i].trimmed();
                    if (tmp.length())
                    {
                        nlist << Q2STDSTRING(tmp);
                    }
                }
                a->setProperty("fonts", nlist);
            }

            if (a->isInstanceOf("sad::animations::FontSize"))
            {
                unsigned int minsize = static_cast<unsigned int>(blk->sbFontSizeStartingSize->value());
                unsigned int maxsize = static_cast<unsigned int>(blk->sbFontSizeEndingSize->value());

                a->setProperty("min_size", minsize);
                a->setProperty("max_size", maxsize);
            }

            if (a->isInstanceOf("sad::animations::SimpleMovement"))
            {
                double sx = blk->dabSimpleMovementStartingPointX->value();
                double sy = blk->dabSimpleMovementStartingPointY->value();

                double ex = blk->dabSimpleMovementEndingPointX->value();
                double ey = blk->dabSimpleMovementEndingPointY->value();

                a->setProperty("start_point", sad::Point2D(sx, sy));
                a->setProperty("end_point", sad::Point2D(ex, ey));
            }

            if (a->isInstanceOf("sad::animations::OptionList"))
            {
                QStringList list = blk->txtOptionListList->toPlainText().split("\n", QString::SkipEmptyParts);
                sad::Vector<sad::String> nlist;
                for(size_t i = 0; i < list.size(); i++)
                {
                    QString tmp = list[i].trimmed();
                    if (tmp.length())
                    {
                        nlist << Q2STDSTRING(tmp);
                    }
                }
                a->setProperty("list", nlist);
            }

            if (a->isInstanceOf("sad::animations::TextureCoordinatesList"))
            {
                QStringList list = blk->txtTextureCoordinatesList->toPlainText().split("\n", QString::SkipEmptyParts);
                sad::Vector<sad::String> nlist;
                for(size_t i = 0; i < list.size(); i++)
                {
                    QString tmp = list[i].trimmed();
                    if (tmp.length())
                    {
                        nlist << Q2STDSTRING(tmp);
                    }
                }
                a->setProperty("list", nlist);
            }

            if (a->isInstanceOf("sad::animations::TextureCoordinatesContinuous"))
            {
                QRectF start = blk->rctTCCStartingRect->value();
                QRectF end = blk->rctTCCEndingRect->value();

                sad::Rect2D kstart;
                sad::Rect2D kend;

                core::typeconverters::QRectFToSadRect2D::convert(start, kstart);
                core::typeconverters::QRectFToSadRect2D::convert(end, kend);

                a->setProperty("start_rect", kstart);
                a->setProperty("end_rect", kend);
            }

            if (a->isInstanceOf("sad::animations::CameraRotation"))
            {
                sad::Point3D pivot(
                    blk->dsbCameraRotationPivotX->value(),
                    blk->dsbCameraRotationPivotY->value(),
                    0.0
                );

                double startangle = blk->dsbCameraRotationStartingAngle->value();
                double endangle = blk->dsbCameraRotationEndingAngle->value();
                a->setProperty("min_angle", startangle);
                a->setProperty("max_angle", endangle);
                a->setProperty("pivot", pivot);
            }

            if (a->isInstanceOf("sad::animations::CameraShaking"))
            {
                sad::Point2D offset(
                    blk->dsbCameraShakingOffsetX->value(),
                    blk->dsbCameraShakingOffsetY->value()
                );

                int freq = blk->sbCameraShakingFrequency->value();
                a->setProperty("frequency", freq);
                a->setProperty("offset", offset);
            }

            if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
            {
                sad::Vector<unsigned long long> majorids;
                QListWidget* w = blk->lstCompositeList;
                for(size_t i = 0; i < w->count(); i++)
                {
                    sad::animations::Animation* ka = w->item(i)->data(Qt::UserRole).value<sad::animations::Animation*>();
                    majorids << ka->MajorId;
                }
                a->setProperty("list", majorids);
            }
            

            sad::Renderer::ref()->database("")->table("animations")->add(a);

            history::animations::New* c = new history::animations::New(a);
            c->commit(m_editor);

            blk->lstAnimations->setCurrentRow(blk->lstAnimations->count() - 1);
            m_editor->history()->add(c);			
        }
        else
        {
            QMessageBox::critical(nullptr, "Coding error", animationtypename + " is not defined");
        }
    }
}

void gui::actions::AnimationActions::removeAnimation() const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        removeAnimationFromDatabase(a, true);
    }
}

void gui::actions::AnimationActions::currentAnimationChanged(int row) const
{
    gui::uiblocks::UIAnimationBlock* blk = m_editor->uiBlocks()->uiAnimationBlock(); 
    core::Editor* e = m_editor;
    if (row > -1)
    {
        QVariant v = blk->lstAnimations->item(row)->data(Qt::UserRole);
        sad::animations::Animation* a = v.value<sad::animations::Animation*>();
        e->shared()->setSelectedAnimation(a);
        std::string nameAsString = a->objectName().c_str();
        e->emitClosure( blocked_bind(blk->txtAnimationName, &QLineEdit::setText, STD2QSTRING(nameAsString))) ;
        e->emitClosure( blocked_bind(blk->dsbAnimationTime, &QDoubleSpinBox::setValue, a->time()) );
        Qt::CheckState cs = (a->looped()) ? Qt::Checked : Qt::Unchecked;
        e->emitClosure( blocked_bind(blk->cbAnimationLooped, &QCheckBox::setCheckState, cs) );
        // Easing properties updates
        e->emitClosure( blocked_bind(blk->cmbAnimationEasingType, &QComboBox::setCurrentIndex, a->easing()->functionTypeAsUnsignedInt()));
        e->emitClosure( blocked_bind(blk->dsbAnimationEasingOvershootAmplitude, &QDoubleSpinBox::setValue, a->easing()->overshootAmplitude()));
        e->emitClosure( blocked_bind(blk->dsbAnimationEasingPeriod, &QDoubleSpinBox::setValue, a->easing()->period()));

        if (a->isInstanceOf("sad::animations::Blinking"))
        {
            unsigned int frequency = a->getProperty<unsigned int>("frequency").value();
            e->emitClosure( blocked_bind(blk->sbBlinkingFrequency, &QSpinBox::setValue, frequency) );
        }
        if (a->isInstanceOf("sad::animations::Color"))
        {
            sad::AColor sourcemincolor = a->getProperty<sad::AColor>("min_color").value();
            sad::AColor sourcemaxcolor = a->getProperty<sad::AColor>("max_color").value();
            
            QColor mincolor;
            QColor maxcolor;

            core::typeconverters::SadAColorToQColor::convert(sourcemincolor, mincolor);
            core::typeconverters::SadAColorToQColor::convert(sourcemaxcolor, maxcolor);

            blk->cwColorStartingColor->setBackgroundColor(mincolor);
            blk->cwColorEndingColor->setBackgroundColor(maxcolor);
        }

        if (a->isInstanceOf("sad::animations::Resize"))
        {
            sad::Point2D startsize = a->getProperty<sad::Point2D>("start_size").value();
            sad::Point2D end_size = a->getProperty<sad::Point2D>("end_size").value();

            e->emitClosure( blocked_bind(blk->dabResizeStartingSizeX, &QDoubleSpinBox::setValue, startsize.x()) );
            e->emitClosure( blocked_bind(blk->dabResizeStartingSizeY, &QDoubleSpinBox::setValue, startsize.y()) );

            e->emitClosure( blocked_bind(blk->dabResizeEndingSizeX, &QDoubleSpinBox::setValue, end_size.x()) );
            e->emitClosure( blocked_bind(blk->dabResizeEndingSizeY, &QDoubleSpinBox::setValue, end_size.y()) );
        }

        if (a->isInstanceOf("sad::animations::Rotate"))
        {
            double minangle = a->getProperty<double>("min_angle").value();
            double maxangle = a->getProperty<double>("max_angle").value();

            minangle =  minangle / M_PI * 180.0;
            maxangle =  maxangle / M_PI * 180.0;

            e->emitClosure( blocked_bind(blk->dsbRotateStartingAngle, &QDoubleSpinBox::setValue, minangle) );
            e->emitClosure( blocked_bind(blk->dsbRotateEndingAngle, &QDoubleSpinBox::setValue, maxangle) );
        }

        if (a->isInstanceOf("sad::animations::SimpleMovement"))
        {
            sad::Point2D startpos = a->getProperty<sad::Point2D>("start_point").value();
            sad::Point2D endpos = a->getProperty<sad::Point2D>("end_point").value();

            e->emitClosure( blocked_bind(blk->dabSimpleMovementStartingPointX, &QDoubleSpinBox::setValue, startpos.x()) );
            e->emitClosure( blocked_bind(blk->dabSimpleMovementStartingPointY, &QDoubleSpinBox::setValue, startpos.y()) );

            e->emitClosure( blocked_bind(blk->dabSimpleMovementEndingPointX, &QDoubleSpinBox::setValue, endpos.x()) );
            e->emitClosure( blocked_bind(blk->dabSimpleMovementEndingPointY, &QDoubleSpinBox::setValue, endpos.y()) );
        }

        if (a->isInstanceOf("sad::animations::WayMoving"))
        {
            unsigned long long way = a->getProperty<unsigned long long>("way").value();
            int pos = this->findInComboBoxByMajorId<sad::p2d::app::Way*>(blk->cmbWayAnimationWay, way);
            if (pos >= 0)
            {
                e->emitClosure( blocked_bind(blk->cmbWayAnimationWay, &QComboBox::setCurrentIndex, pos) );
            }
        }

        if (a->isInstanceOf("sad::animations::FontList"))
        {
            QStringList list;
            sad::Vector<sad::String> nlist = a->getProperty<sad::Vector<sad::String> >("fonts").value();
            for(size_t i = 0; i < nlist.size(); i++)
            {
                std::string nlisti = nlist[i].c_str(); 
                list << STD2QSTRING(nlisti);
            }
            e->emitClosure( blocked_bind(blk->txtFontListList, &QTextEdit::setPlainText, list.join("\n")));
        }

        if (a->isInstanceOf("sad::animations::FontSize"))
        {
            int minsize = a->getProperty<unsigned int>("min_size").value();
            int maxsize = a->getProperty<unsigned int>("max_size").value();

            e->emitClosure( blocked_bind(blk->sbFontSizeStartingSize, &QSpinBox::setValue, minsize) );
            e->emitClosure( blocked_bind(blk->sbFontSizeEndingSize, &QSpinBox::setValue, maxsize) );
        }

        if (a->isInstanceOf("sad::animations::OptionList"))
        {
            QStringList list;
            sad::Vector<sad::String> nlist = a->getProperty<sad::Vector<sad::String> >("list").value();
            for(size_t i = 0; i < nlist.size(); i++)
            {
                std::string nlisti = nlist[i].c_str(); 
                list << STD2QSTRING(nlisti);
            }
            e->emitClosure( blocked_bind(blk->txtOptionListList, &QTextEdit::setPlainText, list.join("\n")));
        }

        if (a->isInstanceOf("sad::animations::TextureCoordinatesList"))
        {
            QStringList list;
            sad::Vector<sad::String> nlist = a->getProperty<sad::Vector<sad::String> >("list").value();
            for(size_t i = 0; i < nlist.size(); i++)
            {
                std::string nlisti = nlist[i].c_str(); 
                list << STD2QSTRING(nlisti);
            }
            e->emitClosure( blocked_bind(blk->txtTextureCoordinatesList, &QTextEdit::setPlainText, list.join("\n")));
        }

        if (a->isInstanceOf("sad::animations::TextureCoordinatesContinuous"))
        {
            sad::Rect2D nstartrect = a->getProperty<sad::Rect2D>("start_rect").value();
            sad::Rect2D nendrect = a->getProperty<sad::Rect2D>("end_rect").value();

            QRectF startrect;
            QRectF endrect;

            core::typeconverters::SadRect2DToQRectF::convert(nstartrect, startrect);
            core::typeconverters::SadRect2DToQRectF::convert(nendrect, endrect);

            e->emitClosure( blocked_bind(blk->rctTCCStartingRect, &gui::rectwidget::RectWidget::setValue, startrect) );
            e->emitClosure( blocked_bind(blk->rctTCCEndingRect, &gui::rectwidget::RectWidget::setValue, endrect) );
        }

        if (a->isInstanceOf("sad::animations::CameraRotation"))
        {
            sad::Point3D pivot = a->getProperty<sad::Point3D>("pivot").value();

            e->emitClosure( blocked_bind(blk->dsbCameraRotationPivotX, &QDoubleSpinBox::setValue, pivot.x()) );
            e->emitClosure( blocked_bind(blk->dsbCameraRotationPivotY, &QDoubleSpinBox::setValue, pivot.y()) );

            double startangle = a->getProperty<double>("min_angle").value();
            double endangle = a->getProperty<double>("max_angle").value();

            e->emitClosure( blocked_bind(blk->dsbCameraRotationStartingAngle, &QDoubleSpinBox::setValue, startangle) );
            e->emitClosure( blocked_bind(blk->dsbCameraRotationEndingAngle, &QDoubleSpinBox::setValue, endangle) );
        }

        if (a->isInstanceOf("sad::animations::CameraShaking"))
        {
            sad::Point2D offset = a->getProperty<sad::Point2D>("offset").value();

            e->emitClosure( blocked_bind(blk->dsbCameraShakingOffsetX, &QDoubleSpinBox::setValue, offset.x()) );
            e->emitClosure( blocked_bind(blk->dsbCameraShakingOffsetY, &QDoubleSpinBox::setValue, offset.y()) );

            int frequency = a->getProperty<int>("frequency").value();
            e->emitClosure( blocked_bind(blk->sbCameraShakingFrequency, &QSpinBox::setValue, frequency) );
        }
        
        this->updateCompositeList();		
    }
    else
    {
        e->shared()->setSelectedAnimation(nullptr);
    }
}

void gui::actions::AnimationActions::nameChanged(const QString& name) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        const sad::String oldname = a->objectName();
        if (oldname != Q2STDSTRING(name))
        {
            history::animations::ChangeName* c = new history::animations::ChangeName(a, oldname, Q2STDSTRING(name));
            c->commitWithoutUpdatingUI(this->m_editor);

            this->m_editor->history()->add(c);
        }
    }
}


void gui::actions::AnimationActions::timeChanged(double time) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        double oldtime = a->time();
        if (sad::is_fuzzy_equal(oldtime, time) == false)
        {
            history::animations::ChangeTime* c = new history::animations::ChangeTime(a, oldtime, time);
            c->commitWithoutUpdatingUI(this->m_editor);

            this->m_editor->history()->add(c);
        }
    }
}


void gui::actions::AnimationActions::loopedChanged(bool new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        bool old_value = a->looped();
        if (old_value != new_value)
        {
            history::animations::ChangeLooped* c = new history::animations::ChangeLooped(a, old_value, new_value);
            c->commitWithoutUpdatingUI(this->m_editor);

            this->m_editor->history()->add(c);
        }
    }
}


void gui::actions::AnimationActions::blinkingFrequencyChanged(int new_value) const
{
    const auto new_u_value = static_cast<unsigned int>(new_value);
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::Blinking")) 
        {
            const unsigned int old_value = a->getProperty<unsigned int>("frequency").value();
            if (old_value != new_u_value)
            {
                auto* c = new history::animations::ChangeBlinkingFrequency(a, old_value, new_u_value);
                c->commitWithoutUpdatingUI(this->m_editor);

                this->m_editor->history()->add(c);
            }
        }
    }
}


void gui::actions::AnimationActions::startOnObject() const
{
    core::Shared* s = m_editor->shared();
    if (s->isAnyKindOfAnimationIsRunning() == false 
        && s->selectedObject() != nullptr
        && s->selectedAnimation() != nullptr)
    {
        sad::animations::Instance* i = new sad::animations::Instance();

        sad::animations::Animation* a = s->selectedAnimation();
        sad::SceneNode* node = s->selectedObject();
        sad::db::Object* obj = node;
        if (a->isInstanceOf("sad::animations::CameraShaking") || a->isInstanceOf("sad::animations::CameraRotation"))
        {
            obj = node->scene();
        }
        i->setAnimation(a);
        i->setObject(obj);
        i->setStartTime(0.0);
        m_animation->setEditor(m_editor);
        m_animation->start(i);
    }
}

void gui::actions::AnimationActions::stopOnObject() const
{
    core::Shared* s = m_editor->shared();
    if (s->isAnyKindOfAnimationIsRunning() == true)
    {
        m_animation->setEditor(m_editor);
        m_animation->stop();
    }
}


void gui::actions::AnimationActions::colorChangeStartingColor() const
{
    gui::colorview::ColorView* view = m_editor->uiBlocks()->uiAnimationBlock()->cwColorStartingColor; 
    QColor old_value = view->backgroundColor();
    AColorDialog dlg;
    QList<QList<QColor> > palette = m_editor->panelProxy()->colorPalette();
    dlg.setColorPalette(palette);
    dlg.setSelectedColor(old_value);
    
    if (dlg.exec() == QDialog::Accepted)
    {
        QColor i = dlg.selectedColor();
        m_editor->panelProxy()->setColorPalette(dlg.colorPalette());
        view->setBackgroundColor(i);
        sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
        if (old_value != i && a != nullptr)
        {
            if (a->isInstanceOf("sad::animations::Color"))
            {
                sad::AColor oldcolor;
                sad::AColor newcolor;

                core::typeconverters::QColorToSadAColor::convert(old_value,oldcolor);
                core::typeconverters::QColorToSadAColor::convert(i,newcolor);

                history::animations::ChangeColorColor* c = new history::animations::ChangeColorColor(
                    a,
                    "min_color",
                    view,
                    oldcolor,
                    newcolor
                );
                c->commitWithoutUpdatingUI(this->m_editor);

                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::colorChangeEndingColor() const
{
    gui::colorview::ColorView* view = m_editor->uiBlocks()->uiAnimationBlock()->cwColorEndingColor; 
    QColor old_value = view->backgroundColor();
    AColorDialog dlg;
    dlg.setColorPalette(m_editor->panelProxy()->colorPalette());
    dlg.setSelectedColor(old_value);
    
    if (dlg.exec() == QDialog::Accepted)
    {
        QColor i = dlg.selectedColor();
        m_editor->panelProxy()->setColorPalette(dlg.colorPalette());
        view->setBackgroundColor(i);
        sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
        if (old_value != i && a != nullptr)
        {
            if (a->isInstanceOf("sad::animations::Color"))
            {
                sad::AColor oldcolor;
                sad::AColor newcolor;

                core::typeconverters::QColorToSadAColor::convert(old_value,oldcolor);
                core::typeconverters::QColorToSadAColor::convert(i,newcolor);

                history::animations::ChangeColorColor* c = new history::animations::ChangeColorColor(
                    a,
                    "max_color",
                    view,
                    oldcolor,
                    newcolor
                );
                c->commitWithoutUpdatingUI(this->m_editor);

                this->m_editor->history()->add(c);
            }
        }
    }
}


void gui::actions::AnimationActions::resizeChangeStartingSizeX(double new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a)
    {
        if (a->isInstanceOf("sad::animations::Resize"))
        {
            sad::Point2D old_value = a->getProperty<sad::Point2D>("start_size").value();
            if (sad::is_fuzzy_equal(old_value.x(), new_value) == false)
            {
                sad::Point2D nvalue = old_value;
                nvalue.setX(new_value);

                history::animations::ChangeResizeStartingSize* c = new history::animations::ChangeResizeStartingSize(
                    a,
                    old_value,
                    nvalue
                );
                c->commitWithoutUpdatingUI(this->m_editor);

                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::resizeChangeStartingSizeY(double new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a)
    {
        if (a->isInstanceOf("sad::animations::Resize"))
        {
            sad::Point2D old_value = a->getProperty<sad::Point2D>("start_size").value();
            if (sad::is_fuzzy_equal(old_value.y(), new_value) == false)
            {
                sad::Point2D nvalue = old_value;
                nvalue.setY(new_value);

                history::animations::ChangeResizeStartingSize* c = new history::animations::ChangeResizeStartingSize(
                    a,
                    old_value,
                    nvalue
                );
                c->commitWithoutUpdatingUI(this->m_editor);

                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::resizeChangeEndingSizeX(double new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a)
    {
        if (a->isInstanceOf("sad::animations::Resize"))
        {
            sad::Point2D old_value = a->getProperty<sad::Point2D>("end_size").value();
            if (sad::is_fuzzy_equal(old_value.x(), new_value) == false)
            {
                sad::Point2D nvalue = old_value;
                nvalue.setX(new_value);

                history::animations::ChangeResizeEndingSize* c = new history::animations::ChangeResizeEndingSize(
                    a,
                    old_value,
                    nvalue
                );
                c->commitWithoutUpdatingUI(this->m_editor);

                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::resizeChangeEndingSizeY(double new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a)
    {
        if (a->isInstanceOf("sad::animations::Resize"))
        {
            sad::Point2D old_value = a->getProperty<sad::Point2D>("end_size").value();
            if (sad::is_fuzzy_equal(old_value.y(), new_value) == false)
            {
                sad::Point2D nvalue = old_value;
                nvalue.setY(new_value);

                history::animations::ChangeResizeEndingSize* c = new history::animations::ChangeResizeEndingSize(
                    a,
                    old_value,
                    nvalue
                );
                c->commitWithoutUpdatingUI(this->m_editor);

                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::rotateChangeStartingAngle(double new_value) const
{
    new_value = new_value / 180.0 * M_PI;
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a)
    {
        if (a->isInstanceOf("sad::animations::Rotate"))
        {
            double old_value = a->getProperty<double>("min_angle").value();
            if (sad::is_fuzzy_equal(old_value, new_value) == false)
            {				
                history::animations::ChangeRotateAngle* c = new history::animations::ChangeRotateAngle(
                    a,
                    "min_angle",
                    m_editor->uiBlocks()->uiAnimationBlock()->dsbRotateStartingAngle,
                    old_value,
                    new_value
                );
                c->commitWithoutUpdatingUI(this->m_editor);

                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::rotateChangeEndingAngle(double new_value) const
{
    new_value = new_value / 180.0 * M_PI;
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a)
    {
        if (a->isInstanceOf("sad::animations::Rotate"))
        {
            double old_value = a->getProperty<double>("max_angle").value();
            if (sad::is_fuzzy_equal(old_value, new_value) == false)
            {				
                history::animations::ChangeRotateAngle* c = new history::animations::ChangeRotateAngle(
                    a,
                    "max_angle",
                    m_editor->uiBlocks()->uiAnimationBlock()->dsbRotateEndingAngle,
                    old_value,
                    new_value
                );
                c->commitWithoutUpdatingUI(this->m_editor);

                this->m_editor->history()->add(c);
            }
        }
    }
}


void gui::actions::AnimationActions::wayMovingChangeWay(int row) const
{
    if (row != -1)
    {
        QVariant v = m_editor->uiBlocks()->uiAnimationBlock()->cmbWayAnimationWay->itemData(row, Qt::UserRole);
        sad::p2d::app::Way* w = v.value<sad::p2d::app::Way*>();
        unsigned long long new_value = 0;
        if (w)
        {
            new_value = w->MajorId;
        }
        sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
        if (a)
        {
            if (a->isInstanceOf("sad::animations::WayMoving"))
            {
                unsigned long long old_value = a->getProperty<unsigned long long>("way").value();
                if (old_value != new_value)
                {
                    history::Command* c =new history::animations::ChangeWayMovingWay(a, old_value, new_value);
                    c->commitWithoutUpdatingUI(m_editor);
                    this->m_editor->history()->add(c);
                }
            }
        }
    }
}

void gui::actions::AnimationActions::fontListEditingFinished() const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::FontList"))
        {
            QStringList list = m_editor->uiBlocks()->uiAnimationBlock()->txtFontListList->toPlainText().split("\n", QString::SkipEmptyParts);
            sad::Vector<sad::String> new_value;
            for(size_t i = 0; i < list.size(); i++)
            {
                new_value << Q2STDSTRING(list[i]);
            }

            sad::Vector<sad::String> old_value = a->getProperty<sad::Vector<sad::String> >("fonts").value();
            if (old_value != new_value)
            {
                history::Command* c = new history::animations::ChangeFontListFonts(a, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::fontSizeChangeStartingSize(int new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::FontSize"))
        {
            unsigned int old_value = a->getProperty<unsigned int>("min_size").value();
            if (static_cast<unsigned int>(new_value) != old_value)
            {
                history::Command* c = new history::animations::ChangeFontSizeSize(
                    a, 
                    "min_size",
                    m_editor->uiBlocks()->uiAnimationBlock()->sbFontSizeStartingSize,
                    old_value,
                    new_value
                );

                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::fontSizeChangeEndingSize(int new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::FontSize"))
        {
            unsigned int old_value = a->getProperty<unsigned int>("max_size").value();
            if (static_cast<unsigned int>(new_value) != old_value)
            {
                history::Command* c = new history::animations::ChangeFontSizeSize(
                    a, 
                    "max_size",
                    m_editor->uiBlocks()->uiAnimationBlock()->sbFontSizeEndingSize,
                    old_value,
                    new_value
                );

                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::optionListEditingFinished() const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::OptionList"))
        {
            QTextEdit* widget = m_editor->uiBlocks()->uiAnimationBlock()->txtOptionListList;
            sad::String prop = "list";

            QStringList list = widget->toPlainText().split("\n", QString::SkipEmptyParts);
            sad::Vector<sad::String> new_value;
            for(size_t i = 0; i < list.size(); i++)
            {
                new_value << Q2STDSTRING(list[i]);
            }

            sad::Vector<sad::String> old_value = a->getProperty<sad::Vector<sad::String> >(prop).value();
            if (old_value != new_value)
            {
                history::Command* c = new history::animations::ChangeList(a, prop, widget, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}


void gui::actions::AnimationActions::textureCoordinatesListEditingFinished() const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::TextureCoordinatesList"))
        {
            QTextEdit* widget = m_editor->uiBlocks()->uiAnimationBlock()->txtTextureCoordinatesList;
            sad::String prop = "list";

            QStringList list = widget->toPlainText().split("\n", QString::SkipEmptyParts);
            sad::Vector<sad::String> new_value;
            for(size_t i = 0; i < list.size(); i++)
            {
                new_value << Q2STDSTRING(list[i]);
            }

            sad::Vector<sad::String> old_value = a->getProperty<sad::Vector<sad::String> >(prop).value();
            if (old_value != new_value)
            {
                history::Command* c = new history::animations::ChangeList(a, prop, widget, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::textureCoordinatesChangeStartRect(QRectF value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::TextureCoordinatesContinuous"))
        {
            gui::rectwidget::RectWidget* widget = m_editor->uiBlocks()->uiAnimationBlock()->rctTCCStartingRect;
            sad::String prop = "start_rect";

            sad::Rect2D new_value;
            core::typeconverters::QRectFToSadRect2D::convert(value, new_value);

            sad::Rect2D old_value = a->getProperty< sad::Rect2D >(prop).value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangeRect(a, prop, widget, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::textureCoordinatesChangeEndRect(QRectF value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::TextureCoordinatesContinuous"))
        {
            gui::rectwidget::RectWidget* widget = m_editor->uiBlocks()->uiAnimationBlock()->rctTCCEndingRect;
            sad::String prop = "end_rect";

            sad::Rect2D new_value;
            core::typeconverters::QRectFToSadRect2D::convert(value, new_value);

            sad::Rect2D old_value = a->getProperty< sad::Rect2D >(prop).value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangeRect(a, prop, widget, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::cameraRotationChangePivotX(double new_x) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::CameraRotation"))
        {
            sad::Point3D new_value(new_x, m_editor->uiBlocks()->uiAnimationBlock()->dsbCameraRotationPivotY->value(), 0.0);

            sad::Point3D old_value = a->getProperty< sad::Point3D >("pivot").value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangeCameraPivot(a, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::cameraRotationChangePivotY(double new_y) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::CameraRotation"))
        {
            sad::Point3D new_value(m_editor->uiBlocks()->uiAnimationBlock()->dsbCameraRotationPivotX->value(), new_y, 0.0);

            sad::Point3D old_value = a->getProperty< sad::Point3D >("pivot").value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangeCameraPivot(a, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::cameraRotationChangeStartingAngle(double new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::CameraRotation"))
        {
            sad::String prop = "min_angle";
            QDoubleSpinBox* widget = m_editor->uiBlocks()->uiAnimationBlock()->dsbCameraRotationStartingAngle;
            double old_value = a->getProperty< double >(prop).value();
            if (sad::is_fuzzy_equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangeCameraAngle(a, prop, widget, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::cameraRotationChangeEndingAngle(double new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::CameraRotation"))
        {
            sad::String prop = "max_angle";
            QDoubleSpinBox* widget = m_editor->uiBlocks()->uiAnimationBlock()->dsbCameraRotationEndingAngle;
            double old_value = a->getProperty< double >(prop).value();
            if (sad::is_fuzzy_equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangeCameraAngle(a, prop, widget, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::cameraShakingChangeOffsetX(double new_x) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::CameraShaking"))
        {
            sad::Point2D new_value(new_x, m_editor->uiBlocks()->uiAnimationBlock()->dsbCameraShakingOffsetY->value());

            sad::Point2D old_value = a->getProperty< sad::Point2D >("offset").value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangeCameraOffset(a, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::cameraShakingChangeOffsetY(double new_y) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::CameraShaking"))
        {
            sad::Point2D new_value(m_editor->uiBlocks()->uiAnimationBlock()->dsbCameraShakingOffsetX->value(), new_y);

            sad::Point2D old_value = a->getProperty< sad::Point2D >("offset").value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangeCameraOffset(a, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::cameraShakingChangeFrequency(int new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::CameraShaking"))
        {
            int old_value = a->getProperty< int >("frequency").value();
            if (old_value != new_value)
            {
                history::Command* c = new history::animations::ChangeShakingFrequency(a, old_value, new_value);
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::updateCompositeList() const
{
    sad::Vector<unsigned long long> majorids;
    sad::animations::Animation* sa = m_editor->shared()->selectedAnimation();
    if (sa)
    {
        if (sa->isInstanceOf("sad::animations::Parallel") || sa->isInstanceOf("sad::animations::Sequential"))
        {
            sad::animations::Composite* c = static_cast<sad::animations::Composite*>(sa);
            majorids = c->animationMajorIds();
        }
    }
    QListWidget* candidatelist = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeCandidates;
    int candidaterow = candidatelist->currentRow();
    candidatelist->clear();
    QListWidget* sourcelist = m_editor->uiBlocks()->uiAnimationBlock()->lstAnimations;
    for(size_t i = 0; i < sourcelist->count(); i++)
    {
        QString text = sourcelist->item(i)->text();
        QVariant v = sourcelist->item(i)->data(Qt::UserRole);
        candidatelist->addItem(text);
        candidatelist->item(candidatelist->count() - 1)->setData(Qt::UserRole,  v);
    }

    QListWidget* ownlist = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeList;
    int ownrow = ownlist->currentRow();
    ownlist->clear();
    for(size_t i = 0; i < majorids.count(); i++)
    {
        sad::animations::Animation* a = static_cast<sad::animations::Animation*>(sad::Renderer::ref()->database("")->queryByMajorId(majorids[i]));
        QString name = this->nameForAnimation(a);
        QVariant v;
        v.setValue(a);
        ownlist->addItem(name);
        ownlist->item(ownlist->count() - 1)->setData(Qt::UserRole, v);
    }

    void (QListWidget::*f)(int) = &QListWidget::setCurrentRow;

    if (candidaterow >= 0 && candidatelist->count() > 0)
    {
        if (candidaterow >= candidatelist->count())
        {
            candidaterow = candidatelist->count() - 1;
        }
        invoke_blocked(candidatelist, f, candidaterow);
    }

    if (ownrow >= 0 && ownlist->count() > 0)
    {
        if (ownrow >= ownlist->count())
        {
            ownrow = ownlist->count() - 1;
        }
        invoke_blocked(ownlist, f, ownrow);
    }
}

void gui::actions::AnimationActions::addAnimationToComposite() const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
        {
            QListWidget* candidatelist = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeCandidates;
            int pos = candidatelist->currentRow();
            if (pos > -1 && pos < candidatelist->count())
            {
                sad::animations::Animation* addedanimation = candidatelist->item(pos)->data(Qt::UserRole).value<sad::animations::Animation*>();
                sad::animations::Composite* co = static_cast<sad::animations::Composite*>(a);
                addAnimationToCompositeList(co, addedanimation, true);
            }
        }
    }
}

void gui::actions::AnimationActions::removeAnimationFromComposite() const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
        {
            QListWidget* list = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeList;
            int pos = list->currentRow();
            if (pos > -1 && pos < list->count())
            {				
                sad::animations::Animation* animation = list->item(pos)->data(Qt::UserRole).value<sad::animations::Animation*>();
                sad::animations::Composite* co = static_cast<sad::animations::Composite*>(a);
                removeAnimationFromCompositeList(co, animation, true, pos);
            }
        }
    }
}

void gui::actions::AnimationActions::moveBackInCompositeList() const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
        {
            QListWidget* list = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeList;
            int pos = list->currentRow();
            if (pos > -1 && pos < list->count() - 1)
            {
                sad::animations::Composite* co = static_cast<sad::animations::Composite*>(a);
                history::Command* c = new history::animations::SwapInComposite(co, pos, pos + 1);
                c->commit(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::moveFrontInCompositeList() const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
        {
            QListWidget* list = m_editor->uiBlocks()->uiAnimationBlock()->lstCompositeList;
            int pos = list->currentRow();
            if (pos > 0 && pos < list->count())
            {
                sad::animations::Composite* co = static_cast<sad::animations::Composite*>(a);
                history::Command* c = new history::animations::SwapInComposite(co, pos - 1, pos);
                c->commit(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::simpleMovementChangeStartingPointX(double new_x) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::SimpleMovement"))
        {
            sad::Point2D new_value(new_x, m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementStartingPointY->value());

            sad::Point2D old_value = a->getProperty< sad::Point2D >("start_point").value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
                    a, 
                    "start_point",
                    old_value, 
                    new_value,
                    m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementStartingPointX,
                    m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementStartingPointY
                );
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::simpleMovementChangeStartingPointY(double new_y) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::SimpleMovement"))
        {
            sad::Point2D new_value(m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementStartingPointX->value(), new_y);

            sad::Point2D old_value = a->getProperty< sad::Point2D >("start_point").value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
                    a, 
                    "start_point",
                    old_value, 
                    new_value,
                    m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementStartingPointX,
                    m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementStartingPointY
                );
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::simpleMovementChangeEndingPointX(double new_x) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::SimpleMovement"))
        {
            sad::Point2D new_value(new_x, m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementEndingPointY->value());

            sad::Point2D old_value = a->getProperty< sad::Point2D >("end_point").value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
                    a, 
                    "end_point",
                    old_value, 
                    new_value,
                    m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementEndingPointX,
                    m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementEndingPointY
                );
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::simpleMovementChangeEndingPointY(double new_y) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::SimpleMovement"))
        {
            sad::Point2D new_value(m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementEndingPointX->value(), new_y);

            sad::Point2D old_value = a->getProperty< sad::Point2D >("end_point").value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
                    a, 
                    "end_point",
                    old_value, 
                    new_value,
                    m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementEndingPointX,
                    m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementEndingPointY
                );
                c->commitWithoutUpdatingUI(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
}

void gui::actions::AnimationActions::startPickingStartingPointForSimpleMovement() const
{
    sad::hfsm::Machine* m =  this->m_editor->machine();
    if (this->m_editor->isInEditingState())
    {
        return;
    }
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::SimpleMovement"))
        {
            m_editor->shared()->setEditingSimpleMovementProperty("start_point");
            m->enterState("picking_simple_movement_point");
            m_editor->panelProxy()->lockTypesTab(true);
            m_editor->panelProxy()->highlightState("Please select starting point");
        }
    }
}

void gui::actions::AnimationActions::startPickingEndingPointForSimpleMovement() const
{
    sad::hfsm::Machine* m =  this->m_editor->machine();
    if (this->m_editor->isInEditingState())
    {
        return;
    }
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::SimpleMovement"))
        {
            m_editor->shared()->setEditingSimpleMovementProperty("end_point");
            m->enterState("picking_simple_movement_point");
            m_editor->panelProxy()->lockTypesTab(true);
            m_editor->panelProxy()->highlightState("Please select ending point");
        }
    }
}

void gui::actions::AnimationActions::cancelPickingPointForSimpleMovement() const
{
    this->m_editor->machine()->enterState(this->m_editor->machine()->previousState());
    m_editor->panelProxy()->lockTypesTab(false);
}

void gui::actions::AnimationActions::pickedPointForSimpleMovement(const sad::input::MousePressEvent& e) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        if (a->isInstanceOf("sad::animations::SimpleMovement"))
        {
            sad::Point2D new_value(e.pos2D());
            sad::String propertyName = this->m_editor->shared()->editingSimpleMovementProperty();
            QDoubleSpinBox* xwidget =  m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementEndingPointX;
            QDoubleSpinBox* ywidget =  m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementEndingPointY;
            if (propertyName != "end_point")
            {
                xwidget =  m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementStartingPointX;
                ywidget =  m_editor->uiBlocks()->uiAnimationBlock()->dabSimpleMovementStartingPointY;
            }
            sad::Point2D old_value = a->getProperty< sad::Point2D >(propertyName).value();
            if (sad::equal(old_value, new_value) == false)
            {
                history::Command* c = new history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
                    a, 
                    propertyName,
                    old_value, 
                    new_value,
                    xwidget,
                    ywidget
                );
                c->commit(m_editor);
                this->m_editor->history()->add(c);
            }
        }
    }
    sad::String state = this->m_editor->machine()->previousState();
    this->m_editor->machine()->enterState(state);
    this->m_editor->panelProxy()->lockTypesTab(false);
}

void gui::actions::AnimationActions::easingOvershootAmplitudeChanged(double new_value) const
{
     sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        double old_value = a->easing()->overshootAmplitude();
        if (sad::is_fuzzy_equal(old_value, new_value) == false)
        {
            history::Command* c = new history::animations::ChangeEasingOvershootAmplitude(a, old_value, new_value);
            c->commitWithoutUpdatingUI(this->m_editor);

            this->m_editor->history()->add(c);
        }
    }
}

void gui::actions::AnimationActions::easingPeriodChanged(double new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        double old_value = a->easing()->period();
        if (sad::is_fuzzy_equal(old_value, new_value) == false)
        {
            history::Command* c = new history::animations::ChangeEasingPeriod(a, old_value, new_value);
            c->commitWithoutUpdatingUI(this->m_editor);

            this->m_editor->history()->add(c);
        }
    }
}

void gui::actions::AnimationActions::easingFunctionTypeChanged(int new_value) const
{
    sad::animations::Animation* a = m_editor->shared()->selectedAnimation();
    if (a != nullptr)
    {
        unsigned int old_value = a->easing()->functionTypeAsUnsignedInt();
        unsigned int nv = new_value;
        if (old_value != nv)
        {
            history::Command* c = new history::animations::ChangeEasingFunctionType(a, old_value, nv);
            c->commitWithoutUpdatingUI(this->m_editor);

            this->m_editor->history()->add(c);
        }
    }	
}

