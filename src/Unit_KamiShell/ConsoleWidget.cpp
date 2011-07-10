#include "ConsoleWidget.h"
#include <QKeyEvent>
#include <QTextBlock>
#include <QTextLayout>

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QTextEdit(parent), commandPos(0), welcome(">>"), shortWelcome(">"), mode(NORMAL), hindex(-1)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Text, QColor(236, 233, 216));
    pal.setColor(QPalette::Base, Qt::black);
    setPalette(pal);
    tabPressed.start();
}

ConsoleWidget::~ConsoleWidget()
{

}

void ConsoleWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_C && event->modifiers().testFlag(
            Qt::ControlModifier))
    {
        emit terminate();
    }

    if (mode == READONLY)
    {
        return;
    }

    if (mode == PLAIN)
    {
        append(event->text());
        emit read(event->text());
    }

    switch (event->key())
    {
    case Qt::Key_Tab:
        {
            int ms = tabPressed.restart();

            if (ms < 1000)
            {
                emit tabTab();
            }
            else
            {
                emit tab();
            }
            return;
        }
        break;
    case Qt::Key_Backtab:
        event->ignore();
        break;
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

    case Qt::Key_Down:
        if (hindex > 0)
        {
            QTextEdit::setPlainText(QTextEdit::toPlainText().left(commandPos));
            SetCursorPos(commandPos);
            insertPlainText(history[--hindex]);
        }
        else
        {
            QTextEdit::setPlainText(QTextEdit::toPlainText().left(commandPos));
            SetCursorPos(commandPos);
        }
        return;
        break;

    case Qt::Key_Delete:
        LimitCursorSelectionTo(commandPos);
    case Qt::Key_Right:
        if (textCursor().position() < commandPos)
        {
            SetCursorPos(commandPos, event->modifiers() & Qt::ShiftModifier);
            return;
        }
        break;

    case Qt::Key_Up:
        if (history.size() > hindex + 1)
        {
            QTextEdit::setPlainText(QTextEdit::toPlainText().left(commandPos));
            SetCursorPos(commandPos);
            insertPlainText(history[++hindex]);
        }
        return;
        break;
    case Qt::Key_Home:
        {
            int line = textCursor().block().layout()->lineForTextPosition(
                    textCursor().positionInBlock()).lineNumber();
            if (!line || (event->key() == Qt::Key_Home && event->modifiers()
                    & Qt::ControlModifier))
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

                history.push_front(cmd);
                hindex = -1;
                emit command(cmd);
            }
            else
            {
                append("");
            }

            return;
        }
        break;

    }
    QTextEdit::keyPressEvent(event);
}

void ConsoleWidget::setWelcome(QString text)
{
    welcome = text;
}

void ConsoleWidget::setShortWelcome(QString text)
{
    shortWelcome = text;
}

void ConsoleWidget::append(const QString &text)
{
    QString newText = text;

    if (mode == NORMAL)
    {
        if (newText.size() != 0 && newText[newText.size() - 1] != '\n')
            newText += "\n";
        newText += welcome;
    }

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
    QTextEdit::insertPlainText(newText);
    cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);

    commandPos = textCursor().position();
}

void ConsoleWidget::SetCursorPos(int pos, bool isKeepAnchor)
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(pos, isKeepAnchor ? QTextCursor::KeepAnchor
            : QTextCursor::MoveAnchor);
    setTextCursor(cursor);
}

void ConsoleWidget::LimitCursorSelectionTo(int start)
{
    QTextCursor cursor = textCursor();
    int anchor = cursor.anchor(), position = cursor.position();
    if (anchor != position && (anchor < start || position < start))
    {
        cursor.setPosition(qMax(start, anchor), QTextCursor::MoveAnchor);
        cursor.setPosition(qMax(start, position), QTextCursor::KeepAnchor);
        setTextCursor(cursor);
    }
}

void ConsoleWidget::changeMode(ConsoleMode m)
{
    if (m == mode)
        return;

    mode = m;

    if (mode == NORMAL)
    {
        append("");
    }
}

void ConsoleWidget::reset()
{
    setPlainText("");
}
