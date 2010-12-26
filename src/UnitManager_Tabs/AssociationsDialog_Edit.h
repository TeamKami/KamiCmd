#ifndef ASSOCIATIONSDIALOG_EDIT_H
#define ASSOCIATIONSDIALOG_EDIT_H

#include <QDialog>
#include "ui_AssociationsDialog_Edit.h"
#include "Associations.h"

class AssociationsDialog_Edit : public QDialog
{
	Q_OBJECT

public:
	AssociationsDialog_Edit(QWidget *parent, Association *assc, QVector<QAction *> *actions);
	~AssociationsDialog_Edit();

private:
	Ui::AssociationsDialog_EditClass ui;
	Association *Assc;
	QVector<QAction *> *Actions;
	void SyncButtonsColors();

public slots:
	virtual void accept();
	virtual void reject();

private slots:
	void on_iconButton_clicked();
	void on_backColorButton_clicked();
	void on_backColorEdit_textChanged(const QString &);
	void on_textColorEdit_textChanged(const QString &);
	void on_textColorButton_clicked();
	void on_iconEdit_textChanged(const QString &);
};

#endif // ASSOCIATIONSDIALOG_EDIT_H
