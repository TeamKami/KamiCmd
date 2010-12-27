#include "Copy.h"


FileCopy::FileCopy( QObject *parent /*= 0*/ ) : QObject(parent)
{
	state_ = Paused;
	bytesToCopy = 0;
}

FileCopy::~FileCopy()
{

}

bool FileCopy::copy( const QString & from, const QString & to )
{
	from_.clear();
	from_ << from;
	to_ = to;
}

bool FileCopy::copy( const QStringList & files, const QString & to, const QString & includeMask, const QString & excludeMask )
{
	from_ = files;
	to_ = to;
	includeMask_ = includeMask;
	excludeMask_ = excludeMask;
	
	return true;
}

bool FileCopy::exec()
{
	return true;
}

bool FileCopy::pause()
{
	return true;
}

bool FileCopy::resume()
{
	return true;
}

bool FileCopy::cancel()
{
	return true;
}

FileCopy::OperationState FileCopy::getState() const
{
	return state_;
}

QString FileCopy::getType() const
{
	return "Copying...";
}

int FileCopy::getProgress() const
{
	return 50;
}


