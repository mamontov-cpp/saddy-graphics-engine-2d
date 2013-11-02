/*! \file formattedlabel.h
	\author HiddenSeeker

	Describes a label, which uses callbacks and formatting
	for getting it's text up-to-date
 */
#pragma once
#include "label.h"

namespace sad
{

/*! A special label which can display formattedtext
	that changes dynamically in runtime, using callbacks
 */
class FormattedLabel: public sad::Label
{
SAD_OBJECT
#include "util/formattedlabelarg.h"
public:
	 /*! A default renderer is empty
	  */
	 FormattedLabel();
	 /*! Updates a label
		 \param[in] time a time interval, between two updates
	  */
	 inline void setUpdateInterval(double time) 
	 { 
		 m_update_interval = time; 
	 }
	 /*! Sets a format string for label
		 \param[in] s string
	  */
	 inline FormattedLabel * setFormatString(const sad::String & s ) 
	 { 
		 m_format_string = s; 
		 return this;
	 }
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
	 FormattedLabel * argFPS(sad::Renderer * r = NULL);
 protected:
	 /*! A formatted string, which contains current format
	  */
	 sad::String m_format_string;
	 /*! A substitution arguments
	  */
	 sad::Vector<FormattedLabel::Arg *> m_args;
	 /*! A timer for updating
	  */
	 sad::Timer m_timer;
	 /*! An update interval for label
	  */
	 double m_update_interval;
	 /*! Whether label is created now
	  */
	 bool m_created;
};

}
