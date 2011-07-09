#ifndef _H_I_LEXER_PLUGIN_
#define _H_I_LEXER_PLUGIN_

#include "Unit_TextEditor/Extension.h"

#include <QObject>
#include <QString>
#include <QRegExp>
#include <QVector>
#include <QSettings>

class QsciLexer;

/**
 * \headerfile Unit_TextEditor/ILexerPlugin.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Abstract lexer interface, all lexers must extend this.
 */
class ILexer
{
public:
    /**
     * \brief Get pointer to corresponding instance of QsciLexer. Any two calls of this method must return equal pointers.
     */
    virtual QsciLexer * getLexer() const = 0;
    /**
     * \brief Get unique lexer name.
     */
    virtual QString const & getName() const = 0;
    /**
     * \brief Get array of wildcards describing appropriate files
     */
    virtual QVector<QString> const & getMask() const = 0;
    /**
     * \brief Replace array of wildcards describing appropriate files
     */
    virtual void setMask(QVector<QString> const & mask) = 0;
    /**
     * \param fileName Base name of file.
     * \brief Check, if lexer matches given file
     */
    virtual bool match(QString const & fileName) const = 0;
};

/**
 * \headerfile Unit_TextEditor/ILexerPlugin.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Simple wrapper of QsciLexer
 * \param T Class derived of QsciLexer
 */
template<typename T>
class ILexerPlugin: public ILexer
{
public:
    /**
     * \param name Lexer name
     * \param ext Wildcards wrapped into Extension
     */
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

