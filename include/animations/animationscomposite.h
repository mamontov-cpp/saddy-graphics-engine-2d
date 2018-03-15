/*! \file animationscomposite.h
    

    Defines a an animation, which consists of several animations, inserted into one
 */
#pragma  once
#include "animationsanimation.h"

#include "../sadptrvector.h"
#include "../sadpair.h"
#include "../sadhash.h"
#include "../treedblink.h"
#include "../maybe.h"


namespace sad
{
    
namespace animations
{
    
/*! Defines a composite animation, which consists from several separated
    animations
 */
class Composite: public sad::animations::Animation
{
SAD_OBJECT	
public:
    /*! Constructs new empty composite animation
     */
    Composite();
    /*! Copies other composite animation
        \param[in] a animation
     */
    Composite(const sad::animations::Composite& a);
    /*! Copies other composite animation
        \param[in] a animation
        \return *this
     */
    sad::animations::Composite& operator=(const sad::animations::Composite& a);
    /*! Destroys animation
     */
    ~Composite();
    /*! Called, when animation is started loading from database
        \param[in] t table
     */
    virtual void setTable(sad::db::Table* t);
    /*! Sets tree from links, from physical file
        \param[in] file a file data
     */
    virtual void setPhysicalFile(sad::resource::ResourceFile * file);
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! This function will be applied, when animation is loaded from database.	
        \param[in] v value
        \return whether loading was successfull
     */
    virtual bool load(const picojson::value & v);
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
    /*! Starts an animation. Called on start of instance.
        Calls start on each of animation
        \param[in] i animation instance
     */
    virtual void start(sad::animations::Instance* i);
    /*! Checks, whether animation is applicable to an object. It's applicable if it's applicable
        to every animation.
        \param[in] o object
        \return whether animation is applicable to that object
     */
    virtual bool applicableTo(sad::db::Object* o);
    /*! Returns creators for saving object states
        \return creators list
     */
    virtual const sad::Vector<sad::animations::AbstractSavedObjectStateCreator*>& creators() const;
    /*! Adds new animation to list by major id
        \param[in] majorid a major id of animation
     */
    void add(unsigned long long majorid);
    /*! Inserts new animations to list by major id
        \param[in] majorid a major id
        \param[in] pos position
     */
    void insert(unsigned long long majorid, int pos);
    /*! Swaps two positions with links
        \param[in] pos1 first position
        \param[in] pos2 second position
     */
    void swap(int pos1, int pos2);
    /*! Adds new animation to list
        \param[in] o animation
     */
    void add(sad::animations::Animation* o);
    /*! Inserts new animations to list 
        \param[in] o animation
        \param[in] pos position
     */
    void insert(sad::animations::Animation* o, int pos);
    /*! Removes an animation from specified position
        \param[in] i an index
     */
    void remove(size_t i);
    /*! Returns animation
        \param[in] i an index
     */
    sad::animations::Animation* animation(size_t i) const;
    /*! Returns count of animations in list
        \return count of animations in list
     */
    size_t size() const;
    /*! Clears animations
     */
    void clear();
    /*! Sets animation names
        \param[in] names a names, used for animations
     */
    void setAnimationsNames(const sad::Vector<sad::String>& names);
    /*! Returns animation names
        \return animation names
     */
    sad::Vector<sad::String> animationNames() const;
    /*! Sets animation major ids
        \param[in] ids a major ids
     */
    void setAnimationsMajorId(const sad::Vector<unsigned long long>& ids);
    /*! Returns animation major ids
        \return animation major ids
     */
    sad::Vector<unsigned long long> animationMajorIds() const;
    /*! Returns NULL, because animation manages other stuff by itself
        \param[in] o object
        \return state command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* stateCommand(sad::db::Object* o);
protected:
    /*! Commands list
     */
    typedef sad::Vector<sad::animations::setstate::AbstractSetStateCommand*> Commands;
    /*! Copies other composite animation
        \param[in] a animation
     */
    void copy(const sad::animations::Composite& a);
    /*! Commands list
     */
    sad::Hash<sad::db::Object*, Commands> m_commands;
    /*! A set of links in animations
     */
    sad::PtrVector<sad::TreeDbLink<sad::animations::Animation> > m_links;
    /*! A database, which animation could be linked into
     */
    sad::Maybe<sad::db::Database*> m_database;
    /*! A possible link to tree
     */ 
    sad::Maybe<sad::resource::Tree*> m_tree;
    /*! A non-owning creators 
     */
    sad::Vector<sad::animations::AbstractSavedObjectStateCreator*> m_temp_creators;
};

}

}
