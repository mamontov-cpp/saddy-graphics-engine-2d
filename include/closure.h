/*! \file closure.h
    

    Describes a closure-like thing in terms of C++
 */
#pragma once
#ifdef QT_VERSION
    #include <QMetaType>
#endif

namespace sad
{
/*! Basic class for all of closures. Need to put this stuff for template data
 */
class ClosureBasic
{
 public:
    /*! Executes a closure
     */
    virtual void run() = 0;
    /*! Destroys a closure
     */
    virtual ~ClosureBasic();
};

}


/*! Macro, that starts a closure definition
 */
#define       CLOSURE      {                                      \
                            class ____: public sad::ClosureBasic  \
                                {                                 \
                                 public:                          \
                                         ClosureBasic  * ___()    \
                                         {                        \
                                          return new ____(*this); \
                                         }
/*! Macro for declaration of closure data
 */
#define      CLOSURE_DATA(X)              X
/*! Macro for declaration of closure code
 */
#define      CLOSURE_CODE(X)              virtual void run()  { X }
/*! Macro, that constructs a closure and inits it with data
 */
#define      INITCLOSURE(X)          } ______; X;
/*! Submits a closure to an executor
 */
#define SUBMITCLOSURE( X )   X (______.___() ); } 
/*! Macro for initialization of closure value for data
 */
#define CLSET(PROP,VAL) ______. PROP = VAL;

#ifdef QT_VERSION
    Q_DECLARE_METATYPE(sad::ClosureBasic*); //-V566
#endif
