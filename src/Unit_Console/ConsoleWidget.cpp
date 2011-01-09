#include "ConsoleWidget.h"
#include <QKeyEvent>
#include <QTextBlock>
#include <QTextLayout>

ConsoleWidget::ConsoleWidget(QWidget *parent)
	: QTextEdit(parent), commandPos(0)
{
/*
	QPalette::ColorRole fore = foregroundRole(),
	back = backgroundRole();
	setBackgroundRole(fore);
	setForegroundRole(back);
*/
 	QPalette pal = palette();
 	pal.setColor(QPalette::Text, QColor(236, 233, 216));
	pal.setColor(QPalette::Base, Qt::black);
// 	pal.setColor(QPalette::Light, Qt::black);
// 	pal.setColor(QPalette::Midlight, Qt::black);
// 	pal.setColor(QPalette::Dark, Qt::black);
// 	pal.setColor(QPalette::Mid, Qt::black);
	setPalette(pal);
}

ConsoleWidget::~ConsoleWidget()
{

}

void ConsoleWidget::keyPressEvent( QKeyEvent *event )
{
	switch (event->key())
	{
	case Qt::Key_Tab:
		event->ignore();
		return;

	case Qt::Key_Backspace:
	case Qt::Key_Left:
		if (event->key() == Qt::Key_Backspace)
			LimitCursorSelectionTo(commandPos);

		if (textCursor().position() <= commandPos)
		{
			SetCursorPos(commandPos, event->modifiers() & Qt::ShiftModifier);
			return;
		}
		break;

	case Qt::Key_Delete:
	case Qt::Key_Down:
	case Qt::Key_Right:
		if (event->key() == Qt::Key_Delete)
			LimitCursorSelectionTo(commandPos);

		if (textCursor().position() < commandPos)
		{
			SetCursorPos(commandPos, event->modifiers() & Qt::ShiftModifier);
			return;
		}
		break;

	case Qt::Key_Up:
	case Qt::Key_Home:
		{
			int line = textCursor().block().layout()->lineForTextPosition(textCursor().positionInBlock()).lineNumber();
			if (!line || (event->key() == Qt::Key_Home && event->modifiers() & Qt::ControlModifier))
			{
				SetCursorPos(commandPos, event->modifiers() & Qt::ShiftModifier);
				return;
			}
		}
		break;

// 	case Qt::Key_PageUp:
// 	case Qt::Key_PageDown:

	case Qt::Key_Return:
	case Qt::Key_Enter:
		{
			QString cmd = toPlainText().right(toPlainText().size() - commandPos);
			if (cmd.size())
				emit ExecuteCommand(cmd + '\n');
			else
				SetCursorPos(commandPos);
			return;
		}
		break;

	case 'C':
		if (event->modifiers() & Qt::ControlModifier)
		{
			emit ExecuteCommand("\x03\n");
			return;
		}
		break;
	}
	QTextEdit::keyPressEvent(event);
}

void ConsoleWidget::append( const QString &text )
{
	QString newText = text;

#ifdef Q_OS_WIN32
	if (commandPos)
		newText = text.right(text.size() - text.indexOf('\n'));

	if (newText.startsWith("\f\r\n")) // cls command
	{
		clear();
		newText = newText.right(newText.size() - 3);
	}
#endif
	QTextCursor cursor = textCursor();
	cursor.movePosition(QTextCursor::End);
	setTextCursor(cursor);

	setFontPointSize(11);
#ifdef Q_OS_WIN32
	setFontFamily("Consolas");
#else
	setFontFamily("Courier");
#endif	
	//setFontPointSize(13);

	QTextEdit::append(newText);
	commandPos = textCursor().position();
}

void ConsoleWidget::SetCursorPos( int pos, bool isKeepAnchor )
{
	QTextCursor cursor = textCursor();
	cursor.setPosition(pos, isKeepAnchor ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
	setTextCursor(cursor);
}

void ConsoleWidget::LimitCursorSelectionTo(int start)
{
	QTextCursor cursor = textCursor();
	int anchor = cursor.anchor(),
		position = cursor.position();
	if (anchor != position && (anchor < start || position < start))
	{
		cursor.setPosition(qMax(start, anchor), QTextCursor::MoveAnchor);
		cursor.setPosition(qMax(start, position), QTextCursor::KeepAnchor);
		setTextCursor(cursor);
	}
}