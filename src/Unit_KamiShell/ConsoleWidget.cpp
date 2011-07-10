#include "ConsoleWidget.h"
#include <QKeyEvent>
#include <QTextBlock>
#include <QTextLayout>

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QTextEdit(parent), commandPos(0), readOnly(true)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Text, QColor(236, 233, 216));
    pal.setColor(QPalette::Base, Qt::black);
// 	pal.setColor(QPalette::Light, Qt::black);
// 	pal.setColor(QPalette::Midlight, Qt::black);
// 	pal.setColor(QPalette::Dark, Qt::black);
// 	pal.setColor(QPalette::Mid, Qt::black);
    setPalette(pal);
    shortWelcome = ">";
}

ConsoleWidget::~ConsoleWidget()
{

}

void ConsoleWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_C && event->modifiers().testFlag(Qt::ControlModifier))
    {
        emit terminate();
    }

    if (readOnly)
    {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {
            emit keyPress("\n");
        }
        else
        {
            emit keyPress(event->text());
        }

        QTextEdit::keyPressEvent(event);

        return;
    }

    switch (event->key())
    {
    case Qt::Key_Tab:
        case Qt::Key_Backtab:
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
        int line = textCursor().block().layout()->lineForTextPosition(
                textCursor().positionInBlock()).lineNumber();
        if (!line
                || (event->key() == Qt::Key_Home
                        && event->modifiers() & Qt::ControlModifier))
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
            if (toPlainText()[toPlainText().size() - 1] == '\\')
            {
                QTextEdit::append(shortWelcome);
                QTextCursor cursor = textCursor();
                cursor.movePosition(QTextCursor::End);
                setTextCursor(cursor);
                return;
            }

            QString cmd = toPlainText().right(toPlainText().size() - commandPos);
            cmd.replace("\\\n" + shortWelcome, "\n");

            if (cmd.size())
            {
                QTextEdit::append("");
                QTextCursor cursor = textCursor();
                cursor.movePosition(QTextCursor::End);
                setTextCursor(cursor);
                commandPos = textCursor().position();
                emit ExecuteCommand(cmd);
            }
            else
            {
                append("");
            }

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

void ConsoleWidget::setWelcome(const QString &text)
{
    welcome = text;
}

void ConsoleWidget::append(const QString &text)
{
    QString newText = text;

    if (newText.size() != 0 && newText[newText.size() - 1] != '\n')
        newText += "\n";

    QTextEdit::append(newText + welcome);
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
    commandPos = textCursor().position();
    readOnly = false;
}

void ConsoleWidget::plainOutput(const QString &text)
{
    readOnly = true;

    QTextEdit::insertPlainText(text);
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
    commandPos = textCursor().position();
}

void ConsoleWidget::unlock()
{
    QTextEdit::insertPlainText(welcome);
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
    commandPos = textCursor().position();
    readOnly = false;
}

void ConsoleWidget::SetCursorPos(int pos, bool isKeepAnchor)
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(pos,
            isKeepAnchor ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
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
