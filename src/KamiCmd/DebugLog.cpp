#include "DebugLog.h"

DebugLog::DebugLog(QObject *parent)
	: QObject(parent), dialog(new DebugDialog)
{
	qRegisterMetaType<QtMsgType>("QtMsgType");
	connect(this, SIGNAL(SendText(QtMsgType, const QString &)), dialog, SLOT(outputMessage(QtMsgType, const QString &)), Qt::QueuedConnection);
	connect(this, SIGNAL(ShowDialog()), dialog, SLOT(show()), Qt::QueuedConnection);
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
	emit ShowDialog();
}

