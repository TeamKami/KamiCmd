#ifndef _H_I_LEXER_PLUGIN_
#define _H_I_LEXER_PLUGIN_

#include <Unit_TextEditor/Extension.h>

#include <QObject>
#include <QString>
#include <QRegExp>
#include <QVector>
#include <map>

class QsciLexer;

class ILexer
{
public:
	virtual QsciLexer * getLexer() const = 0;
	virtual QString const & getName() const = 0;
	virtual Extension const & getMask() const = 0;
};

template <typename T>
class ILexerPlugin : public ILexer
{
public:
	ILexerPlugin(QString const & name, Extension const & ext) :
	  name_(name), ext_(ext)
	{
	}

	virtual QsciLexer * getLexer() const
	{
		return new T;
	}

	virtual QString const & getName() const
	{
		return name_;
	}

	virtual Extension const & getMask() const
	{
		return ext_;
	}

private:
	QString name_;
	Extension ext_;

};


#endif //_H_I_LEXER_PLUGIN_



