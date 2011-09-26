#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>
#include "ui_DebugDialog.h"

class DebugDialog : public QDialog
{
	Q_OBJECT

public:
	DebugDialog(QWidget *parent = 0);
	~DebugDialog();
public slots:
	void outputMessage(QtMsgType type, const QString & msg);
private slots:
	void on_saveButton_clicked();
private:
	Ui::DebugDialog ui;
};

#endif // DEBUGDIALOG_H
