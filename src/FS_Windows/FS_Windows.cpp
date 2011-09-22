#include "FS_Windows.h"
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

WCHAR *FS_Windows::GetVeryLongPathName(QString path)
{
	WCHAR *ptr = (WCHAR *)path.constData();
	wcscpy(buf, ptr);
	PathAddBackslash(buf);
	GetLongPathName(buf, buf, 32767);
	return buf;
}

void ToNativeSeparators(WCHAR *str)
{
	for (; *str; str++)
		if (*str == '/')
			*str = '\\';
}

void ToUnitedSeparators(WCHAR *str)
{
	for (; *str; str++)
		if (*str == '\\')
			*str = '/';
}

int FS_Windows::SetPath( QString path )
{
	WCHAR *ptr = (WCHAR *)path.constData();
	wcscpy(buf, ptr);
	ToNativeSeparators(buf);

	if (!PathFileExists(buf))
		return false;

	PathAddBackslash(buf);
	GetLongPathName(buf, buf, 32767);
	path_ = QString::fromUtf16(buf);

	if (hFind_ != INVALID_HANDLE_VALUE)
		FindClose(hFind_);
	hFind_ = INVALID_HANDLE_VALUE;

	dirCount_ = 0;

	QString str(path_);
	if (str.at(str.length() - 1) != '\\')
		str.append("\\");

	str += "*";

	hFind_ = FindFirstFile((WCHAR *)str.constData(), &FindFileData_);
	if (hFind_ == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		if (wcscmp(FindFileData_.cFileName, L".") && wcscmp(FindFileData_.cFileName, L".."))
			dirCount_++;
	} while (FindNextFile(hFind_, &FindFileData_));
	FindClose(hFind_);
	hFind_ = INVALID_HANDLE_VALUE;
	return true;
}

QString FS_Windows::GetPath()
{
	WCHAR *ptr = (WCHAR *)path_.constData();
	wcscpy(buf, ptr);
	ToUnitedSeparators(buf);
	return QString::fromUtf16(buf);
}

int FS_Windows::UpOneLevel()
{
	if (isRoot())
		return false;

	WCHAR *path = (WCHAR *)path_.data(), *slash;
	PathRemoveBackslash(path);

	if (slash = wcsrchr(path, '\\'))
		*++slash = '\0';

	PathRemoveBackslash(path);
	path_.resize((int)wcslen(path));
	return true;
}

void FS_Windows::FileInfoFromFindData(FileInfo &info, WIN32_FIND_DATA &findData)
{
	info.attributes = 
		(findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ? FileInfo::Archive : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED ? FileInfo::Compressed : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? FileInfo::Directory : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED ? FileInfo::Encrypted : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ? FileInfo::Hidden : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL ? FileInfo::Normal : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE ? FileInfo::Offline : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY ? FileInfo::ReadOnly : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT ? FileInfo::ReparsePoint : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE ? FileInfo::SparseFile : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ? FileInfo::System : 0) |
		(findData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY ? FileInfo::Temporary : 0);

	info.size = ((quint64)findData.nFileSizeHigh << 0x20) | findData.nFileSizeLow;

	SYSTEMTIME sysTime;
	FileTimeToSystemTime(&findData.ftCreationTime, &sysTime);
	info.creationTime = QDateTime(QDate(sysTime.wYear, sysTime.wMonth, sysTime.wDay),
		QTime(sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds));
	FileTimeToSystemTime(&findData.ftLastAccessTime, &sysTime);
	info.lastAccessTime = QDateTime(QDate(sysTime.wYear, sysTime.wMonth, sysTime.wDay),
		QTime(sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds));
	FileTimeToSystemTime(&findData.ftLastWriteTime, &sysTime);
	info.lastWriteTime = QDateTime(QDate(sysTime.wYear, sysTime.wMonth, sysTime.wDay),
		QTime(sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds));

// 	WCHAR path[MAX_PATH+2];
// 	wcscpy(path, findData.cFileName);
// 	PathRemoveFileSpec(path);
// 	PathStripPath(findData.cFileName);
// 	PathRemoveBackslash(findData.cFileName);
// 	ToUnitedSeparators(path);
// 
// 	info.path = QString::fromUtf16(path);
	info.path = path_;
	for (int i = 0; i < info.path.size(); i++)
		if (info.path[i] == '\\')
			info.path[i] = '/';
	info.name = QString::fromUtf16(findData.cFileName);
	info.alternateName = QString::fromUtf16(findData.cAlternateFileName);
}

bool FS_Windows::GetFirstFileInfo( FileInfo &info )
{
	currentFile_ = 0;

	if (hFind_ != INVALID_HANDLE_VALUE)
		FindClose(hFind_);
	hFind_ = INVALID_HANDLE_VALUE;

	QString str(path_);
	if (str.at(str.length() - 1) != '\\')
		str.append("\\");
	str += "*";

	hFind_ = FindFirstFile((WCHAR *)str.constData(), &FindFileData_);
	if (hFind_ == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		if (wcscmp(FindFileData_.cFileName, L".") && wcscmp(FindFileData_.cFileName, L".."))
		{
			FileInfoFromFindData(info, FindFileData_);
			return true;
		}
	} while (FindNextFile(hFind_, &FindFileData_));
	return false;
}

bool FS_Windows::GetNextFileInfo( FileInfo &info )
{
	currentFile_++;

	if (currentFile_ < dirCount_ && FindNextFile(hFind_, &FindFileData_))
	{
		FileInfoFromFindData(info, FindFileData_);
		if (currentFile_ == dirCount_ - 1)
		{ // Necessary because of "currentFile_ < dirCount_". Without it - this would be done naturally via "FindNextFile returns false -> FindClose -> return false"
			FindClose(hFind_);
			hFind_ = INVALID_HANDLE_VALUE;
		}
		return true;
	}
	FindClose(hFind_);
	hFind_ = INVALID_HANDLE_VALUE;
	return false;
}

bool FS_Windows::isRoot()
// This function implements crutches for WinAPI function PathIsRoot() for which "\\hc\c$" IS root while "\\hc\c$\" is NOT
{
	WCHAR *str = (WCHAR *)path_.constData();
	int size = path_.size();
	bool isRestoreBackslash = false;
	bool isRoot = PathIsRoot(str);
	
	if (str[size - 1] == '\\')
	{
		str[size - 1] = '\0';
		isRestoreBackslash = true;
	}

	isRoot = isRoot ? isRoot : PathIsRoot(str);

	if (isRestoreBackslash)
		str[size - 1] = '\\';
	return isRoot;
}

uint FS_Windows::GetNumberOfFiles()
{
	return dirCount_;
}

QFile* FS_Windows::GetFile( QString path )
{
	QFile *file = new QFile(path, this);
	if (file->open(QIODevice::ReadOnly))
		return file;
	return NULL;
}

FS_Windows::FS_Windows( QObject *parent )
	: QObject(parent)
{
	hFind_ = INVALID_HANDLE_VALUE;
}