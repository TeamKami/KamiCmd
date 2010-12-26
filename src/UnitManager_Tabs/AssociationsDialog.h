#ifndef ASSOCIATIONSDIALOG_H
#define ASSOCIATIONSDIALOG_H

#include <QDialog>
#include "ui_AssociationsDialog.h"
#include "Associations.h"

class AssociationsDialog : public QDialog
{
	Q_OBJECT

public:
	AssociationsDialog(QWidget *parent = 0);
	void PopulateList(QVector<Association> *ascs, QVector<QAction *> *actions);
	~AssociationsDialog();

private:
	Ui::AssociationsDialogClass ui;
	QVector<Association> *Ascs;
	QVector<QAction *> *Actions;
	QIcon emptyIcon;

private slots:
	void EditAssc();
	void AddAssc();
	void DeleteAssc();
};

#endif // ASSOCIATIONSDIALOG_H
