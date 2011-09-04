#include "CopyProgressDialog.h"
#include "CopyThread.h"
#include "library.h"

#include <QMessageBox>

CopyProgressDialog::CopyProgressDialog(FileCopy *fileCopy, QWidget *parent)
	: QDialog(parent), fileCopy(fileCopy)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	connect(&refreshTimer, SIGNAL(timeout()), SLOT(update()));
}

CopyProgressDialog::~CopyProgressDialog()
{
//	QMessageBox::information(NULL, "", "");
	copyThread->wait();
	copyThread->exit();
}

void CopyProgressDialog::update()
{
	if(fileCopy->GetState() == FileCopy::Finished)
	{
		refreshTimer.stop();
		close();
	}
//	QMessageBox::information(this, "", QString::number(fileCopy->GetProgress()));

	ui.progressBar->setValue(fileCopy->GetProgress());
	ui.sourceLabel->setText(fileCopy->GetFileName());
	
}

int CopyProgressDialog::exec()
{
	copyThread = new CopyThread(this, fileCopy);
	ui.destinationLabel->setText(fileCopy->GetDestination());
	ui.totalSizeLabel->setText(tr("Total: ") + QString::number(fileCopy->GetTotalSize()));
	refreshTimer.start(250);
	copyThread->start();
	return QDialog::exec();
}

QString CopyProgressDialog::bytesToString( qint64 /*s*/ )
{
	return QString();
}
