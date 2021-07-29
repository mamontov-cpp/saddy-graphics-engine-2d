/*! \file keycodes.h
    

    Contains information for all key codes, used in system. To prevent global namespace pollution 
    with macro definitions, since rev 800, all key codes must be parts of corresponding events.
 */
#pragma once

// Those are not placed in input namespace, because we are avoiding long names
// sad::input::F1 is simply too long for user to enter, so trying to not force doing
// using namespace sad::input we allow writing something like sad::F1, sad::Esc, etc.
namespace sad
{
/*! All keyboard keys, handled by input are stored here
 */
enum KeyboardKey
{
    Numeric0 = '0',  //!< A numeric key code for 0 
    Numeric1 = '1',  //!< A numeric key code for 1
    Numeric2 = '2',  //!< A numeric key code for 2
    Numeric3 = '3',  //!< A numeric key code for 3 
    Numeric4 = '4',  //!< A numeric key code for 4 
    Numeric5 = '5',  //!< A numeric key code for 5 
    Numeric6 = '6',  //!< A numeric key code for 6 
    Numeric7 = '7',  //!< A numeric key code for 7 
    Numeric8 = '8',  //!< A numeric key code for 8 
    Numeric9 = '9',  //!< A numeric key code for 9 
    A = 'A',		  //!< A letter 'A'
    B = 'B',		  //!< A letter 'B'
    C = 'C',		  //!< A letter 'C'
    D = 'D',		  //!< A letter 'D'
    E = 'E',		  //!< A letter 'E'
    F = 'F',		  //!< A letter 'F'
    G = 'G',		  //!< A letter 'G'
    H = 'H',		  //!< A letter 'H'
    I = 'I',		  //!< A letter 'I'
    J = 'J',		  //!< A letter 'J'
    K = 'K',		  //!< A letter 'K'
    L = 'L',		  //!< A letter 'L'
    M = 'M',		  //!< A letter 'M'
    N = 'N',		  //!< A letter 'N'
    O = 'O',		  //!< A letter 'O'
    P = 'P',		  //!< A letter 'P'
    Q = 'Q',		  //!< A letter 'Q'
    R = 'R',	  	  //!< A letter 'R'
    S = 'S',		  //!< A letter 'S'
    T = 'T',		  //!< A letter 'T'
    U = 'U',		  //!< A letter 'U'
    V = 'V',		  //!< A letter 'V'
    W = 'W',		  //!< A letter 'W'
    X = 'X',		  //!< A letter 'X'
    Y = 'Y',		  //!< A letter 'Y'
    Z = 'Z',		  //!< A letter 'Z'
    /*! On english keyboard, this is a key, where minus and underscore are located
     */
    Minus = '-',	 
    /*! On english keyboard, this is a key, where equality sign and plus are located
     */
    Equal = '=',	  
    /*! On english keyboard, this is a key, where both slashes are located
     */
    BackSlash = '\\', 
    /*! On english keyboard, this is a key, where this is where tilda is located
     */
    Tilde = '~', 
    /*! A key, where opening square bracket is located 
     */
    OpeningSquareBracket = '[',
    /*! A key, where closing square bracket is located 
     */
    ClosingSquareBracket = ']',
    /*! A key, where semicolon is located
     */
    Semicolon = ';',
    /*! A key, where comma is located
     */
    Comma = ',',
    /*! A key, where period is located
     */
    Period = '.',
    /*! A key, where slash is located
     */
    Slash = '/',
    /*! A key, where apostrophe is located
     */
    Apostrophe = '\'',
    Esc          = 256,  //!< An Escape key
    F1           = 257,  //!< F1 key,
    F2		     = 258,  //!< F2 key
    F3		     = 259,  //!< F3 key
    F4		     = 260,  //!< F4 key
    F5	         = 261,  //!< F5 key
    F6		     = 262,  //!< F6 key
    F7		     = 263,  //!< F7 key
    F8	   	     = 264,  //!< F8 key
    F9           = 265,  //!< F9 key
    F10          = 266,  //!< F10 key
    F11          = 267,  //!< F11 key
    F12          = 268,  //!< F12 key
    Pause        = 269,  //!< A pause key
    Insert       = 270,  //!< An insert key
    Delete       = 271,  //!< A delete key
    Backspace    = 272,  //!< A backspace key
    Enter        = 273,  //!< Enter key
    Space        = 274,  //!< A space key
    PrintScreen  = 275,  //!< A print screen key
    Home         = 276,  //!< A home key
    End          = 277,  //!< An end key,
    PageUp       = 278,  //!< A page up key
    PageDown     = 279,  //!< A page down key
    Tab          = 280,  //!< A tab key
    NumLock      = 281,  //!< Num lock key
    ScrollLock   = 282,  //!< Scroll lock key
    CapsLock     = 283,  //!< Caps lock key
    WindowsKey   = 284,  //!< A windows key
    AppKey       = 285,  //!< A windows app key
    KeyLeft      = 286,  //!< Left key
    KeyRight     = 287,  //!< Right key
    KeyUp        = 288,  //!< Up key
    KeyDown      = 289,  //!< Down key
    LeftShift    = 290,  //!< A left shift key
    RightShift   = 291,  //!< A right shift key
    LeftControl  = 292,  //!< A left control key
    RightControl = 293,  //!< A right control key
    LeftAlt      = 294,  //!< A left alt key
    RightAlt     = 295,  //!< A right alt key
    KeyNone      = 296,  //!< None as no key pressed or invalid key
};


/*! Specified set of mouse manipulator buttons, passed into window events
 */
enum MouseButton
{
    MouseLeft   = -16535,   //!< Left mouse button
    MouseMiddle = -16534,   //!< Middle mouse button
    MouseRight  = -16533,   //!< Right mouse button
    MouseNone   = 0         //!< None as no button pressed or invalid button
};

/*! A special key codes, which is composed with other buttons, presses by user
 */
enum SpecialKey
{
    HoldsControl =  -16538,  //!< User holds control button
    HoldsAlt     =  -16537,  //!< User holds alt button
    HoldsShift   =  -16536,  //!< User holds shift button
};

class String;

/*! Checks, whether keyboard key is valid
    \param[in] key whether keyboard key is valid
 */
bool isValidKeyboardKey(int key);

/*! Returns default key if key is not valid. Can be useful, if key code is read from file
    \param[in] key key
    \param[in] d default keyboard key, which will be returned, if key is not valid
    \return keyboard key
 */
sad::KeyboardKey defaultKeyIfNotValid(sad::KeyboardKey key, sad::KeyboardKey d);

/*! Returns default key if key is not valid. Can be useful, if key code is read from file
    \param[in] key key
    \param[in] d default keyboard key, which will be returned, if key is not valid
    \return keyboard key
 */
int defaultKeyIfNotValidAsInt(int key, int d);

/*! Converts keyboard key to string
    \param[in] key keyboard key
    \return string string representation of key
 */
sad::String keyToString(sad::KeyboardKey key);


/*! Converts keyboard key to string
    \param[in] key keyboard key
    \return string string representation of key
 */
sad::String keyToStringAsInt(int key);

}
