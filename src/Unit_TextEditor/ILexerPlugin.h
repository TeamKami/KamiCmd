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
	virtual QVector<QString> const & getMask() const = 0;
	virtual void setMask(QVector<QString> const & mask) = 0;
	virtual bool match(QString const & fileName) const = 0;
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

		setMask(ext_.ext);
	}

    ~ILexerPlugin()
	{
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

	virtual QVector<QString> const & getMask() const
	{
		return ext_.ext;
	}

	virtual void setMask(QVector<QString> const & m)
	{
		foreach(QString const & s, m)
		{
			mask.push_back(QRegExp(s, Qt::CaseInsensitive, QRegExp::Wildcard));
		}
	}

	virtual bool match(QString const & fileName) const
	{
		foreach(QRegExp const & wildcard, mask)
		{
			if (wildcard.exactMatch(fileName))
				return true;
		}

		return false;
	}

private:
	QString name_;
	Extension ext_;
	QVector<QRegExp> mask;
	T * lexer;
};

inline QVector<ILexer*> & lexers()
{
	static QVector<ILexer*> lexers_;
	return lexers_;
}

#endif //_H_I_LEXER_PLUGIN_



