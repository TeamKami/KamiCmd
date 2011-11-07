#ifndef OQWIDGET_H
#define OQWIDGET_H

#include <QDialog>
#include "ui_OperationQueueDialog.h"

class IOperationsQueue;

class OperationsQueueDialog : public QDialog
{
	Q_OBJECT

public:
	OperationsQueueDialog(QWidget *parent = 0);
	~OperationsQueueDialog();

private slots:
	void on_addDummyButton_clicked();

private:
	Ui::OQWidget ui;
	IOperationsQueue *operationsQueue_;
};

#endif // OQWIDGET_H
