#include "Unit_KamiShell/Unit_KamiShell.h"
#include "library.h"
#include "../IPanel.h"
#include <QTextCodec>

Unit_KamiShell::Unit_KamiShell(QWidget *parent) :
        IUnit(parent), cmd(0)
{
// 	layout = new QVBoxLayout(this);
// 	layout->setMargin(0);
    console = new ConsoleWidget(this);
    //layout->addWidget(editor);
    QMainWindow::setCentralWidget(console);

    connect(console, SIGNAL(command(QString)),
            SLOT(ExecuteCommand(QString)));
    connect(console, SIGNAL(terminate()),
                SLOT(terminate()));
    connect(console, SIGNAL(read(QString)),
                    SLOT(keyPress(QString)));

    directory = QDir::current();
    console->setWelcome(directory.path() + " $ ");
    console->append("Welcome to KamiShell");
}

QString Unit_KamiShell::GetText()
{
    return text;
}

void Unit_KamiShell::SaveState(QSettings &/*set*/)
{
}

void Unit_KamiShell::LoadState(QSettings &/*set*/)
{
}

void Unit_KamiShell::Create(IUnit *createdFrom)
{
    if (IPanel *panel = dynamic_cast<IPanel *>(createdFrom))
        path = panel->GetPath();
    else
        path = "/";
    text = "KamiShell";
    emit
    TextChanged();

    CreateConsole();
}

void Unit_KamiShell::finished(int status, QProcess::ExitStatus exitStatus)
{
    if (cmd)
    {
        if (exitStatus == QProcess::NormalExit)
        {
            //console->append(QString("Exited with code ") + status);
            //console->unlock();
        }
        else
        {
            console->append("Command failed (" + cmd->errorString() + ")\n");
        }

        console->changeMode(NORMAL);
        //delete cmd;
        cmd = 0;
    }
}

void Unit_KamiShell::terminate()
{
    if (cmd)
    {
        cmd->terminate();
        cmd->kill();
    }
}

void Unit_KamiShell::error(QProcess::ProcessError error)
{
    console->append("Command failed (" + cmd->errorString() + ")\n");
    console->changeMode(NORMAL);
    cmd = 0;
}

void Unit_KamiShell::CreateConsole()
{

}

void Unit_KamiShell::readyReadStandardOutput()
{
    console->append(ConvertFromConsoleCP(cmd->readAllStandardOutput()));
}

void Unit_KamiShell::keyPress(QString s)
{
    if (cmd)
    {
        cmd->write(ConvertToConsoleCP(s));
    }
}

void Unit_KamiShell::ExecuteCommand(QString command)
{
    QStringList sl = command.split(" ", QString::SkipEmptyParts);
    QString program = sl[0];
    sl.removeFirst();

    if (innerCommand(program, sl))
    {
        return;
    }

    if (!cmd)
    {
        cmd = new QProcess(this);
    }

    cmd->setProcessChannelMode(QProcess::MergedChannels);
    connect(cmd, SIGNAL(readyReadStandardOutput()),
            SLOT(readyReadStandardOutput()));
    connect(cmd, SIGNAL(error(QProcess::ProcessError)),
            SLOT(error(QProcess::ProcessError)));
    connect(cmd, SIGNAL(finished(int, QProcess::ExitStatus)),
            SLOT(finished(int, QProcess::ExitStatus)));

    cmd->setWorkingDirectory(directory.path());

    console->changeMode(PLAIN);

    cmd->start(program, sl);

    //console->append("Command not found: " + command);
    //console->setWelcome(QDir::current().path() + ": ");
}

QString Unit_KamiShell::ConvertFromConsoleCP(QByteArray string)
{
#ifdef Q_OS_WIN32
    QTextCodec *codec = QTextCodec::codecForName("CP866");
    return codec->toUnicode(string);
#else
    return QString::fromUtf8(string);
#endif
}

QByteArray Unit_KamiShell::ConvertToConsoleCP(QString string)
{
#ifdef Q_OS_WIN32
    QTextCodec *codec = QTextCodec::codecForName("CP866");
    return codec->fromUnicode(string);
#else
    return string.toUtf8();
#endif
}

QIcon Unit_KamiShell::GetIcon()
{
    return QIcon();
}

bool Unit_KamiShell::innerCommand(QString const & cmd, QStringList const & args)
{
    #define CMD(name) if (cmd == #name) {name(args); return true;}
    CMD(cd)
    CMD(about)

    #undef CMD
    return false;
}

void Unit_KamiShell::about(QStringList const & args)
{

    QString msg(
            "╔═══════════════════════════════════════════════════╗\n"
            "║  KamiShell v 0.1                                  ║\n"
            "║  Cross-platform shell for KamiCmd                 ║\n"
            "║  Author: Nikolay Filchenko <finomen812@gmail.com> ║\n"
            "╚═══════════════════════════════════════════════════╝"
    );

    console->append(msg);
}

void Unit_KamiShell::cd(QStringList const & args)
{
    if (args.size() > 1)
    {
        console->append("Usage: \n"
                        "  cd <directory>\n"
                        "  cd");
        return;
    }

    if (args.size() == 0)
    {
        directory = QDir::current();
        console->setWelcome(directory.path() + ": ");
        console->append("");
        return;
    }

    QDir d = directory;
    d.cd(args[0]);

    if (!d.exists())
    {
        console->append("Invalid directory");
        return;
    }

    directory = d;
    console->setWelcome(directory.path() + " $ ");
    console->append("");
}
