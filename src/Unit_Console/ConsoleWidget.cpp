#include "ConsoleWidget.h"
#include <QKeyEvent>

ConsoleWidget::ConsoleWidget(QWidget *parent)
	: QTextEdit(parent), commandPos(0)
{

}

ConsoleWidget::~ConsoleWidget()
{

}

void ConsoleWidget::keyPressEvent( QKeyEvent *event )
{
	switch (event->key())
	{
	case Qt::Key_Left:
	case Qt::Key_Right:
	case Qt::Key_Up:
	case Qt::Key_Down:
		if (textCursor().position() < commandPos)
		{
			textCursor().setPosition(commandPos);
			return;
		}
		break;
	}
	QTextEdit::keyPressEvent(event);
}

void ConsoleWidget::append( const QString &text )
{
	textCursor().setPosition(1000);
	commandPos = textCursor().position();
	QTextEdit::append(text);
}
