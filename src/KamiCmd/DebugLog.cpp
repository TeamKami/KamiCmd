#include "DebugLog.h"

DebugLog::DebugLog(QObject *parent)
	: QObject(parent), dialog(new DebugDialog)
{
	connect(this, SIGNAL(SendText(QtMsgType, const QString &)), dialog, SLOT(outputMessage(QtMsgType, const QString &)));
}

DebugLog::~DebugLog()
{

}

void DebugLog::Write( QtMsgType type, const QString & msg )
{
	emit SendText(type, msg);
}

void DebugLog::ShowDebugDialog()
{
	dialog->show();
}

