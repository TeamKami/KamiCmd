#include "FileCopy.h"
#include "IFileSystem.h"

#include <QDir>
#include <QMessageBox>

FileCopy::FileCopy( QObject *parent /*= 0*/ ) 
	: QObject(parent), totalSize(0), bytesCopied(0), state(Paused), currentFileIndex(0)
{
	fileSystem = dynamic_cast<IFileSystem *>(g_Core->QueryModule("FS", 1));
	if(!fileSystem)
		g_Core->DebugWrite("FileCopy", "Can't query File system");
}

FileCopy::~FileCopy()
{
//	delete stateMutex;
	1;
}

void FileCopy::PrepareForCopy( const QString & source, const QString & destination )
{
	files << source;
	to_ = destination;
}

void FileCopy::PrepareForCopy( const QStringList & files, const QString & destination)
{
	this->files = files;
	to_ = destination;
}

void FileCopy::PrepareForCopy( const QVector<FileInfo *> & files, const QString & destination)
{
	for(int i = 0; i < files.size(); i++)
	{
		this->files.append(files.at(i)->path + files.at(i)->name);
		totalSize += files.at(i)->size;
	}
	to_ = destination;
}

bool FileCopy::Exec()
{
	for( ; currentFileIndex < files.size(); currentFileIndex++)
		copyFile(files.at(currentFileIndex), to_ + files.at(currentFileIndex).mid(files.at(currentFileIndex).lastIndexOf('/') + 1));
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
	if(state == Paused)
		state = Running;
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


void FileCopy::cleanUp()
{

}


void FileCopy::copyFile( const QString & from, const QString & to )
{
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
	//	QMessageBox::warning(NULL, tr("Error"), destinationFile.errorString());
		qDebug() << tr("Error") << destinationFile.errorString();
		return;
	}
	
	const uchar * const source = sourceFile.map(0, size - 1);
	uchar *destination = destinationFile.map(0, size - 1);
	if(!source || !destination)
	{ 
//		QMessageBox::warning(NULL, tr("Error"), tr("Can't copy file %1").arg(sourceFile.fileName()));
		qDebug() << "Error" << "Can't copy file " << sourceFile.fileName();
		return;		
	}

	for(int i = 0; i < size; i++)
	{
		destination[i] = source[i];
		bytesCopied++;
	}
}

const QString & FileCopy::GetFileName() const
{
	int i = ( currentFileIndex >= files.size() ) ? files.size() - 1 : currentFileIndex;
	return files.at(i);
}

const QString & FileCopy::GetDestination() const
{
	return to_;
}

int FileCopy::GetTotalSize() const
{
	return totalSize;
}


