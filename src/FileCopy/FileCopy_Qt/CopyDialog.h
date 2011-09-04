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
	QVector< FileInfo *> ScanFolder(const QString & path);
	Ui::CopyDialog ui;
	FileCopy fileCopyOperation;
	IFileSystem *fileSystem;
	QVector<FileInfo> selectedFiles;
};

#endif // COPYDIALOG_H
