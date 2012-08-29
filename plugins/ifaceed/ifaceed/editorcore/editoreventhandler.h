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
			   /** Editor's method
			    */
			   _Callback  m_method;
	 public:
			 /** Constructs new editor
				 \param[in] editor used editor
				 \param[in] cb method
			  */
			 inline Method(_Editor * editor,_Callback  cb)
			 {
				 m_editor = editor;
				 m_method = cb;
			 }
			 /*! Invokes a functor with event
			     \param[in] o event
			*/
			virtual void operator()(const _EventType & o)
			{
				_Callback cb = this->m_method;
				(m_editor->*cb)(o);
			}
			/*! We define this object as non-empty
			 */
			virtual bool empty() { return false;}
	};
};
