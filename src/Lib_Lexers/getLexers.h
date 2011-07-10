#ifndef GETLEXERS_H
#define GETLEXERS_H

#include <QVector>
class ILexer;

/**
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \headerfile Lib_Lexers/getLexers.h
 * \brief Singleton for lexers list
 * \param parent Pointer to parent for Lexers. Used at first call only.
 */
QVector<ILexer *> & getLexers(QObject * parent = 0);

#endif //GETLEXERS_H
