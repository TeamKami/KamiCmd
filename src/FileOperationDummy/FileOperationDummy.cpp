#include "FileOperationDummy.h"
#include "DummyProgressDialog.h"

FileOperationDummy::FileOperationDummy(QObject *parent)
	: IFileOperation(parent), timer(0), progressDialog(NULL)
{
	elapsed = 0;
	duration = 6000;
}

FileOperationDummy::~FileOperationDummy()
{
	delete timer;
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
	timer->start(Interval);
	connect(timer,SIGNAL(timeout()), SLOT(update()));
	return true;
}

void FileOperationDummy::Resume()
{
	m.lock();
	if(!timer->isActive())
		timer->start(Interval);
	state = Running;
	m.unlock();
}

void FileOperationDummy::Pause()
{
	m.lock();
	timer->stop();
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
		timer->stop();
		emit finished();
		state = Finished;
	}
}
