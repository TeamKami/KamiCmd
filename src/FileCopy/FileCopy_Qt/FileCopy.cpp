#include "FileCopy.h"
#include "IFileSystem.h"
#include "library.h"

#include <QDir>
#include <QMessageBox>

FileCopy::FileCopy( QObject *parent /*= 0*/ ) 
    : IFileCopy(parent), state(Running), currentFileIndex(0), currentFileBytesCopied(0), bytesCopied(0)
{
	fileSystem = dynamic_cast<IFileSystem *>(g_Core->QueryModule("FS", 1));
	if(!fileSystem)
		g_Core->DebugWrite("FileCopy", "Can't query File system");

//	for(int i = 0; i < 15; i++)
//		errorHandling[i] = ErrorHandling::AskUser;
}

FileCopy::~FileCopy()
{

}

void FileCopy::PrepareForCopy( const IFilesToCopy & files )
{
	filesToCopy = &files;
//	if(!QFile::exists(files.GetDestination()))
//		destinationDirectory.mkpath(files.GetDestination());
	destinationDirectory.setCurrent(files.GetDestination());
}

bool FileCopy::Exec()
{

	for( ; currentFileIndex < filesToCopy->Count() && GetState() != Canceled; ++currentFileIndex)
	{
        const CopiedFile currentCopiedFile = filesToCopy->GetNextFile();
        if(!destinationDirectory.exists(currentCopiedFile.RelativePath()))
            destinationDirectory.mkpath(currentCopiedFile.RelativePath());
        QString path = filesToCopy->GetDestination() + currentCopiedFile.RelativePath();
        copyFile(currentCopiedFile.GetFile().path + currentCopiedFile.GetFile().name,
                 path + currentCopiedFile.GetFile().name);
	}	


	stateMutex.lock();
	if(state != Canceled)
		state = Finished;
	stateMutex.unlock();
	
	emit finished();

	return true;
}

void FileCopy::Pause()
{
	QMutexLocker locker(&stateMutex);
	if(state != Paused && state != Canceled)
	{
		state = Paused;
		pauseMutex.lock();
	}
}

void FileCopy::Resume()
{
	QMutexLocker locker(&stateMutex);
	if(state == Paused && (state != Canceled || state != Finished))
	{
		state = Running;
		g_Core->DebugWrite("FileCopy_Qt", "State changed to Running");
		pauseMutex.unlock();
	}
}

void FileCopy::Cancel()
{
	QMutexLocker locker(&stateMutex);
	if(state == Paused)
		pauseMutex.unlock();
	state = Canceled;
}

FileCopy::OperationState FileCopy::GetState() const
{
	return state;
}

QString FileCopy::GetType() const
{
	return tr("Copy");
}

int FileCopy::GetProgress() const
{
	qint64 size = filesToCopy->GetTotalSize();
	int percentage = !size ? 100 : bytesCopied / (size / 100.0);
	return percentage;
}

int FileCopy::GetCurrentFileProgress() const
{
	const CopiedFile *file = currentCopiedFile;
		
	int percentage = (!file || !file->GetFile().size) ? 100 : currentFileBytesCopied / (file->GetFile().size / 100.0);
	return percentage;
}

void FileCopy::copyFile( const QString & from, const QString & to )
{
	if(from == to)
		return;

	currentFileBytesCopied = 0;
	QFile sourceFile(from);
	if(!sourceFile.open(QFile::ReadOnly))
	{
		if(processFileError(sourceFile) == IFileCopy::Cancel)
			return;
	}

	const qint64 size = sourceFile.size();

	QFile destinationFile(to);
	if(!destinationFile.open(QFile::ReadWrite) || !destinationFile.resize(size))
	{
		if(processFileError(destinationFile) == IFileCopy::Cancel)
			return;
	}
	
	const uchar *source = sourceFile.map(0, size - 1);
	uchar *destination = destinationFile.map(0, size - 1);

	if(!source || !destination)
	{ 
		g_Core->DebugWrite("FileCopy_Qt", destinationFile.errorString());
		g_Core->DebugWrite("FileCopy_Qt", "Can't copy file " + sourceFile.fileName());
		destinationFile.remove();
		return;		
	}
	bool r = copyMemory(source, destination, 0, size);

	if(GetState() == Canceled && !r)
	{
		destinationFile.unmap(destination);
		destinationFile.close();
		destinationFile.remove();
	}
}

FileCopy::ErrorProcessingDesicion FileCopy::processFileError( const QFile & file )
{
	ErrorHandling handling = GetErrorHandling(file.error());
	switch(handling)
	{
	case AskUser:
		emit reportError(file.fileName(),file.error(), file.errorString());
		return Continue;

	case Ignore:
		return Continue;

	case Retry:
		return Retry;

	case IFileCopy::Cancel:
		return Stop;

     default:
        break;
	}
	return Continue;
}

const QString FileCopy::GetFileName() const
{
	const CopiedFile *file = currentCopiedFile;
	const QString fileName = file ? file->GetFile().name : QString();
	return fileName;
}

const QString & FileCopy::GetDestination() const
{
	return filesToCopy->GetDestination();
}

qint64 FileCopy::GetTotalSize() const
{
	return filesToCopy->GetTotalSize();
}

bool FileCopy::copyMemory( const uchar *src, uchar *dst, int offset, int size )
{
	currentFileBytesCopied = offset;
	static const int chunkSize = 4096;

	const int steps = (size - offset) / chunkSize;
	const int remainderBytes = (size - offset) % chunkSize;
	
	dst += offset;
	src += offset;

	for(int i = 0; i < steps; i++)
	{
		// workaround. since we dont have anything like WaitForSingleObject in Qt, we just try to lock pauseMutex, and if it's already been locked thread will sleep until it's unlocked
		if(state == Paused) 
		{
			pauseMutex.lock();
			pauseMutex.unlock();
		}
		
		if(state == Canceled)
			return false;
		
		memcpy(dst, src, chunkSize);

		dst += chunkSize;
		src += chunkSize;

		bytesCopied += chunkSize;
		currentFileBytesCopied += chunkSize;
	}

	memcpy(dst, src, remainderBytes);

	currentFileBytesCopied += remainderBytes;
	bytesCopied += remainderBytes;
	
	return true;
}

qint64 FileCopy::GetCurrentFileBytesCopied() const
{
	return currentFileBytesCopied;
}

qint64 FileCopy::GetTotalBytesCopied() const
{
	return bytesCopied;
}

const FileInfo * FileCopy::GetCurrentCopiedFile() const
{
	const CopiedFile *file = currentCopiedFile;
	if(file)
		return &file->GetFile();
	return NULL;
}

int FileCopy::GetCurentFileNumber() const
{
	return currentFileIndex + 1;
}

void FileCopy::SetErrorHandling( QFile::FileError error, ErrorHandling handling )
{
	errorHandling[error] = handling;
}

FileCopy::ErrorHandling FileCopy::GetErrorHandling( QFile::FileError error ) const
{
	return errorHandling[error];
}

void FileCopy::ShowProgressDialog(QWidget *parent)
{
    Q_UNUSED(parent)
}

