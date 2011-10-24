#ifndef COPYDIALOG_H
#define COPYDIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QTimer>

#include "FileCopy.h"
#include "ui_CopyDialog.h"

class CopyDialog : public QDialog
{
	Q_OBJECT

public:
	CopyDialog(QWidget *parent = 0);
	~CopyDialog();

private slots:
	void on_copyPushButton_clicked();
private:
	void InitializeCopy();
	void GetSelectedFilesAndCopyPath();
	void GetFilesToCopy();
	QVector< FileInfo > ScanFolder(const QString & path);

	struct FileScanElement
	{
		FileScanElement(const QString p, const FileInfo f)
			: relativePath(p), fileInfo(f)
		{}
		FileScanElement() {}

		QString relativePath;
		FileInfo fileInfo;
	};


	Ui::CopyDialog ui;
	FilesToCopy filesToCopy;
	FileCopy fileCopyOperation;
	QString copyPath;
	IFileSystem *fileSystem;
	QVector<FileInfo> selectedFiles;
};

#endif // COPYDIALOG_H
