#include "FileOperationDummy.h"
#include "DummyProgressDialog.h"

FileOperationDummy::FileOperationDummy(QObject *parent)
	: IFileOperation(parent), timer(0), progressDialog(NULL), state(Running)
{
	elapsed = 0;
	duration = 6000;
}

FileOperationDummy::~FileOperationDummy()
{
	timer->deleteLater();
}

int FileOperationDummy::GetProgress() const
{
	int progress = duration != 0 ? elapsed / (duration / 100.0) : 100;
	return progress;
}

bool FileOperationDummy::Exec()
{
	timer = new QTimer;
	connect(this, SIGNAL(cancelOperation()), timer, SLOT(stop()));
	connect(this, SIGNAL(startOperation()), timer, SLOT(start()));
	connect(this, SIGNAL(pauseOperation()), timer, SLOT(stop()));
	connect(this, SIGNAL(resumeOperation()), timer, SLOT(start()));

	timer->setInterval(Interval);
	emit startOperation();	
	connect(timer,SIGNAL(timeout()), SLOT(update()));
	return true;
}

void FileOperationDummy::Resume()
{
	m.lock();
	emit resumeOperation();
	state = Running;
	m.unlock();
}

void FileOperationDummy::Pause()
{
	m.lock();
	emit pauseOperation();
	state = Paused;
	m.unlock();
}

void FileOperationDummy::Cancel()
{
	m.lock();
	emit cancelOperation();
	state = Canceled;
	m.unlock();
}

IFileOperation::OperationState FileOperationDummy::GetState() const
{
	return state;
}

QString FileOperationDummy::GetType() const
{
	return tr("Dummy Operation");
}

void FileOperationDummy::ShowProgressDialog(QWidget *parent)
{
	if(!progressDialog)
		progressDialog = new DummyProgressDialog(this, parent);
	progressDialog->show();
}

void FileOperationDummy::update()
{
	elapsed += Interval;
	if(elapsed >= duration)
	{
		state = Finished;
		emit finished();
		emit finished(this);
	}
}
