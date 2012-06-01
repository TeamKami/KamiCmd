#include "FS_Qt.h"

int FS_Qt::SetPath( QString path )
{
#ifdef Q_WS_WIN
	for (int i = 0; i < path.size(); i++)
		if (path.at(i) == '\\')
			path[i] = '/';
#endif // Q_WS_WIN

	dir_.setPath(path);
	infoList_ = dir_.entryInfoList(QDir::Hidden | QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst);
	dirCount_ = infoList_.count();

	if (dir_.isReadable())
	{
		path_ = dir_.absolutePath();
		if (path_.size() && path_[path_.size() - 1] != '/')
			path_.append('/');
		return true;
	}
	return false;
}

QString FS_Qt::GetPath()
{
	return QDir::cleanPath(path_);
}

int FS_Qt::UpOneLevel()
{
	if (dir_.cdUp())
	{
		path_ = dir_.path();
		return true;
	}
	return false;
}

void FS_Qt::FillFileInfoFromQ(FileInfo &info, QFileInfo &qInfo)
{
	info.attributes = (qInfo.isDir() ? FileInfo::Directory : 0) |
		(qInfo.isHidden() ? FileInfo::Hidden : 0);

	info.size = qInfo.size();

	info.creationTime = qInfo.created();
	info.lastAccessTime = qInfo.lastRead();
	info.lastWriteTime = qInfo.lastModified();

	info.path = qInfo.absoluteFilePath();
	info.path.truncate(info.path.lastIndexOf('/') + 1);
	info.name = qInfo.fileName();
	info.alternateName = "";
}

bool FS_Qt::GetFirstFileInfo( FileInfo &info )
{
	currentFile_ = 0;
	if (currentFile_ < dirCount_)
	{
		FillFileInfoFromQ(info, infoList_[currentFile_]);
		return true;
	}
	return false;
}

bool FS_Qt::GetNextFileInfo( FileInfo &info )
{
	currentFile_++;

	if (currentFile_ < dirCount_)
	{
		FillFileInfoFromQ(info, infoList_[currentFile_]);
		return true;
	}
	return false;
}

bool FS_Qt::isRoot()
{
	QDir dir(path_);
	return dir.isRoot();
}

uint FS_Qt::GetNumberOfFiles()
{
	return dirCount_;
}

QFile* FS_Qt::GetFile( QString path )
{
	QFile *file = new QFile(path, this);
	if (file->open(QIODevice::ReadOnly))
		return file;
	return NULL;
}

FS_Qt::FS_Qt( QObject *parent )
: QObject(parent)
{
}