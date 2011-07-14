#ifndef ACTIONSDIALOG_H
#define ACTIONSDIALOG_H

#include <QMainWindow>
#include "ui_ActionsDialog.h"

class ActionsDialog : public QMainWindow
{
	Q_OBJECT

public:
	ActionsDialog(QWidget *parent = 0);
	~ActionsDialog();

	void show(QList<QAction *> actions);

private:
	Ui::ActionsDialog ui;
	QList<QAction *> PassedActions;

public slots:
	void on_actionEditShortcut_triggered();
	void on_actionExecute_triggered();
	void on_treeWidget_itemExpanded(QTreeWidgetItem *item);
};

#endif // ACTIONSDIALOG_H
