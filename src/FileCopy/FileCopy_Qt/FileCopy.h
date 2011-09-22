#ifndef FILE_COPY_H
#define FILE_COPY_H

#include <QtCore/QMutex>
#include <QtCore/QList>
#include <QtGui/QDialog>

#include "IFileCopy.h"
#include "IFileSystem.h"
#include "library.h"

class FileCopy : public QObject, public IFileCopy
{
public:
	FileCopy(QObject *parent = 0);
	~FileCopy();

	void PrepareForCopy(const FilesToCopy & files);

	const QString GetFileName() const;
	const QString & GetDestination() const;

	bool Exec();
	void Pause();
	void Resume();
	void Cancel();

	qint64 GetTotalSize() const;
	int GetCurrentFileProgress() const;
	const FileInfo *GetCurrentCopiedFile();
	qint64 GetCurrentFileBytesCopied() const;
	qint64 GetTotalBytesCopied() const;
	OperationState GetState() const;
	QString GetType() const;
	int GetProgress() const;

private:
	void copyFile(const QString & from, const QString & to);
	void copyMemory(const uchar *src, uchar *dst, int offset, int size);

	QMutex stateMutex;
	OperationState state;

	mutable QMutex currentCopiedFileMutex;
	const CopiedFile *currentCopiedFile;
	
	IFileSystem *fileSystem;
	QDir destinationDirectory;
	
	FilesToCopy filesToCopy;
	int currentFileIndex;
	int currentFileBytesCopied;
	qint64 bytesCopied;
};

#endif // FILE_COPY
