#ifndef FILE_COPY_INTERFACE 
#define FILE_COPY_INTERFACE
#include <QtCore>
#include <QString>
#include "IFileOperation.h"

class IFileCopy : public IFileOperation
{
public:
	virtual bool copy(const QString & from, const QString & to) = 0;
	virtual bool copy(const QStringList & files, const QString & to, const QString & includeMask, const QString & excludeMask) = 0;
};

#endif