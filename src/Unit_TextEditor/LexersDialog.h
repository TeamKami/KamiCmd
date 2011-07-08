#ifndef LEXERSSDIALOG_H
#define LEXERSDIALOG_H

#include <QMainWindow>
#include "ui_LexersDialog.h"
#include <QVector>

class QsciLexer;
class ILexer;

class LexersDialog : public QMainWindow
{
	Q_OBJECT

public:
	LexersDialog(QWidget *parent = 0);
	~LexersDialog();

	void show(QVector<ILexer*> const & lexers, QsciLexer * lexer);

private:
	Ui::LexersDialog ui;

public slots:
	void on_actionSelect_triggered();
signals:
	void setLexer(QsciLexer * l);
};

#endif // LEXERSDIALOG_H
