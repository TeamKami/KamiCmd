#include "CopyProgressDialog.h"
#include "CopyThread.h"
#include "library.h"

#include <QMessageBox>


CopyProgressDialog::CopyProgressDialog(FileCopy *fileCopy, QWidget *parent)
	: QDialog(parent), fileCopy(fileCopy), ticksPassed(0), oldTotalCopied(0)
{
	ui.setupUi(this);
	memset(bytesCopiedBetweenTicks, 0, sizeof(bytesCopiedBetweenTicks));
	setAttribute(Qt::WA_DeleteOnClose);
	connect(&refreshTimer, SIGNAL(timeout()), SLOT(update()));
} 

CopyProgressDialog::~CopyProgressDialog()
{
	copyThread->wait();
	copyThread->exit();
}

void CopyProgressDialog::update()
{
	const int state = fileCopy->GetState();
	if(state == FileCopy::Finished)
	{
		refreshTimer.stop();
		accept();
	}

	if(state != FileCopy::Running && state != FileCopy::ForcedRunning)
		return;
	
	updateSpeed();

	ui.progressBar->setValue(fileCopy->GetProgress());
	ui.totalSizeLabel->setText(tr("Total: ") + formatSize(fileCopy->GetTotalBytesCopied()) 
		+ '/' + formatSize(fileCopy->GetTotalSize()));	
	
	
	if(const FileInfo *file = fileCopy->GetCurrentCopiedFile())
	{
		ui.sourceLabel->setText(file->name);
		ui.currentFileBytesCopied->setText(formatSize(fileCopy->GetCurrentFileBytesCopied())	 + '/' + formatSize(file->size));
	}
	
	ui.progressBar_2->setValue(fileCopy->GetCurrentFileProgress());		
}

void CopyProgressDialog::show()
{
	ui.time->setText(QDateTime::currentDateTime().toString() + "/ ???");

	copyThread = new CopyThread(this, fileCopy);
	ui.destinationLabel->setText(fileCopy->GetDestination());
	refreshTimer.start(250);
	copyThread->start();
	
	QDialog::show();
}

QString CopyProgressDialog::formatSize( qint64 size )
{
	// Qt note:
	// According to the Si standard KB is 1000 bytes, KiB is 1024
	// but on windows sizes are calculated by dividing by 1024 so we do what they do.
	const quint64 kb = 1024;
	const quint64 mb = 1024 * kb;
	const quint64 gb = 1024 * mb;
	const quint64 tb = 1024 * gb;
	const quint64 pb = 1024 * tb;
	quint64 bytes = size;

	if (bytes >= 10*pb)
		return QLocale().toString((bytes + tb/2) / tb) + QString::fromLatin1(" TB");
	if (bytes >= 10*tb)
		return QLocale().toString((bytes + gb/2) / gb) + QString::fromLatin1(" GB");
	if (bytes >= 10*gb)
		return QLocale().toString((bytes + mb/2) / mb) + QString::fromLatin1(" MB");
	if (bytes >= gb)
		return QLocale().toString((bytes + kb/2) / kb) + QString::fromLatin1(" KB");
	return QLocale().toString(bytes) + QString::fromLatin1(" B");
}

void CopyProgressDialog::cancelCopy()
{
	fileCopy->Pause();
	int r = QMessageBox::question(this, tr("Warning"),
		tr("Are you sure you want to cancel copy?"),
		QMessageBox::Yes | QMessageBox::No);
   	if(r == QMessageBox::Yes)
 	{
 		fileCopy->Cancel();
		reject();
	}
 	else
 		fileCopy->Resume();
}

void CopyProgressDialog::updateSpeed()
{
	// speed is calculated by averaging speed in last 10 update ticks

	qint64 totalCopied = fileCopy->GetTotalBytesCopied();
	bytesCopiedBetweenTicks[ticksPassed %= ticksMeasured] = totalCopied - oldTotalCopied;

	ticksPassed++;
	oldTotalCopied = totalCopied;
	
	qint64 t = 0;
	for(int i = 0; i < ticksMeasured; i++)
		t += bytesCopiedBetweenTicks[i];
	int speed = t / 2.5;

	ui.speed->setText(tr("Speed: ") + formatSize(speed) + "/s");
}

void CopyProgressDialog::on_pauseResume_clicked()
{

	if(fileCopy->GetState() == FileCopy::Running)
	{
		ui.pauseResume->setText(tr("Resume"));
		fileCopy->Pause();
	}
	else if(fileCopy->GetState() == FileCopy::Paused)
	{
		ui.pauseResume->setText(tr("Pause"));
		fileCopy->Resume();
	}
}

void CopyProgressDialog::on_cancelCopy_clicked()
{
	cancelCopy();
}

void CopyProgressDialog::HandleError( const QString filePath, QFile::FileError error, const QString errorText )
{
	switch(error)
	{
	case QFile::OpenError:
	case QFile::PermissionsError:
	case QFile::ReadError:
	case QFile::FatalError:
	case QFile::RenameError:
	case QFile::UnspecifiedError:
	case QFile::RemoveError:
	case QFile::ResizeError:
		{

		}
	}
}
