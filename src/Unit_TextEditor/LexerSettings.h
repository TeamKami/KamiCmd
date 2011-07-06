#ifndef LEXERSSDIALOG_H
#define LEXERSDIALOG_H

#include <QWidget>
#include "ui_LexerSettings.h"
#include <QThread>

class QsciLexer;

class LexerSettings : public QWidget
{
	Q_OBJECT

public:
	LexerSettings(QWidget *parent = 0);
	~LexerSettings();

protected:
	void showEvent(QShowEvent * event);

private:
	Ui::LexerSettings ui;
	
public slots:
	void save();
signals:
	void updateLexers();
};


#endif // LEXERSDIALOG_H
