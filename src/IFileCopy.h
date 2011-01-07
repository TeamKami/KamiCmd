#ifndef FILE_COPY_INTERFACE 
#define FILE_COPY_INTERFACE
#include <QtCore>

#include "IFileOperation.h"

class QString;
class FileInfo;

class IFileCopy : public IFileOperation
{
public:
	virtual bool PrepareForCopy(const QString & from, const QString & to) = 0;
	virtual bool PrepareForCopy(const QStringList & files, const QString & to, const QString & includeMask, const QString & excludeMask) = 0;
	virtual bool PrepareForCopy(const QList<FileInfo *> & files, const QString & to, const QString & includeMask, const QString & excludeMask) = 0;

};

#endif