#ifndef _H_OBJECT_WRAPPER_
#define _H_OBJECT_WRAPPER_

#include <QObject>
#include "../Unit_TextEditor/ILexerPlugin.h"

class Object : public QObject
{
	Q_OBJECT
};

template <typename T>
class LexerWrapper : public ILexerPlugin<T>, public Object
{
public:
	LexerWrapper(QString const & name, extension const & ext) :
	  ILexerPlugin<T>(name, ext)
	{
	}
};

#endif //_H_OBJECT_WRAPPER_