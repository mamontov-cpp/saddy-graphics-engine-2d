/*! \file   label.h
    \author HiddenSeeker

	Contains a definition of simple label node, that can be added to scene
*/
#include "scene.h"
#include "fontmanager.h"
#include "input.h"
#include "renderer.h"
#include "3rdparty/format/format.h"
#include "os/timer.h"
#pragma once

typedef sad::BasicFont *  LabelFont;
/*! \class Label
    Class of simple label on scene
*/
class Label: public sad::BasicNode, public sad::ResizeEventHandler
{
 SAD_OBJECT
 private:
	    LabelFont  m_fnt;        //!<  Associated font
		hst::string      m_str;        //!<  String
		pointf           m_p;          //!<  point
		pointf           m_rend_point; //!<  Current rendering point
		sad::Renderer *  m_renderer;  //!< Input data
 public:
	    void operator()(const sad::ResizeEvent & o);
		/*! Returns a font reference
		    \return font 
		*/
		inline LabelFont & font();
		/*! Returns a string
		    \return string
		*/
		inline hst::string &  string();
		/*! Returns a point position
		    \return m_p
		*/
		inline pointf &  point();
		/*! Returns a rendering point position
		    \return m_rend_point
		*/
		inline pointf &  render_point();
		/*! Creates a label
			\param[in] renderer associated renderer
		*/
		Label(sad::Renderer * renderer = sad::Renderer::ref());
		/*! Creates a simple node
		    \param[in] fnt  font object
			\param[in] str  rendered string
			\param[in] p    point
			\param[in] renderer associated renderer
		*/
		Label(
		      LabelFont  fnt,
		      const hst::string & str,
			  const pointf      & p,
			  sad::Renderer * renderer = sad::Renderer::ref()
			 );
        /*! Renders it
		*/
		virtual void render();
		/*! Destructs it
		*/
		virtual ~Label();
};


/*! A special label which can display formatted, text, that changes dynamically in runtime
 */
class FormattedLabel: public Label
{
 SAD_OBJECT
 public:
	 class Arg
	 {
		public:
			/*! Returns substuted arg in notions of format library
				\return substitituted argument
			 */
			virtual fmt::BasicFormatter<char>::Arg * substitute() = 0;
			virtual ~Arg();
	 };
	 template<
		typename _Value
	 >
	 class StaticArg: public FormattedLabel::Arg
	 {
		private:
			_Value m_arg; //!< Argument for substitution
		public:
			StaticArg(const _Value & arg) : m_arg(arg) { }
			virtual fmt::BasicFormatter<char>::Arg * substitute() 
			{ 
				return new fmt::BasicFormatter<char>::Arg(m_arg);
			}
	 };
	 template<
		 typename _Value
	 >
	 class ZeroFunctionArg: public FormattedLabel::Arg
	 {
		 private:
			_Value (*m_f)(); 
		public:
			ZeroFunctionArg(_Value  (*f)()) : m_f(f) { }
			virtual fmt::BasicFormatter<char>::Arg * substitute() 
			{ 
				return new fmt::BasicFormatter<char>::Arg( m_f() );
			}
	 };
	 template<
		typename _Class,
		typename _Value
	 >
	 class ZeroMethodArg: public FormattedLabel::Arg
	 {
 		 private:
			_Class * m_o;
			_Value (_Class::*m_f)(); 
		public:
			ZeroMethodArg(_Class * o, _Value (_Class::*f)()) : m_o(o), m_f(f) { }
			virtual fmt::BasicFormatter<char>::Arg * substitute() 
			{ 
				return new fmt::BasicFormatter<char>::Arg( (m_o->*m_f)() );
			}
	 };
	 template<
		typename _Class,
		typename _Value
	 >
	 class ZeroConstMethodArg: public FormattedLabel::Arg
	 {
 		 private:
			_Class * m_o;
			_Value (_Class::*m_f)() const; 
		public:
			ZeroConstMethodArg(_Class * o, _Value (_Class::*f)() const) : m_o(o), m_f(f) { }
			virtual fmt::BasicFormatter<char>::Arg * substitute() 
			{ 
				return new fmt::BasicFormatter<char>::Arg( (m_o->*m_f)() );
			}
	 };
	 template<
		typename _RetValue,
		typename _Class,
		typename _Value
	 >
	 class ZeroCastedConstMethodArg: public FormattedLabel::Arg
	 {
 		 private:
			_Class * m_o;
			_Value (_Class::*m_f)() const; 
		public:
			ZeroCastedConstMethodArg(_Class * o, _Value (_Class::*f)() const) : m_o(o), m_f(f) { }
			virtual fmt::BasicFormatter<char>::Arg * substitute() 
			{ 
				return new fmt::BasicFormatter<char>::Arg( (_RetValue)((m_o->*m_f)()) );
			}
	 };
	 template<
		typename _Class1,
		typename _Class2,
		typename _Value
	 >
	 class ZeroMethodsCompositionArg: public FormattedLabel::Arg
	 {
 		 private:
			_Class1 * m_o;
			_Class2 * (_Class1::*m_f)();
			_Value (_Class2::*m_g)();

		public:
			ZeroMethodsCompositionArg(_Class1 * o, 
				                      _Class2 * (_Class1::*f)(), 
									  _Value (_Class2::*g)() ) : m_o(o), m_f(f), m_g(g) 
			{ 
			}
			virtual fmt::BasicFormatter<char>::Arg * substitute() 
			{ 
				_Class2 * p = (m_o->*m_f)();
				return new fmt::BasicFormatter<char>::Arg( (p->*m_g)() );
			}
	 };
	 template<
		typename _Class1,
		typename _Class2,
		typename _Class3,
		typename _Value
	 >
	 class ZeroCompatibleCompositionArg3C: public FormattedLabel::Arg
	 {
 		 private:
			_Class1 * m_o;
			_Class2 * (_Class1::*m_f)();
			_Value (_Class3::*m_g)() const;

		public:
			ZeroCompatibleCompositionArg3C(_Class1 * o, 
				                        _Class2 * (_Class1::*f)(), 
									    _Value (_Class3::*g)() const ) : m_o(o), m_f(f), m_g(g) 
			{ 
			}
			virtual fmt::BasicFormatter<char>::Arg * substitute() 
			{ 
				_Class2 * p = (m_o->*m_f)();
				_Class3 * k = static_cast<_Class3*>(p);
				return new fmt::BasicFormatter<char>::Arg( (k->*m_g)() );
			}
	 };
 protected:
	 /*! A formatted string, which contains current format
	  */
	 hst::string m_format_string;
	 /*! A substitution arguments
	  */
	 sad::vector<FormattedLabel::Arg *> m_args;
	 /*! A timer for updating
	  */
	 os::timer m_timer;
	 /*! An update interval for label
	  */
	 double m_update_interval;
	 /*! Whether label is created now
	  */
	 bool m_created;
 public:
	 /*! Sets a renderer for a label
		 \param[in] renderer a renderer
	  */
	 FormattedLabel(sad::Renderer * renderer = sad::Renderer::ref());
	 /*! Sets a font for rendering
		 \param[in] fnt font
	  */
	 inline void setFont(LabelFont fnt) { this->font() = fnt;}
	 /*! Sets an upper-left point for rendering
		 \param[in] p point
	  */
	 inline void setPoint(const pointf & p) { this->point() = p; this->render_point() = p; }
	 /*! Sets an upper-left point for rendering
		 \param[in] x x coordinate
		 \param[in] y y coordinate
	  */
	 inline void setPoint(double x, double y) { setPoint(pointf(x, y)); }
	 /*! Updates a label
		 \param[in] time a time interval, between two updates
	  */
	 inline void setUpdateInterval(double time) { m_update_interval = time; }
	 /*! Sets a format string for label
		 \param[in] s string
	  */
	 inline FormattedLabel * setFormatString(const hst::string & s ) 
	 { 
		 m_format_string = s; 
		 return this;
	 }
	 /*! Sets a font with specified name
		 \param[in] fnt font
		 \param[in] r renderer
	  */
	 void setFont(const hst::string & fnt, sad::Renderer * r = sad::Renderer::ref());
	 /*! Updates a label
	  */
	 virtual void update();
	 /*! Renders a label
	  */
	 virtual void render();
	 /*! A formatted label for data
	  */
	 virtual ~FormattedLabel();

	 /*! Adds an argument to label
		 \param[in] a argument
		 \return this
	  */
	 inline FormattedLabel * addArg(FormattedLabel::Arg * a) 
	 { 
		 m_args << a; 
		 return this;
	 }
	 /*! Adds an argument to label
		 \param[in] f a function
		 \return this
	  */
	 template<
		typename _Value
	 >
	 inline FormattedLabel * arg(_Value (*f)())
	 {
		 return this->addArg(new FormattedLabel::ZeroFunctionArg<_Value>(f));
	 }
	 /*! Adds an argument to label
		 \param[in] v a value
		 \return this
	  */
	 template<
		typename _Value
	 >
	 inline FormattedLabel * arg(const _Value & v)
	 {
		 return this->addArg(new FormattedLabel::StaticArg<_Value>(v));
	 }
	 /*! Adds an argument to label
		 \param[in] o object
		 \param[in] f a function
		 \return this
	  */
	 template<
		typename _Class,
		typename _Value
	 >
	 inline FormattedLabel * arg(_Class * o, _Value (_Class::*f)())
	 {
		 return this->addArg(new FormattedLabel::ZeroMethodArg<_Class,_Value>(o, f));
	 }
	  /*! Adds an argument to label
		 \param[in] o object
		 \param[in] f a function
		 \return this
	  */
	 template<
		typename _Class,
		typename _Value
	 >
	 inline FormattedLabel * arg(_Class * o, _Value (_Class::*f)() const)
	 {
		 return this->addArg(new FormattedLabel::ZeroConstMethodArg<_Class,_Value>(o, f));
	 }
	 /*! Adds an argument to label
		 \param[in] o object
		 \param[in] f a first composed method
		 \param[in] g a second composed method
		 \return this
	  */
	 template<
		typename _Class1,
		typename _Class2,
		typename _Value
	 >
	 inline FormattedLabel * arg(_Class1 * o, 
				                 _Class2 * (_Class1::*f)(), 
								 _Value (_Class2::*g)()
								 )
	 {
		 return this->addArg(
			 new FormattedLabel::ZeroMethodsCompositionArg<_Class1,_Class2, _Value>(o, f, g)
		 );
	 }
	 /*! Adds an argument to label
		 \param[in] o object
		 \param[in] f a function
		 \param[in] g a second composed method
		 \return this
	  */
	 template<
		typename _Class1,
		typename _Class2,
		typename _Class3,
		typename _Value
	 >
	 inline FormattedLabel * arg(_Class1 * o, 
				                 _Class2 * (_Class1::*f)(), 
								 _Value (_Class3::*g)() const
								 )
	 {
		 return this->addArg(
			 new FormattedLabel::ZeroCompatibleCompositionArg3C<_Class1,_Class2, _Class3, _Value>(o, f, g)
		 );
	 }
	 /*! Adds an argument to label
		 \param[in] o object
		 \param[in] f a function
		 \return this
	  */
	 template<
		typename _Retval,
		typename _Class,
		typename _Value
	 >
	 inline FormattedLabel * castedConstArg(_Class * o, _Value (_Class::*f)() const)
	 {
		 return this->addArg(new FormattedLabel::ZeroCastedConstMethodArg<_Retval,_Class,_Value>(o, f));
	 }
	 /*! Adds fps argument, taken from specified renderer
		 \param[in] r renderer
	  */
	 FormattedLabel * argFPS(sad::Renderer * r = sad::Renderer::ref());
};


//======================================Source code=====================================
LabelFont & Label::font()   { return m_fnt;  }
hst::string &     Label::string() { return m_str;  }
pointf &          Label::point()    { return m_p; }
pointf &		  Label::render_point() { return m_rend_point;}
