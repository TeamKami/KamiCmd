#include "FileCopy.h"
#include "IFileSystem.h"
#include "ILibrary.h"

#include <QDir>
#include <QMessageBox>

FileCopy::FileCopy( QObject *parent /*= 0*/ ) 
	: QObject(parent), bytesCopied(0), state(Running), currentFileIndex(0), currentCopiedFile(0), currentFileBytesCopied(0)
{
	fileSystem = dynamic_cast<IFileSystem *>(g_Core->QueryModule("FS", 1));
	if(!fileSystem)
		g_Core->DebugWrite("FileCopy", "Can't query File system");

	for(int i = 0; i < 15; i++)
		errorHandling[i] = ErrorHandling::AskUser;
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

	for( ; currentFileIndex < filesToCopy.Count() && GetState() != Canceled; ++currentFileIndex)
	{
		currentCopiedFile = &filesToCopy.GetNextFile();
		if(!destinationDirectory.exists(currentCopiedFile->RelativePath()))
			destinationDirectory.mkpath(currentCopiedFile->RelativePath());
		QString path = filesToCopy.GetDestination() + currentCopiedFile->RelativePath();
		copyFile(currentCopiedFile->GetFile().path + currentCopiedFile->GetFile().name,
				 path + currentCopiedFile->GetFile().name);
	}
	
	currentCopiedFile = NULL;

	stateMutex.lock();
	if(state != Canceled)
		state = Finished;
	stateMutex.unlock();
	
	return true;
}

void FileCopy::Pause()
{
	QMutexLocker locker(&stateMutex);
	if(state != Paused && state != Error && state != Canceled)
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
		qDebug() << "State changed from " << state << "to Running";
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
	qint64 size = filesToCopy.GetTotalSize();
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
	currentFileBytesCopied = 0;
	QFile sourceFile(from);
	if(!sourceFile.open(QFile::ReadOnly))
	{
		processFileError(sourceFile);

		if(state == Canceled) // if processFileError() canceled copy
			return;
	}

	const int size = sourceFile.size();

	QFile destinationFile(to);
	if(!destinationFile.open(QFile::ReadWrite) || !destinationFile.resize(size))
	{
		processFileError(destinationFile);

		if(GetState() == Canceled) // if processFileError() canceled copy
			return;
	}
	
	const uchar *source = sourceFile.map(0, size - 1);
	uchar *destination = destinationFile.map(0, size - 1);

	if(!source || !destination)
	{ 
		qDebug() << "Error" << "Can't copy file " << sourceFile.fileName();
		return;		
	}
	bool r = copyMemory(source, destination, 0, size);
// 	
// 	destinationFile.unmap(destination);
// 	sourceFile.unmap(const_cast<uchar *>(source));
// 	destinationFile.close();
// 	sourceFile.close();

	if(GetState() == Canceled && !r)
		destinationFile.remove();
}

FileCopy::ErrorProcessingDesicion FileCopy::processFileError( const QFile & file )
{
	ErrorHandling handling = GetErrorHandling(file.error());
	switch(handling)
	{
	case ErrorHandling::AskUser:
		emit reportError(file.fileName(),file.error(), file.errorString());
		return ErrorProcessingDesicion::Continue;

	case ErrorHandling::Ignore:
		return ErrorProcessingDesicion::Continue;

	case ErrorHandling::Retry:
		return ErrorProcessingDesicion::Retry;

	case ErrorHandling::Cancel:
		return ErrorProcessingDesicion::Stop;

	}
}

const QString FileCopy::GetFileName() const
{
	const CopiedFile *file = currentCopiedFile;
	const QString fileName = file ? file->GetFile().name : QString();
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
		// workaround. since we dont have anything like WaitForSingleObject in Qt, we just try to lock pauseMutex, and it it's already been locked thread will sleep until it's unlocked
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

