#ifndef _H_I_LEXER_PLUGIN_
#define _H_I_LEXER_PLUGIN_

#include <QObject>
#include <QString>
#include <QRegExp>
#include <vector>
#include <map>

class QsciLexer;

struct extension
{
	extension & operator | (QString const & e)
	{
		ext.push_back(e);
		return *this;
	}

	std::vector<QString> ext;
};

class ILexer
{
public:
	virtual QsciLexer * getLexer() const = 0;
	virtual QString const & getName() const = 0;
	virtual extension const & getMask() const = 0;
};

template <typename T>
class ILexerPlugin : public ILexer
{
public:
	ILexerPlugin(QString const & name, extension const & ext) :
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

	virtual extension const & getMask() const
	{
		return ext_;
	}

private:
	QString name_;
	extension ext_;

};


#endif //_H_I_LEXER_PLUGIN_



