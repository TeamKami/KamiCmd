#include "Copy.h"
#include "IFileSystem.h"

FileCopy::FileCopy( QObject *parent /*= 0*/ ) : QObject(parent)
{
	state_ = Paused;
	bytesToCopy = 0;
}

FileCopy::~FileCopy()
{

}

bool FileCopy::PrepareForCopy( const QString & from, const QString & to )
{
	from_.clear();
	from_ << from;
	to_ = to;
	return true;
}

bool FileCopy::PrepareForCopy( const QStringList & files, const QString & to, const QString & includeMask, const QString & excludeMask )
{
	from_ = files;
	to_ = to;
	includeMask_ = includeMask;
	excludeMask_ = excludeMask;
	
	return true;
}

bool FileCopy::PrepareForCopy( const QList<FileInfo *> & files, const QString & to, const QString & includeMask, const QString & excludeMask )
{
	return true;
}

bool FileCopy::Exec()
{

	return true;
}

bool FileCopy::Pause()
{
	return true;
}

bool FileCopy::Resume()
{
	return true;
}

bool FileCopy::Cancel()
{
	return true;
}

FileCopy::OperationState FileCopy::GetState() const
{
	return state_;
}

QString FileCopy::GetType() const
{
	return "Copying...";
}

int FileCopy::GetProgress() const
{
	return 50;
}


