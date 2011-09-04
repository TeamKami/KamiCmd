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
private:
	QString bytesToString(qint64 s);
	Ui::CopyProgressDialog ui;
	FileCopy *fileCopy;
	IOperationsQueue *operationsQueue;
	QTimer refreshTimer;
	CopyThread *copyThread;
};

#endif // COPYPROGRESSDIALOG_H
