#ifndef FS_WINDOWS_H
#define FS_WINDOWS_H

#include "../IFileSystem.h"
#include "library.h"
#include "windows.h"
#pragma warning(disable:4996)

class FS_Windows : public QObject, public IFileSystem
{
	Q_OBJECT

public:
	FS_Windows(QObject *parent);
	virtual int SetPath(QString path);
	virtual QString GetPath();
	virtual uint GetNumberOfFiles();
	virtual int UpOneLevel();
	virtual bool isRoot();
	virtual QFile *GetFile(QString path);
	virtual bool GetFirstFileInfo(FileInfo &info);
	virtual bool GetNextFileInfo(FileInfo &info);

	WCHAR *GetVeryLongPathName(QString path);
	void FileInfoFromFindData(FileInfo &info, WIN32_FIND_DATA &findData);

private:
	QString path_;
	int currentFile_;
	int dirCount_;

	HANDLE hFind_;
	WIN32_FIND_DATA FindFileData_;
	WCHAR buf[32768];
};

#endif // FS_WINDOWS_H
