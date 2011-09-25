#include "DebugDialog.h"

DebugDialog::DebugDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

DebugDialog::~DebugDialog()
{

}

void DebugDialog::outputMessage(QtMsgType type, const QString &msg)
{
	switch (type)
	{
	case QtDebugMsg:
		ui.debugBrowser->append(msg);
		break;

	case QtWarningMsg:
		ui.debugBrowser->append(tr("-- WARNING: %1").arg(msg));
		break;

	case QtCriticalMsg:
		ui.debugBrowser->append(tr("-- CRITICAL: %1").arg(msg));
		break;

	case QtFatalMsg:
		ui.debugBrowser->append(tr("-- FATAL: %1").arg(msg));
		break;
	}
}
