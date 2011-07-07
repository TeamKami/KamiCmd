#ifndef LEXERSSDIALOG_H
#define LEXERSDIALOG_H

#include <QDialog>
#include "ui_SettingsDialog.h"
#include <map>

class QsciScintilla;
class SciSettings;

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	SettingsDialog(QWidget * parent = 0, SciSettings * set = 0);
	~SettingsDialog();

	void show();

private:
	Ui::SettingsDialog ui;
	bool created;
	SciSettings * settings;
	
public slots:
	void onDialogButton(QAbstractButton * button);
	void onPageSelected(QTreeWidgetItem * item, int column);
signals:
	void saveSettings();
	void settingsChanged();
};

#endif // LEXERSDIALOG_H
