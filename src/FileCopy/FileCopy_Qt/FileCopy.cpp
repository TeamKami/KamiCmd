#include "FileCopy.h"
#include "IFileSystem.h"

#include <QDir>
#include <QMessageBox>

FileCopy::FileCopy( QObject *parent /*= 0*/ ) 
	: QObject(parent), totalSize(0), bytesCopied(0), state(Paused), currentFileIndex(0), currentCopiedFile(0), currentFileBytesCopied(0)
{
	fileSystem = dynamic_cast<IFileSystem *>(g_Core->QueryModule("FS", 1));
	if(!fileSystem)
		g_Core->DebugWrite("FileCopy", "Can't query File system");
}

FileCopy::~FileCopy()
{

}

void FileCopy::PrepareForCopy( const FilesToCopy & files )
{
	filesToCopy = files;
	totalSize = filesToCopy.GetTotalSize();
}

bool FileCopy::Exec()
{
	FileInfo info;
	QDir dir(filesToCopy.GetDestination());
	for( ; currentFileIndex < filesToCopy.Count(); ++currentFileIndex)
	{
		currentCopiedFile = &filesToCopy.GetNextFile();
		if(!dir.exists(currentCopiedFile->RelativePath()))
			dir.mkpath(currentCopiedFile->RelativePath());
		copyFile(currentCopiedFile->GetFile().path + currentCopiedFile->GetFile().name,
				 filesToCopy.GetDestination() + currentCopiedFile->RelativePath() + currentCopiedFile->GetFile().name);
	}
	
	currentCopiedFile = NULL;

	QMutexLocker locker(&stateMutex);
	state = Finished;
	
	return true;
}

void FileCopy::Pause()
{
	QMutexLocker locker(&stateMutex);
	state = Paused;
}

void FileCopy::Resume()
{
	QMutexLocker locker(&stateMutex);
	if(state != Finished || state != Error)
	{
		state = Running;
		qDebug() << "State changed from " << state << "to Running";
	}
}

void FileCopy::Cancel()
{
	QMutexLocker locker(&stateMutex);
	state = Finished;
}

FileCopy::OperationState FileCopy::GetState() const
{
	QMutexLocker locker(const_cast<QMutex *>(&stateMutex));
	return state;
}

QString FileCopy::GetType() const
{
	return tr("Copy");
}

int FileCopy::GetProgress() const
{
	if(!totalSize)
		return 100;
	return bytesCopied / (totalSize / 100);
}

int FileCopy::GetCurrentFileProgress() const
{
	if(!currentCopiedFile)
		return 100;
	return currentFileBytesCopied / (currentCopiedFile->GetFile().size / 100);
}

void FileCopy::copyFile( const QString & from, const QString & to )
{
	currentFileBytesCopied = 0;
	QFile sourceFile(from);
	if(!sourceFile.open(QFile::ReadOnly))
	{
		QMessageBox::warning(NULL, tr("Error"), tr("File %1 can't be opened").arg(from));
		qDebug() << "Error" <<  "File"  << from  << "can't be opened";
		return;
	}

	const int size = sourceFile.size();

	QFile destinationFile(to);
	if(!destinationFile.open(QFile::ReadWrite) || !destinationFile.resize(size))
	{
		qDebug() << tr("Error") << destinationFile.errorString();
		return;
	}
	
	const uchar * const source = sourceFile.map(0, size - 1);
	uchar *destination = destinationFile.map(0, size - 1);
	if(!source || !destination)
	{ 
		qDebug() << "Error" << "Can't copy file " << sourceFile.fileName();
		return;		
	}

	for(int i = 0; i < size; i++)
	{
/*
		if(state == Paused || state == Error)
			return;
*/
		currentFileBytesCopied++;
		destination[i] = source[i];
		bytesCopied++;
	}
}

const QString FileCopy::GetFileName() const
{
	if(currentCopiedFile != NULL)
		return currentCopiedFile->GetFile().name;
	return QString();
}

const QString & FileCopy::GetDestination() const
{
	return filesToCopy.GetDestination();
}

int FileCopy::GetTotalSize() const
{
	return totalSize;
}

