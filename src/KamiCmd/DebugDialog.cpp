#include "DebugDialog.h"

#include <QtCore/QTextStream>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

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
