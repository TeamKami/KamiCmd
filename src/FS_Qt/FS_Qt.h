#ifndef FS_QT_H
#define FS_QT_H

#include "../IFileSystem.h"
#include "library.h"
#include <QtCore/QDir>

class FS_Qt : public QObject, public IFileSystem
{
	Q_OBJECT

public:
	FS_Qt(QObject *parent);
	virtual int SetPath(QString path);
	virtual QString GetPath();
	virtual uint GetNumberOfFiles();
	virtual int UpOneLevel();
	virtual bool isRoot();
	virtual QFile *GetFile(QString path);
	virtual bool GetFirstFileInfo(FileInfo &info);
	virtual bool GetNextFileInfo(FileInfo &info);

	void FillFileInfoFromQ(FileInfo &info, QFileInfo &qInfo);

private:
	QString path_;
	QDir dir_;
	QFileInfoList infoList_;
	int currentFile_;
	int dirCount_;
};

#endif // FS_QT_H
