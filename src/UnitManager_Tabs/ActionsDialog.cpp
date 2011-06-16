#include "ActionsDialog.h"
#include <QMessageBox>

ActionsDialog::ActionsDialog(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog);
}

ActionsDialog::~ActionsDialog()
{

}

void ActionsDialog::show( QList<QAction *> actions )
{
	PassedActions = actions;
	for (int i = 0; i < actions.size();)
	{
		QString currentRoot = actions.at(i)->data().toString();
		QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui.treeWidget);
		rootItem->setText(0, currentRoot);

		for (; i < actions.size() && actions.at(i)->data().toString() == currentRoot; i++)
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(rootItem);
			item->setText(0, actions.at(i)->text());
			item->setText(1, actions.at(i)->shortcut().toString(QKeySequence::NativeText));
			item->setDisabled(!actions.at(i)->isEnabled());
			item->setIcon(0, actions.at(i)->icon());
			item->setData(0, Qt::UserRole, i);
		}
	}

	ui.treeWidget->resizeColumnToContents(0);
	ui.treeWidget->resizeColumnToContents(1);
	QMainWindow::show();
}

void ActionsDialog::on_actionEditShortcut_triggered()
{
	QMessageBox::information(this, "", "edit?");
}

void ActionsDialog::on_actionExecute_triggered()
{
	PassedActions.at(ui.treeWidget->currentItem()->data(0, Qt::UserRole).toInt())->trigger();
}
