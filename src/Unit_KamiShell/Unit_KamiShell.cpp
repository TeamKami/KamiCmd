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

    connect(console, SIGNAL(tab(const QString &)),
                        SLOT(tab(const QString &)));
    connect(console, SIGNAL(tabTab(const QString &)),
                            SLOT(tabTab(const QString &)));

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

void Unit_KamiShell::finished(int /*status*/, QProcess::ExitStatus exitStatus)
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

void Unit_KamiShell::error(QProcess::ProcessError /*error*/)
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
    QString tc = command;
    tc.replace("\\ ", QChar(1));
    QStringList sl = tc.split(" ", QString::SkipEmptyParts);

    for (int i = 0; i < sl.size(); ++i)
    {
        sl[i].replace(QChar(1), " ");
    }

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

void Unit_KamiShell::about(QStringList const & /*args*/)
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

#ifdef Q_OS_WIN32
#define PCOMP s1[len].toUpper() == s2[len].toUpper()
#else
#define PCOMP s1[len] == s2[len]
#endif

void common(QString & s1, QString const & s2)
{
    int len = 0;

    for (; len < s1.size() && len < s2.size() && PCOMP; ++len);

    s1 = s1.left(len);

}
#undef PCOMP

void Unit_KamiShell::tab(QString const & command)
{
    QStringList names = complete(command, true);

    if (names.size() == 0)
    {
        return;
    }

    QString name = names[0];

    foreach(QString const & s, names)
    {
        common(name, s);
    }

    name.replace(" ", "\\ ");

    if (name.size())
    {
        QString tc = command;
        tc.replace("\\ ", QChar(1));
        QStringList sl = tc.split(" ", QString::KeepEmptyParts);

        for (int i = 0; i < sl.size(); ++i)
        {
            sl[i].replace(QChar(1), "\\ ");
        }

        if (sl.size())
        {
            sl[sl.size() - 1] = name;
        }
        else
        {
            sl.push_back(name);
        }

        QString cmd;

        QString space = "";

        foreach (QString const & s, sl)
        {
            cmd += space + s;
            space = " ";
        }

        console->complete(cmd);
    }
}

void Unit_KamiShell::tabTab(QString const & command)
{
    QStringList names = complete(command);

    if (names.size() > 1)
    {
        QString hint;
        QString delim = "\n  ";

        foreach (QString const & s, names)
        {
            hint += delim + s;
        }

        console->hint(hint);
    }

    tab(command);

}

QStringList Unit_KamiShell::complete(QString const & command, bool full)
{
    QString tc = command;
    tc.replace("\\ ", QChar(1));

    QStringList sl = tc.split(" ", QString::KeepEmptyParts);

    QString ac = sl.size() ? sl[sl.size() - 1] : "";
    ac.replace(QChar(1), " ");

    int end = ac.lastIndexOf("/");

    QString prefix = ac;
    QDir d = directory;
    QString cpath = "";

    if (end != -1)
    {
        prefix = ac.right(ac.size() - end - 1);

        d = QDir(cpath = ac.left(end + 1));
    }

    if (cpath.size() && cpath[cpath.size() - 1] != '/')
    {
        cpath += "/";
    }

    if (d.isRelative())
    {
        QDir t = directory;
        t.cd(d.path());
    }

    if (d.exists())
    {
#ifndef Q_OS_WIN32
#define PFILTER | QDir::CaseSensitive
#else
#define PFILTER
#endif
        QStringList names = d.entryList(QStringList(prefix + "*"), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot PFILTER);
#undef PFILTER
        if (full)
        {
            for (int i = 0; i < names.size(); ++i)
            {
                names[i] = cpath + names[i];
            }
        }

        return names;
    }

    return QStringList();
}
