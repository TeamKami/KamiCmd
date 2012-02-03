#ifndef FILES_TO_COPY
#define FILES_TO_COPY

#include "IFileCopy.h"

class FilesToCopy : public IFilesToCopy
{

public:
	FilesToCopy();
	~FilesToCopy();

	virtual void AddFile( const QString & relativePath, const FileInfo & file );
	virtual void AddFiles( const QString & relativePath, const QVector<FileInfo> & files );

	virtual const CopiedFile* GetNextFile() const;

	virtual void SetDestination( QString destination );
	virtual const QString & GetDestination() const;

	virtual int Count() const;

	virtual qint64 GetTotalSize() const;

private:

	void DumpFiles();

	mutable QMap< QString, QVector<FileInfo> >::const_iterator it;
	mutable int currentFileNumber;
	int fileCount;
	QString destination;
	QMap <QString, QVector<FileInfo> > files; // key is relative path
	qint64 totalSize;
};


#endif // FILES_TO_COPY
