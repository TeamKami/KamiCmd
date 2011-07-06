#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include <QWidget>
#include "ui_EditorSettings.h"

class QsciScintilla;

class EditorSettings : public QWidget
{
	Q_OBJECT

public:
	EditorSettings(QWidget *parent = 0, QsciScintilla * sci = 0);
	~EditorSettings();

private:
	Ui::EditorSettings ui;
	QsciScintilla * sci_;
	
public slots:
	void save();
};


#endif // EDITORSETTINGS_H
