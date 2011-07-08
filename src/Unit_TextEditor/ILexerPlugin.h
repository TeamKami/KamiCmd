#ifndef _H_I_LEXER_PLUGIN_
#define _H_I_LEXER_PLUGIN_

#include <Unit_TextEditor/Extension.h>

#include <QObject>
#include <QString>
#include <QRegExp>
#include <QVector>
#include <QSettings>

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
	  name_(name), ext_(ext), lexer(new T)
	{
		QSettings set;
		set.setIniCodec("UTF-8");
		set.beginGroup("ILexerPlugin");
		lexer->readSettings(set);
		set.endGroup();
	}

    ~ILexerPlugin()
	{
		/*QSettings set;
		set.setIniCodec("UTF-8");
		set.beginGroup("ILexerPlugin");
		lexer->writeSettings(set);
		set.endGroup();*/
		delete lexer;
	}

	virtual QsciLexer * getLexer() const
	{
		return lexer;
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
	T * lexer;
};


#endif //_H_I_LEXER_PLUGIN_



