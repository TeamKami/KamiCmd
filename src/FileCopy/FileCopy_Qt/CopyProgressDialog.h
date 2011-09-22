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
	int exec();
private slots:
	void update();
	void reject();
private:
	void updateSpeed();
	QString formatSize(qint64 size);
	Ui::CopyProgressDialog ui;
	FileCopy *fileCopy;
	IOperationsQueue *operationsQueue;
	QTimer refreshTimer;
	CopyThread *copyThread;
	int ticksPassed;
	qint64 bytesCopiedBetweenTicks[10];
	qint64 oldTotalCopied;
};

#endif // COPYPROGRESSDIALOG_H
