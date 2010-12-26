#include "AssociationsDialog.h"
#include "AssociationsDialog_Edit.h"
#include <QToolBar>
#include <QMessageBox>

//#define DARK

AssociationsDialog::~AssociationsDialog()
{

}

AssociationsDialog::AssociationsDialog(QWidget *parent)
: QDialog(parent)
{
	QPixmap pix(16, 16);
	pix.fill(QColor(0, 0, 0, 0));
	emptyIcon.addPixmap(pix);

	ui.setupUi(this);
	QToolBar *toolBar = new QToolBar(this);
	toolBar->setAllowedAreas(Qt::TopToolBarArea);
	ui.verticalLayout->addWidget(toolBar);
	
	QAction *addAct = new QAction(tr("Ins Add"), this);
	addAct->setShortcut(Qt::Key_Insert);
	toolBar->addAction(addAct);
	toolBar->addSeparator();
	QAction *editAct = new QAction(tr("F4 Edit"), this);
	editAct->setShortcuts(QList<QKeySequence>() << Qt::Key_F4 << Qt::Key_Return << Qt::Key_Enter);
	toolBar->addAction(editAct);
	toolBar->addSeparator();
	QAction *deleteAct = new QAction(tr("F8 Delete"), this);
	deleteAct->setShortcuts(QList<QKeySequence>() << Qt::Key_F8 << Qt::Key_Delete);
	toolBar->addAction(deleteAct);

#ifdef DARK
	QPalette pal = ui.treeWidget->palette();
	pal.setColor(QPalette::Base, QColor(33, 38, 38));
	ui.treeWidget->setPalette(pal);
#endif

	connect(addAct, SIGNAL(triggered()), this, SLOT(AddAssc()));
	connect(editAct, SIGNAL(triggered()), this, SLOT(EditAssc()));
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(DeleteAssc()));
	connect(ui.treeWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditAssc()));
}

void AssociationsDialog::PopulateList( QVector<Association> *ascs, QVector<QAction *> *actions )
{
	Ascs = ascs;
	Actions = actions;
	ui.treeWidget->clear();

	for (int i = 0; i < ascs->size(); i++)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeWidget);
		item->setText(0, ascs->at(i).description);
		item->setText(1, ascs->at(i).mask);
		if (!ascs->at(i).icon.isNull())
			item->setIcon(0, ascs->at(i).icon);
		else
			item->setIcon(0, emptyIcon);
		if (ascs->at(i).backColor.isValid())
			for (int j = 0; j < ui.treeWidget->columnCount(); j++)
				item->setBackgroundColor(j, ascs->at(i).backColor);
		if (ascs->at(i).textColor.isValid())
			for (int j = 0; j < ui.treeWidget->columnCount(); j++)
				item->setTextColor(j, ascs->at(i).textColor);
	}
	if (!ascs->isEmpty())
		ui.treeWidget->setCurrentIndex(ui.treeWidget->model()->index(0, 0));
}

void AssociationsDialog::AddAssc()
{
	Association newAssc;
	for (int i = 0; i < Actions->size(); i++)
		newAssc.actions.append(QString());
	AssociationsDialog_Edit *dialog = new AssociationsDialog_Edit(this, &newAssc, Actions);
	if (dialog->exec() == QDialog::Accepted)
	{
		Ascs->append(newAssc);
		PopulateList(Ascs, Actions);
		ui.treeWidget->setCurrentIndex(ui.treeWidget->model()->index(ui.treeWidget->model()->rowCount() - 1, 0));
	}
}

void AssociationsDialog::EditAssc()
{
	if (!Ascs->isEmpty())
	{
		AssociationsDialog_Edit *dialog = new AssociationsDialog_Edit(this, &(*Ascs)[ui.treeWidget->currentIndex().row()], Actions);
		int current = ui.treeWidget->currentIndex().row();
		if (dialog->exec() == QDialog::Accepted)
		{
			PopulateList(Ascs, Actions);
			ui.treeWidget->setCurrentIndex(ui.treeWidget->model()->index(current, 0));
		}
	}
}

void AssociationsDialog::DeleteAssc()
{
	if (!Ascs->isEmpty())
	{
		int res = QMessageBox::question(this, tr("File Associations"), tr("Are you sure you want to delete \"\n%1\"?").arg(Ascs->at(ui.treeWidget->currentIndex().row()).description), QMessageBox::Yes, QMessageBox::No);
		if (res == QMessageBox::Yes)
		{
			Ascs->remove(ui.treeWidget->currentIndex().row());
			PopulateList(Ascs, Actions);
		}
	}
}