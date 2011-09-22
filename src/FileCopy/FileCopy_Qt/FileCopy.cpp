#include "FileCopy.h"
#include "IFileSystem.h"

#include <QDir>
#include <QMessageBox>

FileCopy::FileCopy( QObject *parent /*= 0*/ ) 
	: QObject(parent), bytesCopied(0), state(Paused), currentFileIndex(0), currentCopiedFile(0), currentFileBytesCopied(0)
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
	if(!QFile::exists(files.GetDestination()))
		destinationDirectory.mkpath(files.GetDestination());
	destinationDirectory.setCurrent(files.GetDestination());
}

bool FileCopy::Exec()
{

	for( ; currentFileIndex < filesToCopy.Count(); ++currentFileIndex)
	{
		currentCopiedFileMutex.lock();
		currentCopiedFile = &filesToCopy.GetNextFile();
		currentCopiedFileMutex.unlock();

		if(!destinationDirectory.exists(currentCopiedFile->RelativePath()))
			destinationDirectory.mkpath(currentCopiedFile->RelativePath());
		QString path = filesToCopy.GetDestination() + currentCopiedFile->RelativePath();
		copyFile(currentCopiedFile->GetFile().path + currentCopiedFile->GetFile().name,
				 path + currentCopiedFile->GetFile().name);
	}
	
	currentCopiedFileMutex.lock();
	currentCopiedFile = NULL;
	currentCopiedFileMutex.unlock();

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
	int size = filesToCopy.GetTotalSize();
	if(!size)
		return 100;
	return bytesCopied / (size / 100);
}

int FileCopy::GetCurrentFileProgress() const
{
	currentCopiedFileMutex.lock();
	const CopiedFile *file = currentCopiedFile;
	currentCopiedFileMutex.unlock();
		
	int percentage = (!file || !file->GetFile().size) ? 100 : currentFileBytesCopied / (file->GetFile().size / 100);
	return percentage;
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
	
	const uchar *source = sourceFile.map(0, size - 1);
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
	currentCopiedFileMutex.lock();
	const QString fileName = currentCopiedFile ? currentCopiedFile->GetFile().name : QString();
	currentCopiedFileMutex.unlock();

	return fileName;
}

const QString & FileCopy::GetDestination() const
{
	return filesToCopy.GetDestination();
}

qint64 FileCopy::GetTotalSize() const
{
	return filesToCopy.GetTotalSize();
}

void FileCopy::copyMemory( const uchar *src, uchar *dst, int offset, int size )
{
	currentFileBytesCopied = offset;
	static const int chunkSize = 4096;

	const int steps = (size - offset) / chunkSize;
	const int remainderBytes = (size - offset) % chunkSize;
	
	dst += offset;
	src += offset;

	for(int i = 0; i < steps; i++)
	{
		memcpy(dst + i * chunkSize, src + i*chunkSize, chunkSize);
		
		bytesCopied += chunkSize;
		currentFileBytesCopied += chunkSize;
	}

	memcpy(dst + steps * chunkSize, src + steps * chunkSize, remainderBytes);
	currentFileBytesCopied += remainderBytes;
	bytesCopied += remainderBytes;
}

