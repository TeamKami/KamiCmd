#ifndef COPYWIDGET_H
#define COPYWIDGET_H

#include <QDialog>
#include "ui_CopyWidget.h"

struct FileInfo;
class CopyWidget : public QDialog
{
	Q_OBJECT

public:
	CopyWidget(QWidget *parent = 0);
	~CopyWidget();

private:
	Ui::CopyWidget ui;
	QVector<const FileInfo *> files;

private slots:
	void on_copyPushButtonClicked();
};

#endif // COPYWIDGET_H
