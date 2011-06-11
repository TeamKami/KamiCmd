#include "Unit_Console.h"
#include "library.h"
#include "../IPanel.h"
#include <QTextCodec>

Unit_Console::Unit_Console(QWidget *parent)
	: IUnit(parent)
{
	layout = new QVBoxLayout(this);
	layout->setMargin(0);
	console = new ConsoleWidget(this);
	//layout->addWidget(editor);
	layout->addWidget(console);

	connect(console, SIGNAL(ExecuteCommand(QString)), this, SLOT(ExecuteCommand(QString)));
}

QString Unit_Console::GetText()
{
	return text;
}

void Unit_Console::SaveState( QSettings &/*set*/ )
{
}

void Unit_Console::LoadState( QSettings &/*set*/ )
{
}

void Unit_Console::Create( IUnit *createdFrom )
{
	if (IPanel *panel = dynamic_cast<IPanel *>(createdFrom))
		path = panel->GetPath();
	else
		path = "/";
	text = "Console";
	emit TextChanged();

	CreateConsole();
}

void Unit_Console::CreateConsole()
{
	cmd = new QProcess(this);
	cmd->setProcessChannelMode(QProcess::MergedChannels);

	connect(cmd, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));

#ifdef Q_OS_WIN32
	cmd->start("cmd.exe");
#else
	cmd->start("sh");
	cmd->write("pwd\n");
#endif
}

void Unit_Console::readyReadStandardOutput()
{
	console->append(ConvertFromConsoleCP(cmd->readAllStandardOutput()));
}

void Unit_Console::ExecuteCommand( QString command )
{
	cmd->write(ConvertToConsoleCP(command));
#ifdef Q_OS_UNIX
	cmd->write("pwd\n");
#endif
}

QString Unit_Console::ConvertFromConsoleCP( QByteArray string )
{
#ifdef Q_OS_WIN32
	QTextCodec *codec = QTextCodec::codecForName("CP866");
	return codec->toUnicode(string);
#else
	return QString::fromUtf8(string);
#endif
}

QByteArray Unit_Console::ConvertToConsoleCP( QString string )
{
#ifdef Q_OS_WIN32
	QTextCodec *codec = QTextCodec::codecForName("CP866");
	return codec->fromUnicode(string);
#else
	return string.toUtf8();
#endif
}

QIcon Unit_Console::GetIcon()
{
	return QIcon();
}
