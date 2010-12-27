#ifndef OQWIDGET_H
#define OQWIDGET_H

#include <QDialog>
#include "ui_oqwidget.h"

class IOperationsQueue;

class OQWidget : public QDialog
{
	Q_OBJECT

public:
	OQWidget(QWidget *parent = 0);
	~OQWidget();

private:
	Ui::OQWidget ui;
	IOperationsQueue *operationsQueue_;
};

#endif // OQWIDGET_H
