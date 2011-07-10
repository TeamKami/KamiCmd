#ifndef UNIT_KAMISHELL_H
#define UNIT_KAMISHELL_H

#include <QtGui>
#include "IUnitManager.h"
#include "Unit_KamiShell/ConsoleWidget.h"
#include <QProcess>

class Unit_KamiShell: public IUnit
{
    Q_OBJECT

public:
    Unit_KamiShell(QWidget *parent);
    virtual void Create(IUnit *createdFrom);
    virtual void Link(IUnit * /*withUnit*/)
    {
    }
    ;
    virtual QString GetText();
    virtual QIcon GetIcon();
    virtual void SaveState(QSettings &set);
    virtual void LoadState(QSettings &set);

    QString ConvertFromConsoleCP(QByteArray string);
    QByteArray ConvertToConsoleCP(QString string);

private:
    QVBoxLayout *layout;
    ConsoleWidget *console;
    QString text;
    QString path;

    QProcess *cmd;
    QDir directory;
    void CreateConsole();
    bool innerCommand(QString const & cmd, QStringList const & args);

    void cd(QStringList const & args);
    void about(QStringList const & args);
signals:
    void TextChanged();

private slots:
    void readyReadStandardOutput();
    void ExecuteCommand(QString cmd);
    void finished(int, QProcess::ExitStatus);
    void error(QProcess::ProcessError);
    void terminate();
    void keyPress(QString);
    void tab(QString const &);
    void tabTab(QString const &);
    QStringList complete(QString const &, bool full = false);
};

#endif // UNIT_KAMISHELL_H
