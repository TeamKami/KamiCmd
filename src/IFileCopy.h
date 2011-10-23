#ifndef FILE_COPY_INTERFACE
#define FILE_COPY_INTERFACE

#include <QtCore>

#include "IFileOperation.h"

class QString;
struct FileInfo;
class FilesToCopy;

class IFileCopy : public IFileOperation
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

	virtual ~IFileCopy() {}
	virtual void PrepareForCopy(const FilesToCopy & files) = 0;
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



class FilesToCopy
{
public:
	FilesToCopy();
	~FilesToCopy();

	void AddFile(const QString & relativePath, const FileInfo & file);
	void AddFiles(const QString & relativePath, const QVector<FileInfo> & files);
	const QString & GetDestination() const;
	void SetDestination(QString destination);
	int Count() const;
	const CopiedFile GetNextFile() const;
	qint64 GetTotalSize() const;

	void PrintAllFiles();

private:
	mutable QMap< QString, QVector<FileInfo> >::const_iterator it;
	mutable int currentFileNumber;
	int fileCount;
	QString destination;
	QMap <QString, QVector<FileInfo> > files; // key is relative path
	qint64 totalSize;
};

#endif
