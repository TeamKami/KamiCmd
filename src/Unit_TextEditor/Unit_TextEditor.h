#ifndef UNIT_PICTUREVIEW_H
#define UNIT_PICTUREVIEW_H

#include <QtGui>
#include "../IUnitManager.h"
#include "../IPanel.h"
#include <QTextEdit>
#include <QScrollBar>
#include <map>

class QsciLexer;

class QsciScintilla;

/**
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief QScintilla based text editor unit with code highlighting/autocompleteion
 */
class Unit_TextEditor : public IUnit
{
	Q_OBJECT

public:
	Unit_TextEditor(QWidget *parent);
	virtual void Create(IUnit *createdFrom);
	virtual void Link(IUnit * /*withUnit*/) {};
	virtual QString GetText();
	virtual QIcon GetIcon();

	virtual void SaveState(QSettings &set);
	virtual void LoadState(QSettings &set);

private:
	QString text;
	QString path;

	QsciScintilla * editor;
	IPanel *hostPanel;

signals:
	void TextChanged();

public slots:
};

#endif // UNIT_PICTUREVIEW_H
