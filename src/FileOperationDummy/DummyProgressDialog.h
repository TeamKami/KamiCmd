#ifndef DUMMYPROGRESSDIALOG_H
#define DUMMYPROGRESSDIALOG_H

#include <QDialog>
#include <QtCore/QTimer>
#include "ui_dummyprogressdialog.h"

class IFileOperation;
class IOperationsQueue;
class FileOperationDummy;

class DummyProgressDialog : public QDialog
{
	Q_OBJECT

public:
	DummyProgressDialog(IFileOperation *fileOperation, QWidget *parent);
	~DummyProgressDialog();

	QWidget *ProgressBar();

private slots:
	void timeout();
	void on_pause_clicked();
	void on_cancel_clicked();

private:
	enum {UpdateInterval};
	Ui::DummyProgressDialog ui;
	IFileOperation *f;
	QTimer timer;
	IOperationsQueue *queue;
};

#endif // DUMMYPROGRESSDIALOG_H
