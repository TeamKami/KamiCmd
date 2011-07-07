#ifndef EXTENSION_H
#define EXTENSION_H

#include <QVector>

/**
 * \headerfile Unit_TextEditor/Extension.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Accumulator for QStrings. Used to collect wildcards for lexers.
 */
struct Extension
{
	/**
	 * \param e QString to be added
	 */
	Extension & operator | (QString const & e)
	{
		ext.push_back(e);
		return *this;
	}

	QVector<QString> ext;
};

#endif //EXTENSION_H