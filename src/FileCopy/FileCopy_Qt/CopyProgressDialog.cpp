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
	
	ui.progressBar->setValue(fileCopy->GetProgress());
	ui.sourceLabel->setText(fileCopy->GetFileName());
	ui.totalSizeLabel->setText(tr("Total: ") + formatSize(fileCopy->GetTotalBytesCopied()) 
		+ '/' + formatSize(fileCopy->GetTotalSize()));	
	
	
	if(const FileInfo *file = fileCopy->GetCurrentCopiedFile())
	{
		ui.currentFileBytesCopied->setText(formatSize(fileCopy->GetCurrentFileBytesCopied()) 
			+ '/' + formatSize(file->size));
		ui.progressBar_2->setValue(fileCopy->GetCurrentFileProgress());		
	}
}

int CopyProgressDialog::exec()
{
	copyThread = new CopyThread(this, fileCopy);
	ui.destinationLabel->setText(fileCopy->GetDestination());
	refreshTimer.start(250);
	copyThread->start();
	return QDialog::exec();
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
		return QLocale().toString((bytes + tb/2) / tb) + QString::fromLatin1(" T");
	if (bytes >= 10*tb)
		return QLocale().toString((bytes + gb/2) / gb) + QString::fromLatin1(" G");
	if (bytes >= 10*gb)
		return QLocale().toString((bytes + mb/2) / mb) + QString::fromLatin1(" M");
	if (bytes >= gb)
		return QLocale().toString((bytes + kb/2) / kb) + QString::fromLatin1(" K");
	return QLocale().toString(bytes);
}
