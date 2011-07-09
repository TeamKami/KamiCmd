#ifndef _H_OBJECT_WRAPPER_
#define _H_OBJECT_WRAPPER_

#include <QObject>
#include "Unit_TextEditor/ILexerPlugin.h"
#include "Unit_TextEditor/Extension.h"

/**
 * \headerfile Lib_Lexers/ObjectWrapper.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Base class for wrapper. Needed because Q_OBJECT does not support templates.
 */
class Object: public QObject
{
Q_OBJECT
public:
    /**
     * \param parent Parent passed to QObject(QObject*)
     */
    Object(QObject * parent = 0) :
        QObject(parent)
    {
    }
};

/**
 * \headerfile ObjectWrapper.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Wrapper used to create ILexerPlugin<T> and pass it through ILibrary interface
 * \param T Lexer class
 */
template<typename T>
class LexerWrapper: public ILexerPlugin<T> , public Object
{
public:
    /**
     * \param parent Parent passed to Object(QObject*)
     * \param name Unique lexer name
     * \ext Wich files highlight with this lexer
     */
    LexerWrapper(QObject * parent = 0, QString const & name = "",
            Extension const & ext = Extension()) :
        ILexerPlugin<T> (name, ext), Object(parent)
    {
    }
};

#endif //_H_OBJECT_WRAPPER_
