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

	void PrepareForCopy(const QString & source, const QString & to);
	void PrepareForCopy(const QStringList & files, const QString & destination);
	void PrepareForCopy(const QVector<FileInfo *> & files, const QString & destination);

	const QString & GetFileName() const;
	const QString & GetDestination() const;
	int GetTotalSize() const;
	bool Exec();
	void Pause();
	void Resume();
	void Cancel();
	OperationState GetState() const;
	QString GetType() const;
	int GetProgress() const;

private:
	void copyFile(const QString & from, const QString & to);
	void cleanUp();

	QMutex stateMutex;
	IFileSystem *fileSystem;
	OperationState state;
	QStringList files;
	QString to_;

	int currentFileIndex;
	quint64 bytesToCopy;
	quint64 totalSize;
	qint64 bytesCopied;
};

#endif // FILE_COPY
