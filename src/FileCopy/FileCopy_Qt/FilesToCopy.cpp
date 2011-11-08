#include "FilesToCopy.h"

#include "IFileSystem.h"


CopiedFile::CopiedFile( const QString * relativePath, const FileInfo * file )
	: relativePath(relativePath), file(file)
{

}

CopiedFile::CopiedFile() : relativePath(0), file(0)
{

}

CopiedFile::~CopiedFile()
{

}

const QString & CopiedFile::RelativePath() const
{
	return *relativePath;
}

const FileInfo & CopiedFile::GetFile() const
{
	return *file;
}

FilesToCopy::FilesToCopy()
	: fileCount(0), currentFileNumber(-1), totalSize(0)
{

}

FilesToCopy::~FilesToCopy()
{

}

void FilesToCopy::AddFile( const QString & relativePath, const FileInfo & file )
{
	totalSize += file.size;
	if(files.contains(relativePath))
		files[relativePath].append(file);
	else
	{
		QVector<FileInfo> t;
		t.append(file);
		files.insert(relativePath, t);
	}
	
	++fileCount;
}

void FilesToCopy::AddFiles(const QString & relativePath, const QVector<FileInfo> & files)
{
	foreach(const FileInfo & file, files)
		AddFile(relativePath, file);
}

int FilesToCopy::Count() const
{
	return fileCount;
}

const CopiedFile FilesToCopy::GetNextFile() const
{
	if(currentFileNumber == -1)
	{
		it = files.begin();
		currentFileNumber = 0;
	}

	const QVector<FileInfo> & v = it.value();
	if(currentFileNumber >= v.size())
	{
		++it;
		currentFileNumber = 0;
	}

	return CopiedFile(&it.key(), &v.at(currentFileNumber++));;
}

void FilesToCopy::SetDestination( QString destination )
{
	this->destination = destination;
}

const QString & FilesToCopy::GetDestination() const
{
	return destination;
}

qint64 FilesToCopy::GetTotalSize() const
{
	return totalSize;
}

void FilesToCopy::DumpFiles()
{
	QMap< QString, QVector<FileInfo> >::const_iterator i = files.begin();
	for( ; i != files.end() ;  ++i )
		foreach(const FileInfo & info, i.value())
		{
			qDebug() << i.key() << " " << info.name;
		}
}
