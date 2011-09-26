#include "DebugLog.h"

DebugLog::DebugLog(QObject *parent)
	: QObject(parent), dialog(new DebugDialog)
{
	qRegisterMetaType<ICoreFunctions::DebugWriteImportance>("ICoreFunctions::DebugWriteImportance");
	connect(this, SIGNAL(SendText(ICoreFunctions::DebugWriteImportance, const QString &)), dialog, SLOT(outputMessage(ICoreFunctions::DebugWriteImportance, const QString &)), Qt::QueuedConnection);
	connect(this, SIGNAL(ShowDialog()), dialog, SLOT(show()), Qt::QueuedConnection);
}

DebugLog::~DebugLog()
{

}

void DebugLog::Write( ICoreFunctions::DebugWriteImportance type, const QString & msg )
{
	emit SendText(type, msg);
}

void DebugLog::ShowDebugDialog()
{
	emit ShowDialog();
}

