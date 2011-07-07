#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include <QWidget>
#include "ui_EditorSettings.h"

class SciSettings;

class EditorSettings : public QWidget
{
	Q_OBJECT

public:
	EditorSettings(QWidget *parent = 0, SciSettings * sci = 0);
	~EditorSettings();

private:
	Ui::EditorSettings ui;
	SciSettings * sci_;
	
public slots:
	void save();
};


#endif // EDITORSETTINGS_H
