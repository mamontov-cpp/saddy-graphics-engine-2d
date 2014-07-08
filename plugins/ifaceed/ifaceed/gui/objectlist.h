/** \file objectlist.h
	\author HiddenSeeker
	
	Describes an GUI list of objects, preserving activity and all other stuff
 */
#include <QListWidget>
#include <vector>
class AbstractScreenObject;
class ScreenTemplate;
class BehaviourSharedData;
class EditorLog;

namespace gui
{

/*! Defines an object list, as presented on screen
 */
class ScreenObjectList
{
  private:
	  QListWidget * m_widget;
	  std::vector<AbstractScreenObject *> m_simple_list;
	  bool m_selfchanged; //! Whether it's screen object changed 
  public:
	  inline ScreenObjectList() { m_selfchanged = false; }
	  /** Sets a widget for list
	   */
	  inline void setWidget(QListWidget * widget) { m_widget = widget; }
	  /** Updates every row in widget
	   */
	  void updateWidget(ScreenTemplate * t, AbstractScreenObject * selectedObject);
	  /** Returns row by index
	   */
	  inline AbstractScreenObject * row(int index) {return m_simple_list[index];}
	  /** Whether self chaged
	   */
	  inline bool selfChanged() 
	  { 
		  bool oldc = m_selfchanged; 
		  m_selfchanged = false;
		  return oldc; 
	  } //!< Whether we updated from self
};

}