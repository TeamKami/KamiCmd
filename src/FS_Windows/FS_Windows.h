#ifndef FS_WINDOWS_H
#define FS_WINDOWS_H

#ifdef FS_WINDOWS_LIB
# define FS_WINDOWS_EXPORT Q_DECL_EXPORT
#else
# define FS_WINDOWS_EXPORT Q_DECL_IMPORT
#endif

#include "../IFileSystem.h"
#include "library.h"
#include "windows.h"
#pragma warning(disable:4996)

class FS_Windows : public QObject, public IFileSystem
{
	Q_OBJECT

	// Inherited
public:
	FS_Windows(QObject *parent);
	int SetPath(QString path);
	QString GetPath();
	uint GetNumberOfFiles();
	int UpOneLevel();
	bool isRoot();
	
	QFile *GetFile(QString path);
	bool GetFirstFileInfo(FileInfo &info);
	bool GetNextFileInfo(FileInfo &info);
	// Local

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
