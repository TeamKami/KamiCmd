#include "DebugDialog.h"
#include "library.h"

#include <QtCore/QTextStream>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include "IUnitManager.h"

DebugDialog::DebugDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	QList<QPair<ICoreFunctions::DebugWriteImportance, QString> > writeLog = g_Core->GetDebugWriteLog();
	for (int i = 0; i < writeLog.size(); i++)
		outputMessage(writeLog.at(i).first, writeLog.at(i).second);
	connect(g_Core, SIGNAL(DebugMessageReceived(ICoreFunctions::DebugWriteImportance, QString)), SLOT(outputMessage(ICoreFunctions::DebugWriteImportance, QString)));
}

DebugDialog::~DebugDialog()
{

}

void DebugDialog::outputMessage(ICoreFunctions::DebugWriteImportance type, QString msg)
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
	default:
		ui.debugBrowser->append(msg);
		break;
	}
}

void DebugDialog::on_saveButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Log"), "log.txt", tr("Text files (*.txt)"));
	if(fileName.isEmpty())
		return;
	
	QFile file(fileName);
	if(!file.open(QFile::WriteOnly))
	{
		QMessageBox::warning(this, tr("Warning"), tr("File %1 can not be opened for writing").arg(fileName));
		return;
	}

	QTextStream stream(&file);
	stream << ui.debugBrowser->toPlainText();
}
