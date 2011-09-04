#ifndef FILE_COPY_INTERFACE 
#define FILE_COPY_INTERFACE
#include <QtCore>

#include "IFileOperation.h"

class QString;
struct FileInfo;

class IFileCopy : public IFileOperation
{
public:
	virtual ~IFileCopy() {};
	virtual void PrepareForCopy(const QString & source, const QString & to) = 0;
	virtual void PrepareForCopy(const QStringList & files, const QString & destination) = 0;
	virtual void PrepareForCopy(const QVector<FileInfo *> & files, const QString & destination) = 0;
};

#endif