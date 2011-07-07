#ifndef ASSOCIATIONSDIALOG_H
#define ASSOCIATIONSDIALOG_H

#include <QMainWindow>
#include "ui_AssociationsDialog.h"
#include "Associations.h"

class AssociationsDialog : public QMainWindow
{
	Q_OBJECT

public:
	AssociationsDialog(QWidget *parent = 0);
	void PopulateList(QVector<Association> *ascs, QVector<QAction *> *actions);
	~AssociationsDialog() {};

protected:
	virtual void closeEvent(QCloseEvent *event);

private:
	Ui::AssociationsDialogClass ui;
	QVector<Association> *Ascs;
	QVector<QAction *> *Actions;
	QIcon emptyIcon;

private slots:
	void on_actionEdit_triggered();
	void on_actionAdd_triggered();
	void on_actionDelete_triggered();

signals:
	void Closing();
};

#endif // ASSOCIATIONSDIALOG_H
