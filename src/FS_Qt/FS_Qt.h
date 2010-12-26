#ifndef FS_QT_H
#define FS_QT_H

#ifdef FS_QT_LIB
# define FS_QT_EXPORT Q_DECL_EXPORT
#else
# define FS_QT_EXPORT Q_DECL_IMPORT
#endif

#include "../IFileSystem.h"
#include "library.h"
#include <QDir>

class FS_Qt : public QObject, public IFileSystem
{
	Q_OBJECT

	// Inherited
public:
	FS_Qt(QObject *parent);
	int SetPath(QString path);
	QString GetPath();
	uint GetNumberOfFiles();
	int UpOneLevel();
	bool isRoot();

	QFile *GetFile(QString path);
	bool GetFirstFileInfo(FileInfo &info);
	bool GetNextFileInfo(FileInfo &info);
	// Local

	void FillFileInfoFromQ(FileInfo &info, QFileInfo &qInfo);

private:
	QString path_;
	QDir dir_;
	QFileInfoList infoList_;
	int currentFile_;
	int dirCount_;
};

#endif // FS_QT_H
