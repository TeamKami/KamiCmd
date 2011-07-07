#ifndef UNIT_PICTUREVIEW_H
#define UNIT_PICTUREVIEW_H

#include <QtGui>
#include "../IUnitManager.h"
#include "../IPanel.h"
#include <QTextEdit>
#include <QScrollBar>
#include <map>

#include "LexersDialog.h"
#include "SettingsDialog.h"
class QsciLexer;

class QsciScintilla;
class Action;
class ActionManager;

/**
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief QScintilla based text editor unit with code highlighting/autocompleteion
 */
class Unit_TextEditor : public IUnit
{
	Q_OBJECT

public:
	Unit_TextEditor(QWidget *parent);
	~Unit_TextEditor();
	virtual void Create(IUnit *createdFrom);
	virtual void Link(IUnit * /*withUnit*/) {};
	virtual QString GetText();
	virtual QIcon GetIcon();

	virtual void SaveState(QSettings &set);
	virtual void LoadState(QSettings &set);

private:
	void loadModules();

	QString text;
	QString path;
	bool edited;
	QsciScintilla * editor;
	IPanel *hostPanel;
	LexersDialog * ld;
	SettingsDialog * sd;
	QList<QAction *> Actions;
	ActionManager *am;

signals:
	void TextChanged();
	void settingsChanged();

private slots:
	void onEdit();
	void selectLexer();
	void setLexer(QsciLexer * l);
	void settings();
	void updateSettings();

public slots:
	void LoadSettings();
	void LoadSettings(QSettings & set);
	void SaveSettings();
	
};

#endif // UNIT_PICTUREVIEW_H
