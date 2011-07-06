#ifndef LEXERSSDIALOG_H
#define LEXERSDIALOG_H

#include <QDialog>
#include "ui_SettingsDialog.h"
#include <map>

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

	void show();

private:
	Ui::SettingsDialog ui;
	
public slots:
	void onDialogButton(QAbstractButton * button);
	void onPageSelected(QTreeWidgetItem * item, int column);
signals:
	void saveSettings();
};

#endif // LEXERSDIALOG_H
