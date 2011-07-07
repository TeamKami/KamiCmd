#ifndef SCISETTINGS_H
#define SCISETTINGS_H

#include <QObject>
#include <QVector>
#include <QSettings>
#include <QList>
#include "ui_EditorSettings.h"

class QsciScintilla;
class SettingsDialog;
class ICoreFunctions;

class SciSetting
{
public:
	virtual void apply(QsciScintilla * editor) = 0;
	virtual void load(QsciScintilla * editor) = 0;
	virtual void read(QSettings & set) = 0;
	virtual void write(QSettings & set) = 0;
	virtual void setUi(Ui::EditorSettings & ui) = 0;
	virtual void getUi(Ui::EditorSettings & ui) = 0;
};

class SciSettings : public QObject
{
	Q_OBJECT
public:
	SciSettings(QObject * parent = 0, ICoreFunctions *core = 0);
	~SciSettings();
	void applySettings(QsciScintilla * editor);
	void setUi(Ui::EditorSettings & ui);
	void getUi(Ui::EditorSettings & ui);
signals:
	void settingsChanged();
public slots:
	void showSettings();

private:
	void loadSettings();
	void saveSettings();
	QVector<SciSetting*> settings;
	SettingsDialog * sd;
	QList<QAction *> Actions;
	ICoreFunctions *g_Core;
private slots:
	void settingsUpdated();
};

#endif //SCISETTINGS_H