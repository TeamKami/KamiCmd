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

	int GetTotalSize() const;
	int GetCurrentFileProgress() const;
	OperationState GetState() const;
	QString GetType() const;
	int GetProgress() const;

private:
	void copyFile(const QString & from, const QString & to);

	QMutex stateMutex;
	IFileSystem *fileSystem;
	
	OperationState state;
	FilesToCopy filesToCopy;
	const CopiedFile *currentCopiedFile;
	int currentFileIndex;
	int currentFileBytesCopied;
	quint64 bytesToCopy;
	quint64 totalSize;
	qint64 bytesCopied;
};

#endif // FILE_COPY
