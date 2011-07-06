#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include <QWidget>
#include "ui_EditorSettings.h"

class EditorSettings : public QWidget
{
	Q_OBJECT

public:
	EditorSettings(QWidget *parent = 0);
	~EditorSettings();

private:
	Ui::EditorSettings ui;
	
public slots:
	void save();
};


#endif // EDITORSETTINGS_H
