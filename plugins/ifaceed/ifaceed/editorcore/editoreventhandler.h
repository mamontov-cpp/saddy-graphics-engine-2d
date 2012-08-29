/*! \file editoreventhandler.h
	\author HiddenSeeker

	Describes a saddy event handler for event
 */
#include <input.h>

/** A container, which describes, what type of editor, should we use
 */
template<typename _Editor>
class HandlerFor
{
 public:
	/** A class, which represents a callback
	 */
	template<typename _Callback,typename _EventType>
	class Method: public sad::BasicEventHandler<_EventType>
	{
	 protected:	
			   /** Parent editor
			    */
			   _Editor * m_editor;
	 public:
			 /** Constructs new editor
				 \param[in] editor used editor
			  */
			 inline Method(_Editor * editor)
			 {
				 m_editor = editor;
			 }
			 /*! Invokes a functor with event
			     \param[in] o event
			*/
			virtual void operator()(const _EventType & o)
			{
				(m_editor->*_Callback)(o);
			}
			/*! We define this object as non-empty
			 */
			virtual bool empty() { return false;}
	};
};
