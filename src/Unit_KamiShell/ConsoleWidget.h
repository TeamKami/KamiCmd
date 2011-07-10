#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QTextEdit>

class ConsoleWidget: public QTextEdit
{
Q_OBJECT

public:
    ConsoleWidget(QWidget *parent);
    ~ConsoleWidget();

    virtual void keyPressEvent(QKeyEvent *event);

public slots:
    virtual void append(const QString &text);
    virtual void plainOutput(const QString &text);
    virtual void setWelcome(const QString &text);
    virtual void unlock();
private:
    bool readOnly;
    int commandPos;
    QString welcome, shortWelcome;
    void SetCursorPos(int pos, bool isKeepAnchor = false);
    void LimitCursorSelectionTo(int start);

signals:
    void ExecuteCommand(QString command);
    void terminate();
    void keyPress(QString);
};

#endif // CONSOLEWIDGET_H
