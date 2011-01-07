#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QTextEdit>

class ConsoleWidget : public QTextEdit
{
	Q_OBJECT

public:
	ConsoleWidget(QWidget *parent);
	~ConsoleWidget();

	virtual void keyPressEvent(QKeyEvent *event);

public slots:
	virtual void append(const QString &text);

private:
	int commandPos;
};

#endif // CONSOLEWIDGET_H
