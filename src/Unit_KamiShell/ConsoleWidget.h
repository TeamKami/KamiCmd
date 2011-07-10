#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QTextEdit>
#include <QTime>

enum ConsoleMode {
    NORMAL,
    PLAIN,
    READONLY
};

class ConsoleWidget: public QTextEdit
{
    Q_OBJECT

public:
    ConsoleWidget(QWidget *parent);
    ~ConsoleWidget();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

public slots:
    void setWelcome(QString);
    void setShortWelcome(QString);
    void changeMode(ConsoleMode);
    void reset();
    virtual void append(const QString &);
private:
    int hindex;
    QStringList history;
    QTime tabPressed;
    ConsoleMode mode;
    QString welcome, shortWelcome;
    int commandPos;
    void SetCursorPos(int pos, bool isKeepAnchor = false);
    void LimitCursorSelectionTo(int start);

signals:
    void command(QString);
    void read(QString);
    void terminate();
    void tab();
    void tabTab();
};

#endif // CONSOLEWIDGET_H
