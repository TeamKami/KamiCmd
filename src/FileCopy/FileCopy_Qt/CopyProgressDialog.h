#ifndef COPYPROGRESSDIALOG_H
#define COPYPROGRESSDIALOG_H

#include <QtGui/QDialog>
#include "FileCopy.h"
#include "ui_CopyProgressDialog.h"

class CopyThread;

class CopyProgressDialog : public QDialog
{
	Q_OBJECT

public:
	CopyProgressDialog(FileCopy *fileCopy, QWidget *parent = 0);
	~CopyProgressDialog();
	void show();

private slots:
	void update();
	void cancelCopy();
	void on_pauseResume_clicked();
	void on_cancelCopy_clicked();
	void HandleError( const QString filePath, QFile::FileError error, const QString errorText );

private:
	void updateSpeed();
	QString formatSize(qint64 size);

	Ui::CopyProgressDialog ui;
	FileCopy *fileCopy;
	IOperationsQueue *operationsQueue;
	QTimer refreshTimer;
	CopyThread *copyThread;
	int ticksPassed;
	static const int ticksMeasured = 10;
	qint64 bytesCopiedBetweenTicks[ticksMeasured];
	qint64 oldTotalCopied;
};

#endif // COPYPROGRESSDIALOG_H
