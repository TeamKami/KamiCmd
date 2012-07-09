#ifndef FILE_COPY_INTERFACE
#define FILE_COPY_INTERFACE

#include <QtCore>

#include "IFileOperation.h"

class QString;
struct FileInfo;
class IFilesToCopy;

class IFileCopy : public QObject, public IFileOperation
{
public:

	enum ErrorHandling // behavior on error
	{
		Ignore,
		Cancel,
		Retry,
		Wait,
		AskUser
	};
	IFileCopy(QObject *parent = 0) : QObject(parent) {}
	virtual ~IFileCopy() {}
	virtual void PrepareForCopy(const IFilesToCopy & files) = 0;
};

class CopiedFile
{
public:
	CopiedFile(const QString * relativePath, const FileInfo * file);
	CopiedFile();
	~CopiedFile();
	const QString & RelativePath() const;
	const FileInfo & GetFile() const;
private:
	const QString *relativePath;
	const FileInfo *file;
};



class IFilesToCopy
{
public:

	virtual ~IFilesToCopy() {};

	virtual void AddFile(const QString & relativePath, const FileInfo & file) = 0;
	virtual void AddFiles(const QString & relativePath, const QVector<FileInfo> & files) = 0;

	virtual const QString & GetDestination() const = 0;
	virtual void SetDestination(QString destination) = 0;

	virtual int Count() const = 0;

	virtual const CopiedFile *GetNextFile() const = 0;

	virtual qint64 GetTotalSize() const = 0;

};

#endif
