#ifndef MODULESDIALOG_H
#define MODULESDIALOG_H

#include <QDialog>
#include "ui_ModulesDialog.h"

class ModulesDialog : public QDialog
{
	Q_OBJECT

public:
	ModulesDialog(QWidget *parent = 0);

private:
	void populateTree();

	Ui::ModulesDialog ui;
	QIcon libraryIcon, moduleIcon;
};

#endif // MODULESDIALOG_H
