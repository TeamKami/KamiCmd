#include "FileOperationTask.h"
#include "library.h"

#include <QEventLoop>

FileOperationTask::FileOperationTask( IFileOperation *fileOperation )
	: operation(fileOperation)
{
}

FileOperationTask::~FileOperationTask()
{
	g_Core->DebugWrite("OperationQueue", "File Operation task is complete. Deleting...");
	dynamic_cast<QObject *>(operation.operation)->deleteLater();
}

void FileOperationTask::run()
{	
	operation.Exec();
	QEventLoop loop;
	QObject::connect(dynamic_cast<QObject *>(operation.operation), SIGNAL(finished()), &loop, SLOT(quit()), Qt::QueuedConnection);
	loop.exec();
}

